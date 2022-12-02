/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_KASAN_CHECKS_H
#define _LINUX_KASAN_CHECKS_H

<<<<<<< HEAD
#if defined(__SANITIZE_ADDRESS__) || defined(__KASAN_INTERNAL)
=======
#ifdef CONFIG_KASAN
>>>>>>> rebase
void kasan_check_read(const volatile void *p, unsigned int size);
void kasan_check_write(const volatile void *p, unsigned int size);
#else
static inline void kasan_check_read(const volatile void *p, unsigned int size)
{ }
static inline void kasan_check_write(const volatile void *p, unsigned int size)
{ }
#endif

#endif
