#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"

#include "../include/pixel_art.h"
#include <ctype.h>

Image* load_image(const char* filename) {
    if (!filename) {
        fprintf(stderr, "Error: filename is NULL\n");
        return NULL;
    }

    Image* img = malloc(sizeof(Image));
    if (!img) {
        fprintf(stderr, "Error: failed to allocate memory for Image structure\n");
        return NULL;
    }

    img->data = stbi_load(filename, &img->width, &img->height, &img->channels, 0);

    if (!img->data) {
        fprintf(stderr, "Error: failed to load image '%s': %s\n", filename, stbi_failure_reason());
        free(img);
        return NULL;
    }

    printf("Loaded image: %s (%dx%d, %d channels)\n", filename, img->width, img->height, img->channels);
    return img;
}

int save_image(const char* filename, const Image* img) {
    if (!filename || !img || !img->data) {
        fprintf(stderr, "Error: invalid parameters for save_image\n");
        return 0;
    }

    const char* ext = strrchr(filename, '.');
    if (!ext) {
        fprintf(stderr, "Error: no file extension found in '%s'\n", filename);
        return 0;
    }
    ext++;

    int result = 0;
    char ext_lower[10];
    strncpy(ext_lower, ext, sizeof(ext_lower) - 1);
    ext_lower[sizeof(ext_lower) - 1] = '\0';
    for (int i = 0; ext_lower[i]; i++) {
        ext_lower[i] = tolower(ext_lower[i]);
    }

    if (strcmp(ext_lower, "png") == 0) {
        result = stbi_write_png(filename, img->width, img->height, img->channels, img->data, img->width * img->channels);
    } else if (strcmp(ext_lower, "bmp") == 0) {
        result = stbi_write_bmp(filename, img->width, img->height, img->channels, img->data);
    } else if (strcmp(ext_lower, "tga") == 0) {
        result = stbi_write_tga(filename, img->width, img->height, img->channels, img->data);
    } else if (strcmp(ext_lower, "jpg") == 0 || strcmp(ext_lower, "jpeg") == 0) {
        result = stbi_write_jpg(filename, img->width, img->height, img->channels, img->data, 90); // 90% quality
    } else {
        fprintf(stderr, "Error: unsupported file format '%s'\n", ext);
        return 0;
    }

    if (result) {
        printf("Saved image: %s (%dx%d, %d channels)\n", filename, img->width, img->height, img->channels);
    } else {
        fprintf(stderr, "Error: failed to save image '%s'\n", filename);
    }

    return result;
}

void free_image(Image* img) {
    if (img) {
        if (img->data) {
            stbi_image_free(img->data);
        }
        free(img);
    }
}

void print_image_info(const Image* img) {
    if (!img) {
        printf("Image: NULL\n");
        return;
    }

    printf("Image Info:\n");
    printf("  Dimensions: %dx%d\n", img->width, img->height);
    printf("  Channels: %d\n", img->channels);
    printf("  Data size: %d bytes\n", img->width * img->height * img->channels);
}
