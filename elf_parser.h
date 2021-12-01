#pragma once

#define BITS32 (1)
#define BITS64 (2)
#define ELF_MAGIC ("\x7F\x45\x4c\x46")
#define MIPS (0x8)


#include "error.h"
#include "def.h"

typedef struct e_indent_s
{
	UINT EI_MAG;
	BYTE EI_CLASS;
	BYTE EI_DATA;
	BYTE EI_VERSION;
	BYTE EI_OSABI;
	BYTE EI_ABIVERSION;
	BYTE EI_PAD[7];
} e_indent_t;

typedef struct elf32_file_headers_s
{
	e_indent_t e_indent;
	USHORT e_type;
	USHORT e_machine;
	UINT e_version;
	UINT e_entry;
	UINT e_phoff;
	UINT e_shoff;
	UINT e_flags;
	USHORT e_ehsize;
	USHORT e_phentsize;
	USHORT e_phnum;
	USHORT e_shentsize;
	USHORT e_shnum;
	USHORT e_shstrndx;
} elf32_file_headers_t;

typedef struct elf64_file_headers_s
{
	e_indent_t e_indent;
	USHORT e_type;
	USHORT e_machine;
	UINT e_version;
	ULLONG e_entry;
	ULLONG e_phoff;
	ULLONG e_shoff;
	UINT e_flags;
	USHORT e_ehsize;
	USHORT e_phentsize;
	USHORT e_phnum;
	USHORT e_shentsize;
	USHORT e_shnum;
	USHORT e_shstrndx;
} elf64_file_headers_t;

typedef struct elf32_program_header_s
{
	UINT p_type;
	UINT p_offset;
	UINT p_vaddr;
	UINT p_paddr;
	UINT p_filesz;
	UINT p_memsz;
	UINT p_flags;
	UINT p_align;
} elf32_program_header_t;

typedef struct elf64_program_header_s
{
	UINT p_type;
	UINT p_flags;
	ULLONG p_offset;
	ULLONG p_vaddr;
	ULLONG p_paddr;
	ULLONG p_filesz;
	ULLONG p_memsz;
	ULLONG p_align;
} elf64_program_header_t;

typedef struct elf32_section_header_s
{
	UINT sh_name;
	UINT sh_type;
	UINT sh_flags;
	UINT sh_addr;
	UINT sh_offset;
	UINT sh_size;
	UINT sh_link;
	UINT sh_info;
	UINT sh_addralign;
	UINT sh_entsize;
} elf32_section_header_t;

typedef struct elf64_section_header_s
{
	UINT sh_name;
	UINT sh_type;
	ULLONG sh_flags;
	ULLONG sh_addr;
	ULLONG sh_offset;
	ULLONG sh_size;
	UINT sh_link;
	UINT sh_info;
	ULLONG sh_addralign;
	ULLONG sh_entsize;
} elf64_section_header_t;

typedef enum architecture_e { ARCHITECTURE_32BIT, ARCHITECTURE_64BIT } architecture_t;

typedef struct elf_headers_s
{
	architecture_t architecture;

	union
	{
		elf32_file_headers_t bits32;
		elf64_file_headers_t bits64;
	} file_headers;

	union
	{
		elf32_program_header_t* bits32;
		elf64_program_header_t* bits64;
	} program_headers_arr;

	union
	{
		elf32_section_header_t* bits32;
		elf64_section_header_t* bits64;
	} section_headers_arr;
} elf_headers_t;

error_t parse_elf_headers(BYTE* file_buf, UINT file_size, elf_headers_t* headers);
error_t find_code_section(elf_headers_t* headers, ULLONG* section_size, ULLONG* section_offset);
void free_headers(elf_headers_t* headers);
