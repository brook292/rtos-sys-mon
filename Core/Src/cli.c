#include "cli.h"
#include "ring_buffer.h"
#include <string.h>

static RingBuffer cli_buffer;
static uint8_t line_buf[CLI_LINE_BUF_SIZE];
static uint16_t line_len = 0;

void CLI_Init(void) {
    RingBuffer_Init(&cli_buffer);
    line_len = 0;
}

bool CLI_PutChar(uint8_t byte) {
    return RingBuffer_Put(&cli_buffer, byte);
}

bool CLI_GetLine(uint8_t* dst, uint16_t max_len) {
    uint8_t byte;
    while (RingBuffer_Get(&cli_buffer, &byte)) {
        if (line_len < CLI_LINE_BUF_SIZE - 1) {
            if (byte == '\r' || byte == '\n') {
                line_buf[line_len] = '\0';
                uint16_t len = (line_len < max_len) ? line_len : max_len - 1;
                memcpy(dst, line_buf, len + 1);
                line_len = 0;
                return true;
            }
            else {
                line_buf[line_len++] = byte;
            }
        }
        else {
            line_len = 0;  // overflow
        }
    }
    return false;
}
