/**
 * Ideal Indirection Lab
 * CS 241 - Fall 2017
 */
#include "mmu.h"
#include <assert.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

mmu *mmu_create() {
  mmu *my_mmu = calloc(1, sizeof(mmu));
  my_mmu->tlb = tlb_create();
  return my_mmu;
}


void mmu_read_from_virtual_address(mmu *this, uintptr_t virtual_address,
                                   size_t pid, void *buffer, size_t num_bytes) {
    assert(this);
    assert(pid < MAX_PROCESS_ID);
    assert(num_bytes + (virtual_address % PAGE_SIZE) <= PAGE_SIZE);
	
	if (virtual_address == 0) {
  		mmu_raise_segmentation_fault(this);
  		return;
  	}
	
	
	if (this->curr_pid != pid) {
		this->curr_pid = pid;
		tlb_flush(&this->tlb);
	}
	
	
	/*if (address_in_segmentations(this->segmentations[pid], virtual_address) == 0) {
  		mmu_raise_segmentation_fault(this);
  		return;
  	}*/
	
	page_table_entry* pte = tlb_get_pte(&this->tlb, virtual_address >> 12);
	if (!pte) {
		//cache not found
		mmu_tlb_miss(this);
		uint32_t dir_off = virtual_address >> 22;
		page_directory_entry* this_dir_entry = &((this->page_directories[pid]->entries)[dir_off]);
		// need ask
		if (this_dir_entry->present == 0) {
			mmu_raise_page_fault(this);
			//void *ask_kernel_for_frame(page_table_entry *entry);
			void* ask_frame = ask_kernel_for_frame((page_table_entry*)this_dir_entry);
			this_dir_entry->base_addr = ((uint32_t)ask_frame) >> 12; 
			read_page_from_disk((page_table_entry*)this_dir_entry);
			this_dir_entry->present = 1;
		}
		page_table* pt = (page_table*)(uintptr_t)(this_dir_entry->base_addr << 12);
		uint32_t pt_off = (virtual_address >> 12) & 0x3FF;
		pte = &pt->entries[pt_off];
		tlb_add_pte(&this->tlb,virtual_address >> 12, pte);
	}
	
	// check pte present
	if (pte->present == 0) {
		// tan(pi/2), ask_frame
		mmu_raise_page_fault(this);
		void* ask_frame = ask_kernel_for_frame(pte);
		pte->base_addr = ((uint32_t)ask_frame) >> 12; 
		read_page_from_disk(pte);
		pte->present = 1;
	}
	
	// check segfault
	uint32_t phys_addr_base = (pte->base_addr) << 12;

	int segf_flag = (address_in_segmentations(this->segmentations[pid], virtual_address) == 0);
	if (segf_flag) {
		//printf("read seg\n");
		mmu_raise_segmentation_fault(this);
		return;
	}
	pte->accessed = 1;
	
	uint32_t real_addr = phys_addr_base + (virtual_address & 0xFFF);
	
	memcpy(buffer, (void*)(uintptr_t)real_addr, num_bytes);
}

void mmu_write_to_virtual_address(mmu *this, uintptr_t virtual_address,
                                  size_t pid, const void *buffer,
                                  size_t num_bytes) {
    assert(this);
    assert(pid < MAX_PROCESS_ID);
    assert(num_bytes + (virtual_address % PAGE_SIZE) <= PAGE_SIZE);
  	
  	if (virtual_address == 0) {
  		mmu_raise_segmentation_fault(this);
  		return;
  	}
  	
  	
  	
  	if (this->curr_pid != pid) {
		this->curr_pid = pid;
		tlb_flush(&this->tlb);
	}
  	
  	/*if (address_in_segmentations(this->segmentations[pid], virtual_address) == 0) {
  		mmu_raise_segmentation_fault(this);
  		return;
  	}*/
  
  	page_table_entry* pte = tlb_get_pte(&this->tlb, virtual_address >> 12);
	if (!pte) {
		//cache not found
		mmu_tlb_miss(this);
		uint32_t dir_off = virtual_address >> 22;
		page_directory_entry* this_dir_entry = &((this->page_directories[pid]->entries)[dir_off]);
		// need ask
		if (this_dir_entry->present == 0) {
			mmu_raise_page_fault(this);
			//void *ask_kernel_for_frame(page_table_entry *entry);
			void* ask_frame = ask_kernel_for_frame((page_table_entry*)this_dir_entry);
			this_dir_entry->base_addr = ((uint32_t)ask_frame) >> 12; 
			read_page_from_disk((page_table_entry*)this_dir_entry);
			this_dir_entry->present = 1;
		}
		page_table* pt = (page_table*)(uintptr_t)(this_dir_entry->base_addr << 12);
		uint32_t pt_off = (virtual_address >> 12) & 0x3FF;
		pte = &pt->entries[pt_off];
		tlb_add_pte(&this->tlb,virtual_address >> 12, pte);
	}
	// check pte present
	//printf("pte->pres = %d", pte->present);
	if (pte->present == 0) {
		// tan(pi/2), ask_frame
		mmu_raise_page_fault(this);
		void* ask_frame = ask_kernel_for_frame(pte);
		pte->base_addr = ((uint32_t)ask_frame) >> 12; 
		read_page_from_disk(pte);
		pte->present = 1;
		if (virtual_address >= 0x08052000 && virtual_address <= 0x08072000) pte->read_write = 1;
	}
	// check segfault
	uint32_t phys_addr_base = ((uint32_t)(pte->base_addr)) << 12;
	//printf("pte_base_addr = %x\n", pte->base_addr);
	//printf("phys_addr_base = %x\n", phys_addr_base);
	int segf_flag = (address_in_segmentations(this->segmentations[pid], virtual_address) == 0) + (pte->read_write == 0);
	if (segf_flag) {
		//printf("write seg\n");
		mmu_raise_segmentation_fault(this);
		return;
	}
	pte->accessed = 1;
	pte->dirty = 1;
	
	uint32_t real_addr = phys_addr_base + (virtual_address & 0x00000FFF);
	
  	memcpy((void*)(uintptr_t)real_addr, buffer, num_bytes);
  
  
}

