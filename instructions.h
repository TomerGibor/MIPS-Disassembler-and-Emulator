#pragma once
#define NUM_INSTRUCTIONS (64)
#define NUM_FUNCTS (64)
#define MAX_SPECIALS (4)
#define NUM_REGISTERS (32)

#include "def.h"
#include "error.h"


typedef struct R_instruction_s {
	uint funct : 6;
	uint shift : 5;
	uint rd : 5;
	uint rt : 5;
	uint rs : 5;
	uint opcode : 6;
} R_instruction_t;

typedef struct I_instruction_s {
	int IMM : 16;
	uint rt : 5;
	uint rs : 5;
	uint opcode : 6;
} I_instruction_t;

typedef struct J_instruction_s {
	uint IMM : 26;
	uint opcode : 6;
} J_instruction_t;

typedef enum instruction_type_e {
	ILLEGAL_INSTRUCTION,
	R_INSTRUCTION,
	I_INSTRUCTION,
	J_INSTRUCTION
} instruction_type_t;

typedef enum instruction_part_e {
	INSTRUCTION_PART_NO_SPECIAL,
	INSTRUCTION_PART_RS,
	INSTRUCTION_PART_RT,
} instruction_part_t;

typedef struct special_instruction_data_s {
	instruction_part_t instruction_part_to_compare;

	struct {
		byte value;
		char* name;
	} instructions[MAX_SPECIALS];
} special_instruction_data_t;

typedef struct instruction_s {
	instruction_type_t type;

	union {
		R_instruction_t r_instruction;
		I_instruction_t i_instruction;
		J_instruction_t j_instruction;
	} instruction;
} instruction_t;

typedef struct instruction_entry_s {
	instruction_type_t type;
	char* name;
	error_t (*handler)(instruction_t instruction);
	special_instruction_data_t special;
} instruction_entry_t;


typedef struct R_funct_entry_s {
	char* name;
	byte is_shift;
	error_t (*handler)(instruction_t instruction);
} R_funct_entry_t;


extern instruction_entry_t instructions[NUM_INSTRUCTIONS];
extern R_funct_entry_t R_functs[NUM_FUNCTS];
extern char* register_names[NUM_REGISTERS];
