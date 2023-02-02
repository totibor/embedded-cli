# embedded-cli
 Simple command-line interface for embedded devices. It is able to invoke functions on the device. 

## Integration
Provided example is for STM32 microcontroller.

Copy cli.c and cli.h files into your project then include the provided header file.
```c
#include "cli.h"
```
Create an array of commands. A command is defined by its name, short description and the function invoked by the command. Function must use `int (*handler)(int argc, char *argv[])` definition.
```c
Command_Typedef shellCommands[] = {
  {"help", "List all available commands", cli_cmd_help},
  {"x", "eXamine memory", cli_cmd_help},
  {"setid", "Set device id", cli_cmd_setid},
};
```
Create an instance of the shell then provide a function to send string via UART, array containing the commands and the number of commands.
```c
Shell_TypeDef shell;

shell.cli_print = HAL_UART_Transmit_wraper;
shell.commands = shellCommands;
shell.numberOfCommands = 3;

CLI_Init(&shell);
```
In your `main` function call `HAL_UART_Receive_IT` with the recieve buffer provided by shell instance. This will start receive the size of RX buffer amount of data in non blocking mode. Then call `CLI_Process` peridically to parse command-line input from user.
```c
int main(void)
{
    HAL_UART_Receive_IT(&huart2, shell.UART_RxBuffer, sizeof(shell.UART_RxBuffer));

    while (1)
    {
        CLI_Process(&shell);
    }
}
```

You can use `shell.cli_print` in your file where you included cli.h to print to the shell.
```c
shell.cli_print("\r\nCLI for STM32, v0.1\r\n> ");
```


Wrapper for HAL library function `HAL_UART_Transmit`. The command-line interface API is written such a way that the wrapper function expects only one null-terminated string argument. `huart2` is the initialization structure for UART peripheral and CLI_UART_SEND_TIMEOUT is the set timeout in cli.h file.
```c
void HAL_UART_Transmit_wraper(char *message)
{
  HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), CLI_UART_SEND_TIMEOUT);
}
```

At reception end of transfer `HAL_UART_RxCpltCallback` is executed. Reception end depends when RX buffer provided for  `HAL_UART_Receive_IT` is full. `HAL_UART_Receive_IT` must be called again to start recieve characters. In current implementation RX buffer is a one element long array so `HAL_UART_RxCpltCallback` executes for every keystroke.
```c
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == &huart2)
  {
    CLI_ReadCharacter(&shell);
    HAL_UART_Receive_IT(&huart2, shell.UART_RxBuffer, sizeof(shell.UART_RxBuffer));
  }
}
```

