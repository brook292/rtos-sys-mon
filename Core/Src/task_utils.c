#include "cmsis_os.h"
#include "main.h"

void TaskCreateSafe(osThreadFunc_t func, void *arg, const osThreadAttr_t *attr, osThreadId_t *outHandle) {
    *outHandle = osThreadNew(func, arg, attr);

    if (*outHandle == NULL) {
    	Error_Handler();
    }
}
