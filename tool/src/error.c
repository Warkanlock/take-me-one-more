#include "error.h"

/**
 * @brief This function handles internal errors
 *
 * This function is in charge of handling errors and
 * printing the corresponding message.
 *
 * @param message The message to print
 * @return void
 */
void handle_error(char* message)
{
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
void throw_error(char* message)
{
    perror(message);
    exit(-1);
}
