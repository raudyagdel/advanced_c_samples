//
// Created by raudy on 1/31/2024.
//
#include "image.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <jpeglib.h>
#include <time.h>

void generate_rgb_color(const char *filename) {
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    JSAMPLE* image_buffer;
    int row_stride;

    FILE* outfile = fopen(filename, "wb");
    if (!outfile) {
        fprintf(stderr, "Error opening output file\n");
        return;
    }

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = 256;
    cinfo.image_height = 256;
    cinfo.input_components = 3; // RGB channels
    cinfo.in_color_space = JCS_RGB; // Color space

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, 100, TRUE);

    jpeg_start_compress(&cinfo, TRUE);

    row_stride = cinfo.image_width * 3; // RGB components
    image_buffer = (JSAMPLE*)malloc(row_stride);


    // Random By Seed
    srand((unsigned int)time(NULL));

    // Random RGB Color Values
    JSAMPLE red = rand() % 255;
    JSAMPLE green = rand() % 255;
    JSAMPLE blue = rand() % 255;

    while (cinfo.next_scanline < cinfo.image_height) {
        for (int i = 0; i < row_stride; i += 3) {
            // Set RGB values for each pixel
            image_buffer[i] = red;
            image_buffer[i + 1] = green;
            image_buffer[i + 2] = blue;
        }
        jpeg_write_scanlines(&cinfo, &image_buffer, 1);
    }

    free(image_buffer);
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    fclose(outfile);
}

int get_random_value(int min, int max) {
    return min + rand() % (max - min + 1);
}

void generate_random_image(const char *filename, int width, int height) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing\n");
        return;
    }

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    jpeg_stdio_dest(&cinfo, file);

    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, 75, TRUE);

    jpeg_start_compress(&cinfo, TRUE);

    JSAMPLE *row = (JSAMPLE *)malloc(3 * cinfo.image_width * sizeof(JSAMPLE));
    while (cinfo.next_scanline < cinfo.image_height) {
        for (int i = 0; i < cinfo.image_width; i++) {
            row[i * 3] = (JSAMPLE)get_random_value(0, 255); // Red channel
            row[i * 3 + 1] = (JSAMPLE)get_random_value(0, 255); // Green channel
            row[i * 3 + 2] = (JSAMPLE)get_random_value(0, 255); // Blue channel
        }
        jpeg_write_scanlines(&cinfo, &row, 1);
    }

    free(row);

    jpeg_finish_compress(&cinfo);
    fclose(file);

    jpeg_destroy_compress(&cinfo);
}

void read_in_blocks(const char *filename) {
    // Open input file name
    FILE *fileIn = fopen(filename, "rb");
    if (fileIn == NULL) {
        fprintf(stderr, "Error opening fileIn\n");
        return;
    }

    // Allocate memory for the buffer
    char *buffer = (char *) malloc(BUFFER_SIZE);
    if (buffer == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        fclose(fileIn);
        return;
    }

    // Create output file name
    char* out_filename[FILENAME_MAX];
    get_copy_file_name(filename, out_filename);


    // Open the output file for writing
    FILE* fileOut = fopen(out_filename, "wb"); // Open a new file for writing in binary mode
    if (fileOut == NULL) {
        fprintf(stderr, "Error opening output file\n");
        free(buffer);
        fclose(fileOut);
        return;
    }

    // Read and process the fileIn in blocks of BUFFER_SIZE bytes
    size_t bytes_read;
    do {
        bytes_read = fread(buffer, 1, BUFFER_SIZE, fileIn);
        // Write the block of data to the output file
        if (bytes_read > 0) {
            fwrite(buffer, 1, bytes_read, fileOut);
        }
    } while (bytes_read == BUFFER_SIZE);

    // Check for errors or end-of-fileIn
    if (ferror(fileIn)) {
        fprintf(stderr, "Error reading fileIn\n");
    } else {
        printf("File read and copy successfully\n");
    }

    // Clean up
    free(buffer);
    fclose(fileIn);
}


void get_copy_file_name(const char* filename, char* result) {
    // Find the last occurrence of '.'
    const char* lastDot = strrchr(filename, '.');

    // Create copy name
    if (lastDot != NULL) {
        size_t length = lastDot - filename;
        strncpy(result, filename, length);
        result[length] = '\0';
        strcat(result, "_copy");
        strcat(result, lastDot);
    } else {
        strcpy(result, filename);
        strcat(result, "_copy");
    }
}