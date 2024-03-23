#include "beam.h"
#include "error.h"
#include "file.h"
#include "global.h"

#define ENABLE_DIFFERENCE 1
#define ENABLE_USE_DIFFERENCE 1

/**
 * @brief This function is in charge of listening to a directory
 *
 * It's the main function of the program and it's in charge of
 * listen to a directory for changes and execute the corresponding
 * action.
 *
 * @return void
 */
void supervisor(char* path)
{
    // compute the difference file from inception director
#if ENABLE_DIFFERENCE
    FilesContainer files = read_dir(path, AVOID_DIRS);
    compute_difference(&files);
    free_files_container(&files);
#endif

    // use the difference file to generate the corresponding original file from difference
#if ENABLE_USE_DIFFERENCE
    FilesContainer diff_files = read_dir(GLOBAL_DIFFERENCE_PATH, READ_DIRS);
    use_difference(&diff_files);
    free_files_container(&diff_files);
#endif
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        handle_error("No path was provided. Please provide one\n");
    }

    char* path = validate_path(argv[1]);

    head_information();
    supervisor(path);

    return 0;
}