void mmu_tlb_miss(mmu *this) { this->num_tlb_misses++; }

void mmu_raise_page_fault(mmu *this) { this->num_page_faults++; }

void mmu_raise_segmentation_fault(mmu *this) {
  this->num_segmentation_faults++;
}

void mmu_add_process(mmu *this, size_t pid) {
  assert(pid < MAX_PROCESS_ID);
  this->page_directories[pid] =
      (page_directory *)ask_kernel_for_frame((page_table_entry *)NULL);
  page_directory *pd = this->page_directories[pid];
  this->segmentations[pid] = calloc(1, sizeof(vm_segmentations));
  vm_segmentations *segmentations = this->segmentations[pid];

  // Note you can see this information in a memory map by using
  // cat /proc/self/maps
  segmentations->segments[STACK] =
      (vm_segmentation){.start = 0xBFFFE000,
                        .end = 0xC07FE000, // 8mb stack
                        .permissions = READ | WRITE,
                        .grows_down = true};

  segmentations->segments[MMAP] = (vm_segmentation){.start = 0xC07FE000,
                                                    .end = 0xC07FE000,
                                                    .permissions = READ | EXEC,
                                                    .grows_down = true};

  segmentations->segments[HEAP] = (vm_segmentation){.start = 0x08072000,
                                                    .end = 0x08072000,
                                                    .permissions = READ | WRITE,
                                                    .grows_down = false};

  segmentations->segments[BSS] = (vm_segmentation){.start = 0x0805A000,
                                                   .end = 0x08072000,
                                                   .permissions = READ | WRITE,
                                                   .grows_down = false};

  segmentations->segments[DATA] = (vm_segmentation){.start = 0x08052000,
                                                    .end = 0x0805A000,
                                                    .permissions = READ | WRITE,
                                                    .grows_down = false};

  segmentations->segments[TEXT] = (vm_segmentation){.start = 0x08048000,
                                                    .end = 0x08052000,
                                                    .permissions = READ | EXEC,
                                                    .grows_down = false};

  // creating a few mappings so we have something to play with (made up)
  // this segment is made up for testing purposes
  segmentations->segments[TESTING] =
      (vm_segmentation){.start = PAGE_SIZE,
                        .end = 3 * PAGE_SIZE,
                        .permissions = READ | WRITE,
                        .grows_down = false};
  // first 4 mb is bookkept by the first page directory entry
  page_directory_entry *pde = &(pd->entries[0]);
  // assigning it a page table and some basic permissions
  pde->base_addr = ((uintptr_t)ask_kernel_for_frame((page_table_entry *)NULL) >>
                    NUM_OFFSET_BITS);
  pde->present = true;
  pde->read_write = true;
  pde->user_supervisor = true;

  // setting entries 1 and 2 (since each entry points to a 4kb page)
  // of the page table to point to our 8kb of testing memory defined earlier
  for (int i = 1; i < 3; i++) {
    page_table *pt =
        (page_table *)((uintptr_t)pde->base_addr << NUM_OFFSET_BITS);
    page_table_entry *pte = &(pt->entries[i]);
    pte->base_addr = ((uintptr_t)ask_kernel_for_frame(pte) >> NUM_OFFSET_BITS);
    pte->present = true;
    pte->read_write = true;
    pte->user_supervisor = true;
  }
}

void mmu_remove_process(mmu *this, size_t pid) {
  assert(pid < MAX_PROCESS_ID);
  // example of how to BFS through page table tree for those to read code.
  page_directory *pd = this->page_directories[pid];
  if (pd) {
    for (size_t vpn1 = 0; vpn1 < NUM_ENTRIES; vpn1++) {
      page_directory_entry *pde = &(pd->entries[vpn1]);
      if (pde->present) {
        page_table *pt =
            (page_table *)((uintptr_t)(pde->base_addr << NUM_OFFSET_BITS));
        for (size_t vpn2 = 0; vpn2 < NUM_ENTRIES; vpn2++) {
          page_table_entry *pte = &(pt->entries[vpn2]);
          if (pte->present) {
            void *frame =
                (void *)((uintptr_t)(pte->base_addr << NUM_OFFSET_BITS));
            return_frame_to_kernel(frame);
          }
          remove_swap_file(pte);
        }
        return_frame_to_kernel(pt);
      }
    }
    return_frame_to_kernel(pd);
  }

  this->page_directories[pid] = NULL;
  free(this->segmentations[pid]);
  this->segmentations[pid] = NULL;

  if (this->curr_pid == pid) {
    tlb_flush(&(this->tlb));
  }
}

void mmu_delete(mmu *this) {
  for (size_t pid = 0; pid < MAX_PROCESS_ID; pid++) {
    mmu_remove_process(this, pid);
  }

  tlb_delete(this->tlb);
  free(this);
  remove_swap_files();
}
