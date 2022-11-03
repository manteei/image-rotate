#include <stdio.h>
#include <malloc.h>
#include <stdint.h>
#include "../elem/bmp.h"

int main(void)
{
    const char* filepath = "C:\\Users\\VivoBook\\CLionProjects\\popitka#2\\res\\picture.bmp";
    const char* resPath = "C:\\Users\\VivoBook\\CLionProjects\\popitka#2\\res\\newPicture2.bmp";
    rotateImage(filepath, resPath);
}

