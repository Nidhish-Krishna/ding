#ifndef STDLIB_H
#define STDLIB_H
#include<stdlib.h>
#endif

#ifndef STORAGE
#define STORAGE
#include "storage.h"
#endif

#ifndef BOOLEAN_H
#define BOOLEAN_H
typedef enum boolean{True=1,False=0} boolean;
#endif

void setup();
void freeSetup();
