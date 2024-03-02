#include "beam.h"

/**
 * @brief This function process the content of a path
 *
 * This function is in charge of processing the content of a path
 * and manipulate the bytes of the file.
 *
 * @param path The path of the image to process
 * @return PixelImage* The image object
 */
PixelImage* process_image(char* path)
{
    FILE* file = fopen(path, "r");

    if (file == NULL)
    {
        throw_error("Could not open file. Aborting operation. \n");
    }

    // allocate memory for the content of the file
    PixelImage* image = (PixelImage*)malloc(sizeof(PixelImage));
    fscanf(file, "%d %d %d", &image->width, &image->height, &image->max_color);

    // allocate memory for the pixels of the image (2D array of pixels)
    // e.g:
    // 0,0 0,1 0,2
    // 1,0 1,1 1,2
    // 2,0 2,1 2,2
    // ...
    //
    // where each element is a pixel (r, g, b)
    // e.g:
    // 0,0 = (255, 255, 255)
    // 0,1 = (255, 255, 255)
    image->pixels = (Pixel**)malloc(sizeof(Pixel*) * image->height);

    // allocate memory for each row of pixels
    for (int i = 0; i < image->height; i++)
    {
        image->pixels[i] = (Pixel*)malloc(sizeof(Pixel) * image->width);
    }

    // read pixel data
    for (int i = 0; i < image->height; i++)
    {
        for (int j = 0; j < image->width; j++)
        {
            fscanf(file, "%hhu %hhu %hhu", &image->pixels[i][j].r, &image->pixels[i][j].g, &image->pixels[i][j].b);
        }
    }

    // close the file
    fclose(file);

    // return the image
    return image;
}

/**
 * @brief This function creates an image
 *
 * This function is in charge of creating an image
 * and allocating the memory for it.
 *
 * @param width The width of the image
 * @param height The height of the image
 * @param color_depth The color depth of the image
 * @return PixelImage* The image created
 */
PixelImage* create_image(unsigned int width, unsigned int height, unsigned int color_depth)
{
    PixelImage* img = (PixelImage*)malloc(sizeof(PixelImage));

    // initialize the image
    img->height = height;
    img->width = width;
    img->max_color = color_depth;

    // allocate memory for the pixels
    img->pixels = (Pixel**)malloc(sizeof(Pixel*) * img->height);

    // allocate memory for each row of pixels (otherwise segmentation fault will
    // occur)
    for (int i = 0; i < img->height; i++)
    {
        img->pixels[i] = (Pixel*)malloc(sizeof(Pixel) * img->width);
    }

    return img;
}

/**
 * @brief This function frees the memory used by an image
 *
 * This function is in charge of freeing the memory used by an image
 *
 * @param img The image to free
 * @return void
 */
void free_image(PixelImage* img)
{
    for (int i = 0; i < img->height; i++)
    {
        free(img->pixels[i]);
    }

    free(img->pixels);
    free(img);
}

/**
 * @brief Computes difference between images
 *
 * This function is in charge of computing the difference between images
 *
 * @param base The base image
 * @param current The current image
 *
 * @return PixelImage* The difference between the images
 */
PixelImage* process_difference(PixelImage* base, PixelImage* current)
{
    PixelImage* diff = create_image(base->width, base->height, base->max_color);

    for (int i = 0; i < base->height; i++)
    {
        for (int j = 0; j < base->width; j++)
        {
            Pixel* base_pixel = &base->pixels[i][j];
            Pixel* current_pixel = &current->pixels[i][j];

            // calculate the difference between the pixels (absolute value is
            // needed)
            diff->pixels[i][j].r = abs(current_pixel->r - base_pixel->r);
            diff->pixels[i][j].g = abs(current_pixel->g - base_pixel->g);
            diff->pixels[i][j].b = abs(current_pixel->b - base_pixel->b);
        }
    }

    return diff;
}

/**
 * @brief This function stores the difference between images
 *
 * This function is in charge of storing the difference between images
 * in a binary file.
 *
 * @param difference The difference between images
 * @param inception_file The file to store the difference
 * @param index The index of the difference
 * @return void
 */
void store_difference(PixelImage* difference, File* inception_file)
{
    // create if required path to store the difference
    bool create_path = true;
    char* full_path = get_inference_path(inception_file->path, create_path);

    // open path at full_path
    FILE* file = fopen(full_path, "wb");

    if (file == NULL)
    {
        throw_error("Could not open file to store the difference (maybe you don't "
                    "permissions to write to disk). Aborting operation. \n");
    }

    int pixel_count = difference->width * difference->height;
    size_t data_size = sizeof(Pixel) * pixel_count;
    Pixel* data = (Pixel*)malloc(data_size);

    // store in data pixels from the difference
    for (int i = 0; i < difference->height; i++)
    {
        for (int j = 0; j < difference->width; j++)
        {
            data[i * difference->width + j] = difference->pixels[i][j];
        }
    }

    // write the difference to the file
    int items_written = fwrite(data, sizeof(Pixel), pixel_count, file);

    if (items_written != pixel_count)
    {
        throw_error("Failed to write the difference to the file.\n");
    }

    // close the file
    fclose(file);
}

