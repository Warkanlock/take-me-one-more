#ifndef FILE_H
#define FILE_H

#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "error.h"
#include "global.h"

/**
 * @brief This struct represents a file
 */
typedef struct
{
    char* name;
    char* path;
    unsigned char type;
} File;

typedef struct
{
    File* files;
    unsigned int total_nodes;
    char* parent_dir;
} FilesContainer;

char* cast_file_type(unsigned char type);
char* get_parent_dir(const char* path);
FilesContainer read_dir(char* path, bool avoid_dirs);
void free_files_container(FilesContainer* files_container);
void create_directory(const char* path);
char* get_inference_path(char* file_path, bool create_path, char* file_name);
char* extract_path_components(const char* path);
char* validate_path(const char* path);

#endif
