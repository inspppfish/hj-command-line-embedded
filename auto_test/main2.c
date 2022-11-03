//
// Created by airvoltex on 22-11-4.
//
#include <stdio.h>
#include "test2.h"

int main () {
    //1

    for (enum Command_line_error e = command_line_no_error; e <= command_line_last_error+1; e++) {
        printf("%s\n", error_analyze(e));
    }

    return 0;
}