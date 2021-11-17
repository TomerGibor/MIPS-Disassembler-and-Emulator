#pragma once
#define NUM_INSTRUCTIONS (22)
#define NUM_FUNCTS (23)

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
	J_INSTRUCTION
} instruction_type_t;

typedef struct instruction_entry_s
{
	UINT opdode;
	instruction_type_t type;
	char* name;
} instruction_entry_t;


typedef struct R_funct_entry_s
{
	UINT funct : 6;
	char* name;
	BYTE is_shift;
} R_funct_entry_t;

extern instruction_entry_t instructions[NUM_INSTRUCTIONS];
extern R_funct_entry_t R_functs[NUM_FUNCTS];
extern char* register_names[];
