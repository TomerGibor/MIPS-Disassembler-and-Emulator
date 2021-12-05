#define _CRT_SECURE_NO_WARNINGS
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

error_t read_file(const char* filepath, byte** out, uint* file_size) {
	FILE* fp = fopen(filepath, "rb");
	if (!fp)
		return ERROR_OPEN_FILE;
	fseek(fp, 0, SEEK_END);
	*file_size = ftell(fp);
	rewind(fp);
	*out = malloc(*file_size);
	if (fread(*out, 1, *file_size, fp) != *file_size) {
		free(*out);
		return ERROR_OPEN_FILE;
	}
	return ERROR_OK;
}
