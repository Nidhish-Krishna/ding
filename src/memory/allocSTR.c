#include "memory/allocSTR.h"

static char getEscape(char letter)
{
  switch(letter)
  {
    case 'n':
      return '\n';

    case 'b':
      return '\b';

    case 'r':
      return '\r';

    case '0':
      return '\0';

    case 'v':
      return '\v';

    default:
      return letter;
  }
}

char* allocateString(char *word, char* length)
{
  char* str;
  char count;  
  boolean dQuotes = False;

  count = 0;
  str = (char*) malloc(1);

  do
  {
    if (*word == '\\' && dQuotes)
    {
      word++;
      char escape = getEscape(*word);

      str = (char*) realloc(str,count+1);
      *(str+ count++) = escape;
      
      continue;
    }
    if (*word == '"') { dQuotes = !dQuotes; continue; }
    else {;}

    if (!dQuotes) continue;
    else
    {
      str = (char*) realloc(str,count+1);
      *(str+ count++) = *word;
    }
  }while(*(++word));

  str = (char*) realloc(str,count+1);
  *(str+ count++) = '\0';

  *length = count-1;
  return str;
}
