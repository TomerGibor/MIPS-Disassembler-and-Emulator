#include "instruction_handlers.h"

#include <stdio.h>

#include "memory.h"
#include "syscalls.h"

error_t handle_r_functs(instruction_t instruction) {
	if (!R_functs[instruction.instruction.r_instruction.funct].handler) {
		printf("Invalid funct!\n");
		return ERROR_ILLEGAL_INSTRUCTION;
	}
	return R_functs[instruction.instruction.r_instruction.funct].handler(instruction);
}

error_t handle_bgez_specials(instruction_t instruction) {
	int rs = cpu.registers[instruction.instruction.i_instruction.rs];
	int condition = 0;

	switch (instruction.instruction.i_instruction.rt) {
	case 0x0:
		//bltz - if(rs<0) pc+=offset*4
		condition = rs < 0;
		break;
	case 0x1:
		//bgez - if(rs<=0) pc+=offset*4
		condition = rs >= 0;
		break;
	case 0x10:
		//bltzal - r31=pc; if(rs<0) pc+=offset*4
		cpu.registers[31] = cpu.pc;
		condition = rs < 0;
		break;
	case 0x11:
		//bgezal - r31=pc; if(rs>=0) pc+=offset*4
		cpu.registers[31] = cpu.pc;
		condition = rs >= 0;
		break;
	default:
		return ERROR_ILLEGAL_INSTRUCTION;

	}
	if (condition)
		cpu.pc += instruction.instruction.i_instruction.IMM * 4;
	return ERROR_OK;
}

/* pc=pc_upper|(target<<2) */
error_t handle_j(instruction_t instruction) {
	cpu.pc = ((cpu.pc >> 28) << 28) | (instruction.instruction.j_instruction.IMM << 2);
	return ERROR_OK;
}

/* r31=pc; pc=target<<2 */
error_t handle_jal(instruction_t instruction) {
	cpu.registers[31] = cpu.pc;
	cpu.pc = instruction.instruction.j_instruction.IMM << 2;
	return ERROR_OK;
}

/* if (rs==rt) pc+=offset*4 */
error_t handle_beq(instruction_t instruction) {
	int rt = cpu.registers[instruction.instruction.i_instruction.rt];
	int rs = cpu.registers[instruction.instruction.i_instruction.rs];
	if (rs == rt)
		cpu.pc += instruction.instruction.i_instruction.IMM * 4;
	return ERROR_OK;
}

/* if(rs!=rt) pc+=offset*4 */
error_t handle_bne(instruction_t instruction) {
	int rt = cpu.registers[instruction.instruction.i_instruction.rt];
	int rs = cpu.registers[instruction.instruction.i_instruction.rs];
	if (rs != rt)
		cpu.pc += instruction.instruction.i_instruction.IMM * 4;
	return ERROR_OK;
}

/* if(rs<=0) pc+=offset*4 */
error_t handle_blez(instruction_t instruction) {
	int rs = cpu.registers[instruction.instruction.i_instruction.rs];
	if (rs <= 0)
		cpu.pc += instruction.instruction.i_instruction.IMM * 4;
	return ERROR_OK;
}

/* if(rs>0) pc+=offset*4 */
error_t handle_bgtz(instruction_t instruction) {
	int rs = cpu.registers[instruction.instruction.i_instruction.rs];
	if (rs > 0)
		cpu.pc += instruction.instruction.i_instruction.IMM * 4;
	return ERROR_OK;
}

/* rt=rs+imm */
error_t handle_addi(instruction_t instruction) {
	int* rt = &cpu.registers[instruction.instruction.i_instruction.rt];
	int rs = cpu.registers[instruction.instruction.i_instruction.rs];
	*rt = rs + instruction.instruction.i_instruction.IMM;
	return ERROR_OK;
}

/* rt=rs+imm */
error_t handle_addiu(instruction_t instruction) {
	int* rt = &cpu.registers[instruction.instruction.i_instruction.rt];
	int rs = cpu.registers[instruction.instruction.i_instruction.rs];
	*rt = (uint)rs + (uint)instruction.instruction.i_instruction.IMM;
	return ERROR_OK;
}

