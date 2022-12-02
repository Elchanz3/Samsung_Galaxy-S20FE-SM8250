/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_KDEV_T_H
#define _LINUX_KDEV_T_H

#include <uapi/linux/kdev_t.h>

#define MINORBITS	20
#define MINORMASK	((1U << MINORBITS) - 1)

#define MAJOR(dev)	((unsigned int) ((dev) >> MINORBITS))
#define MINOR(dev)	((unsigned int) ((dev) & MINORMASK))
#define MKDEV(ma,mi)	(((ma) << MINORBITS) | (mi))

#define print_dev_t(buffer, dev)					\
	sprintf((buffer), "%u:%u\n", MAJOR(dev), MINOR(dev))

#define format_dev_t(buffer, dev)					\
	({								\
		sprintf(buffer, "%u:%u", MAJOR(dev), MINOR(dev));	\
		buffer;							\
	})

/* acceptable for old filesystems */
<<<<<<< HEAD
static inline bool old_valid_dev(dev_t dev)
=======
static __always_inline bool old_valid_dev(dev_t dev)
>>>>>>> rebase
{
	return MAJOR(dev) < 256 && MINOR(dev) < 256;
}

<<<<<<< HEAD
static inline u16 old_encode_dev(dev_t dev)
=======
static __always_inline u16 old_encode_dev(dev_t dev)
>>>>>>> rebase
{
	return (MAJOR(dev) << 8) | MINOR(dev);
}

<<<<<<< HEAD
static inline dev_t old_decode_dev(u16 val)
=======
static __always_inline dev_t old_decode_dev(u16 val)
>>>>>>> rebase
{
	return MKDEV((val >> 8) & 255, val & 255);
}

<<<<<<< HEAD
static inline u32 new_encode_dev(dev_t dev)
=======
static __always_inline u32 new_encode_dev(dev_t dev)
>>>>>>> rebase
{
	unsigned major = MAJOR(dev);
	unsigned minor = MINOR(dev);
	return (minor & 0xff) | (major << 8) | ((minor & ~0xff) << 12);
}

<<<<<<< HEAD
static inline dev_t new_decode_dev(u32 dev)
=======
static __always_inline dev_t new_decode_dev(u32 dev)
>>>>>>> rebase
{
	unsigned major = (dev & 0xfff00) >> 8;
	unsigned minor = (dev & 0xff) | ((dev >> 12) & 0xfff00);
	return MKDEV(major, minor);
}

<<<<<<< HEAD
static inline u64 huge_encode_dev(dev_t dev)
=======
static __always_inline u64 huge_encode_dev(dev_t dev)
>>>>>>> rebase
{
	return new_encode_dev(dev);
}

<<<<<<< HEAD
static inline dev_t huge_decode_dev(u64 dev)
=======
static __always_inline dev_t huge_decode_dev(u64 dev)
>>>>>>> rebase
{
	return new_decode_dev(dev);
}

<<<<<<< HEAD
static inline int sysv_valid_dev(dev_t dev)
=======
static __always_inline int sysv_valid_dev(dev_t dev)
>>>>>>> rebase
{
	return MAJOR(dev) < (1<<14) && MINOR(dev) < (1<<18);
}

<<<<<<< HEAD
static inline u32 sysv_encode_dev(dev_t dev)
=======
static __always_inline u32 sysv_encode_dev(dev_t dev)
>>>>>>> rebase
{
	return MINOR(dev) | (MAJOR(dev) << 18);
}

<<<<<<< HEAD
static inline unsigned sysv_major(u32 dev)
=======
static __always_inline unsigned sysv_major(u32 dev)
>>>>>>> rebase
{
	return (dev >> 18) & 0x3fff;
}

<<<<<<< HEAD
static inline unsigned sysv_minor(u32 dev)
=======
static __always_inline unsigned sysv_minor(u32 dev)
>>>>>>> rebase
{
	return dev & 0x3ffff;
}

#endif
