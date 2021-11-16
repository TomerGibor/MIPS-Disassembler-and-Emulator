#pragma once
#include "def.h"
#include "error.h"
#include "instructions.h"
#define INSTRUCTION_LEN_BYTES (4)


error_t disass_code(BYTE* code, ULLONG code_size, ULLONG code_offset);
