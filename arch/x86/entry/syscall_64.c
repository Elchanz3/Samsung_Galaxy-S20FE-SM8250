// SPDX-License-Identifier: GPL-2.0
/* System call table for x86-64. */

#include <linux/linkage.h>
#include <linux/sys.h>
#include <linux/cache.h>
<<<<<<< HEAD
#include <linux/syscalls.h>
#include <asm/asm-offsets.h>
#include <asm/syscall.h>

extern asmlinkage long sys_ni_syscall(void);

SYSCALL_DEFINE0(ni_syscall)
{
	return sys_ni_syscall();
}

=======
#include <asm/asm-offsets.h>
#include <asm/syscall.h>

/* this is a lie, but it does not hurt as sys_ni_syscall just returns -EINVAL */
extern asmlinkage long sys_ni_syscall(const struct pt_regs *);
>>>>>>> rebase
#define __SYSCALL_64(nr, sym, qual) extern asmlinkage long sym(const struct pt_regs *);
#include <asm/syscalls_64.h>
#undef __SYSCALL_64

#define __SYSCALL_64(nr, sym, qual) [nr] = sym,

asmlinkage const sys_call_ptr_t sys_call_table[__NR_syscall_max+1] = {
	/*
	 * Smells like a compiler bug -- it doesn't work
	 * when the & below is removed.
	 */
<<<<<<< HEAD
	[0 ... __NR_syscall_max] = &__x64_sys_ni_syscall,
=======
	[0 ... __NR_syscall_max] = &sys_ni_syscall,
>>>>>>> rebase
#include <asm/syscalls_64.h>
};
