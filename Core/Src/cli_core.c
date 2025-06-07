#include "cli_core.h"
#include "cli_cmd.h"
#include "cli_uart.h"

#define MAX_TOKEN 8

static RingBuffer cli_buffer;
static uint8_t line_buf[CLI_LINE_BUF_SIZE];
static uint16_t line_len = 0;
static  volatile bool init_flag;

extern const size_t num_commands;

bool CLI_Core_Init(UART_HandleTypeDef* huart) {
	if(!CLI_UART_Init(huart)) return false;
    RingBuffer_Init(&cli_buffer);
    line_len = 0;
	init_flag = true; // init done
	return init_flag;
}

bool CLI_Core_ProcessCommand(char *input_line){
	char *argv[MAX_TOKEN];
	uint8_t argc=0;


	if(!init_flag) return init_flag;
	char *token = strtok(input_line, " ");
	while(token && argc< MAX_TOKEN)
		{
		 argv[argc++]=token;
		 token = strtok(NULL, " ");
		}

	if(argc == 0) return false;

	for(int i = 0;i < num_commands; i++)
	  {
		if(strcmp(argv[0],commands[i].name) == 0)
		 {
			commands[i].handler(argc,argv);
			return true;
		 }
	  }
    const char *err = "Unknown command\r\n";

    UART_HandleTypeDef *cli_uart= CLI_UART_Get_CLI_UART();
    HAL_UART_Transmit(cli_uart, (uint8_t *)err, strlen(err), HAL_MAX_DELAY);
    return false;
}




bool CLI_Core_PutChar(uint8_t byte) {
	if(!init_flag) return init_flag;
    return RingBuffer_Put(&cli_buffer, byte);
}

bool CLI_Core_GetLine(uint8_t* dst, uint16_t max_len) {
    uint8_t byte;
	if(!init_flag) return init_flag;
    while (RingBuffer_Get(&cli_buffer, &byte))
         {
		  if (line_len < CLI_LINE_BUF_SIZE - 1)
			{
			  if(byte == '\r' || byte == '\n')
				{
				  line_buf[line_len] = '\0';
				  uint16_t len = (line_len < max_len) ? line_len : max_len - 1;
				  memcpy(dst, line_buf, len + 1);
				  line_len = 0;
				  return true;
				}
			  else
				{
				  line_buf[line_len++] = byte;
				}
			}
		   else
			{
			  line_len = 0;  // overflow
			}
          }
    return false;
}

void CLI_Core_ErrorHandle(const char *reason){

    if (CLI_UART_Is_UART_Ready()) {
    	UART_HandleTypeDef *cli_uart= CLI_UART_Get_CLI_UART();
        HAL_UART_Transmit(cli_uart, (uint8_t *)reason, strlen(reason), HAL_MAX_DELAY);
        __disable_irq();
        while (1);  // Safe mode
    } else {
    	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET); //show reset signal for 1sec before activate
    	HAL_Delay(1000);
        NVIC_SystemReset(); // Fallback
    }

}
