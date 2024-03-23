#include "file.h"

/**
 * @brief This function creates a directory
 * This function is in charge of creating a directory
 * @param path The path of the directory to create
 * @return void
 */
void create_directory(const char* path)
{
    char tmp[MAX_PATH];
    char* p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp), "%s", path);
    len = strlen(tmp);
    if (tmp[len - 1] == '/')
    {
        tmp[len - 1] = 0;
    }
    for (p = tmp + 1; *p; p++)
    {
        if (*p == '/')
        {
            *p = 0;
            mkdir(tmp, S_IRWXU);
            *p = '/';
        }
    }
    mkdir(tmp, S_IRWXU);
}

/**
 * @brief This function will resource the memory used by the files container
 *
 * This function is in charge of freeing the memory used by the files container
 *
 * @param files_container The files container to free
 * @return void
 */
void free_files_container(FilesContainer* files_container)
{
    if (files_container == NULL)
    {
        return;
    }

    if (files_container->total_nodes > 0)
    {
        for (int i = 0; i < files_container->total_nodes; i++)
        {
            if (files_container->files[i].path != NULL)
            {
                free(files_container->files[i].path);
            }
        }
    }

    free(files_container->files);

    // deallocate the parent directory when is not needed
    free(files_container->parent_dir);
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
char* cast_file_type(unsigned char type)
{
    switch (type)
    {
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
 * @brief This function gets the parent directory of a path
 *
 * This function is in charge of getting the parent directory of a path
 * and returning it as a string.
 *
 * @param path The path to get the parent directory from
 * @return char* The parent directory
 */
char* get_parent_dir(const char* path)
{
    char* cwd = malloc(MAX_PATH);
    getcwd(cwd, MAX_PATH);
    return cwd;
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
FilesContainer read_dir(char* path, bool avoid_dirs)
{
    DIR* dir;
    struct dirent* ent;
    FilesContainer files_array;

    // define array to store files found in the directory
    files_array.total_nodes = 0;
    files_array.parent_dir = NULL;
    files_array.files = malloc(sizeof(File) * N_FILES);

    if (path == NULL)
    {
        throw_error("No path was provided. Aborting operation. \n");
    }

    // open the directory and scan
    if ((dir = opendir(path)) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            // create a temporal path to store the full path of the file
            char temporal_path[MAX_PATH];

            // avoid "." and ".." directories (current and parent directories)
            if (strcmp(ent->d_name, CURRENT_DIR) == 0 || strcmp(ent->d_name, PARENT_DIR) == 0 || ent->d_name[0] == '.')
            {
                continue;
            }

            // create a File object with full path
            File file;
            file.name = ent->d_name;
            snprintf(temporal_path, sizeof(temporal_path), "%s/%s", path, ent->d_name);
            file.path = strdup(temporal_path);
            file.type = ent->d_type;

            // add the file to the array
            if (files_array.total_nodes >= N_FILES)
            {
                // TODO: handle this case properly extending the capacity of the
                // array
                throw_error("The number of files in the directory exceeds the "
                            "limit. Aborting "
                            "operation. \n");
            }

            // if it's a directory, read it recursively
            if (avoid_dirs && file.type == DT_DIR)
            {
                FilesContainer files = read_dir(file.path, avoid_dirs);

                for (int i = 0; i < files.total_nodes; i++, files_array.total_nodes++)
                {
                    files_array.files[files_array.total_nodes] = files.files[i];
                }
            }
            else if (file.type == DT_REG)
            {
                files_array.files[files_array.total_nodes++] = file;
            }
        }

        closedir(dir);
    }
    else
    {
        throw_error("Could not open directory. Aborting operation. \n");
    }

    // get current working directory + path to the directory being read
    files_array.parent_dir = get_parent_dir(path);

    return files_array;
}

/**
 * @brief This function gets the directory in charge of tracking the differences
 *
 * This function is in charge of getting the directory in charge of tracking the
 * differences
 *
 * @param file_path The file path to get the directory
 * @param create_path If the path should be created
 * @param file_name The name of the file
 *
 * @return char* The directory in charge of tracking the differences
 */
char* get_inference_path(char* file_path, bool create_path, char* file_name)
{
    char* directory = (char*)malloc(strlen(GLOBAL_DIFFERENCE_PATH) + strlen(file_path) + 1);
    char* full_path = (char*)malloc(strlen(GLOBAL_DIFFERENCE_PATH) + strlen(file_path) +
                                    strlen(GLOBAL_DIFFERENCE_FILE_NAME) + strlen(GLOBAL_DIFFERENCE_FILE_EXTENSION) + 3);

    sprintf(directory, "%s/%s", GLOBAL_DIFFERENCE_PATH, file_path);
    sprintf(full_path, "%s/%s.%s", directory, file_name, GLOBAL_DIFFERENCE_FILE_EXTENSION);

    // create directory if any child does not exists yet
    if (create_path)
    {
        create_directory(directory);
    }

    return full_path;
}

char* extract_path_components(const char* path)
{
    char* final_path = (char*)malloc(strlen(path));

    // get first slash
    const char* first = strchr(path, '/');

    // get last slash
    const char* last = strrchr(path, '/');

    if (!first || !last || first == last)
    {
        handle_error("Invalid path. \n");
    }

    first += 1;
    int length = last - first;

    if (length > 0)
    {
        // assign the final path (extract from first to the length of chars
        // between first and last)
        sprintf(final_path, "%.*s", length, first);
        return final_path;
    }
    else
    {
        return NULL;
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
char* validate_path(const char* path)
{
    if (path == NULL)
    {
        throw_error("No path was provided. Please provide one\n");
    }

    if ((opendir(path)) != NULL)
    {
        // continue since the path exists in the first place
    }
    else
    {
        throw_error("Could not open directory. Please verify the path");
    }

    char* canonical_path = realpath(path, NULL);
    if (canonical_path == NULL)
    {
        throw_error("Could not canonicalize the path. Please verify the path");
    }

    // Return the canonical_path directly
    return (char*)path;
}
