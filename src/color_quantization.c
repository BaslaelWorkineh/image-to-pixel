#include "../include/pixel_art.h"
#include <math.h>

Palette* create_palette(int capacity) {
    Palette* palette = malloc(sizeof(Palette));
    if (!palette) {
        fprintf(stderr, "Error: failed to allocate memory for palette\n");
        return NULL;
    }

    palette->colors = malloc(capacity * sizeof(Color));
    if (!palette->colors) {
        fprintf(stderr, "Error: failed to allocate memory for palette colors\n");
        free(palette);
        return NULL;
    }

    palette->count = 0;
    palette->capacity = capacity;
    return palette;
}

void free_palette(Palette* palette) {
    if (palette) {
        if (palette->colors) {
            free(palette->colors);
        }
        free(palette);
    }
}

void add_color_to_palette(Palette* palette, uint8_t r, uint8_t g, uint8_t b) {
    if (!palette || palette->count >= palette->capacity) {
        return;
    }

    palette->colors[palette->count].r = r;
    palette->colors[palette->count].g = g;
    palette->colors[palette->count].b = b;
    palette->count++;
}

double color_distance(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2) {
    int dr = r1 - r2;
    int dg = g1 - g2;
    int db = b1 - b2;
    return sqrt(dr*dr + dg*dg + db*db);
}


Color find_closest_color(uint8_t r, uint8_t g, uint8_t b, const Palette* palette) {
    if (!palette || palette->count == 0) {
        Color default_color = {r, g, b};
        return default_color;
    }

    double min_distance = color_distance(r, g, b, 
                                       palette->colors[0].r, 
                                       palette->colors[0].g, 
                                       palette->colors[0].b);
    int closest_index = 0;

    for (int i = 1; i < palette->count; i++) {
        double distance = color_distance(r, g, b,
                                       palette->colors[i].r,
                                       palette->colors[i].g,
                                       palette->colors[i].b);
        if (distance < min_distance) {
            min_distance = distance;
            closest_index = i;
        }
    }

    return palette->colors[closest_index];
}


Image* quantize_colors(const Image* src, const Palette* palette) {
    if (!src || !src->data || !palette) {
        fprintf(stderr, "Error: invalid parameters for quantize_colors\n");
        return NULL;
    }

    Image* dst = malloc(sizeof(Image));
    if (!dst) {
        fprintf(stderr, "Error: failed to allocate memory for quantized image\n");
        return NULL;
    }

    dst->width = src->width;
    dst->height = src->height;
    dst->channels = src->channels;
    dst->data = malloc(src->width * src->height * src->channels);

    if (!dst->data) {
        fprintf(stderr, "Error: failed to allocate memory for quantized image data\n");
        free(dst);
        return NULL;
    }

    for (int i = 0; i < src->width * src->height; i++) {
        int pixel_idx = i * src->channels;
        
        uint8_t r = src->data[pixel_idx];
        uint8_t g = src->data[pixel_idx + 1];
        uint8_t b = src->data[pixel_idx + 2];

        Color closest = find_closest_color(r, g, b, palette);

        dst->data[pixel_idx] = closest.r;
        dst->data[pixel_idx + 1] = closest.g;
        dst->data[pixel_idx + 2] = closest.b;

        if (src->channels == 4) {
            dst->data[pixel_idx + 3] = src->data[pixel_idx + 3];
        }
    }

    printf("Quantized image colors using palette with %d colors\n", palette->count);
    return dst;
}


