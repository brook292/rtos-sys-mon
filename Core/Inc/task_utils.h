#pragma once

#include "cmsis_os.h"

void TaskCreateSafe(osThreadFunc_t func, void *arg, const osThreadAttr_t *attr, osThreadId_t *outHandle);
