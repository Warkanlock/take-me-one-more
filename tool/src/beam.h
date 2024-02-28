#ifndef BEAM_H
#define BEAM_H

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

#endif
