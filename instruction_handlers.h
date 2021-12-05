#pragma once
#include "error.h"
#include "instructions.h"
#include "cpu.h"

error_t handle_r_functs(instruction_t instruction);
error_t handle_bgez_specials(instruction_t instruction);
error_t handle_j(instruction_t instruction);
error_t handle_jal(instruction_t instruction);
error_t handle_beq(instruction_t instruction);
error_t handle_bne(instruction_t instruction);
error_t handle_blez(instruction_t instruction);
error_t handle_bgtz(instruction_t instruction);
error_t handle_addi(instruction_t instruction);
error_t handle_addiu(instruction_t instruction);
error_t handle_slti(instruction_t instruction);
error_t handle_sltiu(instruction_t instruction);
error_t handle_andi(instruction_t instruction);
error_t handle_ori(instruction_t instruction);
error_t handle_xori(instruction_t instruction);
error_t handle_lui(instruction_t instruction);
error_t handle_mfc0_mft0(instruction_t instruction);
error_t handle_lb(instruction_t instruction);
error_t handle_lh(instruction_t instruction);
error_t handle_lw(instruction_t instruction);
error_t handle_lbu(instruction_t instruction);
error_t handle_lhu(instruction_t instruction);
error_t handle_sb(instruction_t instruction);
error_t handle_sh(instruction_t instruction);
error_t handle_sw(instruction_t instruction);

error_t handle_sll(instruction_t instruction);
error_t handle_srl(instruction_t instruction);
error_t handle_sra(instruction_t instruction);
error_t handle_sllv(instruction_t instruction);
error_t handle_srlv(instruction_t instruction);
error_t handle_srav(instruction_t instruction);
error_t handle_jr(instruction_t instruction);
error_t handle_jalr(instruction_t instruction);
error_t handle_syscall(instruction_t instruction);
error_t handle_mfhi(instruction_t instruction);
error_t handle_mthi(instruction_t instruction);
error_t handle_mflo(instruction_t instruction);
error_t handle_mtlo(instruction_t instruction);
error_t handle_mult(instruction_t instruction);
error_t handle_multu(instruction_t instruction);
error_t handle_div(instruction_t instruction);
error_t handle_divu(instruction_t instruction);
error_t handle_add(instruction_t instruction);
error_t handle_addu(instruction_t instruction);
error_t handle_sub(instruction_t instruction);
error_t handle_subu(instruction_t instruction);
error_t handle_and(instruction_t instruction);
error_t handle_or(instruction_t instruction);
error_t handle_xor(instruction_t instruction);
error_t handle_nor(instruction_t instruction);
error_t handle_slt(instruction_t instruction);
error_t handle_sltu(instruction_t instruction);
