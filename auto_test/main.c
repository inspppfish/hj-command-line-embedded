#include <stdio.h>
#include <string.h>
#include "test1.h"

int main () {
    command_line_input_buffer_t buffer;
    command_line_input_buffer_init(&buffer, 20);

    // 1
    enum Command_line_input_error e = copy_to_command_line_input_buffer(&buffer,
            "this string is too lang to copy",
            sizeof("this string is too lang to copy"));
    printf("error: %d\n", e);

    // 2
    e = copy_to_command_line_input_buffer(&buffer,
    "this is ok",
    sizeof ("this is ok"));
    printf("error: %d\n", e);

    // 3
    printf("%s\n", buffer.data);

    //4
    printf("size: %d, string_size: %d, free space: %d\n",
           buffer.size, (int)sizeof ("this is ok"), get_free_space(&buffer));

    return 0;
}