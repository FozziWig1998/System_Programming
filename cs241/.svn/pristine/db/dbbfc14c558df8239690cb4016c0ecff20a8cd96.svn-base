/**
* Finding Filesystems
* CS 241 - Fall 2017
*/
#include "format.h"
#include "fs.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>


void cat_less(file_system* fs, inode* file_inode, uint64_t size) {
	for (int i = 0; i < NUM_DIRECT_INODES; i++) {
		if (size <= 16 * KILOBYTE) {
			data_block begin = fs->data_root[file_inode->direct[i]];
			//fprintf(stderr, "%x\n", file_inode->direct[i] );
			for (uint64_t j = 0; j < size; j++) {
				printf("%c", begin.data[j]);
			}
			size = 0;
		} else {
			data_block begin = fs->data_root[file_inode->direct[i]];
			for (int j = 0; j < 16 * KILOBYTE; j++) {
				printf("%c", begin.data[j]);
			}
			size -= 16 * KILOBYTE;
		}
		if (size == 0) break;
	}
}

void ls_less(file_system* fs, inode* file_inode, uint64_t size) {
	for (int i = 0; i < NUM_DIRECT_INODES; i++) {
		if (size <= 16 * KILOBYTE) {
			data_block begin = fs->data_root[file_inode->direct[i]];
			/*for (uint64_t j = 0; j < size; j++) {
				printf("%c", begin.data[j]);
			}*/
			for (uint64_t j = 0; j < size; j += 256) {
				//printf("%s\n", &begin.data[j]);
				dirent to_fill;
				make_dirent_from_string(&begin.data[j], &to_fill);
				inode* this_inode = &fs->inode_root[to_fill.inode_num];
				if (is_file(this_inode)) print_file(to_fill.name);
				else print_directory(to_fill.name);
			}
			size = 0;
		} else {
			data_block begin = fs->data_root[file_inode->direct[i]];
			for (int j = 0; j < 16 * KILOBYTE; j += 256) {
				dirent to_fill;
				make_dirent_from_string(&begin.data[j], &to_fill);
				inode* this_inode = &fs->inode_root[to_fill.inode_num];
				if (is_file(this_inode)) print_file(to_fill.name);
				else print_directory(to_fill.name);
			}
			size -= 16 * KILOBYTE;
		}
		if (size == 0) break;
	}
}

void print_ls(file_system *fs, data_block* block, uint64_t size) {
	for (uint64_t j = 0; j < size; j += 256) {
		dirent to_fill;
		make_dirent_from_string(&block->data[j], &to_fill);
		inode* this_inode = &fs->inode_root[to_fill.inode_num];
		if (is_file(this_inode)) print_file(to_fill.name);
		else print_directory(to_fill.name);
	}
}

void fs_ls(file_system *fs, char *path) {
	inode* file_inode = get_inode(fs, path);
	if (!file_inode) {
		print_no_file_or_directory();
		return;
	}
	if (is_file(file_inode)) {
		print_file(path);
		return;
	}
	uint64_t size = file_inode->size;
	if (size <= NUM_DIRECT_INODES * 16 * KILOBYTE) {
		ls_less(fs, file_inode, size);
	} else {
		uint64_t diff = size - NUM_DIRECT_INODES * 16 * KILOBYTE;
		ls_less(fs, file_inode, NUM_DIRECT_INODES * 16 * KILOBYTE);
		data_block indir = fs->data_root[file_inode->indirect];
		for (int i = 0; i < 100; i += 4) {
		 //	fprintf(stderr, "%x\n", indir.data[i]);
		 	if (diff == 0) break;
		 	int this_block_num = indir.data[i];
			if (diff <= 16 * KILOBYTE) {
				print_ls(fs, &fs->data_root[this_block_num], diff); 
				diff = 0;
			} else {
				print_ls(fs, &fs->data_root[this_block_num], 16 * KILOBYTE); 
				diff -= 16 * KILOBYTE;
			}
		} 
	}
}

void print_block(uint64_t size, data_block* block) {
	for (uint64_t i = 0; i < size; i++) {
		printf("%c", block->data[i]);
	}
}

void fs_cat(file_system *fs, char *path) {
	inode* file_inode = get_inode(fs, path);
	if (!file_inode) {
		print_no_file_or_directory();
		return;
	}
	uint64_t size = file_inode->size;
	if (size <= NUM_DIRECT_INODES * 16 * KILOBYTE) {
		// no indirect
		cat_less(fs, file_inode, size);
	} else {
		//indirect
		uint64_t diff = size - NUM_DIRECT_INODES * 16 * KILOBYTE;
		cat_less(fs, file_inode, NUM_DIRECT_INODES * 16 * KILOBYTE);
		data_block indir = fs->data_root[file_inode->indirect];
		for (int i = 0; i < 100; i += 4) {
		 //	fprintf(stderr, "%x\n", indir.data[i]);
		 	if (diff == 0) break;
		 	int this_block_num = indir.data[i];
			if (diff <= 16 * KILOBYTE) {
				print_block(diff, &fs->data_root[this_block_num]);
				diff = 0;
			} else {
				print_block(16 * KILOBYTE, &fs->data_root[this_block_num]);
				diff -= 16 * KILOBYTE;
			}
		} 
	}
}
