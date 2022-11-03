//
// Created by airvoltex on 22-10-29.
//

//               _   _                 _ _      __  __
//              /_\ (_)_ ____   _____ | | |_ ___\ \/ /
//             //_\\| | '__\ \ / / _ \| | __/ _ \\  /
//            /  _  \ | |   \ V / (_) | | ||  __//  \
//            \_/ \_/_|_|    \_/ \___/|_|\__\___/_/\_\

#include "sys_app/command_line.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>


//
//   implement buffer_t
//

typedef command_line_buffer_t buffer_t;
typedef enum Command_line_error error_t;
#define asChar(x) *(char *)(x)

int get_free_space(buffer_t *buffer);
int get_head_tail_distance(buffer_t *buffer);

void command_line_buffer_init(buffer_t *buffer, size_t size, int n) {
    buffer->size = n;
    buffer->head = 0;
    buffer->tail = -1;
    buffer->data = malloc(size);
    asChar(buffer->data+0) = 'Z';
}

enum Command_line_error copy_to_command_line_buffer(buffer_t *buffer, const void *source, size_t size, int n) {
    if (n <= get_free_space(buffer)) {
        if (n + buffer->tail < buffer->size) {
            memcpy(buffer->data+buffer->tail+1, source, size);
            buffer->tail += n;
        } else {
            int right = buffer->size - (buffer->tail+1);
            int left = n - right;
            memcpy(buffer->data + buffer->tail + 1, source, right*(size/n));
            memcpy(buffer->data, source+right, left*(size/n));
            buffer->tail = left-1;
        }
        return command_line_no_error;
    } else {
        return command_line_input_buffer_full;
    }
}

int get_used_space (const buffer_t * buffer) {
    const int head = buffer->head;
    int tail = buffer->tail; // 想象把环形缓冲区展开成一条纸带
    tail = head <= tail + 1 ? tail : tail + buffer->size;
    return tail - buffer->head + 1;
}

int get_free_space(buffer_t * buffer) {
    return buffer->size - get_used_space(buffer);
}

int empty(buffer_t * buffer) {
    return buffer->tail + 1 == buffer->head;
}


void * pop(buffer_t * buffer) {
    void * val = buffer->data + buffer->head;
    if (!empty(buffer)) {
        buffer->head = (buffer->head+1)%buffer->size;
    } else {
        assert(0); ; // 这个函数是最底层的出队列函数之一，如果调用有问题只能assert了
    }
    return val;
}

enum Command_line_error  pop_string (buffer_t * buffer, void * arr, size_t n) {
    char * out = arr;
    if (n > get_used_space(buffer)) {
        return command_line_no_so_much_element;
    }
    while (n--) {
         *out = *(char*)pop(buffer);
         out++;
    }
    return command_line_no_error;
}

void * pop_all (buffer_t * buffer, void * arr) {
    char * out = arr;
    while(!empty(buffer)) {
        *out = *(char*)pop(buffer);
        out++;
    }
    *out = '\0';
    return out;
}

void * command_line_pop_all (buffer_t * buffer, void * arr) {
    return pop_all (buffer, arr);
}

//todo:test
void * find (buffer_t * buffer, char target) {
    for (int i = buffer->head;  i != buffer->tail+1; i = (i+1)%buffer->size) {
        if (asChar(buffer->data+i) == target) {
            return buffer->data + i;
        }
    }
}

void * grep_str (buffer_t * buffer, char* target, int len) {
    // todo: kmp or lib
}


//
//   implement statement_t
//


// 另一种方式是直接在缓冲区上处理， 这样需要手写对于命令的解析
//error_t for_each(buffer_t * buffer, void * end, error_t fun(char *, void *), void * param) {
//    for (int i = buffer->head; i != buffer->tail+1; i = (i+1)%buffer->size) {
//        error_t error = fun(buffer->data + 1, param);
//        if (error) {
//            return error;
//        }
//    }
//    return command_line_no_error;
//}
//
//error_t for_space_do( char * c, void * param) {
//    statement_t * statement = param;
//    if (*c == ' ' /*0x20*/) {
//        statement->argv[++statement->argc] = c;
//    }
//    return command_line_no_error;
//}

//todo:test
error_t copy_out(buffer_t * buffer, const char * end, char * target) {
    for (int i = buffer->head; buffer->data+i != end; i = (i+1)%buffer->size) {
        if (empty(buffer)) {
            return command_line_no_so_much_element;
        }
        *target = *(char*)buffer->data+i;
        target++;
    }
    return command_line_no_error;
}

//todo:test
error_t separate(statement_t * statement) {
    char * p = strtok(statement->data, " ");
    while (p) {
        statement->argv[statement->argc] = p;
        statement->argc++;
        p = strtok(NULL, " ");
    }
    return command_line_no_error;
}

// 为了能更多用到库函数， 这里就把缓冲区里的东西copy出来处理了
// todo:test
error_t command_line_buffer_analyze (buffer_t * buffer, statement_t * statement) {
    char * end = find(buffer, '\n');
    if (*end == '\n') {
        *end = '\0'; // ?不知道用不用
        error_t res = copy_out(buffer, end, statement->data);
        separate(statement);
        if (res) {
            return res;
        }
    } else {
        return command_line_no_complete_statement;
    }
}

#define error_judge_condition(e) \
                    if (error == (e)) {\
                        char * ans = malloc(sizeof (#e));    \
                        strcpy(ans, (#e));                   \
                        return ans;                          \
                    }

char * error_analyze (error_t error) {
    error_judge_condition(command_line_no_error)
    else error_judge_condition(command_line_input_buffer_full)
    else error_judge_condition(command_line_no_so_much_element)
    else error_judge_condition(command_line_no_complete_statement)
    else error_judge_condition(command_line_too_much_parameter)
    else error_judge_condition(command_line_last_error)
    else {
        char * ans = malloc(sizeof ("unknown error")+1);
        ans = strcpy(ans, "unknown error");
        ans[sizeof ("unknown error")/sizeof (char)] = '\0';
        return ans;
    }
}
