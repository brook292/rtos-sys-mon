# STM32 System Monitor

A FreeRTOS-based system monitor running on STM32 Nucleo-F401RE with UART CLI, self-diagnostics, and power management.

## Features
- LED heartbeat task (blinks every 1 sec)
- UART command line interface (CLI)
- Self-diagnostics (RAM check, stack check)
- Sleep/Stop power mode
- Modular task-based architecture

## How to Build
1. Open the project in STM32CubeIDE
2. Build and flash to Nucleo board
3. Open UART terminal at 115200 baud

## CLI Commands

| Command | Description                    |
|---------|--------------------------------|
| help    | Show command list              |
| status  | Print system uptime            |
| diag    | Show heap status               |
| sleep X | Enter STOP mode for X seconds   |
| reset   | Software reset MCU             |


