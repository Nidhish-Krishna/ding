#include "parse/flow.h"
#include "memory/allocate.h"

#include "codes/flag.h"
#include "codes/node.h"
#include "codes/token.h"
#include "codes/datatype.h"

#include "net/getmember.h"
#include "net/setmember.h"
#include "net/assembling.h"

#include "datastructures/bst.h"
#include "datastructures/ntree.h"
#include "datastructures/stack.h"
#include "datastructures/buffer.h"

static char flag;
static short buffer_ind;
static boolean null_node, isMemb;
static short* payload_len;

// BIT TYPE MEMBER SPECIFIC
short allocd;
short* bit_specific;
char** extracted_bit_values;
// BIT TYPE MEMBER SPECIFIC

static void resetFlagsAndCounters()
{
  buffer_ind = flag = 0;
  null_node = isMemb = False;
}

static UserVariable* getNewUVObject(char type, char* value, char length)
{
  UserVariable *new_UVObj = (UserVariable*) malloc(sizeof(UserVariable));
  new_UVObj->datatype = type;
  new_UVObj->identifier = NULL;
  new_UVObj->value = value;
  new_UVObj->length = length;
  return new_UVObj;
}

static Node* getRawVariable(UserVariable* data)
{
  Node *new_node = (Node*) malloc(sizeof(Node));
  new_node->key = NULL;
  new_node->type = RAW_VARIABLE_NODE;
  new_node->data = (char*)data;
  return new_node;
}

static Node* allocate(char* word, char flag)
{
  char *len = malloc(1);
  Node* raw_node = NULL;
  unsigned char *val = NULL;
  switch(flag)
  {
    case IP4_TRUE_FLAG:
      val = allocateIP4(word,len);
      if(val == NULL) { free(len); return NULL; }
      else {;}
      raw_node = getRawVariable(getNewUVObject(IP4_TYPE_CODE, val, 4));
      break;
    case MAC_TRUE_FLAG:
      val = allocateMAC(word,len);
      if(val == NULL) { free(len); return NULL; }
      else {;}
      raw_node = getRawVariable(getNewUVObject(MAC_TYPE_CODE, val, 6));
      break;
    case DQUOTE_TOKEN:
      val = allocateString(word,len);
      raw_node = getRawVariable(getNewUVObject(STRING_TYPE_CODE, val, *len));
      break;
    case COLON_TOKEN:
    case HEXSTREAM_FLAG:
      val = allocateStream(word,len);
      if(val == NULL) { free(len); return NULL; }
      else {;}
      raw_node = getRawVariable(getNewUVObject(STREAM_TYPE_CODE, val, *len));
      break;
    case BIT_FLAG:
      val = allocateBit(word,len);
      if(val == NULL) { free(len); return NULL; }
      else {;}
      raw_node = getRawVariable(getNewUVObject(BIT_TYPE_CODE, val, *len));
      break;
    case NUMBER_FLAG:
      val = allocateNumber(word,len);
      if(val == NULL) { free(len); return NULL; }
      else {;}
      raw_node = getRawVariable(getNewUVObject(NUMBER_TYPE_CODE, val, *len));
      break;
    case PERIOD_FLAG:
      fprintf(stderr, ERROR"Invalid packet membership access !\nUndeclared Packet !\n"RESET);
      free(len);
      return NULL;
    default:
      free(len);
      return NULL;
  }
  free(len);
  return raw_node;
}

