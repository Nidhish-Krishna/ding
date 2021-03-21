#include "datastructures/stack.h"

// DO NOT FREE STACKNODES
// FREE ONLY STACKS

static Stack* initStack(short size, boolean dynamic)
{
  Stack* new_stack = (Stack*) malloc(sizeof(Stack));
  new_stack-> size = size;
  new_stack-> current = -1;
  new_stack-> dynamic = dynamic;
  new_stack->stack_nodes = (char**) malloc(size*sizeof(char*));

  return new_stack;
}

Stack* initDynamicStack()
{
  return initStack(STACK_DEFAULT_SIZE, True);
}

Stack* initFixedStack(short size)
{
  return initStack(size, False);
}

void pushStack(Stack* stack, char* data)
{
  if (stack->current >= (stack->size)-1)
  {
    if(stack->dynamic)
    {
      stack->size += STACK_DEFAULT_SIZE;
      stack->stack_nodes = (char**) realloc(stack->stack_nodes, (stack->size)*sizeof(char*));
    }
    else
    {
      fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET" Stack Full !\n");
      return;
    }
  }
  else {;}

  stack->current += 1;
  *((stack->stack_nodes)+(stack->current)) = data;
}

char* popStack(Stack* stack)
{
  if((stack->current)<0) { fprintf(stderr, DEBUG_B"[DEBUG INFO]:"RESET " Stack Empty !\n"); return NULL; }
  else {;}

  char* data = *((stack->stack_nodes)+(stack->current));
  stack->current -= 1;

  return data;
}

void resetStack(Stack* stack)
{
  stack-> current = -1;
}

void freeStack(Stack* stack)
{
  free(stack->stack_nodes);
  free(stack);
}


/*
======================================================================
|----------------- DEBUG FUNCTIONS USED BY THE AUTHOR ---------------|
|--------------------------------------------------------------------|
|-USE THESE OR DEFINE YOUR OWN FUNCTIONS TO DEBUG DURING DEVELOPMENT-|
======================================================================

void printStack(Stack* stack, short current)
{
  if (current < 0) return;
  else
  {
    printf("");
    printStack(stack, current-1);
  }
}
*/
