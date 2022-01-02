#include "elf_parser.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static error_t parse_elf_file_headers(byte* file_buf, elf_headers_t* headers) {
	if (memcmp(file_buf, &ELF_MAGIC, sizeof(ELF_MAGIC) - 1)) {
		printf("Not an ELF file!\n");
		return ERROR_ELF_HEADERS;
	}
	if (*(file_buf + 0x4) == BITS32) // EI_CLASS = 1 => 32 bit
	{
		memcpy(&headers->file_headers.bits32, file_buf, sizeof(elf32_file_headers_t));
		headers->architecture = ARCHITECTURE_32BIT;
	}
	else if (*(file_buf + 0x4) == BITS64) // EI_CLASS = 2 => 64 bit
	{
		memcpy(&headers->file_headers.bits64, file_buf, sizeof(elf64_file_headers_t));
		headers->architecture = ARCHITECTURE_64BIT;
	}
	else
		return ERROR_ELF_HEADERS;
	return ERROR_OK;
}

static error_t parse_elf_program_headers(byte* file_buf, uint file_size, elf_headers_t* headers) {
	if (headers->architecture == ARCHITECTURE_32BIT && headers->file_headers.bits32.e_phoff + headers->file_headers.
		bits32.e_phnum * headers->file_headers.bits32.e_phentsize <= file_size) {
		headers->program_headers_arr.bits32 = (elf32_program_header_t*)malloc(
			headers->file_headers.bits32.e_phnum * headers->file_headers.bits32.e_phentsize);
		memcpy(headers->program_headers_arr.bits32, file_buf + headers->file_headers.bits32.e_phoff,
		       headers->file_headers.bits32.e_phnum * headers->file_headers.bits32.e_phentsize);
	}
	else if (headers->architecture == ARCHITECTURE_64BIT && headers->file_headers.bits64.e_phoff + headers->file_headers
		.bits64.e_phnum * headers->file_headers.bits64.e_phentsize <= file_size) {
		headers->program_headers_arr.bits64 = (elf64_program_header_t*)malloc(
			headers->file_headers.bits64.e_phnum * headers->file_headers.bits64.e_phentsize);
		memcpy(headers->program_headers_arr.bits64, file_buf + headers->file_headers.bits64.e_phoff,
		       headers->file_headers.bits64.e_phnum * headers->file_headers.bits64.e_phentsize);
	}
	else
		return ERROR_ELF_HEADERS;
	return ERROR_OK;
}

static error_t parse_elf_section_headers(byte* file_buf, uint file_size, elf_headers_t* headers) {
	if (headers->architecture == ARCHITECTURE_32BIT && headers->file_headers.bits32.e_shoff + headers->file_headers.
		bits32.e_shnum * headers->file_headers.bits32.e_shentsize <= file_size) {
		headers->section_headers_arr.bits32 = (elf32_section_header_t*)malloc(
			headers->file_headers.bits32.e_shnum * headers->file_headers.bits32.e_shentsize);
		memcpy(headers->section_headers_arr.bits32, file_buf + headers->file_headers.bits32.e_shoff,
		       headers->file_headers.bits32.e_shnum * headers->file_headers.bits32.e_shentsize);
	}
	else if (headers->architecture == ARCHITECTURE_64BIT && headers->file_headers.bits64.e_shoff + headers->file_headers
		.bits64.e_shnum * headers->file_headers.bits64.e_shentsize <= file_size) {
		headers->section_headers_arr.bits64 = (elf64_section_header_t*)malloc(
			headers->file_headers.bits64.e_shnum * headers->file_headers.bits64.e_shentsize);
		memcpy(headers->section_headers_arr.bits64, file_buf + headers->file_headers.bits64.e_shoff,
		       headers->file_headers.bits64.e_shnum * headers->file_headers.bits64.e_shentsize);
	}
	else
		return ERROR_ELF_HEADERS;
	return ERROR_OK;
}


error_t parse_elf_headers(byte* file_buf, uint file_size, elf_headers_t* headers) {
	error_t err = ERROR_OK;
	if (!headers || file_size < sizeof(elf_headers_t))
		return ERROR_INVALID_ARGUMENT;

	err = parse_elf_file_headers(file_buf, headers);
	if (!IS_SUCCESS(err))
		return err;

	err = parse_elf_program_headers(file_buf, file_size, headers);
	if (!IS_SUCCESS(err))
		return err;

	err = parse_elf_section_headers(file_buf, file_size, headers);
	if (!IS_SUCCESS(err))
		return err;

	return ERROR_OK;
}

error_t find_code_section(elf_headers_t headers, ullong* section_size, ullong* section_offset) {
	int i = 0;

	if (headers.architecture == ARCHITECTURE_32BIT) {
		for (; i < headers.file_headers.bits32.e_shnum; i++) {
			if (headers.section_headers_arr.bits32[i].sh_addr >= headers.file_headers.bits32.e_entry && headers.
				file_headers.bits32.e_entry <= headers.section_headers_arr.bits32[i].sh_addr + headers.
				section_headers_arr.
				bits32[i].sh_size) {
				*section_size = headers.section_headers_arr.bits32[i].sh_size;
				*section_offset = headers.section_headers_arr.bits32[i].sh_offset;
				return ERROR_OK;
			}
		}
		printf("Code section not found!\n");
		return ERROR_NOT_FOUND;
	}
	if (headers.architecture == ARCHITECTURE_64BIT) {
		for (; i < headers.file_headers.bits64.e_shnum; i++) {
			if (headers.section_headers_arr.bits64[i].sh_offset >= headers.file_headers.bits64.e_entry && headers.
				file_headers.bits64.e_entry <= headers.section_headers_arr.bits64[i].sh_addr + headers.
				section_headers_arr.bits64[i].sh_size) {
				*section_size = headers.section_headers_arr.bits64[i].sh_size;
				*section_offset = headers.file_headers.bits64.e_entry;

				return ERROR_OK;
			}
		}
		printf("Code section not found!\n");
		return ERROR_NOT_FOUND;
	}
	return ERROR_ELF_HEADERS;
}

error_t get_load_address32(elf_headers_t headers, uint file_size, uint* load_at) {
	int i = 0;
	uint min_address = UINT_MAX;

	for (; i < headers.file_headers.bits32.e_phnum; i++) {
		if (headers.program_headers_arr.bits32[i].p_type == PT_LOAD && headers.program_headers_arr.bits32[i].p_vaddr <
			min_address) {
			min_address = headers.program_headers_arr.bits32[i].p_vaddr;
			*load_at = min_address;
		}
	}
	if ((ullong)min_address + file_size >= UINT_MAX)
		return ERROR_INVALID_LOAD_ADDRESS;
	return ERROR_OK;
}

void free_headers(elf_headers_t* headers) {
	if (headers) {
		free(headers->program_headers_arr.bits64);
		free(headers->section_headers_arr.bits64);
	}
}
