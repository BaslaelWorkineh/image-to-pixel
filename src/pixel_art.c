#include "../include/pixel_art.h"

Image* convert_to_pixel_art(const Image* src, int pixel_size, int max_colors) {
    if (!src || !src->data || pixel_size <= 0) {
        fprintf(stderr, "Error: invalid parameters for convert_to_pixel_art\n");
        return NULL;
    }

    (void)max_colors;
    return convert_to_pixel_art_preserve_colors(src, pixel_size);
}




Palette* create_8bit_palette() {
    Palette* palette = create_palette(256);
    if (!palette) return NULL;

    // Add some classic 8-bit colors
    // Black and white
    add_color_to_palette(palette, 0, 0, 0);
    add_color_to_palette(palette, 255, 255, 255);
    
    // Primary colors
    add_color_to_palette(palette, 255, 0, 0);
    add_color_to_palette(palette, 0, 255, 0);
    add_color_to_palette(palette, 0, 0, 255); 
    
    // Secondary colors
    add_color_to_palette(palette, 255, 255, 0); 
    add_color_to_palette(palette, 255, 0, 255);
    add_color_to_palette(palette, 0, 255, 255);
    // Some grays
    add_color_to_palette(palette, 128, 128, 128); 
    add_color_to_palette(palette, 64, 64, 64);  
    add_color_to_palette(palette, 192, 192, 192);
    
    // Some earth tones
    add_color_to_palette(palette, 139, 69, 19); 
    add_color_to_palette(palette, 255, 165, 0); 
    add_color_to_palette(palette, 128, 0, 128); 
    add_color_to_palette(palette, 0, 128, 0); 
    add_color_to_palette(palette, 0, 0, 128); 

    return palette;
}

Image* convert_to_pixel_art_with_palette(const Image* src, int pixel_size) {
    if (!src || !src->data || pixel_size <= 0) {
        fprintf(stderr, "Error: invalid parameters for convert_to_pixel_art_with_palette\n");
        return NULL;
    }

    printf("Converting image to pixel art with 8-bit palette (pixel_size=%d)\n", pixel_size);

    int low_width = src->width / pixel_size;
    int low_height = src->height / pixel_size;
    

    if (low_width < 1) low_width = 1;
    if (low_height < 1) low_height = 1;

    printf("Step 1: Resizing to low resolution (%dx%d)\n", low_width, low_height);
    Image* low_res = resize_image(src, low_width, low_height);
    if (!low_res) {
        fprintf(stderr, "Error: failed to create low resolution image\n");
        return NULL;
    }

    printf("Step 2: Quantizing colors using 8-bit palette\n");
    Palette* palette = create_8bit_palette();
    if (!palette) {
        free_image(low_res);
        return NULL;
    }

    Image* quantized = quantize_colors(low_res, palette);
    free_image(low_res);
    free_palette(palette);
    if (!quantized) {
        fprintf(stderr, "Error: failed to quantize colors\n");
        return NULL;
    }

    printf("Step 3: Scaling back up to %dx%d using nearest neighbor\n", src->width, src->height);
    Image* pixel_art = resize_nearest_neighbor(quantized, src->width, src->height);
    free_image(quantized);
    if (!pixel_art) {
        fprintf(stderr, "Error: failed to scale up pixel art\n");
        return NULL;
    }

    printf("Pixel art conversion with palette complete!\n");
    return pixel_art;
}

Image* convert_to_pixel_art_preserve_colors(const Image* src, int pixel_size) {
    if (!src || !src->data || pixel_size <= 0) {
        fprintf(stderr, "Error: invalid parameters for convert_to_pixel_art_preserve_colors\n");
        return NULL;
    }

    printf("Converting image to high-quality pixel art (pixel_size=%d, no color reduction)\n", pixel_size);

    Image* dst = malloc(sizeof(Image));
    if (!dst) {
        fprintf(stderr, "Error: failed to allocate memory for pixel art image\n");
        return NULL;
    }

    dst->width = src->width;
    dst->height = src->height;
    dst->channels = src->channels;
    dst->data = malloc(src->width * src->height * src->channels);

    if (!dst->data) {
        fprintf(stderr, "Error: failed to allocate memory for pixel art image data\n");
        free(dst);
        return NULL;
    }

    printf("Creating pixel blocks of size %dx%d with original colors\n", pixel_size, pixel_size);

    for (int block_y = 0; block_y < src->height; block_y += pixel_size) {
        for (int block_x = 0; block_x < src->width; block_x += pixel_size) {

            int sample_x = block_x + pixel_size / 2;
            int sample_y = block_y + pixel_size / 2;

            if (sample_x >= src->width) sample_x = src->width - 1;
            if (sample_y >= src->height) sample_y = src->height - 1;

            int sample_idx = (sample_y * src->width + sample_x) * src->channels;

            for (int y = block_y; y < block_y + pixel_size && y < src->height; y++) {
                for (int x = block_x; x < block_x + pixel_size && x < src->width; x++) {
                    int dst_idx = (y * dst->width + x) * dst->channels;

                    for (int c = 0; c < src->channels; c++) {
                        dst->data[dst_idx + c] = src->data[sample_idx + c];
                    }
                }
            }
        }
    }

    printf("High-quality pixel art conversion complete!\n");
    return dst;
}
