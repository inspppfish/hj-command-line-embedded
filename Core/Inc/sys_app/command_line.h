//
// Created by airvoltex on 22-10-29.
//

#ifndef COMMAND_LINE_EMB_COMMAND_LINE_H
#define COMMAND_LINE_EMB_COMMAND_LINE_H

#include <stdint.h>
#include <string.h>

#define COMMAND_LINE_INPUT_BUFFER_SIZE_DEFAULT 100

enum Command_line_error {
    command_line_no_error = 0,
    command_line_input_buffer_full,
    command_line_no_so_much_element,
    command_line_no_complete_statement,
    command_line_too_much_parameter,
    command_line_last_error , // 永远保持它是最下面的一个
};


typedef struct {
    int head; // head是有效数据中的第一位
    int tail; // tail是最后一个有效数据
    // head == tail + 1 (mod size)的情况，就是空队列；特别地，如果tail == -1，就是刚刚初始化，
    // 也就是说 tail == head 的时候是有一位数据的
    // 队列满的时候，tail + 1 == head (mod size)

    int size;
    void * data; // never free
}command_line_buffer_t;

void command_line_buffer_init(command_line_buffer_t *buffer, size_t size, int n);
enum Command_line_error copy_to_command_line_buffer(command_line_buffer_t* buffer, const void * source, size_t size, int n);
void * command_line_pop_all (command_line_buffer_t * buffer, void * arr);
char * error_analyze (enum Command_line_error error);


typedef struct {
    int id;
    int argc;
    char * argv[20]; //argv[argc] = NULL
    char * data;
    char * end;
} statement_t;

#endif //COMMAND_LINE_EMB_COMMAND_LINE_H
