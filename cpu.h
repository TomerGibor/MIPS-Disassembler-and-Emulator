#pragma once
#include "def.h"
#include "instructions.h"


typedef struct cpu_s {
	int registers[NUM_REGISTERS];
	uint pc;
	int HI;
	int LO;
} cpu_t;

extern cpu_t cpu;
