
#include "cli_uart.h"

static  UART_HandleTypeDef *huart_buf;
static  volatile uint8_t rx_data;
static  volatile bool init_flag;

CLI_FLAG_HANDLE cli_flag_handle = {
	.cli_flag_line_ready = (1U << 0),
	.cli_event_obj = NULL
};


bool CLI_UART_Init(UART_HandleTypeDef* huart) {
	huart_buf = huart;
	cli_flag_handle.cli_event_obj = osEventFlagsNew(NULL);
	if (cli_flag_handle.cli_event_obj == NULL ||
		HAL_UART_Receive_IT(huart_buf, (uint8_t*)&rx_data, sizeof(rx_data)) != HAL_OK)
	   {
		return false; // init fail
	   }
	init_flag = true; // init done
	return init_flag;
}

bool CLI_UART_Resume(void) {
	if (init_flag) HAL_UART_Receive_IT(huart_buf, (uint8_t*)&rx_data, sizeof(rx_data));
	return init_flag;
}

bool CLI_UART_Get_CLI_Flag(CLI_FLAG_HANDLE *flag_buf) {
	if(init_flag) *flag_buf = cli_flag_handle;
	return init_flag;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
	if (init_flag)
	{
		if (huart->Instance == huart_buf->Instance)
		{
			CLI_Core_PutChar(rx_data);
			if ((rx_data == '\n' || rx_data == '\r') && cli_flag_handle.cli_event_obj != NULL)
			{
				//set event flag
				osEventFlagsSet(cli_flag_handle.cli_event_obj, cli_flag_handle.cli_flag_line_ready);
			}
			CLI_UART_Resume();
		}
	}
}

bool CLI_UART_Is_UART_Ready(void){
	 return init_flag;

}

UART_HandleTypeDef * CLI_UART_Get_CLI_UART(void){
	return huart_buf;
}

