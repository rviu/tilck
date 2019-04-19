/* SPDX-License-Identifier: BSD-2-Clause */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/time.h>

#include "devshell.h"
#include "sysenter.h"

static char pagebuf[4096];

static void create_test_file(void)
{
   int fd, rc;

   fd = open("/tmp/test1", O_CREAT | O_WRONLY, 0644);

   if (fd < 0) {
      perror("open failed");
      exit(1);
   }

   printf("writing 'a'...\n");
   memset(pagebuf, 'a', 4096);
   rc = write(fd, pagebuf, 4096);
   DEVSHELL_CMD_ASSERT(rc == 4096);

   printf("writing 'b'...\n");
   memset(pagebuf, 'b', 4096);
   rc = write(fd, pagebuf, 4096);
   DEVSHELL_CMD_ASSERT(rc == 4096);

   printf("writing 'c'...\n");
   memset(pagebuf, 'c', 4096);
   rc = write(fd, pagebuf, 4096);
   DEVSHELL_CMD_ASSERT(rc == 4096);

   close(fd);
}

static void write_on_test_file(void)
{
   int fd, rc;
   off_t off;
   char buf[32] = "hello world";

   fd = open("/tmp/test1", O_WRONLY);

   if (fd < 0) {
      perror("open failed");
      exit(1);
   }

   rc = write(fd, buf, 32);
   DEVSHELL_CMD_ASSERT(rc == 32);

   off = lseek(fd, 4096, SEEK_SET);
   DEVSHELL_CMD_ASSERT(off == 4096);

   rc = write(fd, "XXX", 3);
   DEVSHELL_CMD_ASSERT(rc == 3);

   close(fd);
}

static void read_past_end(void)
{
   int rc, fd;
   off_t off;
   char buf[32] = { [0 ... 30] = 'a', [31] = 0 };

   fd = open("/tmp/test1", O_RDONLY);

   if (fd < 0) {
      perror("open failed");
      exit(1);
   }

   off = lseek(fd, 64 * 1024, SEEK_SET);
   printf("off: %d\n", (int)off);

   rc = read(fd, buf, sizeof(buf));
   printf("read returned: %d\n", rc);
   printf("buf: '%s'\n", buf);
   close(fd);

   unlink("/tmp/test1");
}

int cmd_fs1(int argc, char **argv)
{
   create_test_file();
   write_on_test_file();
   read_past_end();
   return 0;
}

int cmd_fs2(int argc, char **argv)
{
   int fd, rc;
   char buf[32];
   struct stat statbuf;

   fd = creat("/tmp/new_file", 0644);
   DEVSHELL_CMD_ASSERT(fd > 0);

   rc = write(fd, "test\n", 5);
   DEVSHELL_CMD_ASSERT(rc == 5);

   close(fd);

   /*
    * Being creat(path, mode) equivalent to:
    *    open(path, O_CREAT|O_WRONLY|O_TRUNC, mode)
    * we expect creat() to succeed even if the file already exists.
    */

   rc = creat("/tmp/new_file", 0644);
   DEVSHELL_CMD_ASSERT(rc > 0);
   close(rc);

   /*
    * Now, since creat() implies O_TRUNC, we have to check that the file has
    * been actually truncated.
    */

   fd = open("/tmp/new_file", O_RDONLY);
   DEVSHELL_CMD_ASSERT(fd > 0);
   rc = read(fd, buf, sizeof(buf));
   DEVSHELL_CMD_ASSERT(rc == 0);
   close(fd);

   rc = stat("/tmp/new_file", &statbuf);
   DEVSHELL_CMD_ASSERT(rc == 0);
   DEVSHELL_CMD_ASSERT(statbuf.st_size == 0);
   DEVSHELL_CMD_ASSERT(statbuf.st_blocks == 0);

   /* Instead, this open() call using O_EXCL is expected to FAIL */
   rc = open("/tmp/new_file", O_CREAT | O_EXCL | O_WRONLY, 0644);

   DEVSHELL_CMD_ASSERT(rc < 0);
   DEVSHELL_CMD_ASSERT(errno == EEXIST);

   unlink("/tmp/new_file");
}