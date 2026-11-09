/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _COMPAT_LINUX_PGTABLE_H
#define _COMPAT_LINUX_PGTABLE_H

#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 8, 0)
#include_next <linux/pgtable.h>
#else
#include <asm/pgtable.h>
#endif

#endif /* _COMPAT_LINUX_PGTABLE_H */
