#include <stdint.h>
#include <malloc.h>

struct pixel { uint8_t b, g, r; }pixel;

struct image {
    uint64_t width;
    uint64_t height;
    struct pixel* data;
};

void makeNewImage(struct image* img, const uint64_t height, const uint64_t width) {
    img->height = height;
    img->width = width;
    img->data = malloc(sizeof(struct pixel) * height * width);
}
static struct image imageNewStruct(const struct image* img) {
    return (struct image) {
            .width = img->height,
            .height = img->width,
            .data = malloc(img->height * img->width * sizeof(struct pixel))
    };
}
static uint64_t calculateNewAddress(const struct image* img, uint64_t i, uint64_t j) {
    uint64_t address = img->height - i - 1 + j * img->height;
    return address;
}
void image_free(struct image* img) {
    free(img->data);
}
struct image imageRotate(const struct image img) {
    struct image newImage = imageNewStruct(&img);
    for (uint64_t i = 0; i < img.height; i++) {
        for (uint64_t j = 0; j < img.width; j++) {
            newImage.data[calculateNewAddress(&img, i, j)] = img.data[i * img.width + j];
        }
    }
    free(img.data);
    return newImage;
}
