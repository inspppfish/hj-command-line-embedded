#include <stdio.h>
#include <string.h>
#include "test1.h"

void debug_output(command_line_buffer_t * buffer) {
    printf("head: %d, tail: %d\n", buffer->head, buffer->tail);
    for (int i = 0; i<20; i++) {
        printf("%c", *(char*)buffer->data+i);
    }
    printf("\n");
}

int main () {
    command_line_buffer_t buffer;
    command_line_buffer_init(&buffer, 20*sizeof(char),20);

    // 1
    enum Command_line_error e = copy_to_command_line_buffer(&buffer,
            "this string is too long to copy",
            sizeof("this string is too long to copy"), sizeof("this string is too long to copy")/sizeof (char));
    printf("error: %d\n", e);

    // 2
    e = copy_to_command_line_buffer(&buffer,
    "this is ok",
    sizeof ("this is ok"), sizeof("this is ok")/sizeof (char));
    printf("error: %d\n", e);

    // 3
    printf("%s\n", (char*)buffer.data);

    // 4
    printf("size: %d, string_size: %d, free space: %d\n",
           buffer.size, (int)sizeof ("this is ok"), get_free_space(&buffer));

    // 5
    char a[21];
    command_line_pop_all(&buffer, a, sizeof(char));
    printf("%s with %d elements left\n", a, get_used_space(&buffer));

    //5
    copy_to_command_line_buffer(&buffer,
                                          "this is ok too",
                                sizeof ("this is ok too"), sizeof ("this is ok too")/sizeof(char));
    printf("size: %d, string_size: %d, free space: %d\n",
           buffer.size, (int)sizeof ("this is ok too"), get_free_space(&buffer));

    //6
    command_line_pop_all(&buffer, a, sizeof (char));
    printf("%s with %d elements left\n", a, get_used_space(&buffer));

    return 0;
}