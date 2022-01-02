#include "emulate_code.h"

#include <stdio.h>
#include <string.h>

#include "disass.h"
#include "memory.h"


error_t emulate_mips(byte* file, uint file_size, uint code_size, uint code_offset, uint load_address) {
	error_t err = ERROR_OK;
	uint instruction = 0;
	byte opcode = 0;
	instruction_t parsed_instruction = {0};
	init_memory();
	load_file_to_memory(file, file_size, load_address);
	cpu.pc = code_offset;

	while (cpu.pc <= code_offset + code_size) {
		memcpy(&instruction, &file[cpu.pc], INSTRUCTION_LEN_BYTES);
		if (!instruction) {
			// nop
			cpu.pc += INSTRUCTION_LEN_BYTES;
			continue;
		}
		opcode = instruction >> 26;
		err = parse_instruction(instruction, &parsed_instruction);
		if (!IS_SUCCESS(err) || !instructions[opcode].handler) {
			printf("Sorry! We've stumbled upon an instruction that is not supported... cpu.pc=%u\n", cpu.pc);
			break;
		}

		cpu.pc += INSTRUCTION_LEN_BYTES;
		err = instructions[opcode].handler(parsed_instruction);
		if (!IS_SUCCESS(err)) {
			printf("An unexpected error has occurred during executing the instruction at: cpu.pc=%u. Error %u\n",
			       cpu.pc - INSTRUCTION_LEN_BYTES, err);
			break;
		}

	}
	free_memory();
}
