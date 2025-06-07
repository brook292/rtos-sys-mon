#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "usart.h"
#include "main.h"
#include "ring_buffer.h"
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "rtc.h"
#include "cmsis_os.h"
#include "cli_uart.h"

#define CLI_LINE_BUF_SIZE 128



bool CLI_Core_Init(UART_HandleTypeDef* huart);
bool CLI_Core_PutChar(uint8_t byte);
bool CLI_Core_GetLine(uint8_t* dst, uint16_t max_len);
bool CLI_Core_ProcessCommand(char *input_line);
void CLI_Core_ErrorHandle(const char *reason);

