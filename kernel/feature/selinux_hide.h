#ifndef __KSU_H_SELINUX_HIDE
#define __KSU_H_SELINUX_HIDE

#ifndef CONFIG_KSU_DISABLE_SELINUX_HIDE
void ksu_selinux_hide_init();
void ksu_selinux_hide_exit();
void ksu_selinux_hide_drop_backup_if_unused();
void ksu_selinux_hide_handle_second_stage();
void ksu_selinux_hide_handle_post_fs_data();
#else
/* 5.4-compat: stubs when selinux_hide is disabled */
static inline void ksu_selinux_hide_init(void) {}
static inline void ksu_selinux_hide_exit(void) {}
static inline void ksu_selinux_hide_drop_backup_if_unused(void) {}
static inline void ksu_selinux_hide_handle_second_stage(void) {}
static inline void ksu_selinux_hide_handle_post_fs_data(void) {}
#endif

#endif
