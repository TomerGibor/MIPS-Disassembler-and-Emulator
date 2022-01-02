#include "memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

memory_segment_t* memory_table = NULL;

void init_memory() {
	int i = 0;

	memory_table = (memory_segment_t*)malloc(MEMORY_SEGMENTS_COUNT * sizeof(memory_segment_t));
	for (; i < MEMORY_SEGMENTS_COUNT; ++i) {
		memory_table[i].start_address = MEMORY_SEGMENT_SIZE * i;
		memory_table[i].allocated = 0;
		memory_table[i].allocated_address = NULL;
	}
}

error_t load_file_to_memory(byte* file, uint file_size, uint load_address) {
	uint i = 0;
	uint pos = 0;
	uint bytes_left = file_size;
	uint bytes_to_write = 0;
	uint start_offset = 0;

	uint start_segment = CALC_SEGMENT_NUM(load_address);
	uint end_segment = CALC_SEGMENT_NUM(load_address + file_size);
	memory_segment_t* segment = {0};
	if (!memory_table) {
		printf("You have to call init_memory before starting to use other memory related functions!\n");
		return ERROR_UNINIT_MEMORY;
	}

	for (i = start_segment; i <= end_segment; i++) {
		segment = &memory_table[i];
		segment->allocated_address = (uint*)malloc(MEMORY_SEGMENT_SIZE);
		segment->allocated = 1;
		start_offset = ((i == start_segment) ? load_address - segment->start_address : 0);
		bytes_to_write = ((MEMORY_SEGMENT_SIZE - start_offset > bytes_left)
			                  ? bytes_left
			                  : MEMORY_SEGMENT_SIZE - start_offset);
		memcpy(segment->allocated_address + start_offset, file + pos, bytes_to_write);
		pos += bytes_to_write;
		bytes_left -= bytes_to_write;
	}
	return ERROR_OK;
}

error_t load_from_memory(uint address, uint* out) {
	memory_segment_t segment = {0};

	if (!memory_table) {
		printf("You have to call init_memory before starting to use other memory related functions!\n");
		return ERROR_UNINIT_MEMORY;
	}
	if (address % 4 != 0) {
		printf("Address %u is not aligned properly!\n", address);
		return ERROR_ADDRESS_NOT_ALIGNED;
	}

	segment = memory_table[CALC_SEGMENT_NUM(address)];
	if (!segment.allocated) {
		printf("Unfortunately this segment is not allocated yet! Store a value in there before loading it.\n");
		return ERROR_UNALLOCATED_MEMORY;
	}
	*out = (uint)segment.allocated_address[address - segment.start_address];
	return ERROR_OK;
}

error_t store_in_memory(uint address, uint value) {
	memory_segment_t* segment = NULL;

	if (!memory_table) {
		printf("You have to call init_memory before starting to use other memory related functions!\n");
		return ERROR_UNINIT_MEMORY;
	}
	if (address % 4 != 0) {
		printf("Address %u is not aligned properly!\n", address);
		return ERROR_ADDRESS_NOT_ALIGNED;
	}

	segment = &memory_table[CALC_SEGMENT_NUM(address)];
	if (!segment->allocated) {
		segment->allocated_address = (uint*)malloc(MEMORY_SEGMENT_SIZE);
		segment->allocated = 1;
	}
	segment->allocated_address[address - segment->start_address] = value;
	return ERROR_OK;
}

void free_memory() {
	int i = 0;

	if (!memory_table)
		return;

	for (; i < MEMORY_SEGMENTS_COUNT; ++i) {
		if (memory_table[i].allocated)
			free(memory_table[i].allocated_address);
	}
}
