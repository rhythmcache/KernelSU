# KernelSU on Linux 5.4 (ARM64) Backport

This branch (`5.4-compat`) adapts KernelSU to build and run as a built-in driver on Linux 5.4 (specifically 5.4.302 ARM64) while keeping 100% compatibility with newer kernels (Linux 5.10+ / GKI) via `LINUX_VERSION_CODE` guards.

---

## 1. Backport Strategy & Design Principles

1. **Zero Core Kernel Invasive Edits**:
   - The driver does not require modifying core kernel subsystems (no edits to `fs/exec.c`, `fs/open.c`, `fs/namespace.c`, etc.).
   - Wire-in is achieved purely through driver registration (`drivers/kernelsu` symlink, `drivers/Kconfig`, `drivers/Makefile`).

2. **Rebase-Friendly Shims**:
   - All header compatibility shims are placed in `kernel/compat/` and injected via `-I$(KSU_KERNEL_DIR)/compat` and `-include $(KSU_KERNEL_DIR)/compat/compat.h`.
   - Stub headers include the upstream header on newer kernels (`#include_next`) and fall back to 5.4-appropriate headers on older kernels.
   - Core files in the fork contain minimal diffs, marked with `/* 5.4-compat */`.

---

## 2. Compatibility Shims Overview

### Header Stubs (`kernel/compat/linux/`)
* **`linux/minmax.h`**: Introduced in Linux 5.12. On `< 5.12.0`, redirects to `<linux/kernel.h>`.
* **`linux/pgtable.h`**: Introduced in Linux 5.8. On `< 5.8.0`, redirects to `<asm/pgtable.h>`.
* **`linux/input-event-codes.h`**: Cleaned up in Linux 5.5. On `< 5.5.0`, redirects to `<uapi/linux/input-event-codes.h>`.

### Task Work & Sched Shims (`kernel/compat/compat.h`)
* **`TWA_RESUME`**: On Linux `< 5.7.0`, `task_work_add(..., bool notify)` takes a boolean flag instead of enum `twa_resume_mode`. Defined `TWA_RESUME` as `true` for `< 5.7.0`.
* **Process Sched Declarations**: Included `<linux/sched/task.h>` and `<linux/sched/signal.h>` to ensure `init_task` and `task_pgrp` are declared across all compilation units.

### Mount / Umount Shims (`kernel/infra/su_mount_ns.c`, `kernel/feature/kernel_umount.c`)
* **`path_umount`**: Linux 5.4 has `path_umount(struct path *path, int flags)` exported/global in `fs/namespace.c`. KernelSU's `extern int path_umount(...)` links directly.
* **`path_mount`**: Introduced in Linux 5.8. KernelSU uses `path_mount(NULL, &root_path, NULL, MS_PRIVATE | MS_REC, NULL)` in `su_mount_ns.c` to make the mount tree private. On `< 5.8.0`, implemented a compatibility helper using `do_mount(NULL, cwd, NULL, flags, NULL)` under `set_fs(KERNEL_DS)`, which invokes `do_change_type` without modifying `fs/namespace.c`.

### Fsnotify Shims (`kernel/manager/pkg_observer.c`)
* **`fsnotify_ops`**: `.handle_inode_event` was added in Linux 5.9. On `< 5.9.0`, `.handle_event` with 8 parameters is used, forwarding `inode`, `mask`, and `name` to `ksu_handle_inode_event`.

### SELinux Adaptation (`kernel/selinux/`)
* **Policydb Access**: Linux 5.10 refactored SELinux state to encapsulate policy inside `selinux_state.policy` (`struct selinux_policy`). On Linux 5.4, policy resides in `selinux_state.ss->policydb` (`struct selinux_ss`). `get_policydb()` accesses `selinux_state.ss->policydb` under RCU.
* **Policy Duplication & Application**: `ksu_dup_sepolicy` and `ksu_destroy_sepolicy` are guarded for `>= 5.10.0`. On Linux 5.4, rules are applied in-place to `selinux_state.ss->policydb`, followed by `reset_avc_cache(0)`.
* **Filename Transitions**: On Linux `< 5.7.0`, `hashtab_insert` takes 3 arguments (`db->filename_trans, new_key, trans`), and `struct filename_trans` is used as the key. Restored compatibility logic from KernelSU's pre-1.0 commits.

---

## 3. `CONFIG_KSU_DISABLE_SELINUX_HIDE`

* Added `CONFIG_KSU_DISABLE_SELINUX_HIDE` (default `y`) to allow building without `feature/selinux_hide.o`.
* **What this means & trade-offs**:
  * **What works**: Root access (`su`), KernelSU Manager APK pairing, module mounting/unmounting, supercalls, allowlist, app profile, and ADB root are fully functional.
  * **What is affected**: Detection evasion. When `selinux_hide` is disabled, root-detection / banking apps that explicitly read `/sys/fs/selinux/policy` and inspect AVC rules can detect custom KernelSU rules (such as the `su` domain). Normal SELinux enforcement for apps remains active and working.

---

## 4. Integration Instructions

To integrate into a Linux 5.4 ARM64 kernel tree:

1. Add this repository as a submodule or copy into the kernel tree:
   ```bash
   git submodule add <repo-url> KernelSU
   ln -s ../KernelSU/kernel drivers/kernelsu
   ```
2. In `drivers/Kconfig`, add:
   ```kconfig
   source "drivers/kernelsu/Kconfig"
   ```
3. In `drivers/Makefile`, add:
   ```makefile
   obj-$(CONFIG_KSU) += kernelsu/
   ```
4. In your defconfig:
   ```kconfig
   CONFIG_KSU=y
   CONFIG_KSU_DISABLE_SELINUX_HIDE=y
   ```
