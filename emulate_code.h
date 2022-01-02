#pragma once
#include "error.h"
#include "instructions.h"
#include "instruction_handlers.h"

error_t emulate_mips(byte* file, uint file_size, uint code_size, uint code_offset, uint load_address);
