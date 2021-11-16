#include "disass.h"

#include <stdio.h>
#include <string.h>
char* registers_name[] = {
	"zero",
	"at",
	"v0",
	"v1",
	"a0",
	"a1",
	"a2",
	"a3",
	"t0",
	"t1",
	"t2",
	"t3",
	"t4",
	"t5",
	"t6",
	"t7",
	"s0",
	"s1",
	"s2",
	"s3",
	"s4",
	"s5",
	"s6",
	"s7",
	"t8",
	"t9",
	"k0",
	"k1",
	"gp",
	"sp",
	"fp",
	"ra"
};

static error_t print_R_instruction(UINT instruction, const char* instruction_name)
{
	int i = 0;
	R_instruction_t r_instruction = { instruction >> 26, (instruction & 0x1f0000) >> 16, (instruction & 0xf800) >> 11, (instruction & 0x7c0)>>6, (instruction & 0x3f)};
	char* name = instruction_name;
	//memcpy(&r_instruction, &instruction, INSTRUCTION_LEN_BYTES);
	if (r_instruction.opcode == 0) //R functs
	{
		for (; i < sizeof(R_functs) / sizeof(R_funct_entry_t); i++)
		{
			if (R_functs[i].funct == r_instruction.funct)
			{
				name = R_functs[i].name;
				break;
			}
		}
	}
	if (!name)
		return ERROR_ILLEGAL_INSTRUCTION;
	printf("%s $%s, $%s, $%s\n", name, registers_name[r_instruction.rs], registers_name[r_instruction.rt], registers_name[r_instruction.rd]);
	return ERROR_OK;
}

static error_t print_I_instruction(UINT instruction, const char* instruction_name)
{
	if (!instruction_name)
		return ERROR_ILLEGAL_INSTRUCTION;
	I_instruction_t i_instruction = { instruction >> 26, (instruction & 0x1f0000) >> 16, (instruction & 0xf800) >> 11, (instruction & 0xffff) };
	//memcpy(&i_instruction, &instruction, INSTRUCTION_LEN_BYTES);
	printf("%s $%s, $%s, %d\n", instruction_name, registers_name[i_instruction.rs], registers_name[i_instruction.rt], i_instruction.IMM);
	return ERROR_OK;
}

static error_t print_J_instruction(UINT instruction, const char* instruction_name)
{
	if (!instruction_name)
		return ERROR_ILLEGAL_INSTRUCTION;
	J_instruction_t j_instruction = { instruction >> 26, (instruction & 0x3ffffff) };
	//memcpy(&j_instruction, &instruction, INSTRUCTION_LEN_BYTES);
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
		opcode = instruction >>26 ;
		err = ERROR_ILLEGAL_INSTRUCTION;
		printf("%#llx:  ", code_offset + i * INSTRUCTION_LEN_BYTES);

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