static boolean evaluate(Node* root, Stack* exec_stack)
{
  if(root == NULL) return True;
  else {;}
  switch(getNodeType(root))
  {
    case EQUAL_NODE:
      if(!evaluate(root->left, exec_stack)) return False;
      else {;}
      if(!evaluate(root->right, exec_stack)) return False;
      else {;}

      Node* rhs_node = (Node*) popStack(exec_stack);
      Node* lhs_node = (Node*) popStack(exec_stack);

      if(rhs_node == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " RHS not found in Stack !\n");
      else {;}
      if(lhs_node == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " LHS not found in Stack !\n");
      else {;}

      if(((Node*)(lhs_node->data))->type == MEMBER_NODE)
      {
        UserVariable* rhs = (UserVariable*) (((Node*)(rhs_node->data))->data);
        UserVariable* member_variable = (UserVariable*)((Node*)(lhs_node->data))->data;
        if((member_variable->datatype == rhs->datatype)||(member_variable->datatype == STREAM_TYPE_CODE)||(rhs->datatype == STREAM_TYPE_CODE)){;}
        else {fprintf(stderr, ERROR"Incompatible types for lhs and rhs !\nCannot Assign !\n"RESET); return False;}

        if(member_variable->length == rhs->length) {;}
        else if(member_variable->length == 255)
        {
          char* rhs_value;
          short rhs_length;
          if(rhs->datatype == PACKET_TYPE_CODE)
          {
            PacketValue* packet_value = (PacketValue*)(rhs->value);
            PacketObject* packet_object = packet_value->packet;
            assemblePacket(packet_object);
            rhs_value = packet_object->assmbld_packet_struct;
            rhs_length = packet_object->packet_len;
          }
          else
          {
            rhs_value = rhs->value;
            rhs_length = rhs->length;
          }
          // VAR SIZED MEMBERS - PAYLOAD DONE... DO OPTIONS
          char i =0;
          *payload_len = member_variable->length = rhs_length;
          char** mem_payload = (char**)member_variable->value;
          *mem_payload = malloc(member_variable->length);
          for(;i<member_variable->length; i++) *(*mem_payload+i) = *(rhs_value+i);
          goto assignEnd;
        }
        else
        {
          fprintf(stderr, ERROR"Incompatible variable sizes for lhs and rhs !\n"RESET);
          fprintf(stderr, ERROR"lhs requires <%d> bytes, but rhs size is <%d> bytes !\n"RESET,member_variable->length, rhs->length);
          fprintf(stderr, ERROR"Cannot Assign !\n"RESET);
          return False;
        }

        if(member_variable->datatype == BIT_TYPE_CODE)
        {
          if(member_variable->length == 1)
          {
            *((member_variable->value)+*((char*)bit_specific+0))|=(*(rhs->value)<<*((char*)bit_specific+1));
          }
          else if(member_variable->length == 2)
          {
            *(member_variable->value+0)|=(*(rhs->value+0)<<*((char*)bit_specific+1));
            *(member_variable->value+1)|=(*(rhs->value+1)<<*((char*)bit_specific+1));
          }
          else {;}
          goto assignEnd;
        }
        else {;}

        char i, *member_value;
        i=0; member_value = member_variable->value;
        for(;i<member_variable->length; i++) *(member_value+i) = *((rhs->value)+i);
        // try freeing mem nodes
        goto assignEnd;
      }
      else {;}
      UserVariable* rhs = (UserVariable*) (((Node*)(rhs_node->data))->data);
      UserVariable* lhs = (UserVariable*) (((Node*)(lhs_node->data))->data);

      if((lhs->datatype == rhs->datatype) || (lhs->datatype == STREAM_TYPE_CODE) || (rhs->datatype == STREAM_TYPE_CODE)) {;}
      else {fprintf(stderr, ERROR"Incompatible types !\nCannot Assign !\n"RESET); return False;}

      if(!lhs->length) free(lhs->value);
      else {;}

      if(rhs->identifier == NULL) lhs->value = rhs->value;
      else
      {
        lhs->value = (char*) malloc(rhs->length); 
        short i=0;
        for(;i<rhs->length;i++) *((lhs->value)+i) = *((rhs->value)+i);
      }

      lhs->length = rhs->length;
      assignEnd:;
      break;

    case VARIABLE_NODE:
    case RAW_VARIABLE_NODE:
    case MEMBER_NODE:
      pushStack(exec_stack, (char*) root);
      if(!evaluate(root->right,exec_stack)) return False;
      else {;}
      break;
    case FUNCTION_NODE:
      pushStack(exec_stack, (char*) root);
      root = root->left;
      while(root != NULL)
      {
        if (getNodeType(root) == FUNCTION_NODE)
        {
          if(!evaluate(root, exec_stack)) return False;
          else {;}
          Node* evaluated_node = (Node*) popStack(exec_stack);

          if(evaluated_node == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " FUNCTION RETURN VALUE not found in Stack !\n");
          else {;}

          root->data = evaluated_node->data;
          free(evaluated_node);
          root = root->right;
        }
        else root = root->right;
      }
      Node* node = (Node*) popStack(exec_stack);

      if(node == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " FUNCTION NODE not found in Stack !\n");
      else {;}

      UserVariable* return_var;
      UserVariable* (*func) (Node* args);
      func = (UserVariable* (*)(Node* args)) (((Node*)(node->data))->data);
      return_var = func(node->left);

      if(return_var == NULL) return False;
      else {;}

      pushStack(exec_stack, (char*) createNode(NULL, 0, (char*) getRawVariable(return_var)));
      break;
  }

  return True;
}

