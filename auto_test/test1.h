//
// Created by airvoltex on 22-10-31.
//

#ifndef COMMAND_LINE_EMB_COMMAND_LINE_TEST_H
#define COMMAND_LINE_EMB_COMMAND_LINE_TEST_H

#include "sys_app/command_line.h"
int get_free_space(command_line_buffer_t * buffer);
int get_used_space (const command_line_buffer_t * buffer);
char * error_analyze (enum Command_line_error error);

#endif //COMMAND_LINE_EMB_COMMAND_LINE_TEST_H
