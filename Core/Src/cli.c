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

/**
 * @brief Match user input to command from command list and execute function assoicated with matching command.
 * This function is called periodacially in main function.
 *
 * @param shell Instance of a shell
 */
void CLI_Process(Shell_TypeDef *shell)
{
    if (!shell->readyToRead)
        return;

    shell->readyToRead = 0;

    // If user just hit enter without provideing commang just print a new line in CLI
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
        if (!CLI_strcmp(argv[0], command->name))
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
 * @brief Process character input from shell's input. Called in interrupt handler
 * called when one character recieve is complete.
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

    if (!isPrintable(*recentCharacter))
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
int CLI_StringToArgv(char *src, char *argv[])
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
 * @brief Inintialize the shell to its default state. Function to print to shell, an array
 * of commands and number of command must be provided by user of this API.
 * 
 * @param shell Instance of a shell
 */
void CLI_Init(Shell_TypeDef *shell)
{
    shell->charCount = 0;
    shell->readyToRead = 0;
    shell->cli_print("\r\nCLI for STM32, v0.1\r\n> ");
}

/**
 * @brief Check if character is tab or whitespace character.
 * 
 * @param ch Character to test.
 * @return True if character is tab or whitespace, false otherwies.
 */
int isWhitespace(uint8_t ch)
{
    if (ch == '\t' || ch == ' ')
        return 1;

    return 0;
}

/**
 * @brief Check if character is punctuation.
 * 
 * @param ch Character to test.
 * @return True if character is punctuation, false otherwies.
 */
int isPunctuation(uint8_t ch)
{
    if (ch == ' ' || ch == '!' || ch == '\"' || ch == '#' || ch == '$' || ch == '%' || ch == '&' || ch == '\'' || ch == '(' || ch == ')' || ch == '*' || ch == '+' || ch == ',' || ch == '-' || ch == '.' || ch == '/' || ch == ':' || ch == ';' || ch == '<' || ch == '=' || ch == '>' || ch == '?' || ch == '@' || ch == '[' || ch == '\\' || ch == ']' || ch == '^' || ch == '`' || ch == '{' || ch == '|' || ch == '}')
        return 1;

    return 0;
}

/**
 * @brief Check if character is alphanumeric.
 * 
 * @param ch Character to test.
 * @return True if character is alphanumeric, false otherwies.
 */
int isAlphaNumeric(uint8_t ch)
{
    if (((ch >= '0') & (ch <= '9')) || ((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')))
        return 1;

    return 0;
}

/**
 * @brief Check if character is a printable character.
 * 
 * @param ch Character to test.
 * @return True if character is alphanumeric, false otherwies.
 */
int isPrintable(uint8_t ch)
{
    if (isAlphaNumeric(ch) || isPunctuation(ch) || isWhitespace(ch))
        return 1;
    return 0;
}

/**
 * @brief Lexicographically compare the null-terminated strings.
 * 
 * @param s String to compare.
 * @param t String to compare.
 * @return Return an integer greater than, equal to, or less than 
 * 0, according as the string s is greater than, equal to, or
 * less than the string t.
 */
int CLI_strcmp(char *s, char *t)
{
    for (; *s == *t; s++, t++)
    {
        if (*s == '\0')
        {
            return 0;
        }
    }
    return *s - *t;
}

/**
 * @brief Append a copy of the null-terminated string src to the end of the null-terminated string dest.
 * 
 * @param dest String to append to.
 * @param src String to append.
 * @return char* Pointer to end of dest string.
 */
char *CLI_strcat(char *dest, char *src)
{
    while (*dest)
        dest++;
    while (*dest++ = *src++)
        ;
    return --dest;
}

/**
 * @brief Find length of string.
 * 
 * @param src String whose length needs to calculated.
 * @return int Number of characters that precede the terminating NULL character.
 */
int strlen(char *src)
{
    int length = 0;
    while (*src++)
        length++;
    return length;
}

