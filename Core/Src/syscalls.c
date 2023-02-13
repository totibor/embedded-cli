#include <stdio.h>
#include <syscalls.h>

UART_HandleTypeDef *gHuart;

void syscallsInit(UART_HandleTypeDef *huart)
{
    gHuart = huart;
    setvbuf(stdout, NULL, _IONBF, 0);
}

int _write(int fd, char *ptr, int len)
{
    /* TODO: Error handling in case UART transmit fails. */
    HAL_UART_Transmit(gHuart, (uint8_t *)ptr, len, HAL_MAX_DELAY);
    return len;
}

int _read(int fd, char *ptr, int len)
{
    /* TODO: Error handling in case UART receive (and transmit) fails. */
    HAL_UART_Receive(gHuart, (uint8_t *)ptr, 1, HAL_MAX_DELAY);
    /* Make user input visible in the terminal. Call _write instead? */
    HAL_UART_Transmit(gHuart, (uint8_t *)ptr, 1, HAL_MAX_DELAY);
    return 1;
}