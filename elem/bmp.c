#include <stdint.h>
#include <stdio.h>
#include <malloc.h>
#include "fileManager.h"
#pragma pack(push, 1)

struct bmp_header{
    uint16_t bfType; //Type
    uint32_t bfileSize; //Size in byte
    uint32_t bfReserved; //Unused
    uint32_t bOffBits; //Offset
    uint32_t biSize; // >=40 headSize
    uint32_t biWidth; // Picture width
    uint32_t biHeight; //Picture height
    uint16_t biPlanes; // = 1
    uint16_t biBitCount; // a bit on pixel
    uint32_t biCompression; // 0 - without compression
    uint32_t biSizeImage;
    uint32_t biXPelsPerMeter;
    uint32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
}bmpHeader;
#pragma pack(pop)
static int calculate(int width){
    return (3 * width + 3) & (-4);
}
static unsigned char** readBMP(const char* path) {
    unsigned char** img;
    FILE* file = openReadFile(path);
    fread(&bmpHeader, sizeof(bmpHeader), 1, file);
    int width = bmpHeader.biWidth;
    int height = bmpHeader.biHeight;
    int trueWidth = calculate(width);
    img = (unsigned char*)calloc(trueWidth * height, sizeof(char));
    fread(img, 1, trueWidth * height, file);
    closeFile(file);
    return img;
}

static int saveBMP(const char* path, unsigned char** img) {
    FILE* file = openWriteFile(path) ;
    int width = bmpHeader.biWidth;
    int height = bmpHeader.biHeight;
    int trueWidth = calculate(width);
    fwrite(&bmpHeader, sizeof(bmpHeader), 1, file);
    fwrite(img, sizeof(char), trueWidth * height, file);
    closeFile(file);
    return 1;
}



int rotateImage(const char* filepath, const char* resPath){
    unsigned char** img = readBMP(filepath);
    saveBMP(resPath, img);
}