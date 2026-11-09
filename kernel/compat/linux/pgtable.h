/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _COMPAT_LINUX_PGTABLE_H
#define _COMPAT_LINUX_PGTABLE_H

#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 8, 0)
#include_next <linux/pgtable.h>
#else
#include <asm/pgtable.h>
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 12, 0)
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

#endif /* _COMPAT_LINUX_PGTABLE_H */
