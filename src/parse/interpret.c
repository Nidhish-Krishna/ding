#include "codes/token.h"

#include "utils/string.h"
#include "datastructures/buffer.h"

#include "parse/flow.h"
#include "parse/interpret.h"
#include "parse/declarations.h"

boolean interpret(char *line, short line_len, boolean shell)
{
  if(shell) goto _shell;
  else {;}
  if      (stringsEqual(line, ".decl:")) { decl_flag = True; func_flag = False; flow_flag = False; return True; }
  else if (stringsEqual(line, ".func:")) { decl_flag = False; func_flag = True; flow_flag = False; return True; }
  else if (stringsEqual(line, ".flow:")) { decl_flag = False; func_flag = False; flow_flag = True; return True; }
  else {;}

  _shell:
  if       (decl_flag) return declareVariables(line, line_len);
  else if (func_flag) return True; /* NOT DEFINED */
  else if (flow_flag) return parseFlow(line, line_len);
  else return True;
}

boolean interpretLines(char* source, int source_length, boolean shell)
{
  Buffer *line;

  line = createBuffer();

  boolean isString = False, comment = False;

  short i=0, line_index = 0, read_lines = 0, paranthesis=0;

  for(;i<source_length;i++)
  {
    switch(*(source+i))
    {
      case COMMENT_TOKEN:
        if(!isString) comment = True;
        else {;}
        break;

      case OPEN_PARANTH_TOKEN:
        if(isString || comment) {;}
        else paranthesis++;
        break;

      case CLOSE_PARANTH_TOKEN:
        if(isString || comment) {;}
        else paranthesis--;
        break;

      case TABSPACE_TOKEN:
        if(!line_index) continue;
        else if((i+1) <source_length)
        {
          switch(*(source+i+1))
          {
            case WHITESPACE_TOKEN:
            case TABSPACE_TOKEN:
            case NULL_TOKEN:
            case NEW_LINE_TOKEN:
            case COMMENT_TOKEN:
              continue;
            default:
              break;
          }
        }
        else {;}
        break;

      case WHITESPACE_TOKEN:
        if(!line_index) continue;
        else if((i+1) <source_length)
        {
          switch(*(source+i+1))
          {
            case WHITESPACE_TOKEN:
            case TABSPACE_TOKEN:
            case NULL_TOKEN:
            case NEW_LINE_TOKEN:
            case COMMENT_TOKEN:
              continue;
            default:
              break;
          }
        }
        else {;}
        break;

      case DQUOTE_TOKEN:
        isString = !isString;
        break;
      default:;
    }

    if(*(source+i) == NEW_LINE_TOKEN || *(source+i) == NULL_TOKEN)
    {
      if(isString)
      {
        fprintf(stderr, ERROR"Incomplete String Declaration !\n"RESET);
        fprintf(stderr, ERROR "Error in Interpreting the following line !\n" RESET);
        fprintf(stderr, ERROR"{"RESET" "ERR_L "%s"RESET" "ERROR "}\n"RESET,line->buffer);
        break;
      }
      else {;}

      if(paranthesis)
      {
        fprintf(stderr, ERROR"Paranthesis count mismatch !\n"RESET);
        fprintf(stderr, ERROR "Error in Interpreting the following line !\n" RESET);
        fprintf(stderr, ERROR"{"RESET" "ERR_L "%s"RESET" "ERROR "}\n"RESET,line->buffer);
        break;
      }
      else {;}

      comment = False;
      isString = False;
      if(!line_index) goto skip_line;
      else {;}

      if(!interpret(line->buffer, line_index+1, shell))
      {
        if(shell) break;
        else {;}
        fprintf(stderr, ERROR "Error in Interpreting the following line !\n" RESET);
        fprintf(stderr, ERROR"{"RESET" "ERR_L "%s"RESET" "ERROR "}\n"RESET,line->buffer);
        break;
      }
      else{;}

      skip_line:;
      line_index = 0;
      *(line->buffer) = 0;
    }
    else if(comment) continue;
    else readToBuffer(line, *(source+i), line_index++);
  }
  freeBuffer(line);
  return True;
}

