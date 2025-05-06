# Firmware Design Document – STM32 System Monitor

## 1. Overview

This firmware is designed for the STM32 Nucleo-F401RE board to monitor internal system status and provide diagnostics via a UART CLI. It uses FreeRTOS for task management and is optimized for low-power operation and modularity.

---

## 2. Objectives

- Provide CLI-based diagnostics (UART via USART2)
- Run system checks (RAM test, task stack monitor)
- Blink LED as heartbeat indicator
- Enter low-power STOP mode on inactivity
- Demonstrate good firmware architecture for resume/portfolio

---

## 3. System Architecture

```text
+---------------------+
|     UART CLI        |
+----------+----------+
           |
           v
+---------------------+
|  Command Processor  |
+----------+----------+
           |
     +-----+-----+
     |           |
+----v----+  +---v----+
| SelfChk |  | Power  |
| Task    |  | Task   |
+---------+  +--------+

+---------------------+
|   Heartbeat Task    |
+---------------------+

---

## 4. RTOS Task Design

| Task Name | Stack Size | Priority | Purpose                                |
| --------- | ---------- | -------- | -------------------------------------- |
| CLI Task  | 512        | Medium   | Handles UART input and command parsing |
| Heartbeat | 128        | Low      | Blinks LED at 1Hz                      |
| SelfCheck | 512        | Medium   | Performs diagnostics, stack checks     |
| PowerMgr  | 256        | High     | Detects inactivity, enters STOP mode   |

---


 ## 5. CLI Specification

| Command  | Description                 | Example Output                    |
| -------- | --------------------------- | --------------------------------- |
| `status` | Show uptime and system info | `[SYS] Uptime: 125s, Tasks: OK`   |
| `diag`   | Run diagnostics             | `[DIAG] RAM: OK, Stack: Clean`    |
| `sleep`  | Enter STOP mode             | `[SYS] Sleeping now...`           |
| `reset`  | Software reset              | `[SYS] Resetting system...`       |
| `help`   | List available commands     | `[CLI] Commands: status, diag...` |

---


 ## 6. Peripheral Usage

| Peripheral  | Use                            | Notes                              |
| ----------- | ------------------------------ | ---------------------------------- |
| USART2      | UART CLI                       | TX/RX via ST-LINK virtual COM port |
| GPIOA Pin 5 | LED heartbeat                  | Output toggled by heartbeat task   |
| EXTI13      | Wake from STOP mode (optional) | Triggered by user button           |
| RTC Wakeup  | Optional timed wake from STOP  | Not enabled by default             |

Peripherals are initialized via STM32CubeMX and main.c startup code.

---


 ## 7. Power Management

-If no UART input is detected for 30 seconds, the system enters STOP mode.

-PowerMgr task uses a software timer to track inactivity.

-The MCU wakes up via UART RX interrupt or EXTI13 (user button).

-System resumes to normal operation after wakeup, preserving all task states.

---


 ## 8. Future Improvements

Add logging over UART to SD card or QSPI (if hardware permits)

Add command history and auto-completion to CLI

Add software watchdog + fault recovery system

Implement OTA update mechanism via UART bootloader

Add unit testing hooks (if using host simulation)

---


 ## 9. Notes

Project generated using STM32CubeMX with FreeRTOS enabled.

UART initialized via MX_USART2_UART_Init().

System tick based on SysTick timer (default in STM32 HAL).

Tested with STM32CubeIDE and STM32CubeProgrammer.

