#ifndef POPITKA_2_PICTURE_H
#define POPITKA_2_PICTURE_H

#include <stdint.h>

struct pixel { uint8_t b, g, r; };

struct image {
    uint64_t width;
    uint64_t height;
    struct pixel* data;
};
void makeNewImage(struct image* , uint64_t , uint64_t);
struct image imageRotate(struct image);
void image_free(struct image* img);
#endif //POPITKA_2_PICTURE_H