/* rt=rs<imm */
error_t handle_slti(instruction_t instruction) {
	int* rt = &cpu.registers[instruction.instruction.i_instruction.rt];
	int rs = cpu.registers[instruction.instruction.i_instruction.rs];
	*rt = rs < instruction.instruction.i_instruction.IMM;
	return ERROR_OK;
}

/* rt=rs<imm */
error_t handle_sltiu(instruction_t instruction) {
	int* rt = &cpu.registers[instruction.instruction.i_instruction.rt];
	int rs = cpu.registers[instruction.instruction.i_instruction.rs];
	*rt = (uint)rs < (uint)instruction.instruction.i_instruction.IMM;
	return ERROR_OK;
}

/* rt=rs&imm */
error_t handle_andi(instruction_t instruction) {
	int* rt = &cpu.registers[instruction.instruction.i_instruction.rt];
	int rs = cpu.registers[instruction.instruction.i_instruction.rs];
	*rt = rs & instruction.instruction.i_instruction.IMM;
	return ERROR_OK;
}

/* rt=rs|imm */
error_t handle_ori(instruction_t instruction) {
	int* rt = &cpu.registers[instruction.instruction.i_instruction.rt];
	int rs = cpu.registers[instruction.instruction.i_instruction.rs];
	*rt = rs | instruction.instruction.i_instruction.IMM;
	return ERROR_OK;
}

/* rt=rs^imm */
error_t handle_xori(instruction_t instruction) {
	int* rt = &cpu.registers[instruction.instruction.i_instruction.rt];
	int rs = cpu.registers[instruction.instruction.i_instruction.rs];
	*rt = rs ^ instruction.instruction.i_instruction.IMM;
	return ERROR_OK;
}

/* rt=imm<<16 */
error_t handle_lui(instruction_t instruction) {
	int* rt = &cpu.registers[instruction.instruction.i_instruction.rt];
	*rt = instruction.instruction.i_instruction.IMM << 16;
	return ERROR_OK;
}

error_t handle_mfc0_mft0(instruction_t instruction) {
	//TODO
	return ERROR_OK;
}

/* rt = *(char*)(offset + rs) */
error_t handle_lb(instruction_t instruction) {
	error_t err = ERROR_OK;
	int* rt = &cpu.registers[instruction.instruction.i_instruction.rt];
	uint address = (uint)instruction.instruction.i_instruction.IMM + (uint)cpu.registers[instruction.instruction.
		i_instruction.rs];
	uint loaded = 0;
	err = load_from_memory(address, &loaded);
	if (!IS_SUCCESS(err))
		return err;
	*rt = (char)loaded;
	return ERROR_OK;
}

/* rt=*(short*)(offset+rs) */
error_t handle_lh(instruction_t instruction) {
	error_t err = ERROR_OK;
	int* rt = &cpu.registers[instruction.instruction.i_instruction.rt];
	uint address = (uint)instruction.instruction.i_instruction.IMM + (uint)cpu.registers[instruction.instruction.
		i_instruction.rs];
	uint loaded = 0;
	err = load_from_memory(address, &loaded);
	if (!IS_SUCCESS(err))
		return err;
	*rt = (short)loaded;
	return ERROR_OK;
}

/* rt=*(int*)(offset+rs) */
error_t handle_lw(instruction_t instruction) {
	error_t err = ERROR_OK;
	int* rt = &cpu.registers[instruction.instruction.i_instruction.rt];
	uint address = (uint)instruction.instruction.i_instruction.IMM + (uint)cpu.registers[instruction.instruction.
		i_instruction.rs];
	uint loaded = 0;
	err = load_from_memory(address, &loaded);
	if (!IS_SUCCESS(err))
		return err;
	*rt = (int)loaded;
	return ERROR_OK;
}

