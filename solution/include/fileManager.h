#ifndef POPITKA_2_FILEMANAGER_H
#define POPITKA_2_FILEMANAGER_H

#include <stdio.h>

FILE* openReadFile(const char* path);
FILE* openWriteFile(const char* path);
void closeFile(FILE* file);

#endif //POPITKA_2_FILEMANAGER_H
