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

void swap_int(int* const a, int* const b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void draw_line(color_t* const buffer, int a_x, int a_y, int b_x, int b_y, uint8_t r, uint8_t g, uint8_t b) {
    
    assert(a_x >= 0 && a_x < WIDTH);
    assert(b_x >= 0 && b_x < WIDTH);
    assert(a_y >= 0 && a_y < HEIGHT);
    assert(b_y >= 0 && b_y < HEIGHT);
    
    if (abs(a_y - b_y) > abs(a_x - b_x)) {
        if (a_y < b_y)
        {
            if (a_x < b_x) {
                for (double y = a_y; y <= b_y; y += 0.001) {
                    double t = (y - a_y) / (b_y - a_y);
                    int x = a_x + t * (b_x - a_x);
                    
                    assert((int)y * WIDTH + x < HEIGHT * WIDTH);
                    
                    
                    buffer[(int)y * WIDTH + x].r = r;
                    buffer[(int)y * WIDTH + x].g = g;
                    buffer[(int)y * WIDTH + x].b = b;
                }
            }
            else if (a_x > b_x) {
                swap_int(&a_x, &b_x);
                swap_int(&a_y, &b_y);
                for (double y = a_y; y <= b_y; y += 0.001) {
                    double t = (y - a_y) / (b_y - a_y);
                    int x = a_x + t * (b_x - a_x);
                    
                   
                    assert((int)y * WIDTH + x < HEIGHT * WIDTH);
                    
                    
                    buffer[(int)y * WIDTH + x].r = r;
                    buffer[(int)y * WIDTH + x].g = g;
                    buffer[(int)y * WIDTH + x].b = b;
                }
            }
            else if (a_x == b_x) {
                const int x = a_x;
                for (double y = a_y; y <= b_y; y += 0.001) {
                    assert((int)y * WIDTH + x < HEIGHT * WIDTH);
                    
                    
                    buffer[(int)y * WIDTH + x].r = r;
                    buffer[(int)y * WIDTH + x].g = g;
                    buffer[(int)y * WIDTH + x].b = b;
                }
            }
        } else if (a_y > b_y) {
            if (a_x < b_x) {
                // this case is wrong.
                // Performing test #15095...
                // (a_x, a_y) = (151, 302) (b_x, b_y) = (402, 0)
                for (double y = b_y; y <= a_y; y += 0.001) {
                    double t = (y - a_y) / (b_y - a_y);
                    int x = a_x + t * (b_x - a_x); // t < 0
                    // fprintf(stderr, "x = %d\tt = %f\n", x, t);
                    assert((int)y * WIDTH + x < HEIGHT * WIDTH);
                    
                    buffer[(int)y * WIDTH + x].r = r;
                    buffer[(int)y * WIDTH + x].g = g;
                    buffer[(int)y * WIDTH + x].b = b;
                }
            }
            else if (a_x > b_x) {
                swap_int(&a_x, &b_x);
                swap_int(&a_y, &b_y);
                for (double y = b_y; y <= a_y; y += 0.001) {
                    double t = (y - a_y) / (a_y - b_y);
                    int x = a_x + t * (b_x - a_x);
                    assert((int)y * WIDTH + x < HEIGHT * WIDTH);
                    
                    buffer[(int)y * WIDTH + x].r = r;
                    buffer[(int)y * WIDTH + x].g = g;
                    buffer[(int)y * WIDTH + x].b = b;
                }
            }
            else if (a_x == b_x) {
                const int x = a_x;
                for (double y = b_y; y <= a_y; y += 0.001) {
                    assert((int)y * WIDTH + x < HEIGHT * WIDTH);
                    buffer[(int)y * WIDTH + x].r = r;
                    buffer[(int)y * WIDTH + x].g = g;
                    buffer[(int)y * WIDTH + x].b = b;
                }
            }
        }
        
        return;
    }
    
    if (a_x > b_x) {
        swap_int(&a_x, &b_x);
        swap_int(&a_y, &b_y);
    }
    
    // x(t) = a_x + t * (b_x - a_x);
    // y(t) = a_y + t * (b_y - a_y);
    // t(x) = (x - a_x) / (b_x - a_x);
    int dx = b_x - a_x;
    int dy = b_y - a_y;
    // fprintf(stderr, "dx: %d\tdy: %d\n", dx, dy);

    for (double x = a_x; x <= b_x; x += .001) {
        const double t = (x - a_x) / (double)dx;
        const int y = a_y + t * dy;
        
        const size_t buffer_index = y * WIDTH + x;
        
        assert (buffer_index < WIDTH * HEIGHT);
        
        buffer[buffer_index].r = r;
        buffer[buffer_index].g = g;
        buffer[buffer_index].b = b;
    }
}

void perform_stress_test(color_t* const buffer) {
    time_t current_time;
    time(&current_time);
    srand((unsigned int)current_time);
    fprintf(stderr, "Performing stress test on %p...\n", buffer);
    for (int test = 0; test < 1000; ++test) {
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
    
    /*
    // Triangle
    draw_line(pixels, 50, 50, 300, 400, 255, 0, 0);
    draw_line(pixels, 50, 50, 300, 400, 0, 255, 255);
    draw_line(pixels, 50, 50, 500, 300, 0, 255, 0);
    draw_line(pixels, 300, 400, 500, 300, 0, 0, 255);
    draw_line(pixels, 500, 300, 300, 400, 0, 0, 255);
    */
    
    perform_stress_test(pixels);
    
    if (!write_image("image.ppm", pixels, WIDTH, HEIGHT)) return -1;
    
    puts("Program complete.");
    
    deallocate();
    
    return 0;
}