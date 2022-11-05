//
// Created by airvoltex on 22-11-4.
//
#include "test3.h"
#include <stdio.h>

int main () {
    command_line_buffer_t buffer;
    command_line_buffer_init(&buffer, sizeof(char)*100, 100);
    copy_to_command_line_buffer(
        &buffer, "this is ok", sizeof("this is ok"), sizeof ("this is ok")/sizeof (char)
    );
    char temp[101];
    command_line_pop_all(&buffer, temp, sizeof(char));
    copy_to_command_line_buffer(
        &buffer, "setGimbalAngle -p100 -r0 -y265\n", sizeof("setGimbalAngle -p100 -r0 -y265\n"), sizeof ("setGimbalAngle -p100 -r0 -y265\n")/sizeof (char)
    );
    statement_t statement;
    statement_init(&statement, 100);
    printf("%s\n", error_analyze(command_line_buffer_analyze(&buffer, &statement)));
    printf("%s\n", statement.data);

    for (int i = 0; i< statement.argc; i++) {
        printf("%s\n", statement.argv[i]);
    }


    return 0;
}