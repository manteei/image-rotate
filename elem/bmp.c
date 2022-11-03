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

struct pixel { uint8_t b, g, r; }pixel;

struct image {
    uint64_t width;
    uint64_t height;
    struct pixel* data;
};

static int calculate(int width){
    return (3 * width + 3) & (-4);
}
static struct image readBMP(const char* path) {
    unsigned char** value;
    char *c;
    int i;
    FILE* file = openReadFile(path);
    fread(&bmpHeader, sizeof(bmpHeader), 1, file);
    uint64_t width = bmpHeader.biWidth;
    uint64_t height = bmpHeader.biHeight;
    uint64_t trueWidth = calculate(width);
    uint64_t pix = trueWidth*height/3;
    struct pixel * data = (struct pixel *) malloc(pix * sizeof(struct pixel));
    c = (char *)data;
    int k = 0;
    while ((i= getc(file))!=EOF){
        *c = i;
        c++;
        k += 1;
    }

    struct image img = {width, height, data};

    closeFile(file);
    return img;
}

static struct image rotate(struct image img){



}


/*
     * int[][] b = new int[a.length][a.length];
        for (int i = 0; i < b.length; i++) {
            for (int j = 0; j < b.length; j++) {
                b[j][i] = a[a.length - i - 1][j];
            }
        }
     */

/*unsigned char* rotate(const struct bmp_header* head, unsigned char** img) {
    unsigned char** newimg;
    int width = bmpHeader.biWidth;
    int height = bmpHeader.biHeight;
    int trueWidth = calculate(width);

    //newimg = (unsigned char **)malloc(height*sizeof(trueWidth));
    newimg = img;
    for(int i = 0; i < height; i++) {
        newimg[i] = (unsigned char *)malloc(trueWidth*sizeof(char));
        for (int j = 0; j < trueWidth; j+=3){}

    }
    return  newimg;
}*/

static int saveBMP(const char* path, struct image  img) {
    FILE* file = openWriteFile(path) ;
    char *c;
    uint64_t width = img.width;
    uint64_t height = img.height;
    uint64_t trueWidth = calculate(width);
    uint64_t pix = trueWidth*height/3;
    fwrite(&bmpHeader, sizeof(bmpHeader), 1, file);

    int size = pix * sizeof(struct pixel);

    c = (char *)&pix;
    for (int i = 0; i<sizeof(int); i++)
    {
        putc(*c++, file);
    }

    c = (char *)img.data;
    for (int i = 0; i < size; i++)
    {
        putc(*c, file);
        c++;
    }


    closeFile(file);
    return 1;
}



int rotateImage(const char* filepath, const char* resPath){
    struct image img = readBMP(filepath);
    saveBMP(resPath, img);
}