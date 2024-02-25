#include <iso646.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>

// program information
#define PROGRAM_NAME "Inter-frame Supervisor"
#define AUTHOR_NAME "Ignacio Brasca"
#define VERSION 1.0f

// useful global constants
#define N_FILES 100
#define MAX_PATH 1024
#define AVOID_DIRS false
#define READ_DIRS true

// useful macros
#define PARENT_DIR ".."
#define CURRENT_DIR "."
#define GLOBAL_DIFFERENCE_PATH ".inference"

/**
* @brief This struct represents a file
*/
typedef struct {
    char *name;
    char *path;
    unsigned char type;
} File;

typedef struct {
    File *files;
    unsigned int total_nodes;
    char* parent_dir;
} FilesContainer;

typedef struct {
    unsigned char r, g, b;
} Pixel;

typedef struct {
    unsigned int width, height;
    unsigned int max_color;
    Pixel **pixels;
} PixelImage;

/**
* @brief This function handles internal errors
*
* This function is in charge of handling errors and
* printing the corresponding message.
*
* @param message The message to print
* @return void
*/
void handle_error(char *message) {
    errno = -1;
    fprintf(stderr, "%s", message);
    exit(errno);
}

/**
* @brief This function handles external errors
*
* This function is in charge of handling errors and
* printing the corresponding message.
*
* @param message The message to print
* @return void
*/
void throw_error(char *message) {
    perror(message);
    exit(-1);
}

/**
* @brief This function prints the program information
*
* This function is in charge of printing the program information
* such as the program name, the author and the version.
*
* @return void
*/
void head_information() {
    printf("%s\n", PROGRAM_NAME);
    printf("Developed by: %s\n", AUTHOR_NAME);
    printf("Version: %.2f\n", VERSION);
}

/**
* @brief This function gets the parent directory of a path
*
* This function is in charge of getting the parent directory of a path
* and returning it as a string.
*
* @param path The path to get the parent directory from
* @return char* The parent directory
*/
char *get_parent_dir(char *path) {
    char *cwd = malloc(MAX_PATH);
    getcwd(cwd, MAX_PATH);
    return cwd;
}

/**
* @brief This function process the content of a path
*
* This function is in charge of processing the content of a path
* and manipulate the bytes of the file.
*
* @param path The path of the image to process
* @return PixelImage* The image object
*/
PixelImage *process_image(char *path) {
    FILE *file = fopen(path, "r");

    if(file == NULL) {
        throw_error("Could not open file. Aborting operation. \n");
    }

    // allocate memory for the content of the file
    PixelImage *image = (PixelImage *)malloc(sizeof(PixelImage));
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
    image->pixels = (Pixel **)malloc(sizeof(Pixel *) * image->height);

    // allocate memory for each row of pixels
    for(int i = 0; i < image->height; i++) {
        image->pixels[i] = (Pixel *)malloc(sizeof(Pixel) * image->width);
    }

    // read pixel data
    for(int i = 0; i < image->height; i++) {
        for(int j = 0; j < image->width; j++) {
            fscanf(file, "%hhu %hhu %hhu", &image->pixels[i][j].r, &image->pixels[i][j].g, &image->pixels[i][j].b);
        }
    }

    // close the file
    fclose(file);

    // return the image
    return image;
}

