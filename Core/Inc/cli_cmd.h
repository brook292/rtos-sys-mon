#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "main.h"
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "rtc.h"
#include "cmsis_os.h"
#include "cli_uart.h"

#define CLI_LINE_BUF_SIZE 128

typedef void (*CommandHandler)(int argc, char* argv[]);

typedef struct {
    const char* name;
    CommandHandler handler;
}CLI_Command;

extern CLI_Command commands[];
extern const size_t num_commands;
 

void CLI_CMD_Help(int argc, char* argv[]);
void CLI_CMD_Status(int argc, char* argv[]);
void CLI_CMD_Diag(int argc, char* argv[]);
void CLI_CMD_Sleep(int argc, char* argv[]);
void CLI_CMD_Reset(int argc, char* argv[]);




