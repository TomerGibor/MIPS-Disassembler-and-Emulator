#include "disass.h"

#include <stdio.h>
#include <string.h>


static error_t print_R_instruction(UINT instruction, const char* instruction_name)
{
	int i = 0;
	char* name = instruction_name;
	R_instruction_t r_instruction = {0};
	BYTE shift = 0;
	memcpy(&r_instruction, &instruction, INSTRUCTION_LEN_BYTES);

	if (r_instruction.opcode == 0) //R functs
	{
		for (; i < sizeof(R_functs) / sizeof(R_funct_entry_t); i++)
		{
			if (R_functs[i].funct == r_instruction.funct)
			{
				name = R_functs[i].name;
				shift = R_functs[i].is_shift;

				break;
			}
		}
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

static error_t print_I_instruction(UINT instruction, const char* instruction_name)
{
	if (!instruction_name)
		return ERROR_ILLEGAL_INSTRUCTION;
	I_instruction_t i_instruction = {0};
	memcpy(&i_instruction, &instruction, INSTRUCTION_LEN_BYTES);
	printf("%s $%s, $%s, %d\n", instruction_name, register_names[i_instruction.rs], register_names[i_instruction.rt],
	       i_instruction.IMM);
	return ERROR_OK;
}

static error_t print_J_instruction(UINT instruction, const char* instruction_name)
{
	if (!instruction_name)
		return ERROR_ILLEGAL_INSTRUCTION;
	J_instruction_t j_instruction = {0};
	memcpy(&j_instruction, &instruction, INSTRUCTION_LEN_BYTES);
	printf("%s %d\n", instruction_name, j_instruction.IMM);
	return ERROR_OK;
}

error_t disass_code(BYTE* code, ULLONG code_size, ULLONG code_offset)
{
	error_t err = ERROR_OK;
	int i = 0, j = 0;
	UINT instruction = 0;
	BYTE opcode = 0;
	for (; i < code_size / INSTRUCTION_LEN_BYTES; i++)
	{
		memcpy(&instruction, &code[i * INSTRUCTION_LEN_BYTES], INSTRUCTION_LEN_BYTES);
		opcode = instruction >> 26;
		err = ERROR_ILLEGAL_INSTRUCTION;
		printf("%#llx:  ", code_offset + i * INSTRUCTION_LEN_BYTES);
		if (!instruction)
		{
			printf("nop\n");
			continue;
		}

		for (j = 0; j < sizeof(instructions) / sizeof(instruction_entry_t); j++)
		{
			if (instructions[j].opdode == opcode)
			{
				switch (instructions[j].type)
				{
				case R_INSTRUCTION:
					err = print_R_instruction(instruction, instructions[j].name);
					break;
				case I_INSTRUCTION:
					err = print_I_instruction(instruction, instructions[j].name);
					break;
				case J_INSTRUCTION:
					err = print_J_instruction(instruction, instructions[j].name);
					break;
				}
				break;
			}
		}
		if (!IS_SUCCESS(err))
			printf("ILLEGAL INSTRUCTION\n");
	}
	return ERROR_OK;
}
