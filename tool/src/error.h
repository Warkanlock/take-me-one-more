#ifndef ERROR_H
#define ERROR_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void handle_error(char* message);
void throw_error(char* message);

#endif
