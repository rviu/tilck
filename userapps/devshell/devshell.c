/* SPDX-License-Identifier: BSD-2-Clause */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <pwd.h>

#include "devshell.h"

bool dump_coverage;
char **shell_argv;
char **shell_env;

static char cmd_arg_buffers[MAX_ARGS][256];
static char *cmd_argv[MAX_ARGS];

static void shell_builtin_cd(int argc)
{
   int rc = 0;
   const char *dest_dir = "/";

   if (argc == 2 && strlen(cmd_argv[1]))
      dest_dir = cmd_argv[1];

   if (argc > 2) {
      printf("cd: too many arguments\n");
      return;
   }

   if (chdir(dest_dir))
      perror("cd");
}

static bool is_file(const char *filepath)
{
   struct stat statbuf;

   if (stat(filepath, &statbuf) < 0)
      return false;

   return (statbuf.st_mode & S_IFMT) == S_IFREG;
}

static void wait_child_cmd(int child_pid)
{
   int wstatus;
   waitpid(child_pid, &wstatus, 0);

   if (!WIFEXITED(wstatus)) {

      int term_sig = WTERMSIG(wstatus);
      printf("\n");

      if (term_sig != SIGINT)
         printf("[shell] command terminated by signal: %d (%s)\n",
                term_sig, strsignal(term_sig));

      return;
   }

   if (WEXITSTATUS(wstatus))
      printf("[shell] command exited with status: %d\n", WEXITSTATUS(wstatus));
}

static void shell_run_child(int argc)
{
   int saved_errno;

   /* Reset all the signal handlers to their default behavior */
   for (int i = 1; i < _NSIG; i++)
     signal(i, SIG_DFL);

   run_if_known_command(cmd_argv[0], argc - 1, cmd_argv + 1);

   /* Since we got here, cmd_argv[0] was NOT a known built-in command */

   if (!is_file(cmd_argv[0]) && argc < MAX_ARGS) {
      if (is_file("/bin/busybox")) {

         for (int i = argc; i > 0; i--)
            cmd_argv[i] = cmd_argv[i - 1];

         cmd_argv[++argc] = NULL;
         cmd_argv[0] = "/bin/busybox";
      }
   }

   execve(cmd_argv[0], cmd_argv, shell_env);

   /* if we got here, execve() failed */
   saved_errno = errno;
   perror(cmd_argv[0]);
   exit(saved_errno);
}

static int parse_cmd_line(const char *cmd_line)
{
   int argc = 0;
   char quote_char;
   char *arg = NULL;
   bool in_arg = false;
   bool in_quotes = false;

   for (const char *p = cmd_line; *p && *p != '\n'; p++) {

      if (!in_arg) {

         if (*p == ' ')
            continue;

         if (argc == MAX_ARGS)
            break;

         in_arg = true;
         cmd_argv[argc] = cmd_arg_buffers[argc];
         arg = cmd_argv[argc];
         argc++;
      }

      if (!in_quotes) {

         if (*p == ' ') {
            in_arg = false;
            *arg = 0;
            continue;
         }

         if (*p == '\'' || *p == '"') {
            in_quotes = true;
            quote_char = *p;
            continue;
         }

      } else {

         if (*p == quote_char) {
            in_quotes = false;
            continue;
         }
      }

      *arg++ = *p;
   }

   if (in_arg)
      *arg = 0;

   cmd_argv[argc] = NULL;

   if (in_quotes) {
      fprintf(stderr, "[shell] ERROR: Unterminated quote %c\n", quote_char);
      return 0;
   }

   return argc;
}

static void process_cmd_line(const char *cmd_line)
{
   int child_pid;
   int argc = parse_cmd_line(cmd_line);

   if (!argc || !cmd_argv[0][0])
      return;

   if (!strcmp(cmd_argv[0], "cd")) {
      shell_builtin_cd(argc);
      return;
   }

   if (!strcmp(cmd_argv[0], "exit")) {
      exit(0);
   }

   if ((child_pid = fork()) < 0) {
      perror("fork failed");
      return;
   }

   if (!child_pid) {
      shell_run_child(argc);
   }

   wait_child_cmd(child_pid);
}

static void show_help_and_exit(void)
{
   show_common_help_intro();

   printf(COLOR_RED "Usage:" RESET_ATTRS "\n\n");
   printf("    devshell %-15s Just run the interactive shell\n", " ");
   printf("    devshell %-15s Show this help and exit\n\n", "-h, --help");

   printf("    Internal test-infrastructure options\n");
   printf("    ------------------------------------\n\n");
   printf("    devshell %-15s List the built-in (test) commands\n\n", "-l");

   printf("    devshell [-dcov] -c <cmd> [arg1 [arg2 [arg3...]]]\n");
   printf("%-28s Run the <cmd> built-in command and exit.\n", " ");
   printf("%-28s In case -c is preceded by -dcov, the devshell\n", " ");
   printf("%-28s also dumps the kernel coverage data on-screen.\n", " ");
   exit(0);
}

static void parse_opt(int argc, char **argv)
{
   for (; argc > 0; argc--, argv++) {

      if (!strlen(*argv))
         continue;

      if (!strcmp(*argv, "-h") || !strcmp(*argv, "--help"))
         show_help_and_exit();

      if (!strcmp(*argv, "-l"))
         dump_list_of_commands_and_exit();

      if (argc == 1)
         goto unknown_opt;

      /* argc > 1 */

      if (!strcmp(*argv, "-dcov")) {
         dump_coverage = true;
         continue;
      }

      if (!strcmp(*argv, "-c")) {
         printf("[shell] Executing built-in command '%s'\n", argv[1]);
         run_if_known_command(argv[1], argc - 2, argv + 2);
         printf("[shell] Unknown built-in command '%s'\n", argv[1]);
         return;
      }

   unknown_opt:
      printf("[shell] Unknown option '%s'\n", *argv);
      break;
   }
}

int main(int argc, char **argv, char **env)
{
   static char cmdline_buf[256];
   static char cwd_buf[256];
   char uc = '#';
   int rc;

   signal(SIGINT, SIG_IGN);
   signal(SIGQUIT, SIG_IGN);

   shell_argv = argv;
   shell_env = env;

   if (argc > 1) {
      parse_opt(argc - 1, argv + 1);
      exit(1);
   }

   /* No command specified in the options: run in interactive mode */
   if (getuid() != 0)
      uc = '$';

   while (true) {

      if (getcwd(cwd_buf, sizeof(cwd_buf)) != cwd_buf) {
         perror("Shell: getcwd() failed");
         return 1;
      }

      printf(COLOR_RED "[TilckDevShell]" RESET_ATTRS ":%s%c ", cwd_buf, uc);
      fflush(stdout);

      rc = read_command(cmdline_buf, sizeof(cmdline_buf));

      if (rc < 0) {
         fprintf(stderr, "I/O error\n");
         break;
      }

      if (rc)
         process_cmd_line(cmdline_buf);
   }

   return 0;
}
