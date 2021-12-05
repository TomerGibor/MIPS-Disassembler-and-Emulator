#pragma once

#define BITS32 (1)
#define BITS64 (2)
#define ELF_MAGIC ("\x7F\x45\x4c\x46")
#define MIPS (0x8)


#include "error.h"
#include "def.h"

typedef struct e_indent_s {
	uint EI_MAG;
	byte EI_CLASS;
	byte EI_DATA;
	byte EI_VERSION;
	byte EI_OSABI;
	byte EI_ABIVERSION;
	byte EI_PAD[7];
} e_indent_t;

typedef struct elf32_file_headers_s {
	e_indent_t e_indent;
	ushort e_type;
	ushort e_machine;
	uint e_version;
	uint e_entry;
	uint e_phoff;
	uint e_shoff;
	uint e_flags;
	ushort e_ehsize;
	ushort e_phentsize;
	ushort e_phnum;
	ushort e_shentsize;
	ushort e_shnum;
	ushort e_shstrndx;
} elf32_file_headers_t;

typedef struct elf64_file_headers_s {
	e_indent_t e_indent;
	ushort e_type;
	ushort e_machine;
	uint e_version;
	ullong e_entry;
	ullong e_phoff;
	ullong e_shoff;
	uint e_flags;
	ushort e_ehsize;
	ushort e_phentsize;
	ushort e_phnum;
	ushort e_shentsize;
	ushort e_shnum;
	ushort e_shstrndx;
} elf64_file_headers_t;

typedef struct elf32_program_header_s {
	uint p_type;
	uint p_offset;
	uint p_vaddr;
	uint p_paddr;
	uint p_filesz;
	uint p_memsz;
	uint p_flags;
	uint p_align;
} elf32_program_header_t;

typedef struct elf64_program_header_s {
	uint p_type;
	uint p_flags;
	ullong p_offset;
	ullong p_vaddr;
	ullong p_paddr;
	ullong p_filesz;
	ullong p_memsz;
	ullong p_align;
} elf64_program_header_t;

typedef struct elf32_section_header_s {
	uint sh_name;
	uint sh_type;
	uint sh_flags;
	uint sh_addr;
	uint sh_offset;
	uint sh_size;
	uint sh_link;
	uint sh_info;
	uint sh_addralign;
	uint sh_entsize;
} elf32_section_header_t;

typedef struct elf64_section_header_s {
	uint sh_name;
	uint sh_type;
	ullong sh_flags;
	ullong sh_addr;
	ullong sh_offset;
	ullong sh_size;
	uint sh_link;
	uint sh_info;
	ullong sh_addralign;
	ullong sh_entsize;
} elf64_section_header_t;

typedef enum architecture_e { ARCHITECTURE_32BIT, ARCHITECTURE_64BIT } architecture_t;

typedef struct elf_headers_s {
	architecture_t architecture;

	union {
		elf32_file_headers_t bits32;
		elf64_file_headers_t bits64;
	} file_headers;

	union {
		elf32_program_header_t* bits32;
		elf64_program_header_t* bits64;
	} program_headers_arr;

	union {
		elf32_section_header_t* bits32;
		elf64_section_header_t* bits64;
	} section_headers_arr;
} elf_headers_t;

error_t parse_elf_headers(byte* file_buf, uint file_size, elf_headers_t* headers);
error_t find_code_section(elf_headers_t* headers, ullong* section_size, ullong* section_offset);
void free_headers(elf_headers_t* headers);
