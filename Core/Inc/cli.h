#ifndef STM32_CLI_H
#define STM32_CLI_H

#include <stdint.h>
#include <string.h>
#include <ctype.h>


#define UART_RX_BUFFER_SIZE 1           /* Every character generates an RxComplete interrupt */
#define CLI_INPUT_BUFFER_SIZE 100       /* How many characters can be read from user */
#define CLI_UART_SEND_TIMEOUT 100000    /* Arbitrary number, change later */

#define SIZE(array, type) (sizeof(array) / sizeof(type))

typedef struct Command
{
  char* name;
  char* help;
  int (*handler)(int argc, char *argv[]);
} Command_Typedef;

typedef struct
{
    uint8_t UART_RxBuffer[UART_RX_BUFFER_SIZE];
    char inputBuffer[CLI_INPUT_BUFFER_SIZE];
    int charCount;
    int readyToRead;
    void (*cli_print)(char *message);
    int numberOfCommands;
    Command_Typedef *commands;
} Shell_TypeDef;

void CLI_Init(Shell_TypeDef *shell);
void CLI_Process(Shell_TypeDef *shell);
void CLI_ReadCharacter(Shell_TypeDef *shell);

#endif /* STM32_CLI_H */