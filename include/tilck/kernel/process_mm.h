/* SPDX-License-Identifier: BSD-2-Clause */

#pragma once
#include <tilck/common/basic_defs.h>
#include <tilck/kernel/fs/vfs.h>
#include <tilck/kernel/paging.h>

struct user_mapping {

   list_node node;

   fs_handle h;
   size_t len;
   size_t off;

   union {
      void *vaddrp;
      uptr vaddr;
   };

   int prot;

};

typedef struct user_mapping user_mapping;

user_mapping *
process_add_user_mapping(fs_handle h, void *v, size_t ln, size_t off, int prot);
void process_remove_user_mapping(user_mapping *um);
void full_remove_user_mapping(process_info *pi, user_mapping *um);
void remove_all_mappings_of_handle(process_info *pi, fs_handle h);
user_mapping *process_get_user_mapping(void *vaddr);

/* Internal functions */
bool user_valloc_and_map(uptr user_vaddr, size_t page_count);
void user_vfree_and_unmap(uptr user_vaddr, size_t page_count);
void user_unmap_zero_page(uptr user_vaddr, size_t page_count);
bool user_map_zero_page(uptr user_vaddr, size_t page_count);

/* Special one-time funcs */
void set_kernel_process_pdir(pdir_t *pdir);
