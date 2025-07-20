#define _CRT_SECURE_NO_WARNINGS

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

enum {
    WIDTH = 800,
    HEIGHT = 600
};

typedef struct color {
    uint8_t r, g, b;
} color_t;

color_t* pixels;

bool initialize(void) {
    pixels = malloc(WIDTH * HEIGHT * sizeof(color_t));
    return pixels ? true : false;
}

bool deallocate(void) {
    free(pixels);
    return true;
}

bool write_image(const char* name, const color_t* const buffer, const unsigned int width, const unsigned int height) {
    
    FILE* const image = fopen(name, "w");
    if (!image) {
        perror("Failed to open image.ppm");
        return false;
    }
    
    fprintf(image, "P3\n%d %d\n256\n", width, height);
    
    for (int h = 0; h < height; ++h) {
        for (int w = 0; w < width; ++w) {
            fprintf(
                image,
                "%d %d %d\t",
                pixels[h * WIDTH + w].r,
                pixels[h * WIDTH + w].g,
                pixels[h * WIDTH + w].b
            );
        }
        fputc('\n', image);
    }
    
    fclose(image);
    
    return true;
}

int main(void) {
    
    if (!initialize()) return -1;
    
    pixels[300 * WIDTH + 400].r = 255;
    pixels[300 * WIDTH + 401].g = 255;
    pixels[300 * WIDTH + 402].b = 255;
    
    if (!write_image("image.ppm", pixels, WIDTH, HEIGHT)) return -1;
    
    puts("Program complete.");
    
    deallocate();
    
    return 0;
}