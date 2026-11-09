/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __KSU_COMPAT_H
#define __KSU_COMPAT_H

#include <linux/version.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 7, 0)
#ifndef TWA_RESUME
/* 5.4-compat: task_work_add takes a bool notify parameter before 5.7 */
#define TWA_RESUME true
#endif

#include <linux/sched/task.h>
#include <linux/sched/signal.h>
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 12, 0)
#include <asm/pgtable.h>

#ifndef pmd_leaf
#ifdef pmd_sect
#define pmd_leaf(pmd) pmd_sect(pmd)
#elif defined(pmd_large)
#define pmd_leaf(pmd) pmd_large(pmd)
#endif
#endif

#ifndef pud_leaf
#ifdef pud_sect
#define pud_leaf(pud) pud_sect(pud)
#elif defined(pud_large)
#define pud_leaf(pud) pud_large(pud)
#endif
#endif

#endif

#endif /* __KSU_COMPAT_H */
