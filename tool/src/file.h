#ifndef FILE_H
#define FILE_H

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

#endif
