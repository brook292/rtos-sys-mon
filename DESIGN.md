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