/* rt=*(Uchar*)(offset+rs) */
error_t handle_lbu(instruction_t instruction) {
	error_t err = ERROR_OK;
	int* rt = &cpu.registers[instruction.instruction.i_instruction.rt];
	uint address = (uint)instruction.instruction.i_instruction.IMM + (uint)cpu.registers[instruction.instruction.
		i_instruction.rs];
	uint loaded = 0;
	err = load_from_memory(address, &loaded);
	if (!IS_SUCCESS(err))
		return err;
	*rt = (uchar)loaded;
	return ERROR_OK;
}

/* rt=*(Ushort*)(offset+rs) */
error_t handle_lhu(instruction_t instruction) {
	error_t err = ERROR_OK;
	int* rt = &cpu.registers[instruction.instruction.i_instruction.rt];
	uint address = (uint)instruction.instruction.i_instruction.IMM + (uint)cpu.registers[instruction.instruction.
		i_instruction.rs];
	uint loaded = 0;
	err = load_from_memory(address, &loaded);
	if (!IS_SUCCESS(err))
		return err;
	*rt = (ushort)loaded;
	return ERROR_OK;
}

/* *(char*)(offset+rs)=rt */
error_t handle_sb(instruction_t instruction) {
	error_t err = ERROR_OK;
	char rt = (char)cpu.registers[instruction.instruction.i_instruction.rt];
	uint address = (uint)instruction.instruction.i_instruction.IMM + (uint)cpu.registers[instruction.instruction.
		i_instruction.rs];
	err = store_in_memory(address, rt);
	return err;
}

/* *(short*)(offset+rs)=rt */
error_t handle_sh(instruction_t instruction) {
	error_t err = ERROR_OK;
	short rt = (short)cpu.registers[instruction.instruction.i_instruction.rt];
	uint address = (uint)instruction.instruction.i_instruction.IMM + (uint)cpu.registers[instruction.instruction.
		i_instruction.rs];
	err = store_in_memory(address, rt);
	return err;
}

/* *(int*)(offset+rs)=rt */
error_t handle_sw(instruction_t instruction) {
	error_t err = ERROR_OK;
	int rt = cpu.registers[instruction.instruction.i_instruction.rt];
	uint address = (uint)instruction.instruction.i_instruction.IMM + (uint)cpu.registers[instruction.instruction.
		i_instruction.rs];
	err = store_in_memory(address, rt);
	return err;
}

/* rd=rt<<sa */
error_t handle_sll(instruction_t instruction) {
	int* rd = &cpu.registers[instruction.instruction.r_instruction.rd];
	int rt = cpu.registers[instruction.instruction.r_instruction.rt];
	*rd = rt << instruction.instruction.r_instruction.shift;
	return ERROR_OK;
}

/* rd=rt>>sa (logical) */
error_t handle_srl(instruction_t instruction) {
	int* rd = &cpu.registers[instruction.instruction.r_instruction.rd];
	int rt = cpu.registers[instruction.instruction.r_instruction.rt];
	*rd = (uint)rt >> instruction.instruction.r_instruction.shift;
	return ERROR_OK;
}

/* rd=rt>>sa (arithmetic)*/
error_t handle_sra(instruction_t instruction) {
	int i = 0;
	int* rd = &cpu.registers[instruction.instruction.r_instruction.rd];
	int rt = cpu.registers[instruction.instruction.r_instruction.rt];

	if (rt > 0)
		*rd = (uint)rt >> instruction.instruction.r_instruction.shift;
	else {
		for (; i < instruction.instruction.r_instruction.shift; i++) {
			*rd /= 2;
			*rd -= 1;
		}
	}
	return ERROR_OK;
}

/* rd=rt<<rs (logical) */
error_t handle_sllv(instruction_t instruction) {
	int* rd = &cpu.registers[instruction.instruction.r_instruction.rd];
	int rt = cpu.registers[instruction.instruction.r_instruction.rt];
	int rs = cpu.registers[instruction.instruction.r_instruction.rs];
	*rd = rt << rs;
	return ERROR_OK;
}

