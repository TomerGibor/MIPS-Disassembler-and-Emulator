#pragma once
#include "def.h"
#include "error.h"
#include "instructions.h"
#define INSTRUCTION_LEN_BYTES (4)


error_t print_disass_code(byte* code, ullong code_size, ullong code_offset);
error_t parse_instruction(uint instruction, instruction_t* parsed_instruction);
