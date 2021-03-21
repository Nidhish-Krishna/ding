#ifndef BOOLEAN_H
#define BOOLEAN_H
typedef enum boolean{True=1,False=0} boolean;
#endif

int length(const char* str);
void clearMemory(char* buff, short size);
boolean stringsEqual(char* str1, char* str2);
char stringsCompare(char* str1, char* str2);

