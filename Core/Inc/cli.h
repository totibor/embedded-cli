#ifndef STM32_CLI_H
#define STM32_CLI_H

#include <stdint.h>

#define CLI_INPUT_BUFFER_SIZE 100       /* How many characters can be read from user */
#define CLI_UART_SEND_TIMEOUT 100000    /* Arbitrary number, change later */
#define UART_RX_BUFFER_SIZE 1           /* Every character generates an RxComplete interrupt */
#define NUMBER_OF_MAX_CMD_ARGS 10       /* 1 command plus 9 command arguments */
#define BACKSPACE_KEY 127               /* ASCII code sent when user hits backsapce key */
#define ENTER_KEY '\r'                  /* Character or character combination when user hits enter key */

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

void shell_Init(Shell_TypeDef *shell);
int isWhitespace(uint8_t ch);
int isPunctuation(uint8_t ch);
int isAlphaNumeric(uint8_t ch);
int isPrintable(uint8_t ch);
int cli_strcompare(char* s, char* t);
char* mystrcat( char* dest, char* src);
int stringToArgv(char* src, char *argv[]);
int strlen(char *src);
void cli_process(Shell_TypeDef *shell);
void readFromShell(Shell_TypeDef *shell);

#endif /* STM32_CLI_H */