/**
 * @brief This function reads the difference between images
 *
 * This function is in charge of reading the difference between images
 * from a binary file.
 *
 * @param diff_path The path to the difference file
 * @param difference The difference between images
 * @param index_diff The index of the difference
 */
void read_difference(char* diff_path, PixelImage* difference, int index_diff)
{
    FILE* file = fopen(diff_path, "rb");

    if (file == NULL)
    {
        throw_error("Could not open to read the difference. \n");
    }

    // calculate the position in the file where the desired difference data
    // starts
    int pixel_count = difference->width * difference->height;
    size_t data_size = sizeof(Pixel) * pixel_count;
    Pixel* data = (Pixel*)malloc(data_size);

    // read items from stream
    size_t items_read = fread(data, sizeof(Pixel), pixel_count, file);

    // iterate over data
    for (int i = 0; i < difference->height; i++)
    {
        for (int j = 0; j < difference->width; j++)
        {
            // copy the data to the difference
            difference->pixels[i][j].r = data[i * difference->width + j].r;
            difference->pixels[i][j].g = data[i * difference->width + j].g;
            difference->pixels[i][j].b = data[i * difference->width + j].b;
        }
    }

    if (items_read != pixel_count)
    {
        throw_error("Could not read the difference from the file. \n");
    }

    // close the file
    fclose(file);
}

/**
 * @brief This function uses the difference between images
 *
 * This function is in charge of using the difference between images
 * and processing it.
 *
 * @param diff_path The path to the difference
 * @param inception_path The path to the inception image
 *
 * @return void
 */
void use_difference(FilesContainer* inception_container)
{
    printf("-- Inception process -- \n");

    // iterate over the files
    for (int i = 0; i < inception_container->total_nodes; i++)
    {
        File* inception_file = &inception_container->files[i];
        char* inception_path = extract_path_components(inception_file->path);
        char* diff_path = inception_file->path;

        printf("inception_path: %s\n", inception_path);
        printf("diff_path: %s\n", diff_path);

        // process the image
        PixelImage* image = process_image(inception_path);

        if (image == NULL)
        {
            throw_error("Could not process the image. Please try again with "
                        "another path \n");
            return;
        }

        // we should read the difference from the file
        PixelImage* difference = create_image(image->width, image->height, image->max_color);

        // read the difference from the file
        read_difference(diff_path, difference, 0);

        // print the difference
        for (int i = 0; i < difference->height; i++)
        {
            for (int j = 0; j < difference->width; j++)
            {
                printf("R: %d, G: %d, B: %d\n", difference->pixels[i][j].r, difference->pixels[i][j].g,
                       difference->pixels[i][j].b);
            }
        }

        // once we have the difference, we should store it
        free_image(image);
    }
}

/**
 * @brief This function computes the difference between images
 *
 * This function is in charge of computing the difference between images
 * and storing the diff from the inception image.
 *
 * Functionality:
 *   - 1. read the first image, store it as an inception image
 *   - 2. read continuously the rest of the images and compute the difference
 *   - 2.1 in case the difference is greater than a threshold, skip the process
 * and start again from 1. with a new inception
 *   - 2.2 in case the width or height of the next image is different, skip the
 * process and start again from 1. with a new inception
 *   - 3. store result of diff in a binary file
 *
 * @param files The files container to process
 * @return void
 */
void compute_difference(FilesContainer* files)
{
    printf("-- Compute differences -- \n");

    // inception metadata
    PixelImage* inception;
    File* inception_file;

    for (int i = files->total_nodes - 1; i >= 0; i--)
    {
        // we should process the file here and return the content
        File file = files->files[i];
        PixelImage* image = process_image(file.path);

        if (inception == NULL)
        {
            printf("Root\t[Inception]: %s (%s) \n", files->files[i].name, files->files[i].path);

            // read the first image and store it as the inception
            inception = process_image(files->files[i].path);

            // store the file as the inception file
            inception_file = &files->files[i];

            // don't process the first image as it's the inception
            continue;
        }
        else
        {
            printf("File\t[%s]: %s (%s) \n", cast_file_type(file.type), file.name, file.path);

            // process difference between inception + image
            PixelImage* diff = process_difference(inception, image);

            // TODO: assess threshold to understand if another inception layer
            // should be created

            // store the difference in a binary file already created
            // this diff.dat will be store in .inferece/{path_dir}/diff.data
            // and this binary will contain all the differences computed across
            // the inception frame
            store_difference(diff, inception_file);

            free_image(diff);
        }

        // once we have the image, we should free the memory
        free_image(image);
    }
}
