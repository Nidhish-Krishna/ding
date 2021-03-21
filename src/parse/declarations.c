
#include "codes/node.h"
#include "codes/token.h"

#include "parse/declarations.h"
#include "datastructures/bst.h"

static void errorPrint(UserVariable* temp_UVObj, unsigned char status, short* index)
{
  switch(status)
  {
    case DATATYPE_NODE:
      fprintf(stderr, ERROR"<%s> is a datatype !\n"RESET,temp_UVObj->identifier);
      break;
    case FUNCTION_NODE:
      fprintf(stderr, ERROR"<%s> is a function !\n"RESET,temp_UVObj->identifier);
      break;
    case VARIABLE_NODE:
      fprintf(stderr, ERROR"<%s> is an already declared variable !\n"RESET,temp_UVObj->identifier);
      break;
  }
  fprintf(stderr, ERROR"Invalid variable declaration <%s> !\n"RESET,temp_UVObj->identifier);
  free(temp_UVObj->identifier);
  free(temp_UVObj);
  free(index);
}

static UserVariable* getNewUVObject(char type)
{
  UserVariable *new_UVObj = (UserVariable*) malloc(sizeof(UserVariable));
  new_UVObj->datatype = type;
  new_UVObj->identifier = (char*) malloc(sizeof(char));
  new_UVObj->value = NULL;
  new_UVObj->length = 0;
  return new_UVObj;
}

static Node* getNewNode(char* name, char* data)
{
  Node *new_node = (Node*) malloc(sizeof(Node));
  new_node->key = name;
  new_node->type = VARIABLE_NODE;
  new_node->data = data;
  return new_node;
}

boolean declareVariables(char* line, short line_len)
{
  char i, type_type;
  Node* temp_node;
  BinarySearchTree* current;
  UserVariable* temp_UVObj;
  boolean isType, valid_ID;
  short *index, id_index;

  index = (short*) malloc(sizeof(short));

  isType = True;
  valid_ID = False;
  current = datatype_tree;
  *index = id_index = 0;

  for(i=0;i<line_len;i++)
  {
    if(isType)
    {
      if (*(line+i) == WHITESPACE_TOKEN)
      {
        if(!*((current->key)+(*index)))
        {
          type_type = current->type;
          *index = 0;
          isType = False;
          current = symbol_tree;
          temp_UVObj = getNewUVObject(type_type);
          continue;
        }
        else
        {
          fprintf(stderr, ERROR"Invalid Datatype !\n"RESET);
          return False;
        }
      }
      else if(*(line+i) == NULL_TOKEN)
      {
        fprintf(stderr, ERROR"No variables declared !\n"RESET);
        return False;
      }
      else {;}

      if (i>=line_len) break;
      else {;}

      current = getNode(current, *(line+i), index);

      if (current == NULL)
      {
        fprintf(stderr, ERROR"Invalid Datatype !\n"RESET);
        return False;
      }
      else {;}
    }

    else
    {
      switch(*(line+i))
      {
        case WHITESPACE_TOKEN:
          continue;
        case COMMA_TOKEN:
          if(!*(line+i+1))
          {
            fprintf(stderr, ERROR"Incomplete variables declaration !\n"RESET);
            return False;
          }
          else {;}
          temp_UVObj->identifier = (char*) realloc(temp_UVObj->identifier, id_index+1);
          *((temp_UVObj->identifier)+id_index++) = 0;
          id_index = 0;

          if (valid_ID)
          {
            temp_node = getNewNode(temp_UVObj->identifier, (char*) temp_UVObj);

            char status = insertNode(&symbol_tree, temp_node);
            if(!status) {;}
            else
            {
              errorPrint(temp_UVObj, status, index);
              return False;
            }
            temp_UVObj = getNewUVObject(type_type);
            valid_ID = False;
            continue;
          }
          else {;}
          if(!*((current->key)+(*index)))
          {
            errorPrint(temp_UVObj, current->type, index);
            return False;
          }
          else
          {
            temp_node = getNewNode(temp_UVObj->identifier, (char*) temp_UVObj);
            char status = insertNode(&symbol_tree, temp_node);
            if(!status) {;}
            else
            {
              errorPrint(temp_UVObj, status, index);
              return False;
            }
            temp_UVObj = getNewUVObject(type_type);
            valid_ID = False;
            continue;
          }
          break;

        case NULL_TOKEN:
          temp_UVObj->identifier = (char*) realloc(temp_UVObj->identifier, id_index+1);
          *((temp_UVObj->identifier)+id_index++) = 0;
          id_index = 0;
          if (valid_ID)
          {
            temp_node = getNewNode(temp_UVObj->identifier, (char*) temp_UVObj);
            char status = insertNode(&symbol_tree, temp_node);
            if(!status) {;}
            else
            {
              errorPrint(temp_UVObj, status, index);
              return False;
            }
            valid_ID = False;
            continue;
          }
          else {;}
          if(!*((current->key)+(*index)))
          {
            errorPrint(temp_UVObj, current->type, index);
            return False;
          }
          else
          {
            temp_node = getNewNode(temp_UVObj->identifier, (char*) temp_UVObj);
            char status = insertNode(&symbol_tree, temp_node);
            if(!status) {;}
            else
            {
              errorPrint(temp_UVObj, status, index);
              return False;
            }
            temp_UVObj = getNewUVObject(type_type);
            valid_ID = False;
            continue;
          }
          break;
      }

      temp_UVObj->identifier = (char*) realloc(temp_UVObj->identifier, id_index+1);
      *((temp_UVObj->identifier)+id_index++) = *(line+i);

      if (i>=line_len) break;
      else {;}

      if(valid_ID) continue;

      current = getNode(current, *(line+i), index);

      if (current == NULL) valid_ID = True;
      else {;}
    }

  }

  free(index);
  return True;
}
