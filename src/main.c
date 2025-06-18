#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "../include/pixel_art.h"

void print_usage(const char* program_name) {
    printf("Usage: %s [OPTIONS] input_image output_image\n", program_name);
    printf("\nConvert images to pixel art style\n");
    printf("\nOptions:\n");
    printf("  -s, --size PIXELS     Pixel size (default: 8)\n");
    printf("  -c, --colors COLORS   For retro mode only (default: 64)\n");
    printf("  -p, --palette         Use predefined 8-bit palette\n");
    printf("  -n, --no-quantize     Preserve all original colors\n");
    printf("  -i, --info            Show input image information\n");
    printf("  -h, --help            Show this help message\n");
    printf("\nExamples:\n");
    printf("  %s input.jpg output.png\n", program_name);
    printf("  %s -s 4 photo.jpg pixel_art.png\n", program_name);
    printf("  %s -p -s 16 image.png retro.png\n", program_name);
    printf("\nSupported formats: JPEG, PNG, TGA, BMP, PSD, GIF, HDR, PIC\n");
}

int main(int argc, char* argv[]) {
    int pixel_size = 8;
    int max_colors = 64;
    int use_palette = 0;
    int preserve_colors = 0;
    int show_info = 0;
    char* input_file = NULL;
    char* output_file = NULL;

    static struct option long_options[] = {
        {"size",        required_argument, 0, 's'},
        {"colors",      required_argument, 0, 'c'},
        {"palette",     no_argument,       0, 'p'},
        {"no-quantize", no_argument,       0, 'n'},
        {"info",        no_argument,       0, 'i'},
        {"help",        no_argument,       0, 'h'},
        {0, 0, 0, 0}
    };

    int option_index = 0;
    int c;

    while ((c = getopt_long(argc, argv, "s:c:pnih", long_options, &option_index)) != -1) {
        switch (c) {
            case 's':
                pixel_size = atoi(optarg);
                if (pixel_size <= 0) {
                    fprintf(stderr, "Error: pixel size must be positive\n");
                    return 1;
                }
                break;
            case 'c':
                max_colors = atoi(optarg);
                if (max_colors <= 0) {
                    fprintf(stderr, "Error: number of colors must be positive\n");
                    return 1;
                }
                break;
            case 'p':
                use_palette = 1;
                break;
            case 'n':
                preserve_colors = 1;
                break;
            case 'i':
                show_info = 1;
                break;
            case 'h':
                print_usage(argv[0]);
                return 0;
            case '?':
                print_usage(argv[0]);
                return 1;
            default:
                abort();
        }
    }

    if (optind + 2 != argc) {
        fprintf(stderr, "Error: input and output files are required\n");
        print_usage(argv[0]);
        return 1;
    }

    input_file = argv[optind];
    output_file = argv[optind + 1];

    printf("Image to Pixel Art Converter\n");
    printf("============================\n");
    printf("Input file: %s\n", input_file);
    printf("Output file: %s\n", output_file);
    printf("Pixel size: %d\n", pixel_size);
    if (preserve_colors) {
        printf("Mode: Preserve original colors (blockiness only)\n");
    } else if (use_palette) {
        printf("Using predefined 8-bit palette\n");
    } else {
        printf("Max colors: %d\n", max_colors);
    }
    printf("\n");

    printf("Loading image...\n");
    Image* input_image = load_image(input_file);
    if (!input_image) {
        fprintf(stderr, "Error: failed to load input image\n");
        return 1;
    }

    if (show_info) {
        print_image_info(input_image);
        printf("\n");
    }

    Image* pixel_art_image;
    if (preserve_colors) {
        pixel_art_image = convert_to_pixel_art_preserve_colors(input_image, pixel_size);
    } else if (use_palette) {
        pixel_art_image = convert_to_pixel_art_with_palette(input_image, pixel_size);
    } else {
        pixel_art_image = convert_to_pixel_art(input_image, pixel_size, max_colors);
    }

    if (!pixel_art_image) {
        fprintf(stderr, "Error: failed to convert image to pixel art\n");
        free_image(input_image);
        return 1;
    }

    printf("\nSaving pixel art image...\n");
    if (!save_image(output_file, pixel_art_image)) {
        fprintf(stderr, "Error: failed to save output image\n");
        free_image(input_image);
        free_image(pixel_art_image);
        return 1;
    }

    printf("\nConversion complete! Pixel art saved to: %s\n", output_file);

    free_image(input_image);
    free_image(pixel_art_image);

    return 0;
}
