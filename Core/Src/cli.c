/**
 * @file cli.c
 * @author totibor
 * @brief Source file for simple command-line interface for embedded devices.
 * @date 2023-01-29
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "cli.h"

static int CLI_StringToArgv(char *src, char *argv[]);

/* 1 command plus 9 command arguments */
#define NUMBER_OF_MAX_CMD_ARGS 10
/* ASCII code sent when user hits backsapce key */
#define BACKSPACE_KEY 127
/* Character or character combination when user hits enter key */
#define ENTER_KEY '\r'

/**
 * @brief Execute function assoicated with matching command.
 *
 * @param shell Instance of a shell
 */
void CLI_Process(Shell_TypeDef *shell)
{
    if (!shell->readyToRead)
        return;

    shell->readyToRead = 0;

    /* If user just hit enter without providing a command then
    print a new line in CLI */
    if (shell->charCount == 0)
    {
        shell->cli_print("\r\n> ");
        return;
    }

    char *argv[NUMBER_OF_MAX_CMD_ARGS];
    int argc = CLI_StringToArgv(shell->inputBuffer, argv);

    Command_Typedef *firstCommand = shell->commands;
    Command_Typedef *lastCommand = &shell->commands[shell->numberOfCommands - 1];

    for (Command_Typedef *command = firstCommand; command <= lastCommand; command++)
    {
        if (!strcmp(argv[0], command->name))
        {
            shell->charCount = 0;
            command->handler(argc, argv);
            return;
        }
    }

    shell->charCount = 0;
    shell->cli_print("\r\nCommand not found: ");
    shell->cli_print(argv[0]);
    shell->cli_print("\r\n> ");
}

/**
 * @brief Process character input from shell's input.
 *
 * @param shell Instance of a shell
 */
void CLI_ReadCharacter(Shell_TypeDef *shell)
{
    uint8_t *recentCharacter = shell->UART_RxBuffer;

    if (*recentCharacter == ENTER_KEY)
    {
        shell->inputBuffer[shell->charCount] = '\0';
        shell->readyToRead = 1;
        return;
    }

    if (*recentCharacter == BACKSPACE_KEY)
    {
        if (shell->charCount == 0)
            return;

        shell->cli_print("\b \b"); // Delete last charcter in terminal window.
        shell->charCount--;        // Delete last character from shell's input buffer.
        return;
    }

    if (!isprint(*recentCharacter))
        return;

    shell->inputBuffer[shell->charCount++] = (char)*recentCharacter;

    char buffer[2];
    buffer[0] = (char)*recentCharacter;
    buffer[1] = '\0';
    shell->cli_print(buffer);
}

/**
 * @brief Split command line input string into a command and it's arguments.
 *
 * @param src Command line input string.
 * @param argv Array to hold the command and it's arguments.
 * @return int Number of arguments (including the command).
 */
static int CLI_StringToArgv(char *src, char *argv[])
{
    // First argument is the name of the command.
    int argc = 1;
    argv[argc - 1] = src;

    while (*src)
    {
        if (*src == '\t' || *src == ' ')
        {
            *src = '\0';
            src++;
            while (*src == '\t' || *src == ' ')
                src++;

            // found next argument
            argc++;
            argv[argc - 1] = src;
        }

        src++;
    }
    return argc;
}

/**
 * @brief Inintialize the shell to its default state.
 *
 * @param shell Instance of a shell
 */
void CLI_Init(Shell_TypeDef *shell)
{
    shell->charCount = 0;
    shell->readyToRead = 0;
    shell->cli_print("\r\nCLI for STM32, v0.1\r\n> ");
}