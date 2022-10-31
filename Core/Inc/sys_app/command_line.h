//
// Created by airvoltex on 22-10-29.
//

#ifndef COMMAND_LINE_EMB_COMMAND_LINE_H
#define COMMAND_LINE_EMB_COMMAND_LINE_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define COMMAND_LINE_INPUT_BUFFER_SIZE_DEFAULT 100

enum Command_line_error {
    command_line_input_no_error = 0,
    command_line_input_buffer_full,
    command_line_no_so_much_element
};



typedef struct {
    int head; // head是有效数据中的第一位
    int tail; // tail是最后一个有效数据
    // head == tail + 1 的情况，就是空队列；特别地，如果tail == -1，就是刚刚初始化，
    // 也就是说 tail == head 的时候是有一位数据的
    // 队列满的时候，(tail + 1)mod size == head

    int size;
    char * data; // never free
}command_line_input_buffer_t;

void command_line_input_buffer_init(command_line_input_buffer_t *buffer, int buffer_size);
enum Command_line_error copy_to_command_line_input_buffer(command_line_input_buffer_t* buffer, const void * source, size_t size);

#endif //COMMAND_LINE_EMB_COMMAND_LINE_H
