#include <stdio.h>

FILE* openReadFile(const char* path){
    FILE* file;
    file = fopen(path,"rb");
    return file;
}
FILE* openWriteFile(const char* path){
    FILE* file;
    file = fopen(path,"wb");
    return file;
}
void closeFile(FILE* file){
    fclose(file);
}