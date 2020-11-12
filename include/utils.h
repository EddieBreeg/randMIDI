#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef UTILS_H
#define UTILS_H

int readBytes(unsigned char* dest, FILE* fp, size_t n);
int bytesToInt(unsigned char* buffer);
short int bytesToShort(unsigned char* buffer);
void writeBytes(FILE* fp, const char* buffer, size_t n);
void intToBytes(unsigned int x, char* buffer);
int scanNotes(char buffer[]);

#endif