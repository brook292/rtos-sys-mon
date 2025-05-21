#pragma once

#include "usart.h"
#include <stdint.h>

extern volatile uint8_t rx_data;
extern UART_HandleTypeDef huart2;
void CLI_UART_Resume(void);  






