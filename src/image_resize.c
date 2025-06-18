#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "../include/stb_image_resize2.h"

#include "../include/pixel_art.h"

Image* resize_image(const Image* src, int new_width, int new_height) {
    if (!src || !src->data || new_width <= 0 || new_height <= 0) {
        fprintf(stderr, "Error: invalid parameters for resize_image\n");
        return NULL;
    }

    Image* dst = malloc(sizeof(Image));
    if (!dst) {
        fprintf(stderr, "Error: failed to allocate memory for resized image\n");
        return NULL;
    }

    dst->width = new_width;
    dst->height = new_height;
    dst->channels = src->channels;
    dst->data = malloc(new_width * new_height * src->channels);

    if (!dst->data) {
        fprintf(stderr, "Error: failed to allocate memory for resized image data\n");
        free(dst);
        return NULL;
    }

    unsigned char *result = stbir_resize_uint8_linear(
        src->data, src->width, src->height, 0,
        dst->data, new_width, new_height, 0,
        (stbir_pixel_layout)src->channels
    );

    if (!result) {
        fprintf(stderr, "Error: failed to resize image\n");
        free(dst->data);
        free(dst);
        return NULL;
    }

    printf("Resized image from %dx%d to %dx%d\n", src->width, src->height, new_width, new_height);
    return dst;
}


Image* resize_nearest_neighbor(const Image* src, int new_width, int new_height) {
    if (!src || !src->data || new_width <= 0 || new_height <= 0) {
        fprintf(stderr, "Error: invalid parameters for resize_nearest_neighbor\n");
        return NULL;
    }

    Image* dst = malloc(sizeof(Image));
    if (!dst) {
        fprintf(stderr, "Error: failed to allocate memory for resized image\n");
        return NULL;
    }

    dst->width = new_width;
    dst->height = new_height;
    dst->channels = src->channels;
    dst->data = malloc(new_width * new_height * src->channels);

    if (!dst->data) {
        fprintf(stderr, "Error: failed to allocate memory for resized image data\n");
        free(dst);
        return NULL;
    }

    float x_scale = (float)src->width / new_width;
    float y_scale = (float)src->height / new_height;

    for (int y = 0; y < new_height; y++) {
        for (int x = 0; x < new_width; x++) {
            
            int src_x = (int)(x * x_scale);
            int src_y = (int)(y * y_scale);
            
            if (src_x >= src->width) src_x = src->width - 1;
            if (src_y >= src->height) src_y = src->height - 1;

            int src_idx = (src_y * src->width + src_x) * src->channels;
            int dst_idx = (y * new_width + x) * dst->channels;

            for (int c = 0; c < src->channels; c++) {
                dst->data[dst_idx + c] = src->data[src_idx + c];
            }
        }
    }

    printf("Resized image using nearest neighbor from %dx%d to %dx%d\n", 
           src->width, src->height, new_width, new_height);
    return dst;
}
