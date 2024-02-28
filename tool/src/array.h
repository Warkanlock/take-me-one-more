#ifndef ARRAY_H
#define ARRAY_H

#include <stdio.h>
#include <stdlib.h>

#define BASE_CAPACITY 256

typedef struct
{
    void** items;    // double array
    size_t count;    // current count of elements of the array
    size_t capacity; // capacity of the array
} dynamic_array;

dynamic_array* create_array();
void release_array(dynamic_array* arr);
void push(dynamic_array* arr, void* value);
void* pop(dynamic_array* arr);
void* get(dynamic_array* arr, unsigned int index);
size_t get_capacity(dynamic_array* arr);
size_t get_count(dynamic_array* arr);

#endif
