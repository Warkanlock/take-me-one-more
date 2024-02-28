#ifndef ARRAY_H
#define ARRAY_H

#include <stdio.h>
#include <stdlib.h>

#define BASE_CAPACITY 256

typedef struct
{
    int* items;
    size_t count;    // current count of elements of the array
    size_t capacity; // capacity of the array
} int_array;

int_array* create_array();
void release_array(int_array* arr);
void push(int_array* arr, int value);
int* pop(int_array* arr);
int* get(int_array* arr, unsigned int index);
size_t get_capacity(int_array* arr);
size_t get_count(int_array* arr);

#endif
