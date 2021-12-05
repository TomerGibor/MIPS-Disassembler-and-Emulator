#pragma once
#include "error.h"
#include "def.h"
#define NUM_SYSCALLS (60)

extern error_t (*syscall_handlers[])();

error_t handle_syscall_print_integer();
error_t handle_syscall_print_float();
error_t handle_syscall_print_double();
error_t handle_syscall_print_string();
error_t handle_syscall_read_integer();
error_t handle_syscall_read_float();
error_t handle_syscall_read_double();
error_t handle_syscall_read_string();
error_t handle_syscall_sbrk();
error_t handle_syscall_exit();
error_t handle_syscall_print_character();
error_t handle_syscall_read_character();
error_t handle_syscall_open_file();
error_t handle_syscall_read_from_file();
error_t handle_syscall_write_to_file();
error_t handle_syscall_close_file();
error_t handle_syscall_exit2();
error_t handle_syscall_time();
error_t handle_syscall_MIDI_out();
error_t handle_syscall_sleep();
error_t handle_syscall_MIDI_out_synchronous();
error_t handle_syscall_print_integer_in_hexadecimal();
error_t handle_syscall_print_integer_in_binary();
error_t handle_syscall_print_integer_as_unsigned();
error_t handle_syscall_set_seed();
error_t handle_syscall_random_int();
error_t handle_syscall_random_int_range();
error_t handle_syscall_random_float();
error_t handle_syscall_random_double();
error_t handle_syscall_ConfirmDialog();
error_t handle_syscall_InputDialogInt();
error_t handle_syscall_InputDialogFloat();
error_t handle_syscall_InputDialogDouble();
error_t handle_syscall_InputDialogString();
error_t handle_syscall_MessageDialog();
error_t handle_syscall_MessageDialogInt();
error_t handle_syscall_MessageDialogFloat();
error_t handle_syscall_MessageDialogDouble();
error_t handle_syscall_MessageDialogString();
