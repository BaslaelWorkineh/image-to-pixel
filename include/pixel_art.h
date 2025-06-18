#ifndef PIXEL_ART_H
#define PIXEL_ART_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

typedef struct {
    unsigned char* data;
    int width;
    int height;
    int channels;
} Image;

typedef struct {
    uint8_t r, g, b;
} Color;

typedef struct {
    Color* colors;
    int count;
    int capacity;
} Palette;

Image* load_image(const char* filename);
int save_image(const char* filename, const Image* img);
void free_image(Image* img);
Image* resize_image(const Image* src, int new_width, int new_height);
Image* resize_nearest_neighbor(const Image* src, int new_width, int new_height);
Palette* create_palette(int capacity);
void free_palette(Palette* palette);
void add_color_to_palette(Palette* palette, uint8_t r, uint8_t g, uint8_t b);
Image* quantize_colors(const Image* src, const Palette* palette);
Image* convert_to_pixel_art(const Image* src, int pixel_size, int max_colors);
Image* convert_to_pixel_art_with_palette(const Image* src, int pixel_size);
Image* convert_to_pixel_art_preserve_colors(const Image* src, int pixel_size);
void print_image_info(const Image* img);
double color_distance(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2);
Color find_closest_color(uint8_t r, uint8_t g, uint8_t b, const Palette* palette);

#endif