/**
* @brief This function reads a directory
*
* This function is in charge of reading a directory and printing
* all the files and directories within it.
*
* @param path The path of the directory to read
* @param avoid_dirs A flag to avoid reading directories
* @return void
*/
FilesContainer read_dir(char *path, bool avoid_dirs) {
    DIR *dir;
    struct dirent *ent;
    FilesContainer files_array;

    // define array to store files found in the directory
    files_array.total_nodes = 0;
    files_array.parent_dir = NULL;
    files_array.files = malloc(sizeof(File) * N_FILES);

    if(path == NULL) {
        throw_error("No path was provided. Aborting operation. \n");
    }

    // open the directory and scan
    if ((dir = opendir (path)) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            // create a temporal path to store the full path of the file
            char temporal_path[MAX_PATH];

            // avoid "." and ".." directories (current and parent directories)
            if (strcmp(ent->d_name, CURRENT_DIR) == 0 || strcmp(ent->d_name, PARENT_DIR) == 0 || ent->d_name[0] == '.') {
                continue;
            }

            // create a File object with full path
            File file;
            file.name   = ent->d_name;
            snprintf(temporal_path, sizeof(temporal_path), "%s/%s", path, ent->d_name);
            file.path = strdup(temporal_path);
            file.type   = ent->d_type;

            // add the file to the array
            if(files_array.total_nodes >= N_FILES) {
                // TODO: handle this case properly extending the capacity of the array
                throw_error("The number of files in the directory exceeds the limit. Aborting operation. \n");
            }

            // if it's a directory, read it recursively
            if(avoid_dirs && file.type == DT_DIR) {
                FilesContainer files = read_dir(file.path, avoid_dirs);

                for(int i = 0; i < files.total_nodes; i++, files_array.total_nodes++) {
                    files_array.files[files_array.total_nodes] = files.files[i];
                }
            } else if (file.type == DT_REG) {
                files_array.files[files_array.total_nodes++] = file;
            }
        }

        closedir (dir);
    } else {
        throw_error("Could not open directory. Aborting operation. \n");
    }

    // get current working directory + path to the directory being read
    files_array.parent_dir = get_parent_dir(path);

    return files_array;
}

/**
* @brief This function casts the file type
*
* This function is in charge of casting the file type
* to a human readable string.
*
* @param type The file type to cast
* @return char* The human readable file type
*/
char *cast_file_type(unsigned char type) {
    switch(type) {
        case DT_REG:
            return "Regular file";
        case DT_DIR:
            return "Directory";
        case DT_LNK:
            return "Symbolic link";
        case DT_FIFO:
            return "FIFO";
        case DT_SOCK:
            return "Socket";
        case DT_BLK:
            return "Block device";
        case DT_CHR:
            return "Character device";
        default:
            return "Unknown";
    }
}

