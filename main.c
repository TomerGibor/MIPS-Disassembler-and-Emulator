#include <stdio.h>
#include <stdlib.h>

#include "disass.h"
#include "error.h"
#include "elf_parser.h"
#include "utils.h"


int main(int argc, char* argv[])
{
	error_t err = ERROR_OK;
	BYTE* file = NULL;
	UINT file_size = 0;
	ULLONG code_section_size = 0;
	ULLONG code_section_offset = 0;

	elf_headers_t headers = {0};

	if (argc != 2)
	{
		printf("Usage: %s <path_to_mips_ELF_file>\n", argv[0]);
		return 1;
	}
	err = read_file(argv[1], &file, &file_size);
	if (!IS_SUCCESS(err))
	{
		printf("Unable to read file %s\n", argv[1]);
		return 1;
	}
	err = parse_elf_headers(file, file_size, &headers);
	if (!IS_SUCCESS(err))
	{
		printf("Failed to parse ELF headers!\n");
		goto cleanup;
	}
	if (headers.file_headers.bits32.e_machine != MIPS)
	{
		printf("Not a MIPS executable!\n");
		free_headers(&headers);
		goto cleanup;
	}
	err = find_code_section(&headers, &code_section_size, &code_section_offset);
	free_headers(&headers);
	if (!IS_SUCCESS(err))
	{
		printf("Failed to get code section size!\n");
		goto cleanup;
	}
	if (code_section_offset + code_section_size > file_size)
	{
		printf("Invalid code section offset/size!\n");
		goto cleanup;
	}

	err = disass_code(file + code_section_offset, code_section_size, code_section_offset);
	free(file);

	if (!IS_SUCCESS(err))
	{
		printf("Failed to disassemble code!\n");
		return 1;
	}
	return 0;

cleanup:
	if (file)
		free(file);
	return 1;
}
