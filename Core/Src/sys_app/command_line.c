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

int get_free_space(buffer_t *buffer);
int get_head_tail_distance(buffer_t *buffer);

void command_line_buffer_init(buffer_t *buffer, size_t size, int n) {
    buffer->size = n;
    buffer->head = 0;
    buffer->tail = -1;
    buffer->data = malloc(size);
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

void * pop_all (buffer_t * buffer, void * arr, size_t single_size) {
    void * out = arr;
    while(!empty(buffer)) {
        memcpy(out, pop(buffer), single_size);
        out += single_size;
    }
    return out;
}

void * command_line_pop_all (buffer_t * buffer, void * arr, size_t single_size) {
    return pop_all (buffer, arr, single_size);
}

//todo:test
void * find (buffer_t * buffer, char target) {
    for (int i = buffer->head;  i != buffer->tail+1; i = (i+1)%buffer->size) {
        if ((*((char*)buffer->data+i)) == target) {
            return buffer->data + i;
        }
    }
    return NULL;
}

void * find_str (buffer_t * buffer, char* target, int len) {
    // todo: kmp or lib
}


//
//   statement_t
//


// 另一种方式是直接在缓冲区上处理， 这样需要手写parse中一部分
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

error_t statement_init(statement_t * statement, size_t size) {
    static int idCount = -1;
    statement->id = ++idCount;
    statement->argc = 0;
    for (int i = 0; i<MAX_TOKEN_COUNT;i++) {
        statement->argv[i] = NULL;
    }
    statement->data = malloc(size);
    statement->end = statement->data;
    return command_line_no_error;
}

error_t copy_out(buffer_t * buffer, const char * end, char * target) {
    for (int i = buffer->head; buffer->data+i != end && i != buffer->tail+1; i = (i+1)%buffer->size) {
        if (empty(buffer)) {
            return command_line_no_so_much_element;
        }
        memcpy(target, buffer->data+i, sizeof(char));
        target++;
    }
    return command_line_no_error;
}


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
error_t command_line_buffer_analyze (buffer_t * buffer, statement_t * statement) {
    char * end = find(buffer, '\n');
    if ( end != NULL) {
        *end = '\0';
        error_t res = copy_out(buffer, end, statement->data);
        separate(statement);
        if (res) {
            return res;
        }
    } else {
        return command_line_no_complete_statement;
    }
    return command_line_no_error;
}


            ///********///
            ///* util *///
            ///********///

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
    else error_judge_condition(command_line_empty_name)
    else error_judge_condition(command_line_no_match_type)
    else error_judge_condition(command_line_last_error)
    else {
        char * ans = malloc(sizeof ("unknown error"));
        ans = strcpy(ans, "unknown error");
        return ans;
    }
}



            ///******************///
            ///* command_type_t *///
            ///******************///

error_t command_type_init(command_type_t * type, char * name, char * optName) {
    int p = 0;
    while (name[p]) {
        p++;
    } // may segfault
    if (p){
        type->name = malloc(p * sizeof(char));
        strcpy(type->name, name);
    } else {
        type->name = NULL;
        return command_line_empty_name;
    }
    p = 0;
    while (optName[p]) {
        p++;
    } // may segfault
    if (p) {
        type->optName = malloc(p * sizeof(char));
        strcpy(type->optName, optName);
    }else {
        type->optName = NULL;
    }
    return command_line_no_error;
}

int type_cmp(command_type_t * type, statement_t * statement) {
    return strcmp(statement->argv[0], type->name);
}

error_t command_line_type_match(command_type_t type[], int n_type, statement_t * statement, command_type_t ** matched) {
    for (int i = 0; i< n_type; i++) {
        if (type_cmp(type + i, statement)) {
            *matched = type + i;
            return command_line_no_error;
        }
    }
    return command_line_no_match_type;
}

error_t default_argument_store_handler(command_t * command) {
    command_type_t * type = command->type;
    statement_t * statement = &command->statement;
    int c;
    int n = 0;
    while (1) {
        (c = getopt(statement->argc, statement->argv, type->optName));
        if (c == -1) {
            break;
        }
        if (c == '?') {
            return command_line_wrong_option_argument;
        }
        option_t * option = &command->options[n];
        option->opt = c;
        option->optArg =(int)strtol(optarg, NULL, 10);
        option->opt_addr = optind -1 + statement->data;
        option->optArg_addr = optarg;
        n++;
    }
    if (n< MAX_OPTION_COUNT ){
        command->options[n].opt = '\0';
    }
    return command_line_no_error;
}
