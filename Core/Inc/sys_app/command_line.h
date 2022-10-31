//
// Created by airvoltex on 22-10-29.
//

#ifndef COMMAND_LINE_EMB_COMMAND_LINE_H
#define COMMAND_LINE_EMB_COMMAND_LINE_H

#include <stdint.h>
#include "string.h"

#define COMMAND_LINE_INPUT_BUFFER_SIZE_DEFAULT 100

enum Command_line_input_error{
    command_line_input_no_error = 0,
    command_line_input_buffer_full
};



typedef struct {
    int head; // head是有效数据中的第一位
    int tail; // tail是最后一个有效数据 // 对于head == tail 的情况， 认为长度为0
    int size;
    char * data; // never free
}command_line_input_buffer_t;

void command_line_input_buffer_init(command_line_input_buffer_t *buffer, int buffer_size);
int copy_to_command_line_input_buffer(command_line_input_buffer_t* buffer, const void * source, size_t size);

#endif //COMMAND_LINE_EMB_COMMAND_LINE_H
