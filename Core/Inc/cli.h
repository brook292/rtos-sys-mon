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

void CLI_Init(void);
bool CLI_PutChar(uint8_t byte);                   
bool CLI_GetLine(uint8_t* dst, uint16_t max_len); 
void CLI_ProcessCommand(char *input_line);
void cmd_help(int argc, char *argv[]);
void cmd_status(int argc, char *argv[]);
void cmd_diag(int argc, char *argv[]);
void cmd_sleep(int argc, char *argv[]);
void cmd_reset(int argc, char *argv[]);

