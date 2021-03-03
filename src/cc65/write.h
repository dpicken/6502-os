#ifndef cc65_write_h
#define cc65_write_h

#include <stdio.h>

#define VIDIPRINTER_PATH "/dev/vidiprinter"
#define VIDIPRINTER_FILENO 3
extern FILE* vidiprinter;

int open_writer(const char* path);

int __fastcall__ write(int fd, const void* buf, unsigned int count);

#endif // ifndef cc65_write_h
