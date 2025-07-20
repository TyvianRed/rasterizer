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
                buffer[h * WIDTH + w].r,
                buffer[h * WIDTH + w].g,
                buffer[h * WIDTH + w].b
            );
        }
        fputc('\n', image);
    }
    
    fclose(image);
    
    return true;
}

void draw_line(color_t* const buffer, int a_x, int a_y, int b_x, int b_y, uint8_t r, uint8_t g, uint8_t b) {
    
    // x(t) = a_x + t * (b_x - a_x);
    // y(t) = a_y + t * (b_y - a_y);
    const int dx = b_x - a_x, dy = b_y - a_y;
    
    fprintf(stderr, "b_x: %d\tb_y: %d\n", b_x, b_y);
    
    for (double t = 0; t <= 1; t += .001) {
        const int x = a_x + t * dx;
        const int y = b_y + t * dy;
        const size_t buffer_index = y * WIDTH + x;
        // fprintf(stderr, "x: %d\ty: %d\tdx: %d\tdy: %d\n", x, y, dx, dy);
        fprintf(stderr, "t: %f\tx: %d\ty: %d\tbuffer_index: %llu\n", t, x, y, buffer_index);
        buffer[buffer_index].r = r;
        buffer[buffer_index].g = g;
        buffer[buffer_index].b = b;
    }
}

int main(void) {
    
    if (!initialize()) return -1;
    
    draw_line(pixels, 100, 100, 300, 300, 255, 0, 0);
    
    if (!write_image("image.ppm", pixels, WIDTH, HEIGHT)) return -1;
    
    puts("Program complete.");
    
    deallocate();
    
    return 0;
}