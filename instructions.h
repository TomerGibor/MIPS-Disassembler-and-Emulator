#pragma once
#define NUM_INSTRUCTIONS (64)
#define NUM_FUNCTS (64)
#define MAX_SPECIALS (4)


#include "def.h"


typedef struct R_instruction_s
{
	UINT funct : 6;
	UINT shift : 5;
	UINT rd : 5;
	UINT rt : 5;
	UINT rs : 5;
	UINT opcode : 6;
} R_instruction_t;

typedef struct I_instruction_s
{
	UINT IMM : 16;
	UINT rt : 5;
	UINT rs : 5;
	UINT opcode : 6;
} I_instruction_t;

typedef struct J_instruction_s
{
	UINT IMM : 26;
	UINT opcode : 6;
} J_instruction_t;

typedef enum instruction_type_e
{
	R_INSTRUCTION,
	I_INSTRUCTION,
	J_INSTRUCTION,
	ILLEGAL_INSTRUCTION
} instruction_type_t;

typedef enum instruction_part_e
{
	INSTRUCTION_PART_NO_SPECIAL,
	INSTRUCTION_PART_RS,
	INSTRUCTION_PART_RT,
} instruction_part_t;

typedef struct special_instruction_data_s
{
	instruction_part_t instruction_part_to_compare;

	struct
	{
		BYTE value;
		char* name;
	} instructions[MAX_SPECIALS];
} special_instruction_data_t;

typedef struct instruction_entry_s
{
	instruction_type_t type;
	char* name;
	special_instruction_data_t special;
} instruction_entry_t;


typedef struct R_funct_entry_s
{
	char* name;
	BYTE is_shift;
} R_funct_entry_t;


extern instruction_entry_t instructions[NUM_INSTRUCTIONS];
extern R_funct_entry_t R_functs[NUM_FUNCTS];
extern char* register_names[];
