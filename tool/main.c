#include <iso646.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>

// program information
#define PROGRAM_NAME "Inter-frame Supervisor"
#define AUTHOR_NAME "Ignacio Brasca"
#define VERSION 1.0f

// useful global constants
#define N_FILES 100
#define MAX_PATH 1024
#define MAX_NAME 256
#define AVOID_DIRS false

// useful macros
#define PARENT_DIR ".."
#define CURRENT_DIR "."

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
    char *cwd = malloc(1024);
    getcwd(cwd, 1024);
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
            char temporal_path[1024];

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

void compute_diff(FilesContainer *files) {
    for(int i = 0; i < files->total_nodes; i++) {
        File file = files->files[i];
        printf("File [%s]: %s (%s) \n", cast_file_type(file.type), file.name, file.path);

        // we should process the file here and return the content
        PixelImage *image = process_image(file.path);

        // print image metadata
        printf("Width: %d \n", image->width);
        printf("Height: %d \n", image->height);
        printf("Color: %u \n", image->max_color);

        // once we have the image, we should free the memory
        free(image->pixels);
        free(image);
    }

    free_files_container(files);
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
    FilesContainer files = read_dir(path, AVOID_DIRS);

    printf("Files found in the directory [%s]: \n", files.parent_dir);

    compute_diff(&files);
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
