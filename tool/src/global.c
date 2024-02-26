#include "global.h"

/**
 * @brief This function prints the program information
 *
 * This function is in charge of printing the program information
 * such as the program name, the author and the version.
 *
 * @return void
 */
void head_information()
{
    printf("%s\n", PROGRAM_NAME);
    printf("Developed by: %s\n", AUTHOR_NAME);
    printf("Version: %.2f\n", VERSION);
}
