#ifndef POPITKA_2_IMG_H
#define POPITKA_2_IMG_H
struct pixel { uint8_t b, g, r; };

struct image {
    uint64_t width, height;
    struct pixel* data;
};
#endif //POPITKA_2_IMG_H
