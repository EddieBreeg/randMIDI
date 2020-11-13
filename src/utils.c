#include "../include/utils.h"

int readBytes(unsigned char* dest, FILE* fp, size_t n)
{
    for(int i = 0; i < n; i++)
    {
        unsigned char c = (unsigned char)fgetc(fp);
        if(c == EOF) return 0;
        dest[i] = c;
    }
    return 1;
}
void writeBytes(FILE* fp, const char* buffer, size_t n)
{
    for(int i = 0; i< n; i++)
        fprintf(fp, "%c", buffer[i]);
}
int bytesToInt(unsigned char* buffer)
{
    int result = 0;
    int e = 1;
    for(int i = sizeof(int)-1; i >=0; i--)
    {
        result += buffer[i] * e;
        e *= 256;
    }
    return result;
}
short int bytesToShort(unsigned char* buffer)
{
    int result = 0;
    int e = 1;
    for(int i = sizeof(short int)-1; i >=0; i--)
    {
        result += buffer[i] * e;
        e *= 256;
    }
    return result;
}
void intToBytes(unsigned int x, char* buffer)
{
    for(int i = sizeof(int)-1; i>=0; i--)
    {
        buffer[i] = (char)x;
        x /= 256;
    }
}
int getString(char* str)
{
    int i = 0;
    char c = (char)getchar();
    while(c!='\n' && c!=EOF)
    {
        str[i] = c;
        i++;
        c = (char)getchar();
    }
    return i;
}
int scanNotes(char buffer[])
{
    char str[20] = {0};
    int count = 0;
    while(getString(str))
    {
        buffer[count] = ((unsigned char)atoi(str)) % 128;
        count++;
        memset(str, 0, 20);
    }
    return count;
}