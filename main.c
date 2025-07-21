#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

enum {
    WIDTH = 800,
    HEIGHT = 600
};

typedef struct color {
    uint8_t r, g, b;
} color_t;

color_t* pixels;

inline bool initialize(void) {
    pixels = malloc(WIDTH * HEIGHT * sizeof(color_t));
    return pixels ? true : false;
}

inline bool deallocate() {
    if (pixels) free(pixels);
    return true;
}

bool write_image(const char* name, const color_t* const buffer, const unsigned int width, const unsigned int height) {
    
    assert(name && buffer);
    
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

inline void swap_int(int* const a, int* const b) {
    assert(a && b);
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

inline void draw_point(color_t* const buffer, int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
    size_t buffer_index = y * WIDTH + x;
    assert (buffer_index < WIDTH * HEIGHT);
    
    buffer[buffer_index].r = r;
    buffer[buffer_index].g = g;
    buffer[buffer_index].b = b;
}

void draw_line(color_t* const buffer, int a_x, int a_y, int b_x, int b_y, uint8_t r, uint8_t g, uint8_t b) {
    
    assert(a_x >= 0 && a_x < WIDTH);
    assert(b_x >= 0 && b_x < WIDTH);
    assert(a_y >= 0 && a_y < HEIGHT);
    assert(b_y >= 0 && b_y < HEIGHT);
    
    int dx = b_x - a_x;
    int dy = b_y - a_y;
    const bool transpose = abs(dy) > abs(dx);
    if (transpose) {
        swap_int(&a_x, &a_y);
        swap_int(&b_x, &b_y);
        swap_int(&dx, &dy);
    }
    
    if (a_x > b_x) {
        swap_int(&a_x, &b_x);
        swap_int(&a_y, &b_y);
        dx = ~dx + 1;
        dy = ~dy + 1;
    }
    
    const double diff = dy * .001 / dx;
    double y = a_y;

    for (double x = a_x; x <= b_x; x += .001) {
        y += diff;
        
        if (transpose) {
            draw_point(pixels, (int)y, x, r, g, b);
            continue;
        }
        
        draw_point(pixels, x, (int)y, r, g, b);
    }
}

void perform_stress_test(color_t* const buffer, size_t count) {
    time_t current_time;
    time(&current_time);
    srand((unsigned int)current_time);
    fprintf(stderr, "Performing stress test on %p...\n", buffer);
    for (size_t test = 0; test < count; ++test) {
        // fprintf(stderr, "Performing test #%d...\n", test);
        const int a_y = rand() % (HEIGHT - 1);
        const int b_y = rand() % (HEIGHT - 1);
        const int a_x = rand() % (WIDTH - 1);
        const int b_x = rand() % (WIDTH - 1);
        const uint8_t r = rand() % 255;
        const uint8_t g = rand() % 255;
        const uint8_t b = rand() % 255;
        // fprintf(stderr, "(a_x, a_y) = (%d, %d)\t(b_x, b_y) = (%d, %d)\n", a_x, a_y, b_x, b_y);
        draw_line(buffer, a_x, a_y, b_x, b_y, r, g, b);
    }
    fprintf(stderr, "Stress test complete.\n");
}

int main(void) {
    
    if (!initialize()) return -1;
    
    // Triangle
    draw_line(pixels, 50, 50, 300, 400, 255, 0, 0);
    draw_line(pixels, 50, 50, 300, 400, 0, 255, 255);
    draw_line(pixels, 50, 50, 500, 300, 0, 255, 0);
    draw_line(pixels, 300, 400, 500, 300, 0, 0, 255);
    draw_line(pixels, 500, 300, 300, 400, 0, 0, 255);
    
    perform_stress_test(pixels, 593);
    
    draw_line(pixels, 50, 50, 300, 400, 255, 0, 0);
    
    if (!write_image("image.ppm", pixels, WIDTH, HEIGHT)) return -1;
    
    puts("Program complete.");
    
    deallocate();
    
    return 0;
}