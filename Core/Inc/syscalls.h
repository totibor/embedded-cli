#ifndef _SYSCALLS_H__
#define _SYSCALLS_H__

#include "stm32f1xx_hal.h"

void syscallsInit(UART_HandleTypeDef *huart);

#endif //#ifndef _SYSCALLS_H__