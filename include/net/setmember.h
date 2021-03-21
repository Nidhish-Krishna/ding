#ifndef STDIO_H
#define STDIO_H
#include<stdio.h>
#endif

#ifndef STDLIB_H
#define STDLIB_H
#include<stdlib.h>
#endif

#ifndef COLOR
#define COLOR
#include "codes/color.h"
#endif

#ifndef STORAGE
#define STORAGE
#include "storage.h"
#endif

UserVariable* makeMemberVariable(PacketObject* packet, short packet_member, char* bit_specific);

