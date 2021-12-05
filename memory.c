#include "memory.h"

#include <stdio.h>
#include <stdlib.h>

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
	memory_segment_t* segment = {0};

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
