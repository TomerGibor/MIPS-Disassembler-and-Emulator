#pragma once
#define MEMORY_SEGMENTS_COUNT (16384)
#define MEMORY_SEGMENT_SIZE (262144) // 2**32 / 16384 = 256 KB
#define CALC_SEGMENT_NUM(address) ((address)/MEMORY_SEGMENT_SIZE)
#include "def.h"
#include "error.h"

typedef struct memory_segment_s {
	uint start_address;
	uint allocated;
	uint* allocated_address;
} memory_segment_t;

void init_memory();
error_t load_file_to_memory(byte* file, uint file_size, uint load_address);
error_t load_from_memory(uint address, uint* out);
error_t store_in_memory(uint address, uint value);
void free_memory();
