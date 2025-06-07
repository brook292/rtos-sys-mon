#pragma once
#include "cli_core.h"

typedef struct{
	uint32_t cli_flag_line_ready;
	osEventFlagsId_t  cli_event_obj;
}CLI_FLAG_HANDLE;

bool CLI_UART_Get_CLI_Flag(CLI_FLAG_HANDLE* flag_buf);
bool CLI_UART_Init(UART_HandleTypeDef* huart);
bool CLI_UART_Resume(void);
bool CLI_UART_Is_UART_Ready(void);
UART_HandleTypeDef * CLI_UART_Get_CLI_UART(void);




