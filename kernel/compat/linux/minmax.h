/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _COMPAT_LINUX_MINMAX_H
#define _COMPAT_LINUX_MINMAX_H

#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 12, 0)
#include_next <linux/minmax.h>
#else
#include <linux/kernel.h>
#endif

#endif /* _COMPAT_LINUX_MINMAX_H */
