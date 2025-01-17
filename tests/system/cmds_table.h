/* SPDX-License-Identifier: BSD-2-Clause */

/*
 * List with all the system tests (shellcmds). DO NOT INCLUDE THIS FILE!
 * This is a PRIVATE header for cmds_table.c. To list all of the tests,
 * access the `cmds_table` array. Example: see userapps/devshell/commands.c.
 */

/*        name,         timeout,  enabled      */

CMD_ENTRY(help,         TT_SHORT, false)
CMD_ENTRY(selftest,     TT_LONG,  false)
CMD_ENTRY(runall,       TT_LONG,  false)
CMD_ENTRY(loop,         TT_MED,   false)
CMD_ENTRY(fpu_loop,     TT_LONG,  false)

CMD_ENTRY(fork0,        TT_MED,    true)
CMD_ENTRY(fork1,        TT_SHORT,  true)
CMD_ENTRY(sysenter,     TT_SHORT,  true)
CMD_ENTRY(fork_se,      TT_MED,    true)
CMD_ENTRY(bad_read,     TT_SHORT,  true)
CMD_ENTRY(bad_write,    TT_SHORT,  true)
CMD_ENTRY(fork_perf,    TT_LONG,   true)
CMD_ENTRY(vfork_perf,   TT_LONG,   true)
CMD_ENTRY(syscall_perf, TT_MED,    true)
CMD_ENTRY(fpu,          TT_SHORT,  true)
CMD_ENTRY(brk,          TT_SHORT,  true)
CMD_ENTRY(mmap,         TT_MED,    true)
CMD_ENTRY(mmap2,        TT_SHORT,  true)
CMD_ENTRY(kcow,         TT_SHORT,  true)
CMD_ENTRY(wpid1,        TT_SHORT,  true)
CMD_ENTRY(wpid2,        TT_SHORT,  true)
CMD_ENTRY(wpid3,        TT_SHORT,  true)
CMD_ENTRY(wpid4,        TT_SHORT,  true)
CMD_ENTRY(wpid5,        TT_SHORT,  true)
CMD_ENTRY(wpid6,        TT_SHORT,  true)
CMD_ENTRY(sigsegv0,     TT_SHORT,  true)
CMD_ENTRY(sigsegv1,     TT_SHORT,  true)
CMD_ENTRY(sigsegv2,     TT_SHORT,  true)
CMD_ENTRY(sigill,       TT_SHORT,  true)
CMD_ENTRY(sigfpe,       TT_SHORT,  true)
CMD_ENTRY(sigabrt,      TT_SHORT,  true)
CMD_ENTRY(sig_ignore,   TT_SHORT,  true)
CMD_ENTRY(bigargv,      TT_SHORT,  true)
CMD_ENTRY(cloexec,      TT_SHORT,  true)
CMD_ENTRY(fs1,          TT_SHORT,  true)
CMD_ENTRY(fs2,          TT_SHORT,  true)
CMD_ENTRY(fs3,          TT_SHORT,  true)
CMD_ENTRY(fs4,          TT_SHORT,  true)
CMD_ENTRY(fs5,          TT_SHORT,  true)
CMD_ENTRY(fs6,          TT_SHORT,  true)
CMD_ENTRY(fs7,          TT_SHORT,  true)
CMD_ENTRY(fs_perf1,     TT_SHORT,  true)
CMD_ENTRY(fs_perf2,     TT_SHORT,  true)
CMD_ENTRY(fmmap1,       TT_SHORT,  true)
CMD_ENTRY(fmmap2,       TT_SHORT,  true)
CMD_ENTRY(fmmap3,       TT_SHORT,  true)
CMD_ENTRY(fmmap4,       TT_SHORT,  true)
CMD_ENTRY(fmmap5,       TT_SHORT,  true)
CMD_ENTRY(fmmap6,       TT_SHORT,  true)
CMD_ENTRY(fmmap7,       TT_SHORT,  true)
CMD_ENTRY(pipe1,        TT_SHORT,  true)
CMD_ENTRY(pipe2,        TT_SHORT,  true)
CMD_ENTRY(pipe3,        TT_SHORT,  true)
CMD_ENTRY(pipe4,        TT_SHORT,  true)
CMD_ENTRY(pipe5,        TT_SHORT,  true)
CMD_ENTRY(pollerr,      TT_SHORT,  true)
CMD_ENTRY(pollhup,      TT_SHORT,  true)
CMD_ENTRY(poll1,        TT_SHORT,  true)
CMD_ENTRY(poll2,        TT_SHORT,  true)
CMD_ENTRY(poll3,        TT_SHORT,  true)
CMD_ENTRY(select1,      TT_SHORT,  true)
CMD_ENTRY(select2,      TT_SHORT,  true)
CMD_ENTRY(select3,      TT_SHORT,  true)
CMD_ENTRY(select4,      TT_SHORT,  true)
CMD_ENTRY(execve0,      TT_SHORT,  true)
CMD_ENTRY(vfork0,       TT_SHORT,  true)
CMD_ENTRY(extra,        TT_MED,    true)
CMD_ENTRY(fatmm1,       TT_SHORT,  true)
CMD_ENTRY(sigmask,      TT_SHORT,  true)
CMD_ENTRY(sig1,         TT_SHORT,  true)
CMD_ENTRY(sig2,         TT_SHORT,  true)
CMD_ENTRY(sig3,         TT_SHORT,  true)
CMD_ENTRY(sig4,         TT_SHORT,  true)
CMD_ENTRY(sig5,         TT_SHORT,  true)
CMD_ENTRY(sig6,         TT_SHORT,  true)
CMD_ENTRY(sig7,         TT_SHORT,  true)
CMD_ENTRY(sig8,         TT_SHORT,  true)
CMD_ENTRY(sig9,         TT_SHORT,  true)
CMD_ENTRY(sig10,        TT_SHORT,  true)
CMD_ENTRY(sig11,        TT_SHORT,  true)
CMD_ENTRY(sig12,        TT_SHORT,  true)
CMD_ENTRY(sig13,        TT_SHORT,  true)
CMD_ENTRY(fork_oom,     TT_MED,    true)
CMD_ENTRY(sigsegv3,     TT_SHORT,  true)
CMD_ENTRY(sigsegv4,     TT_SHORT,  true)
CMD_ENTRY(sigsegv5,     TT_SHORT,  true)
CMD_ENTRY(getuids,      TT_SHORT,  true)
CMD_ENTRY(exit_cb,      TT_SHORT,  true)