/* rd=rt>>rs */
error_t handle_srlv(instruction_t instruction) {
	int* rd = &cpu.registers[instruction.instruction.r_instruction.rd];
	int rt = cpu.registers[instruction.instruction.r_instruction.rt];
	int rs = cpu.registers[instruction.instruction.r_instruction.rs];
	*rd = (uint)rt >> rs;
	return ERROR_OK;
}

/* rd=rt>>rs (arithmetic) */
error_t handle_srav(instruction_t instruction) {
	int i = 0;
	int* rd = &cpu.registers[instruction.instruction.r_instruction.rd];
	int rt = cpu.registers[instruction.instruction.r_instruction.rt];
	int rs = cpu.registers[instruction.instruction.r_instruction.rs];

	if (rt > 0)
		*rd = (uint)rt >> rs;
	else {
		for (; i < rs; i++) {
			*rd /= 2;
			*rd -= 1;
		}
	}
	return ERROR_OK;
}

/* pc=rs */
error_t handle_jr(instruction_t instruction) {
	cpu.pc = cpu.registers[instruction.instruction.r_instruction.rs];
	return ERROR_OK;
}

/* rd = pc; pc = rs */
error_t handle_jalr(instruction_t instruction) {
	cpu.registers[instruction.instruction.r_instruction.rd] = cpu.pc;
	cpu.pc = cpu.registers[instruction.instruction.r_instruction.rs];
	return ERROR_OK;
}

error_t handle_syscall(instruction_t instruction) {
	uint syscall = cpu.registers[2]; //$v0
	if (syscall >= NUM_SYSCALLS || !syscall_handlers[syscall]) {
		printf("Illegal syscall: %u\n", syscall);
		return ERROR_ILLEGAL_SYSCALL;
	}
	return syscall_handlers[syscall]();
}

/* rd=HI */
error_t handle_mfhi(instruction_t instruction) {
	cpu.registers[instruction.instruction.r_instruction.rd] = cpu.HI;
	return ERROR_OK;
}

/* HI=rs */
error_t handle_mthi(instruction_t instruction) {
	cpu.HI = cpu.registers[instruction.instruction.r_instruction.rs];
	return ERROR_OK;
}

/* rd=LO */
error_t handle_mflo(instruction_t instruction) {
	cpu.registers[instruction.instruction.r_instruction.rd] = cpu.LO;
	return ERROR_OK;
}

/* LO=rs */
error_t handle_mtlo(instruction_t instruction) {
	cpu.LO = cpu.registers[instruction.instruction.r_instruction.rs];
	return ERROR_OK;
}

/* HI,LO=rs*rt */
error_t handle_mult(instruction_t instruction) {
	int rt = cpu.registers[instruction.instruction.r_instruction.rt];
	int rs = cpu.registers[instruction.instruction.r_instruction.rs];
	long long res = (long long)rs * rt;
	cpu.HI = (int)((res >> 32) & 0xffffffff);
	cpu.LO = (int)(res & 0xffffffff);
	return ERROR_OK;
}

/* HI,LO=rs*rt */
error_t handle_multu(instruction_t instruction) {
	uint rt = cpu.registers[instruction.instruction.r_instruction.rt];
	uint rs = cpu.registers[instruction.instruction.r_instruction.rs];
	ullong res = (ullong)rs * rt;
	cpu.HI = (int)((res >> 32) & 0xffffffff);
	cpu.LO = (int)(res & 0xffffffff);
	return ERROR_OK;
}

/* HI=rs%rt; LO=rs/rt */
error_t handle_div(instruction_t instruction) {
	int rt = cpu.registers[instruction.instruction.r_instruction.rt];
	int rs = cpu.registers[instruction.instruction.r_instruction.rs];
	cpu.HI = rs % rt;
	cpu.LO = rs / rt;
	return ERROR_OK;
}

/* HI=rs%rt; LO=rs/rt */
error_t handle_divu(instruction_t instruction) {
	uint rt = cpu.registers[instruction.instruction.r_instruction.rt];
	uint rs = cpu.registers[instruction.instruction.r_instruction.rs];
	cpu.HI = rs % rt;
	cpu.LO = rs / rt;
	return ERROR_OK;
}

