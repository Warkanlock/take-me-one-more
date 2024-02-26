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
