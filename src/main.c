#include <stdio.h>
#include <malloc.h>
#include <stdint.h>

unsigned char** readBMP(const char* path);
int saveBMP(const char* path, unsigned char** img);
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


int main(void)
{
    const char* filepath = "C:\\Users\\VivoBook\\CLionProjects\\popitka#2\\res\\picture.bmp";
    const char* resPath = "C:\\Users\\VivoBook\\CLionProjects\\popitka#2\\res\\newPicture.bmp";

    unsigned char** img = readBMP(filepath);
    saveBMP(resPath, img);
}


unsigned char** readBMP(const char* path) {
    unsigned char** img;
    FILE* file;
    long long i, j,u;
    long long v;
    file = fopen(path,"rb");
    if(file == NULL) return 0;
    fread(&bmpHeader, sizeof(bmpHeader), 1, file);


    int width = bmpHeader.biWidth;
    int height = bmpHeader.biHeight;
    int trueWidth = (3 * width + 3) & (-4);
    img = (unsigned char*)calloc(trueWidth * height, sizeof(char));
    fread(img, 1, trueWidth * height, file);
    int s = width * height;
    fclose(file);

    return img;
}

int saveBMP(const char* path, unsigned char** img) {
    FILE* file;
    int i, j;
    file = fopen(path,"wb");
    if(file == NULL) return 0;

    int width = bmpHeader.biWidth;
    int height = bmpHeader.biHeight;
    int trueWidth = (3 * width + 3) & (-4);
    int filesize = 54 + height * trueWidth;

    fwrite(&bmpHeader, sizeof(bmpHeader), 1, file);
    fwrite(img, sizeof(char), trueWidth * height, file);
    fclose(file);
    return 1;
}