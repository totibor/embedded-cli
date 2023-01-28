#include "cli.h"

void cli_process(Shell_TypeDef *shell)
{
    if (!shell->readyToRead) return;

    shell->readyToRead = 0;

    // If user just hit enter without provideing commang just print a new line in CLI
    if(shell->charCount == 0)
    {
        shell->cli_print("\r\n> ");
        return;
    }

    char *argv[NUMBER_OF_MAX_CMD_ARGS];
    int argc = stringToArgv(shell->inputBuffer, argv);

    Command_Typedef *firstCommand = shell->commands;
    Command_Typedef *lastCommand = &shell->commands[shell->numberOfCommands - 1];

    for(Command_Typedef *command = firstCommand; command <= lastCommand; command++)
    {
      if (!cli_strcompare(argv[0], command->name))
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

void readFromShell(Shell_TypeDef *shell)
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
        if (shell->charCount == 0) return;

        shell->cli_print("\b \b");    // Delete last charcter in terminal window.
        shell->charCount--;           // Delete last character from shell's input buffer.
        return;
    }

    if (!isPrintable(*recentCharacter)) return;

    shell->inputBuffer[shell->charCount++] = (char)*recentCharacter;

    char buffer[2];
    buffer[0] = (char)*recentCharacter;
    buffer[1] = '\0';
    shell->cli_print(buffer);
}

void shell_Init(Shell_TypeDef *shell)
{
    shell->charCount = 0;
    shell->readyToRead = 0;
    shell->cli_print("\r\nCLI for STM32, v0.1\r\n> ");
}

int isWhitespace(uint8_t ch)
{
  if (ch == '\t' || ch == ' ')
    return 1;
  
  return 0;
}

int isPunctuation(uint8_t ch)
{
  if (ch == ' ' || ch == '!' || ch == '\"' || ch == '#' || ch == '$' || ch == '%' || ch == '&' || ch == '\'' || ch == '(' || ch == ')' || ch == '*' || ch == '+' || ch == ',' || ch == '-' || ch == '.' || ch == '/' || ch == ':' || ch == ';' || ch == '<' || ch == '=' || ch == '>' || ch == '?' || ch == '@' || ch == '[' || ch == '\\' || ch == ']' || ch == '^' || ch == '`' || ch == '{' || ch == '|' || ch == '}')
    return 1;

  return 0;
}

int isAlphaNumeric(uint8_t ch)
{
  if (((ch >= '0') & (ch <= '9')) || ((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')))
    return 1;

  return 0;
}

int isPrintable(uint8_t ch)
{
  if (isAlphaNumeric(ch) || isPunctuation(ch) || isWhitespace(ch))
    return 1;
  return 0;
}

// Compare two strings.
// Returns 0 if the two strings are the same.
int cli_strcompare(char* s, char* t)
{
  for( ; *s == *t; s++, t++)
  {
    if (*s == '\0') { return 0; }
  }
  return *s - *t;
}

char* mystrcat( char* dest, char* src )
{
  while (*dest) dest++;
  while (*dest++ = *src++);
  return --dest;
}

// Split input string into command and its arguments.
int stringToArgv(char* src, char *argv[])
{
  // First argument is the name of the command.
  int argc = 1;
  argv[argc - 1] = src;

  while (*src)
  {
    if(*src == '\t' || *src == ' ')
    {
      *src = '\0';
      src++;
      while (*src == '\t' || *src == ' ') src++;
      
      // found next argument
      argc++;
      argv[argc - 1] = src;
    }

    src++;
  }
  return argc;
}

int strlen(char *src)
{
  int length = 0;
  while (*src++) length ++;
  return length;
}