/* rd=rs+rt */
error_t handle_add(instruction_t instruction) {
	int* rd = &cpu.registers[instruction.instruction.r_instruction.rd];
	int rt = cpu.registers[instruction.instruction.r_instruction.rt];
	int rs = cpu.registers[instruction.instruction.r_instruction.rs];
	*rd = rs + rt;
	return ERROR_OK;
}

/* rd=rs+rt */
error_t handle_addu(instruction_t instruction) {
	uint* rd = (uint*)&cpu.registers[instruction.instruction.r_instruction.rd];
	uint rt = cpu.registers[instruction.instruction.r_instruction.rt];
	uint rs = cpu.registers[instruction.instruction.r_instruction.rs];
	*rd = rs + rt;
	return ERROR_OK;
}

/* rd=rs-rt */
error_t handle_sub(instruction_t instruction) {
	int* rd = &cpu.registers[instruction.instruction.r_instruction.rd];
	int rt = cpu.registers[instruction.instruction.r_instruction.rt];
	int rs = cpu.registers[instruction.instruction.r_instruction.rs];
	*rd = rs - rt;
	return ERROR_OK;
}

/* rd=rs-rt */
error_t handle_subu(instruction_t instruction) {
	uint* rd = (uint*)&cpu.registers[instruction.instruction.r_instruction.rd];
	uint rt = cpu.registers[instruction.instruction.r_instruction.rt];
	uint rs = cpu.registers[instruction.instruction.r_instruction.rs];
	*rd = rs - rt;
	return ERROR_OK;
}

/* rd=rs&rt */
error_t handle_and(instruction_t instruction) {
	int* rd = &cpu.registers[instruction.instruction.r_instruction.rd];
	int rt = cpu.registers[instruction.instruction.r_instruction.rt];
	int rs = cpu.registers[instruction.instruction.r_instruction.rs];
	*rd = rs & rt;
	return ERROR_OK;
}

/* rd=rs|rt */
error_t handle_or(instruction_t instruction) {
	int* rd = &cpu.registers[instruction.instruction.r_instruction.rd];
	int rt = cpu.registers[instruction.instruction.r_instruction.rt];
	int rs = cpu.registers[instruction.instruction.r_instruction.rs];
	*rd = rs | rt;
	return ERROR_OK;
}

/* rd=rs^rt */
error_t handle_xor(instruction_t instruction) {
	int* rd = &cpu.registers[instruction.instruction.r_instruction.rd];
	int rt = cpu.registers[instruction.instruction.r_instruction.rt];
	int rs = cpu.registers[instruction.instruction.r_instruction.rs];
	*rd = rs ^ rt;
	return ERROR_OK;
}

/* rd=~(rs|rt) */
error_t handle_nor(instruction_t instruction) {
	int* rd = &cpu.registers[instruction.instruction.r_instruction.rd];
	int rt = cpu.registers[instruction.instruction.r_instruction.rt];
	int rs = cpu.registers[instruction.instruction.r_instruction.rs];
	*rd = ~(rs | rt);
	return ERROR_OK;
}

/* rd=rs<rt */
error_t handle_slt(instruction_t instruction) {
	int* rd = &cpu.registers[instruction.instruction.r_instruction.rd];
	int rt = cpu.registers[instruction.instruction.r_instruction.rt];
	int rs = cpu.registers[instruction.instruction.r_instruction.rs];
	*rd = rs < rt;
	return ERROR_OK;
}

/* rd=rs<rt */
error_t handle_sltu(instruction_t instruction) {
	uint* rd = (uint*)&cpu.registers[instruction.instruction.r_instruction.rd];
	uint rt = cpu.registers[instruction.instruction.r_instruction.rt];
	uint rs = cpu.registers[instruction.instruction.r_instruction.rs];
	*rd = rs < rt;
	return ERROR_OK;
}
