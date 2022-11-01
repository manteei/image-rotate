#include <stdio.h>

FILE* openReadFile(const char* path){
    FILE* file;
    file = fopen(path,"rb");
    if(file == NULL) return 33;
    return file;
}
FILE* openWriteFile(const char* path){
    FILE* file;
    file = fopen(path,"wb");
    if(file == NULL) return 33;
    return file;
}
void closeFile(FILE* file){
    fclose(file);
}