//
// Created by airvoltex on 22-11-4.
//

#ifndef COMMAND_LINE_EMB_TEST3_H
#define COMMAND_LINE_EMB_TEST3_H
#include "sys_app/command_line.h"
typedef command_line_buffer_t buffer_t;
int get_used_space (const buffer_t * buffer);
enum Command_line_error copy_out(buffer_t * buffer, const char * end, char * target);

#endif //COMMAND_LINE_EMB_TEST3_H
