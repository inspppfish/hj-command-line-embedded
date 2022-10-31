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
#include "stm32f4xx_it.h"
#include "main.h"
typedef command_line_input_buffer_t buffer_t;



int get_free_space(buffer_t *buffer);
int get_head_tail_distance(buffer_t *buffer);

void command_line_input_buffer_init(buffer_t *buffer, int buffer_size) {
    buffer->size = buffer_size;
    buffer->head = 0;
    buffer->tail = -1;
    buffer->data = malloc(sizeof(char) * buffer_size);
    buffer->data[0] = 'Z';
}

int copy_to_command_line_input_buffer(buffer_t *buffer, const void *source, size_t size) {
    uint32_t n = size / sizeof(char);
    if (n <= get_free_space(buffer)) {
        if (n + buffer->tail < buffer->size) {
            memcpy(buffer->data+buffer->tail+1, source, n*sizeof(char));
            buffer->tail += (int)n;
        } else {
            int right = buffer->size - (buffer->tail+1);
            int left = (int)n - right;
            memcpy(buffer->data + buffer->tail + 1, source, right*sizeof(char));
            memcpy(buffer->data, source+right, left*sizeof(char));
        }
        return command_line_input_no_error;
    } else {
        return command_line_input_buffer_full;
    }
}

int get_free_space(buffer_t * buffer) {
    if(buffer->tail < 0) { // 刚刚初始化
        return buffer->size;
    }

    if (buffer->head <= buffer->tail) { // get_head_tail_distance(buffer) + 1 是已占用的空间
        return buffer->size - (get_head_tail_distance(buffer) + 1);
    }
}

int get_head_tail_distance(buffer_t *buffer) {
    return buffer->tail > buffer->head ? buffer->tail - buffer->head : buffer->head - buffer->tail;
}


extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
void USART1_IRQHandler(void)
{
    /* USER CODE BEGIN USART1_IRQn 0 */

    /* USER CODE END USART1_IRQn 0 */
    HAL_UART_IRQHandler(&huart1);
    /* USER CODE BEGIN USART1_IRQn 1 */
    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE) != RESET) { // 确认发生idle中断
        __HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_IDLE); // 清除idle标志位
        HAL_UART_DMAStop(&huart1);
        int len = RX_BUFFER_LEN - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
//      HAL_UART_Transmit_DMA(&huart1, rx_buffer, len);
        copy_to_command_line_input_buffer(&commandLineInputBuffer, rx_buffer, len * sizeof(char));
        HAL_UART_Receive_DMA(&huart1, rx_buffer, RX_BUFFER_LEN);
    }
    /* USER CODE END USART1_IRQn 1 */
}