#include <stdio.h>

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
#define GLOBAL_DIFFERENCE_FILE_EXTENSION "dat"
#define GLOBAL_DIFFERENCE_FILE_NAME "diff"

void head_information();
