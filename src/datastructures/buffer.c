#include "datastructures/buffer.h"

Buffer* createBuffer()
{
  Buffer* new_buffer = (Buffer*) malloc(sizeof(Buffer));
  new_buffer->size = BUFFER_DEFAULT_SIZE;
  new_buffer->buffer = (char*) malloc(new_buffer->size);
  return new_buffer;
}

void readToBuffer(Buffer* buffer, char data, short index)
{
  if (index+1 >= buffer->size)
  {
    buffer->size += BUFFER_DEFAULT_SIZE;
    buffer->buffer = (char*) realloc(buffer->buffer, buffer->size);
  }
  else {;}
  *((buffer->buffer)+index) = data;
  *((buffer->buffer)+index+1) = 0;
}

void freeBuffer(Buffer* buffer)
{
  free(buffer->buffer);
  free(buffer);
}

