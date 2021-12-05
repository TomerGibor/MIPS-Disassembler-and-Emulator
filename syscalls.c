#include "syscalls.h"


error_t (*syscall_handlers[])() = {
	NULL,
	handle_syscall_print_integer,
	handle_syscall_print_float,
	handle_syscall_print_double,
	handle_syscall_print_string,
	handle_syscall_read_integer,
	handle_syscall_read_float,
	handle_syscall_read_double,
	handle_syscall_read_string,
	handle_syscall_sbrk,
	handle_syscall_exit,
	handle_syscall_print_character,
	handle_syscall_read_character,
	handle_syscall_open_file,
	handle_syscall_read_from_file,
	handle_syscall_write_to_file,
	handle_syscall_close_file,
	handle_syscall_exit2,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	handle_syscall_time,
	handle_syscall_MIDI_out,
	handle_syscall_sleep,
	handle_syscall_MIDI_out_synchronous,
	handle_syscall_print_integer_in_hexadecimal,
	handle_syscall_print_integer_in_binary,
	handle_syscall_print_integer_as_unsigned,
	NULL,
	NULL,
	NULL,
	handle_syscall_set_seed,
	handle_syscall_random_int,
	handle_syscall_random_int_range,
	handle_syscall_random_float,
	handle_syscall_random_double,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	handle_syscall_ConfirmDialog,
	handle_syscall_InputDialogInt,
	handle_syscall_InputDialogFloat,
	handle_syscall_InputDialogDouble,
	handle_syscall_InputDialogString,
	handle_syscall_MessageDialog,
	handle_syscall_MessageDialogInt,
	handle_syscall_MessageDialogFloat,
	handle_syscall_MessageDialogDouble,
	handle_syscall_MessageDialogString,
};

error_t handle_syscall_print_integer() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_print_float() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_print_double() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_print_string() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_read_integer() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_read_float() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_read_double() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_read_string() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_sbrk() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_exit() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_print_character() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_read_character() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_open_file() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_read_from_file() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_write_to_file() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_close_file() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_exit2() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_time() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_MIDI_out() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_sleep() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_MIDI_out_synchronous() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_print_integer_in_hexadecimal() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_print_integer_in_binary() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_print_integer_as_unsigned() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_set_seed() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_random_int() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_random_int_range() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_random_float() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_random_double() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_ConfirmDialog() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_InputDialogInt() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_InputDialogFloat() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_InputDialogDouble() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_InputDialogString() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_MessageDialog() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_MessageDialogInt() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_MessageDialogFloat() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_MessageDialogDouble() {
	return ERROR_UNSUPPORTED_SYSCALL;
}

error_t handle_syscall_MessageDialogString() {
	return ERROR_UNSUPPORTED_SYSCALL;
}