/**
* @brief This function will resource the memory used by the files container
*
* This function is in charge of freeing the memory used by the files container
*
* @param files_container The files container to free
* @return void
*/
void free_files_container(FilesContainer *files_container) {
    for (int i = 0; i < files_container->total_nodes; i++) {
        free(files_container->files[i].path);
    }

    free(files_container->files);

    // deallocate the parent directory when is not needed
    free(files_container->parent_dir);
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
PixelImage *create_image(unsigned int width, unsigned int height, unsigned int color_depth) {
    PixelImage *img = (PixelImage *)malloc(sizeof(PixelImage));

    // initialize the image
    img->height = height;
    img->width = width;
    img->max_color = color_depth;

    // allocate memory for the pixels
    img->pixels = (Pixel **)malloc(sizeof(Pixel *) * img->height);

    // allocate memory for each row of pixels (otherwise segmentation fault will occur)
    for(int i = 0; i < img->height; i++) {
        img->pixels[i] = (Pixel *)malloc(sizeof(Pixel) * img->width);
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
void free_image(PixelImage *img) {
    for(int i = 0; i < img->height; i++) {
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
PixelImage *process_difference(PixelImage* base, PixelImage* current) {
    PixelImage *diff = create_image(base->width, base->height, base->max_color);

    for(int i = 0; i < base->height; i++) {
        for(int j = 0; j < base->width; j++) {
            Pixel *base_pixel = &base->pixels[i][j];
            Pixel *current_pixel = &current->pixels[i][j];

            // calculate the difference between the pixels (absolute value is needed)
            diff->pixels[i][j].r = abs(current_pixel->r - base_pixel->r);
            diff->pixels[i][j].g = abs(current_pixel->g - base_pixel->g);
            diff->pixels[i][j].b = abs(current_pixel->b - base_pixel->b);
        }
    }

    return diff;
}

/**
* @brief This function creates a directory
* This function is in charge of creating a directory
* @param path The path of the directory to create
* @return void
*/
void create_directory(const char *path) {
    char tmp[MAX_PATH];
    char *p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp), "%s", path);
    len = strlen(tmp);
    if(tmp[len - 1] == '/') {
        tmp[len - 1] = 0;
    }
    for(p = tmp + 1; *p; p++) {
        if(*p == '/') {
            *p = 0;
            mkdir(tmp, S_IRWXU);
            *p = '/';
        }
    }
    mkdir(tmp, S_IRWXU);
}

/**
* @brief This function gets the directory in charge of tracking the differences
*
* This function is in charge of getting the directory in charge of tracking the differences
*
* @param file_path The file path to get the directory
* @param create_path If the path should be created
*
* @return char* The directory in charge of tracking the differences
*/
char *get_inference_path(char *file_path, bool create_path) {
    char *directory = (char *)malloc(strlen(GLOBAL_DIFFERENCE_PATH) + strlen(file_path));
    char *full_path = (char *)malloc(strlen(GLOBAL_DIFFERENCE_PATH) + strlen(file_path));

    sprintf(directory, "%s/%s", GLOBAL_DIFFERENCE_PATH, file_path);
    sprintf(full_path, "%s/diff.dat", directory);

    // create directory if any child does not exists yet
    if(create_path) {
        create_directory(directory);
    }

    return full_path;
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
void store_difference(PixelImage *difference, File *inception_file) {
    // create if required path to store the difference
    bool create_path = true;
    char *full_path = get_inference_path(inception_file->path, create_path);

    // open path at full_path
    FILE *file = fopen(full_path, "wb");

    if(file == NULL) {
        throw_error("Could not open file to store the difference (maybe you don't permissions to write to disk). Aborting operation. \n");
    }

    int pixel_count = difference->width * difference->height;
    size_t data_size = sizeof(Pixel) * pixel_count;
    Pixel *data = (Pixel *)malloc(data_size);

    // store in data pixels from the difference
    for(int i = 0; i < difference->height; i++) {
        for(int j = 0; j < difference->width; j++) {
            data[i * difference->width + j] = difference->pixels[i][j];
        }
    }

    // write the difference to the file
    int items_written = fwrite(data, sizeof(Pixel), pixel_count, file);

    if (items_written != pixel_count) {
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
void read_difference(char *diff_path, PixelImage *difference, int index_diff) {
    FILE *file = fopen(diff_path, "rb");

    if (file == NULL) {
        throw_error("Could not open to read the difference. \n");
    }

    // calculate the position in the file where the desired difference data starts
    int pixel_count = difference->width * difference->height;
    size_t data_size = sizeof(Pixel) * pixel_count;
    Pixel *data = (Pixel *)malloc(data_size);

    // read items from stream
    size_t items_read = fread(data, sizeof(Pixel), pixel_count, file);

    // iterate over data
    for(int i = 0; i < difference->height; i++) {
        for(int j = 0; j < difference->width; j++) {
            // copy the data to the difference
            difference->pixels[i][j].r = data[i * difference->width + j].r;
            difference->pixels[i][j].g = data[i * difference->width + j].g;
            difference->pixels[i][j].b = data[i * difference->width + j].b;
        }
    }

    if (items_read != pixel_count) {
        throw_error("Could not read the difference from the file. \n");
    }

    // close the file
    fclose(file);
}

char *extract_path_components(const char *path) {
    char *final_path = (char *)malloc(strlen(path));

    // get first slash
    const char *first = strchr(path, '/');

    // get last slash
    const char *last = strrchr(path, '/');

    if (!first || !last || first == last) {
        handle_error("Invalid path. \n");
    }

    first += 1;
    int length = last - first;

    if (length > 0) {
        // assign the final path
        sprintf(final_path, "%.*s", length, first);
        return final_path;
    } else {
        return NULL;
    }
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
void use_difference(FilesContainer *inception_container) {
    printf("-- Inception process -- \n");

    // iterate over the files
    for(int i = 0; i < inception_container->total_nodes; i++) {
        File *inception_file = &inception_container->files[i];
        char *inception_path = extract_path_components(inception_file->path);
        char *diff_path = inception_file->path;

        printf("inception_path: %s\n", inception_path);
        printf("diff_path: %s\n", diff_path);

        // process the image
        PixelImage *image = process_image(inception_path);

        if (image == NULL) {
            throw_error("Could not process the image. Please try again with another path \n");
            return;
        }

        // we should read the difference from the file
        PixelImage *difference = create_image(image->width, image->height, image->max_color);

        // read the difference from the file
        read_difference(diff_path, difference, 0);

        // print the difference
        for(int i = 0; i < difference->height; i++) {
            for(int j = 0; j < difference->width; j++) {
                printf("R: %d, G: %d, B: %d\n", difference->pixels[i][j].r, difference->pixels[i][j].g, difference->pixels[i][j].b);
            }
        }

        // once we have the difference, we should store it
        free_image(image);
    }
    // // for now just process the image from: difference + image = beam
    // PixelImage *image = process_image(inception_path);

    // if (image == NULL) {
    //     throw_error("Could not process the image. Please try again with another path \n");
    //     return;
    // }

    // // we should read the difference from the file
    // PixelImage *difference = create_image(image->width, image->height, image->max_color);

    // // read the difference from the file
    // read_difference(diff_path, difference, 0);

    // // print the difference
    // for(int i = 0; i < difference->height; i++) {
    //     for(int j = 0; j < difference->width; j++) {
    //         printf("R: %d, G: %d, B: %d\n", difference->pixels[i][j].r, difference->pixels[i][j].g, difference->pixels[i][j].b);
    //     }
    // }

    // // once we have the image, we should free the memory
    // free_image(image);
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
*   - 2.1 in case the difference is greater than a threshold, skip the process and start again from 1. with a new inception
*   - 2.2 in case the width or height of the next image is different, skip the process and start again from 1. with a new inception
*   - 3. store result of diff in a binary file
*
* @param files The files container to process
* @return void
*/
void compute_difference(FilesContainer *files) {
    printf("-- Compute differences -- \n");

    // inception metadata
    PixelImage *inception;
    File *inception_file;

    for(int i = files->total_nodes - 1; i >= 0; i--) {
        // we should process the file here and return the content
        File file = files->files[i];
        PixelImage *image = process_image(file.path);

        if(inception == NULL) {
            printf("Root\t[Inception]: %s (%s) \n", files->files[i].name, files->files[i].path);

            // read the first image and store it as the inception
            inception = process_image(files->files[i].path);

            // store the file as the inception file
            inception_file = &files->files[i];

            // don't process the first image as it's the inception
            continue;
        } else {
            printf("File\t[%s]: %s (%s) \n", cast_file_type(file.type), file.name, file.path);

            // process difference between inception + image
            PixelImage *diff = process_difference(inception, image);

            // TODO: assess threshold to understand if another inception layer should be created

            // store the difference in a binary file already created
            // this diff.dat will be store in .inferece/{path_dir}/diff.data
            // and this binary will contain all the differences computed across the inception
            // frame
            store_difference(diff, inception_file);

            free_image(diff);
        }

        // once we have the image, we should free the memory
        free_image(image);
    }
}

/**
* @brief This function is in charge of listening to a directory
*
* It's the main function of the program and it's in charge of
* listen to a directory for changes and execute the corresponding
* action.
*
* @return void
*/
void supervisor(char *path) {
    // prepare the difference file
    FilesContainer files = read_dir(path, AVOID_DIRS);
    compute_difference(&files);

    // use the difference file
    FilesContainer diff_files = read_dir(GLOBAL_DIFFERENCE_PATH, READ_DIRS);
    use_difference(&diff_files);

    // free memory used
    free_files_container(&files);
}

/**
* @brief This function validates the path
*
* This function is in charge of validating the parameter
* passed to the program.
*
* @param path The path to validate
* @return char* The validated path
*/
char *validate_path(char *path) {
    if(path == NULL) {
        throw_error("No path was provided. Please provide one\n");
    }

    // check path was a valid directory
    DIR *dir = opendir(path);

    if (dir) {
        closedir(dir);
    } else {
        throw_error("Could not open directory. Please verify the path");
    }

    // canonicalize the path
    char *canonical_path = realpath(path, NULL);

    if(canonical_path == NULL) {
        throw_error("Could not canonicalize the path. Please verify the path");
    }

    return path;
}

int main(int argc, char *argv[]) {

    if(argc < 2) {
        handle_error("No path was provided. Please provide one\n");
    }

    char *path = validate_path(argv[1]);

    head_information();
    supervisor(path);

    return 0;
}
