#pragma once
#include "elf_parser.h"

error_t read_file(const char* filepath, byte** out, uint* file_size);
