#include "cli.h"

static RingBuffer cli_buffer;
static uint8_t line_buf[CLI_LINE_BUF_SIZE];
static uint16_t line_len = 0;

typedef void (*CommandHandler)(int argc,char *argv[]);

typedef struct {
	const char *name;
	CommandHandler handler;
}CLI_Command;

CLI_Command commands[]={
	    {"help",  cmd_help},
	    {"status", cmd_status},
	    {"diag",  cmd_diag},
	    {"sleep", cmd_sleep},
	    {"reset", cmd_reset},
};

#define NUM_COMMANDS (sizeof(commands) / sizeof(commands[0]))
#define MAX_TOKEN 8


void cmd_help(int argc, char *argv[]) {
    const char *msg =
        "Available commands:\r\n"
        "  help         - show this message\r\n"
        "  status       - show system status\r\n"
        "  diag         - run diagnostics\r\n"
        "  sleep [s]    - enter STOP mode and wake after s seconds\r\n"
        "  reset        - reset MCU\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
}

void cmd_status(int argc, char *argv[]) {
    char buf[64];
    uint32_t uptime_ms = xTaskGetTickCount();

    snprintf(buf, sizeof(buf), "[SYS] Uptime: %lus\r\n", uptime_ms / 1000);
    HAL_UART_Transmit(&huart2, (uint8_t *)buf, strlen(buf), HAL_MAX_DELAY);
}

void cmd_diag(int argc, char *argv[]) {
    char buf[64];

    snprintf(buf, sizeof(buf), "[DIAG] Heap free: %u bytes\r\n", xPortGetFreeHeapSize());
    HAL_UART_Transmit(&huart2, (uint8_t *)buf, strlen(buf), HAL_MAX_DELAY);
}

void cmd_sleep(int argc, char *argv[]) {
    uint32_t seconds = 5; // default 5s

    if (argc >= 2)
      {
        seconds = (uint32_t)atoi(argv[1]);
        if (seconds == 0) seconds = 5;  // fallback
      }

    char msg[64];
    snprintf(msg, sizeof(msg), "[SYS] Sleeping %lus via RTC...\r\n", seconds);
    HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
    osDelay(10);


    // Disable wakeup to reconfigure
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

    HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
    if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, seconds, RTC_WAKEUPCLOCK_CK_SPRE_16BITS) != HAL_OK) {
        const char *err = "[SYS] Failed to set wakeup timer\r\n";
        HAL_UART_Transmit(&huart2, (uint8_t *)err, strlen(err), HAL_MAX_DELAY);
        return;
    }

    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

    // MCU resumed here
    SystemClock_Config();
    MX_USART2_UART_Init();
    CLI_UART_Resume();

    const char *done = "[SYS] Woke up from RTC wakeup\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t *)done, strlen(done), HAL_MAX_DELAY);
}

void cmd_reset(int argc, char *argv[]) {
    const char *msg = "[SYS] Resetting system...\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
    osDelay(10);
    NVIC_SystemReset();
}



void CLI_ProcessCommand(char *input_line){
	char *argv[MAX_TOKEN];
	uint8_t argc=0;

	char *token = strtok(input_line, " ");
	while(token && argc< MAX_TOKEN)
		{
		 argv[argc++]=token;
		 token = strtok(NULL, " ");
		}

	if(argc == 0) return;

	for(int i = 0;i < NUM_COMMANDS; i++)
	  {
		if(strcmp(argv[0],commands[i].name) == 0)
		 {
			commands[i].handler(argc,argv);
			return;
		 }
	  }
    const char *err = "Unknown command\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t *)err, strlen(err), HAL_MAX_DELAY);
}


void CLI_Init(void) {
    RingBuffer_Init(&cli_buffer);
    line_len = 0;
}

bool CLI_PutChar(uint8_t byte) {
    return RingBuffer_Put(&cli_buffer, byte);
}

bool CLI_GetLine(uint8_t* dst, uint16_t max_len) {
    uint8_t byte;
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
