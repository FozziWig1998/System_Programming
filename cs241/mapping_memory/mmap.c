/**
 * Mapping Memory Lab
 * CS 241 - Fall 2017
 */
#include "mmap.h"
#include "mmu.h"
#include "page_table.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


mmap *mmap_create(char *path_to_file) {
  // TODO implement me!
  
  	mmap* ret = malloc(sizeof(mmap));
  	ret->mmu = mmu_create();
  	ret->stream = fopen(path_to_file, "rw");
	struct stat st;
	stat(path_to_file, &st);
	ret->length = st.st_size;
	
	  
  	return ret;
}

size_t mmap_read(mmap *this, void *buffer, size_t offset,
                 size_t bytes_to_read) {
  // TODO implement me!
  uintptr_t v_addr = mmap->start_address;
  mmu* this_mmu = mmap->mmu;
  
  return 0;
}

size_t mmap_write(mmap *this, void *buffer, size_t offset,
                  size_t bytes_to_write) {
  // TODO implement me!
  return 0;
}

void munmap(mmap *this) {
  // TODO implement me!
}