boolean parseFlow(char *line, short line_len)
{
  char i, *buffer;
  short *index;
  Node* line_tree;
  Stack* line_stack;
  Buffer* bufferObj;
  BinarySearchTree* current;
  boolean isString, ctoken, status;

  index = (short*) malloc(sizeof(short));
  bit_specific = (short*) malloc(sizeof(short));
  extracted_bit_values = (char**) malloc(sizeof(char*));

  i=-1;
  line_tree = NULL;
  current = symbol_tree;
  *index = buffer_ind = flag = allocd = 0;
  ctoken = null_node = isString = isMemb = False;

  bufferObj = createBuffer();
	buffer = bufferObj->buffer;
  line_stack = initDynamicStack();

  while(i<line_len)
  {
    i++;

    switch(*(line+i))
    {
      // SEPARATOR TOKENS //

      case TABSPACE_TOKEN:
        if(!isString) continue;
        else {;}
        break;

      case WHITESPACE_TOKEN:
        if(!isString) continue;
        else {;}
        break;

      case EQUAL_TOKEN:
        if(!*(line+i+1))
        {
          fprintf(stderr, ERROR"Incomplete line !\nEnd of line encountered without rhs !\n"RESET);

          status = False;
          goto deallocate_and_exit;
        }
        else {;}
        if(isString) break;
        else {;}
        if(!null_node)
        {
          if(!*((current->key)+(*index)))
          {
            if(isMemb)
            {
              UserVariable* packet_variable = (UserVariable*) popStack(line_stack);

              if(packet_variable == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " PACKET -1 not found in Stack !\n");
              else {;}

              PacketObject* current_packet = (PacketObject*)(((PacketValue*)(packet_variable->value))->packet);
              UserVariable* member_variable = makeMemberVariable(current_packet, current->type, (char*) bit_specific);
              if (member_variable == NULL)
              {
                fprintf(stderr, ERROR"Undeclared variable <%s> !\n"RESET,buffer);

                status = False;
                goto deallocate_and_exit;
              }
              else {;}
              line_tree = createNode(NULL, 0, (char*) createNode("=\0", EQUAL_NODE, NULL));
              insertNodeNTree(line_tree, createNode(NULL, 0, (char*) createNode("NULL", MEMBER_NODE, (char*) member_variable)));

              resetFlagsAndCounters();
              current = symbol_tree;
              *index =0;
              continue;
            }
            else {;}

            if(line_tree == NULL)
            {
              line_tree = createNode(NULL, 0, (char*) createNode("=\0", EQUAL_NODE, NULL));
              insertNodeNTree(line_tree, createNode(NULL, 0, (char*) current));
            }
            else{;}

            resetFlagsAndCounters();
            *index = 0;
            current = symbol_tree;
            continue;
          }
          else
          {
            while(True)
            {
              current = getNode(current, 0, index);
              if (current == NULL)
              {
                fprintf(stderr, ERROR"Undeclared variable <%s> !\n"RESET, buffer);

                status = False;
                goto deallocate_and_exit;
              }
              else if(!*((current->key)+(*index)))
              {
                if(line_tree == NULL)
                {
                  line_tree = createNode(NULL, 0, (char*) createNode("=\0", EQUAL_NODE, NULL));
                  insertNodeNTree(line_tree, createNode(NULL, 0, (char*) current));
                }
                else {;}

                resetFlagsAndCounters();
                *index = 0;
                current = symbol_tree;
                break;
              }
              else {;}
            }
            continue;
          }
        }
        else
        {
          if(isMemb) fprintf(stderr, ERROR"Invalid Member Name <%s> !\n"RESET, buffer);
          else fprintf(stderr, ERROR"Undeclared variable <%s> !\n"RESET, buffer);

          status = False;
          goto deallocate_and_exit;
        }
        break;

      case OPEN_PARANTH_TOKEN:
        if(isString) break;
        else {;}
        if(ctoken) continue;
        else ctoken = True;
        if(!null_node)
        {
          if(!*((current->key)+(*index)))
          {
            if(line_tree == NULL)
            {
              line_tree = createNode(NULL, 0, (char*) current);
              pushStack(line_stack, (char*) line_tree);
            }
            else
            {
              Node* temp = createNode(NULL, 0, (char*) current);
              insertNodeNTree(line_tree, temp);
              pushStack(line_stack, (char*) line_tree);
              line_tree = temp;
            }

            resetFlagsAndCounters();
            *index = 0;
            current = symbol_tree;
            continue;
          }
          else
          {
            while(True)
            {
              current = getNode(current, 0, index);
              if (current == NULL)
              {
                fprintf(stderr, ERROR"Undeclared variable <%s> !\n"RESET, buffer); 

                status = False;
                goto deallocate_and_exit;
              }
              else if(!*((current->key)+(*index)))
              {
                if(line_tree == NULL)
                {
                  line_tree = createNode(NULL, 0, (char*) current);
                  pushStack(line_stack, (char*) line_tree);
                }
                else
                {
                  Node* temp = createNode(NULL, 0, (char*) current);
                  insertNodeNTree(line_tree, temp);
                  pushStack(line_stack, (char*) line_tree);
                  line_tree = temp;
                }
                resetFlagsAndCounters();
                *index = 0;
                current = symbol_tree;
                break;
              }
              else {;}
            }
            continue;
          }
        }
        else
        {
          if(isMemb)
          {
            fprintf(stderr, ERROR"Invalid Member Name <%s> !\n"RESET, buffer); 

            status = False;
            goto deallocate_and_exit;
          }
          else {;}
          Node* raw_var = allocate(buffer, flag);
          if(raw_var == NULL)
          {
            fprintf(stderr, ERROR"Undeclared variable <%s> !\n"RESET,buffer);

            status = False;
            goto deallocate_and_exit;
          }
          else {;}
          insertNodeNTree(line_tree, createNode(NULL, 0, (char*) raw_var));
          resetFlagsAndCounters();
          *index = 0;
          current = symbol_tree;
          continue;
        }
        break;
      case CLOSE_PARANTH_TOKEN:
        if(isString) break;
        else {;}

        if(ctoken)
        {
          line_tree = (Node*) popStack(line_stack);

          if(line_tree == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " NESTED FUNCTION RETURN VALUE not found in Stack !\n");
          else {;}

          continue;
        }
        else ctoken = True;

        if(!null_node)
        {
          if(!*((current->key)+(*index)))
          {

            if(isMemb)
            {
              UserVariable* packet_variable = (UserVariable*) popStack(line_stack);

              if(packet_variable == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " PACKET NODE -2 not found in Stack !\n");
              else {;}

              PacketObject* current_packet = (PacketObject*)(((PacketValue*)(packet_variable->value))->packet);
              UserVariable* member_variable = getMemberVariable(current_packet, current->type);
              if (member_variable == NULL) return False;
              else {;}

              insertNodeNTree(line_tree, createNode(NULL, 0, (char*) createNode("NULL", RAW_VARIABLE_NODE, (char*) member_variable)));

              resetFlagsAndCounters();
              *index = 0;
              current = symbol_tree;
              continue;
            }
            else {;}


            if(line_tree == NULL) line_tree = createNode(NULL, 0, (char*) current);
            else insertNodeNTree(line_tree, createNode(NULL, 0, (char*) current));
            line_tree = (Node*) popStack(line_stack);

            if(line_tree == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " OUTER FUNCTION NODE - 1 not found in Stack !\n");
            else {;}

            *index = buffer_ind = flag = 0;
            current = symbol_tree;
            null_node = False;
            continue;
          }
          else
          {
            while(True)
            {
              current = getNode(current, 0, index);
              if (current == NULL)
              {
                fprintf(stderr, ERROR"Undeclared variable <%s> !\n"RESET, buffer); 

                status = False;
                goto deallocate_and_exit;
              }
              else if(!*((current->key)+(*index)))
              {
                if(line_tree == NULL) line_tree = createNode(NULL, 0, (char*) current);
                else insertNodeNTree(line_tree, createNode(NULL, 0, (char*) current));

                line_tree = (Node*) popStack(line_stack);

                if(line_tree == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " OUTER FUNCTION NODE - 2 not found in Stack !\n");
                else {;}

                resetFlagsAndCounters();
                *index = 0;
                current = symbol_tree;
                break;
              }
              else {;}
            }
            continue;
          }
        }
        else
        {
          if(isMemb)
          {
            fprintf(stderr, ERROR"Invalid Member Name <%s> !\n"RESET, buffer); 

            status = False;
            goto deallocate_and_exit;
          }
          else {;}
          Node* raw_var = allocate(buffer, flag);
          if(raw_var == NULL)
          {
            fprintf(stderr, ERROR"Undeclared variable <%s> !\n"RESET,buffer);

            status = False;
            goto deallocate_and_exit;
          }
          else {;}
          insertNodeNTree(line_tree, createNode(NULL, 0, (char*) raw_var));
          line_tree = (Node*) popStack(line_stack);

          if(line_tree == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " OUTER FUNCTION NODE - 3 not found in Stack !\n");
          else {;}

          resetFlagsAndCounters();
          *index = 0;
          current = symbol_tree;
          continue;
        }
        break;
      case COMMA_TOKEN:
        if(isString) break;
        else {;}
        if(ctoken) continue;
        else ctoken = True;
        if(!null_node)
        {
          if(!*((current->key)+(*index)))
          {

            if(isMemb)
            {
              UserVariable* packet_variable = (UserVariable*) popStack(line_stack);

              if(packet_variable == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " PACKET NODE - 3 not found in Stack !\n");
              else {;}

              PacketObject* current_packet = (PacketObject*)(((PacketValue*)(packet_variable->value))->packet);
              UserVariable* member_variable = getMemberVariable(current_packet, current->type);
              if (member_variable == NULL) return False;
              else {;}

              insertNodeNTree(line_tree, createNode(NULL, 0, (char*) createNode("NULL", RAW_VARIABLE_NODE, (char*) member_variable)));

              resetFlagsAndCounters();
              *index = 0;
              current = symbol_tree;
              continue;
            }
            else {;}

            if(line_tree == NULL) line_tree = createNode(NULL, 0, (char*) current);
            else insertNodeNTree(line_tree, createNode(NULL, 0, (char*) current));
            resetFlagsAndCounters();
            *index = 0;
            current = symbol_tree;
            continue;
          }
          else
          {
            while(True)
            {
              current = getNode(current, 0, index);
              if (current == NULL)
              {
                fprintf(stderr, ERROR"Undeclared variable <%s> !\n"RESET, buffer); 

                status = False;
                goto deallocate_and_exit;
              }
              else if(!*((current->key)+(*index)))
              {
                if(line_tree == NULL) line_tree = createNode(NULL, 0, (char*) current);
                else insertNodeNTree(line_tree, createNode(NULL, 0, (char*) current));
                resetFlagsAndCounters();
                *index = 0;
                current = symbol_tree;
                break;
              }
              else {;}
            }
            continue;
          }
        }
        else
        {
          if(isMemb)
          {
            fprintf(stderr, ERROR"Invalid Member Name <%s> !\n"RESET, buffer); 

            status = False;
            goto deallocate_and_exit;
          }
          else {;}
          Node* raw_var = allocate(buffer, flag);
          if(raw_var == NULL)
          {
            fprintf(stderr, ERROR"Undeclared variable <%s> !\n"RESET,buffer);

            status = False;
            goto deallocate_and_exit;
          }
          else {;}
          insertNodeNTree(line_tree, createNode(NULL, 0, (char*) raw_var));
          resetFlagsAndCounters();
          *index = 0;
          current = symbol_tree;
          continue;
        }
        break;
      case NULL_TOKEN:
        if(isString) break;
        else {;}

        if(ctoken) break;
        else {;}
        if(!null_node)
        {
          if(isMemb)
          {
            UserVariable* packet_variable = (UserVariable*) popStack(line_stack);

            if(packet_variable == NULL) fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " PACKET NODE - 4 not found in Stack !\n");
            else {;}

            PacketObject* current_packet = (PacketObject*)(((PacketValue*)(packet_variable->value))->packet);
            UserVariable* member_variable = getMemberVariable(current_packet, current->type);
            if (member_variable == NULL) return False;
            else {;}

            insertNodeNTree(line_tree, createNode(NULL, 0, (char*) createNode("NULL", RAW_VARIABLE_NODE, (char*) member_variable)));

            resetFlagsAndCounters();
            *index = 0;
            current = symbol_tree;
            break;
          }
          else {;}


          if(!*((current->key)+(*index)))
          {
            if(line_tree == NULL) line_tree = createNode(NULL, 0, (char*) current);
            else insertNodeNTree(line_tree, createNode(NULL, 0, (char*) current));
            resetFlagsAndCounters();
            *index = 0;
            current = symbol_tree;
            break;
          }
          else {;}
        }
        else
        {
          if(isMemb)
          {
            fprintf(stderr, ERROR"Invalid Member Name <%s> !\n"RESET, buffer); 

            status = False;
            goto deallocate_and_exit;
          }
          else {;}

          Node* raw_var = allocate(buffer, flag);
          if(raw_var == NULL)
          {
            fprintf(stderr, ERROR"Undeclared variable <%s> !\n"RESET,buffer);

            status = False;
            goto deallocate_and_exit;
          }
          else {;}
          insertNodeNTree(line_tree, createNode(NULL, 0, (char*) raw_var));
          resetFlagsAndCounters();
          *index = 0;
          current = symbol_tree;
          break;
        }
        break;

      // NON - SEPARATOR TOKENS //
      case PERIOD_TOKEN:
        if(isString) break;
        else {;}

        if(ctoken) continue;
        else {;}
        if(!null_node)
        {
          if(!*((current->key)+(*index)))
          {
            if(((UserVariable*)(current->data))->datatype != PACKET_TYPE_CODE)
            {
              fprintf(stderr, ERROR"Invalid membership access !\nVariable <%s> is not a packet !\n"RESET,buffer);

              status = False;
              goto deallocate_and_exit;
            }
            else if (!((UserVariable*)(current->data))->length)
            {
              fprintf(stderr, ERROR"Invalid membership access !\nPacket type is not defined for <%s> !\n"RESET,buffer);

              status = False;
              goto deallocate_and_exit;
            }
            else {;}
            pushStack(line_stack, current->data);

            *index = buffer_ind = flag = 0;
            PacketValue* value = (PacketValue*) ((UserVariable*)(current->data))->value;
            payload_len = &((PacketObject*)(value->packet))->payload_len;
            current = (Node*)(value->member_tree);
            null_node = False; isMemb = True;
            continue;
          }
          else {;}
        }
        else {;}

        flag += PERIOD_FLAG;
        break;
      case HYPHEN_TOKEN:
        if(isString) break;
        else {;}
        flag += HYPHEN_FLAG;
        break;
      case DQUOTE_TOKEN:
        isString = !isString;
        flag = DQUOTE_TOKEN;
        break;
      case HEXSTREAM_FLAG:
        if(!buffer_ind) flag = HEXSTREAM_FLAG;
        else {;}
        break;
      case COLON_TOKEN:
        flag = COLON_TOKEN;
        break;
      case b_TOKEN:
        if(flag == COLON_TOKEN) flag = BIT_FLAG;
        else {;}
        break;
      case B_TOKEN:
        if(flag == COLON_TOKEN) flag = NUMBER_FLAG;
        else {;}
        break;
    }

    if(!*(line+i)) break;
    else {;}

    ctoken = False;

    readToBuffer(bufferObj, *(line+i), buffer_ind++);

    if(!null_node)
    {
      current = getNode(current, *(line+i), index);
      if (current == NULL) null_node = True;
      else { ; }
    }
    else {;}
  }
  resetStack(line_stack);
  status = evaluate(line_tree, line_stack);

  deallocate_and_exit:
    free(index);
    freeBuffer(bufferObj);
    free(bit_specific);
    freeNTree(line_tree);
    freeStack(line_stack);

    short z = 0;
    for(;z<allocd;z++) free(*(extracted_bit_values+z));

  return status;
}
