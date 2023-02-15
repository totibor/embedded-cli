#include <stdio.h>
#include <errno.h>
#include <syscalls.h>

UART_HandleTypeDef *gHuart;

void syscallsInit(UART_HandleTypeDef *huart)
{
    gHuart = huart;
    setvbuf(stdout, NULL, _IONBF, 0);
}

/**
 * @brief Writes up to count bytes from the buffer starting at buf
 * to the file referred to by the file descriptor fd.
 *
 * @param fd File descriptor of file into which data is written.
 * @param ptr Data to be written.
 * @param len Number of bytes.
 * @return int The number of bytes written
 */
int _write(int fd, char *ptr, int len)
{
    HAL_StatusTypeDef status;
    status = HAL_UART_Transmit(gHuart, (uint8_t *)ptr, len, HAL_MAX_DELAY);

    if (status == HAL_OK)
    {
        return len;
    }
    else
    {
        errno = EIO;
    }

    return -1;
}

/**
 * @brief Attempts to read up to count bytes from file descriptor fd
 * into the buffer starting at buf.
 *
 * @param fd File descriptor referring to the open file.
 * @param ptr Storage location for data.
 * @param len Maximum number of bytes to read.
 * @return int The number of bytes read
 */
int _read(int fd, char *ptr, int len)
{
    HAL_StatusTypeDef status;
    status = HAL_UART_Receive(gHuart, (uint8_t *)ptr, 1, HAL_MAX_DELAY);

    if (status == HAL_OK)
    {
        HAL_UART_Transmit(gHuart, (uint8_t *)ptr, 1, HAL_MAX_DELAY);
        return 1;
    }
    else
    {
        errno = EIO;
    }

    return -1;
}