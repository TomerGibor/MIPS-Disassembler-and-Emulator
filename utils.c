#define _CRT_SECURE_NO_WARNINGS
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

error_t read_file(const char* filepath, BYTE** out)
{
	size_t size = 0;
	FILE* fp = fopen(filepath, "rb");
	if (!fp)
		return ERROR_OPEN_FILE;
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	rewind(fp);
	*out = malloc(size);
	if (fread(*out, 1, size, fp) != size)
	{
		free(*out);
		return ERROR_OPEN_FILE;
	}
	return ERROR_OK;
}
