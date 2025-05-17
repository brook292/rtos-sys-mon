#pragma once
#include <stdint.h>
#include <stdbool.h>

#define CLI_LINE_BUF_SIZE 128

void CLI_Init(void);
bool CLI_PutChar(uint8_t byte);                   
bool CLI_GetLine(uint8_t* dst, uint16_t max_len); 




