//
// Created by insppp on 11/11/22.
//
#include "sys_app/command_line.h"

typedef enum Command_line_error error_t;
typedef  command_line_buffer_t  buffer_t;

error_t command_line_input_buffer_init (buffer_t * buffer, size_t single_size, int n) {
    command_line_buffer_init(buffer, single_size, n);
    return command_line_no_error;
}

error_t command_line_output_buffer_init(buffer_t * buffer, size_t single_size, int n) {
    command_line_buffer_init(buffer, single_size, n);
    return command_line_no_error;
}

