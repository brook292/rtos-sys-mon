#include "cli_cmd.h"
#include "cli_core.h"



#define DEFAULT_SLEEP_SECONDS 5
#define CLI_MSG_BUF_SIZE 64


CLI_Command commands[] = {
        {"help",  CLI_CMD_Help},
        {"status", CLI_CMD_Status},
        {"diag",  CLI_CMD_Diag},
        {"sleep", CLI_CMD_Sleep},
        {"reset", CLI_CMD_Reset},
};

const size_t num_commands = sizeof(commands) / sizeof(commands[0]);


void CLI_CMD_Help(int argc, char* argv[]) {
    const char* msg =
        "Available commands:\r\n"
        "  help         - show this message\r\n"
        "  status       - show system status\r\n"
        "  diag         - run diagnostics\r\n"
        "  sleep [s]    - enter STOP mode and wake after s seconds\r\n"
        "  reset        - reset MCU\r\n";

    UART_HandleTypeDef *cli_uart= CLI_UART_Get_CLI_UART();
    HAL_UART_Transmit(cli_uart, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}

void CLI_CMD_Status(int argc, char* argv[]) {
    char buf[CLI_MSG_BUF_SIZE];
    uint32_t uptime_ms = xTaskGetTickCount();

    snprintf(buf, sizeof(buf), "[SYS] Uptime: %lus\r\n", uptime_ms / 1000);

    UART_HandleTypeDef *cli_uart= CLI_UART_Get_CLI_UART();
    HAL_UART_Transmit(cli_uart, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);
}

void CLI_CMD_Diag(int argc, char* argv[]) {
    char buf[CLI_MSG_BUF_SIZE];

    snprintf(buf, sizeof(buf), "[DIAG] Heap free: %u bytes\r\n", xPortGetFreeHeapSize());

    UART_HandleTypeDef *cli_uart= CLI_UART_Get_CLI_UART();
    HAL_UART_Transmit(cli_uart, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);
}

void CLI_CMD_Sleep(int argc, char* argv[]) {
    uint32_t seconds = DEFAULT_SLEEP_SECONDS;
    UART_HandleTypeDef *cli_uart= CLI_UART_Get_CLI_UART();

    if (argc >= 2)
    {
        seconds = (uint32_t)atoi(argv[1]);
        if (seconds == 0) seconds = 5;  // fallback
    }

    char msg[CLI_MSG_BUF_SIZE];
    snprintf(msg, sizeof(msg), "[SYS] Sleeping %lus via RTC...\r\n", seconds);


    HAL_UART_Transmit(cli_uart, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
    osDelay(10);


    // Disable wakeup to reconfigure
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

    HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
    if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, seconds, RTC_WAKEUPCLOCK_CK_SPRE_16BITS) != HAL_OK) {
        const char* err = "[SYS] Failed to set wakeup timer\r\n";


        HAL_UART_Transmit(cli_uart, (uint8_t*)err, strlen(err), HAL_MAX_DELAY);
        return;
    }

    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

    // MCU resumed here
    SystemClock_Config();
    MX_USART2_UART_Init();
    CLI_UART_Resume();

    const char* done = "[SYS] Woke up from RTC wakeup\r\n";
    HAL_UART_Transmit(cli_uart, (uint8_t*)done, strlen(done), HAL_MAX_DELAY);
}

void CLI_CMD_Reset(int argc, char* argv[]) {
    const char* msg = "[SYS] Resetting system...\r\n";

    UART_HandleTypeDef *cli_uart= CLI_UART_Get_CLI_UART();
    HAL_UART_Transmit(cli_uart, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
    osDelay(10);
    NVIC_SystemReset();
}






