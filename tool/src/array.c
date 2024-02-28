#include "array.h"

/**
 * Create a new dynamic array for integers
 */
int_array* create_array() { return (int_array*)malloc(sizeof(int_array)); }

/**
 * Free the memory used by the array
 */
void release_array(int_array* arr)
{
    free(arr->items);
    free(arr);
}

/**
 * Add a value to the array
 *
 * @param arr The array to add the value to
 * @param value The value to add to the array
 */
void push(int_array* arr, int value)
{
    // allocate memory for the array
    if (arr->count >= arr->capacity)
    {
        // if capacity is 0, set it to BASE_CAPACITY
        if (arr->capacity == 0)
        {
            arr->capacity = BASE_CAPACITY;
        }
        else
        {

            arr->capacity *= 2;
        }

        // reallocate memory for the array for the new capacity
        arr->items = (int*)realloc(arr->items, arr->capacity * sizeof(*arr->items));
    }

    // add the value to the array
    arr->items[arr->count++] = value;
}

/**
 * Remove the last value from the array
 *
 * @param arr The array to remove the value from
 * @return The value that was removed from the array
 */
int* pop(int_array* arr)
{
    if (arr->count == 0)
    {
        return NULL;
    }

    return &arr->items[--arr->count];
}
