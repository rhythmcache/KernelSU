/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _COMPAT_LINUX_INPUT_EVENT_CODES_H
#define _COMPAT_LINUX_INPUT_EVENT_CODES_H

#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 5, 0)
#include_next <linux/input-event-codes.h>
#else
#include <uapi/linux/input-event-codes.h>
#endif

#endif /* _COMPAT_LINUX_INPUT_EVENT_CODES_H */
