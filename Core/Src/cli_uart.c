#include "cli_uart.h"


void CLI_UART_Resume(void) {
    HAL_UART_Receive_IT(&huart2, (uint8_t*)&rx_data, sizeof(rx_data));
}
