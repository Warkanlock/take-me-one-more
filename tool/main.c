#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

// program information
#define PROGRAM_NAME "Inter-frame Supervisor"
#define AUTHOR_NAME "Ignacio Brasca"
#define VERSION 1

// useful macros
#define PARENT_DIR ".."
#define CURRENT_DIR "."

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
    printf("Version: %d\n", VERSION);
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
void read_dir(char *path) {
    DIR *dir;
    struct dirent *ent;

    if(path == NULL) {
        throw_error("No path was provided. Aborting operation. \n");
    }

    // open the directory and scan
    if ((dir = opendir (path)) != NULL) {
        while ((ent = readdir (dir)) != NULL) {

            // avoid "." and ".." directories (current and parent directories)
            if (strcmp(ent->d_name, CURRENT_DIR) == 0 || strcmp(ent->d_name, PARENT_DIR) == 0) {
                continue;
            }

            // check if it's a directory or a file
            if (ent->d_type == DT_DIR) {
                printf("Directory: ");
            } else {
                printf("File: ");
            }

            printf("%s\n", ent->d_name);
        }

        closedir (dir);
    } else {
        // could not open directory
        throw_error("Could not open directory. Aborting operation. \n");
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
    read_dir(path);
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
