#include <stdint.h>
#include <stdio.h>
#include <malloc.h>
#include "fileManager.h"
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

const uint8_t trash_byte = 0;

struct pixel { uint8_t b, g, r; }pixel;

struct image {
    uint64_t width;
    uint64_t height;
    struct pixel* data;
};

static int calculate(int width){
    return (3 * width + 3) & (-4);
}//(4 - width * 3 % 4) % 4;

static size_t calculate_padding(const uint64_t width) {
    return (4 - width * 3 % 4) % 4;
}
static struct image readBMP(const char* path) {
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
    while ((i= getc(file))!=EOF){
        *c = i;
        c++;
    }
    struct image img = {width, height, data};
    closeFile(file);
    return img;
}

static struct bmp_header bmp_header_init(const uint64_t width, const uint64_t height) {
    const size_t padding = calculate_padding(width);
    const uint32_t bOffBits = 54;
    return (struct bmp_header) {
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

}

static struct image imageNewStruct(const struct image* img) {
    return (struct image) {
            .width = img->height,
            .height = img->width,
            .data = malloc(img->height * img->width * sizeof(struct pixel))
    };
}
static uint64_t calculateNewAddress(const struct image* img, uint64_t i, uint64_t j) {
    return img->height - i + 1 + j * img->height;
}

struct image image_rotate(const struct image img) {
    struct image newImage = imageNewStruct(&img);

    for (uint64_t i = 0; i < img.height; i++) {

        for (uint64_t j = 0; j < img.width; j++) {
            newImage.data[calculateNewAddress(&img, i, j)] =
                    img.data[i * img.width + j];
        }
    }

    return newImage;
}




static int saveBMP(const char* path, struct image  img) {
    FILE* file = openWriteFile(path) ;
    uint64_t width = img.width;
    uint64_t height = img.height;

    const struct bmp_header header = bmp_header_init(width, height);
    fwrite(&header, sizeof(bmpHeader), 1, file);
    const size_t padding = calculate_padding(img.width);

    for (size_t i = 0; i < height; i++) {
        fwrite(&(img.data[i * width]), sizeof(struct pixel), width, file);
        fwrite(&trash_byte, sizeof(trash_byte), padding, file);
    }
    closeFile(file);
    return 1;
}

int rotateImage(const char* filepath, const char* resPath){
    struct image img = image_rotate(readBMP(filepath));
    saveBMP(resPath, img);
}