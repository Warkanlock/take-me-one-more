#include <iso646.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

// program information
#define PROGRAM_NAME "Inter-frame Supervisor"
#define AUTHOR_NAME "Ignacio Brasca"
#define VERSION 1.0f

// useful global constants
#define N_FILES 100

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
* @brief This function reads a directory
*
* This function is in charge of reading a directory and printing
* all the files and directories within it.
*
* @param path The path of the directory to read
* @return void
*/
FilesContainer read_dir(char *path) {
    DIR *dir;
    struct dirent *ent;
    FilesContainer files_array;

    // define array to store files found in the directory
    files_array.total_nodes = 0;
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
            if (strcmp(ent->d_name, CURRENT_DIR) == 0 || strcmp(ent->d_name, PARENT_DIR) == 0) {
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
            if(file.type == DT_DIR) {
                FilesContainer files = read_dir(file.path);

                for(int i = 0; i < files.total_nodes; i++, files_array.total_nodes++) {
                    files_array.files[files_array.total_nodes] = files.files[i];
                }
            } else {
                files_array.files[files_array.total_nodes++] = file;
            }
        }

        closedir (dir);
    } else {
        throw_error("Could not open directory. Aborting operation. \n");
    }

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
* @brief This function is in charge of listening to a directory
*
* It's the main function of the program and it's in charge of
* listen to a directory for changes and execute the corresponding
* action.
*
* @return void
*/
void supervisor(char *path) {
    printf("Listening to directory: %s\n", path);
    FilesContainer files = read_dir(path);

    for(int i = 0; i < files.total_nodes; i++) {
        char *file_type = cast_file_type(files.files[i].type);

        printf("File [%s]: %s (%s) \n", file_type, files.files[i].name, files.files[i].path);
    }
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

    head_information();
    supervisor(validate_path(argv[1]));
    return 0;
}
