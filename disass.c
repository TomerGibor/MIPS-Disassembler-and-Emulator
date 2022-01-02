#include "disass.h"

#include <stdio.h>
#include <string.h>

static byte value_from_instruction_part(instruction_part_t instruction_part, uint instruction) {
	R_instruction_t r_instruction = {0};
	memcpy(&r_instruction, &instruction, INSTRUCTION_LEN_BYTES);
	switch (instruction_part) {
	case INSTRUCTION_PART_RT: return r_instruction.rt;
	case INSTRUCTION_PART_RS: return r_instruction.rs;
	default: return 0xff;
	}
}

static error_t print_R_instruction(uint instruction, const char* instruction_name) {
	char* name = instruction_name;
	R_instruction_t r_instruction = {0};
	byte shift = 0;
	memcpy(&r_instruction, &instruction, INSTRUCTION_LEN_BYTES);

	if (r_instruction.opcode == 0) //R functs
	{
		name = R_functs[r_instruction.funct].name;
		shift = R_functs[r_instruction.funct].is_shift;
	}
	if (!name)
		return ERROR_ILLEGAL_INSTRUCTION;
	if (shift)
		printf("%s $%s, $%s %#x\n", name, register_names[r_instruction.rs], register_names[r_instruction.rt],
		       r_instruction.shift);
	else
		printf("%s $%s, $%s, $%s\n", name, register_names[r_instruction.rs], register_names[r_instruction.rt],
		       register_names[r_instruction.rd]);
	return ERROR_OK;
}

static error_t print_I_instruction(uint instruction, const char* instruction_name) {
	if (!instruction_name)
		return ERROR_ILLEGAL_INSTRUCTION;
	I_instruction_t i_instruction = {0};
	memcpy(&i_instruction, &instruction, INSTRUCTION_LEN_BYTES);
	printf("%s $%s, $%s, %d\n", instruction_name, register_names[i_instruction.rs], register_names[i_instruction.rt],
	       i_instruction.IMM);
	return ERROR_OK;
}

static error_t print_J_instruction(uint instruction, const char* instruction_name) {
	if (!instruction_name)
		return ERROR_ILLEGAL_INSTRUCTION;
	J_instruction_t j_instruction = {0};
	memcpy(&j_instruction, &instruction, INSTRUCTION_LEN_BYTES);
	printf("%s %d\n", instruction_name, j_instruction.IMM);
	return ERROR_OK;
}

error_t print_disass_code(byte* code, ullong code_size, ullong code_offset) {
	error_t err = ERROR_OK;
	int i = 0, j = 0;
	uint instruction = 0;
	byte opcode = 0;
	char* instruction_name = NULL;

	for (; i < code_size / INSTRUCTION_LEN_BYTES; i++) {
		memcpy(&instruction, &code[i * INSTRUCTION_LEN_BYTES], INSTRUCTION_LEN_BYTES);
		opcode = instruction >> 26;
		printf("%#llx:  ", code_offset + i * INSTRUCTION_LEN_BYTES);
		if (!instruction) {
			printf("nop\n");
			continue;
		}

		instruction_name = instructions[opcode].name;

		if (instructions[opcode].special.instruction_part_to_compare) // handle special instruction naming
		{
			for (j = 0; j < MAX_SPECIALS; j++) {
				if (value_from_instruction_part(instructions[opcode].special.instruction_part_to_compare, instruction)
					== instructions[opcode].special.instructions[j].value) {
					instruction_name = instructions[opcode].special.instructions[j].name;
					break;
				}
			}
			if (!instruction_name) {
				printf("ILLEGAL INSTRUCTION\n");
				continue;
			}
		}
		switch (instructions[opcode].type) {
		case R_INSTRUCTION:
			err = print_R_instruction(instruction, instruction_name);
			break;
		case I_INSTRUCTION:
			err = print_I_instruction(instruction, instruction_name);
			break;
		case J_INSTRUCTION:
			err = print_J_instruction(instruction, instruction_name);
			break;
		case ILLEGAL_INSTRUCTION:
			printf("ILLEGAL INSTRUCTION\n");
			err = ERROR_OK;
			break;
		}
		if (!IS_SUCCESS(err))
			printf("ILLEGAL INSTRUCTION\n");
	}
	return ERROR_OK;
}

error_t parse_instruction(uint instruction, instruction_t* parsed_instruction) {
	error_t err = ERROR_OK;
	byte opcode = 0;

	opcode = instruction >> 26;
	parsed_instruction->type = instructions[opcode].type;

	switch (parsed_instruction->type) {
	case R_INSTRUCTION:
		memcpy(&parsed_instruction->instruction.r_instruction, &instruction, INSTRUCTION_LEN_BYTES);
		break;
	case I_INSTRUCTION:
		memcpy(&parsed_instruction->instruction.i_instruction, &instruction, INSTRUCTION_LEN_BYTES);
		break;
	case J_INSTRUCTION:
		memcpy(&parsed_instruction->instruction.j_instruction, &instruction, INSTRUCTION_LEN_BYTES);
		break;
	case ILLEGAL_INSTRUCTION:
		err = ILLEGAL_INSTRUCTION;
		break;
	}
	return err;
}
