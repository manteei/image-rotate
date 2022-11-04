#include "../include/fileManager.h"
#include "../include/picture.h"
#define BMP_TYPE 0x4d42
#define BMP_SIZE 40
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


static size_t calculatePadding(const uint64_t width) {
    return (4 - width * 3 % 4) % 4;
}

static struct bmp_header fillBMPHeader(const uint64_t width, const uint64_t height) {
    const size_t padding = calculatePadding(width);
    const uint32_t bOffBits = 54;
    struct bmp_header bmpHead = {
            .biWidth = width,
            .biHeight = height,
            .bfileSize = bOffBits + height * (width + padding) * 3 + padding,
            .biSizeImage = width * height * 3,
            .bfType = BMP_TYPE,
            .bfReserved = 0,
            .bOffBits = bOffBits,
            .biSize = BMP_SIZE,
            .biPlanes = 1,
            .biBitCount = 24,
            .biCompression = 0,
            .biXPelsPerMeter = 0,
            .biYPelsPerMeter = 0,
            .biClrUsed = 0,
            .biClrImportant = 0
    };
    return bmpHead;

}

static struct image readBMP(FILE* file) {
    struct image img;
    fread(&bmpHeader, sizeof(bmpHeader), 1, file);
    uint64_t width = bmpHeader.biWidth;
    uint64_t height = bmpHeader.biHeight;
    makeNewImage(&img, height, width);

    const long padding = (long)calculatePadding(img.width);

    for (size_t i = 0; i < height; i++) {
        fread(&(img.data[i * width]), sizeof(struct pixel), width, file);
        fseek(file, padding, SEEK_CUR);
    }

    return img;
}

static void saveBMP(FILE* file, struct image  img) {
    const struct pixel zero[] = {{0},{0},{0}};
    uint64_t width = img.width;
    uint64_t height = img.height;
    const struct bmp_header header = fillBMPHeader(width, height);
    fwrite(&header, sizeof(bmpHeader), 1, file);
    const size_t padding = calculatePadding(img.width);

    for (size_t i = 0; i < height; i++) {
        fwrite(&(img.data[i * width]), sizeof(struct pixel), width, file);
        fwrite(zero, 1, padding, file);
    }
}

void makeFinalRotate(const char* filepath, const char* resPath){
    FILE* file = openReadFile(filepath);
    FILE* newfile = openWriteFile(resPath) ;
    struct image img = imageRotate(readBMP(file));
    closeFile(file);
    saveBMP(newfile, img);
    closeFile(newfile);
    image_free(&img);
}