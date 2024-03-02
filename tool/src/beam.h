#ifndef BEAM_H
#define BEAM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"

typedef struct
{
    unsigned char r, g, b;
} Pixel;

typedef struct
{
    unsigned int width, height;
    unsigned int max_color;
    Pixel** pixels;
} PixelImage;

/** Image treatment */
PixelImage* process_image(char* path);
PixelImage* create_image(unsigned int width, unsigned int height, unsigned int color_depth);
void free_image(PixelImage* img);

/** Difference mechanism */
PixelImage* process_difference(PixelImage* base, PixelImage* current);
void store_difference(PixelImage* difference, File* inception_file);
void read_difference(char* diff_path, PixelImage* difference, int index_diff);
void use_difference(FilesContainer* inception_container);
void compute_difference(FilesContainer* files);

#endif
