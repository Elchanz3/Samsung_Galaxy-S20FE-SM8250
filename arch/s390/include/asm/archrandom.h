/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Kernel interface for the s390 arch_random_* functions
 *
<<<<<<< HEAD
 * Copyright IBM Corp. 2017
=======
 * Copyright IBM Corp. 2017, 2022
>>>>>>> rebase
 *
 * Author: Harald Freudenberger <freude@de.ibm.com>
 *
 */

#ifndef _ASM_S390_ARCHRANDOM_H
#define _ASM_S390_ARCHRANDOM_H

#ifdef CONFIG_ARCH_RANDOM

#include <linux/static_key.h>
<<<<<<< HEAD
#include <linux/atomic.h>
=======
#include <linux/preempt.h>
#include <linux/atomic.h>
#include <asm/cpacf.h>
>>>>>>> rebase

DECLARE_STATIC_KEY_FALSE(s390_arch_random_available);
extern atomic64_t s390_arch_random_counter;

<<<<<<< HEAD
bool s390_arch_random_generate(u8 *buf, unsigned int nbytes);

static inline bool arch_has_random(void)
=======
static inline bool __must_check arch_get_random_long(unsigned long *v)
>>>>>>> rebase
{
	return false;
}

<<<<<<< HEAD
static inline bool arch_has_random_seed(void)
{
	if (static_branch_likely(&s390_arch_random_available))
		return true;
	return false;
}

static inline bool arch_get_random_long(unsigned long *v)
{
	return false;
}

static inline bool arch_get_random_int(unsigned int *v)
{
	return false;
}

static inline bool arch_get_random_seed_long(unsigned long *v)
{
	if (static_branch_likely(&s390_arch_random_available)) {
		return s390_arch_random_generate((u8 *)v, sizeof(*v));
=======
static inline bool __must_check arch_get_random_int(unsigned int *v)
{
	return false;
}

static inline bool __must_check arch_get_random_seed_long(unsigned long *v)
{
	if (static_branch_likely(&s390_arch_random_available) &&
	    in_task()) {
		cpacf_trng(NULL, 0, (u8 *)v, sizeof(*v));
		atomic64_add(sizeof(*v), &s390_arch_random_counter);
		return true;
>>>>>>> rebase
	}
	return false;
}

<<<<<<< HEAD
static inline bool arch_get_random_seed_int(unsigned int *v)
{
	if (static_branch_likely(&s390_arch_random_available)) {
		return s390_arch_random_generate((u8 *)v, sizeof(*v));
=======
static inline bool __must_check arch_get_random_seed_int(unsigned int *v)
{
	if (static_branch_likely(&s390_arch_random_available) &&
	    in_task()) {
		cpacf_trng(NULL, 0, (u8 *)v, sizeof(*v));
		atomic64_add(sizeof(*v), &s390_arch_random_counter);
		return true;
>>>>>>> rebase
	}
	return false;
}

#endif /* CONFIG_ARCH_RANDOM */
#endif /* _ASM_S390_ARCHRANDOM_H */
