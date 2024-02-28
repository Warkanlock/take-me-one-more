#include "array.h"

/**
 * Create a new dynamic array for integers
 */
dynamic_array* create_array() { return (dynamic_array*)malloc(sizeof(dynamic_array)); }

/**
 * Free the memory used by the array
 */
void release_array(dynamic_array* arr)
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
void push(dynamic_array* arr, void* value)
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
        arr->items = (void*)realloc(arr->items, arr->capacity * sizeof(*arr->items));
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
void* pop(dynamic_array* arr)
{
    if (arr->count == 0)
    {
        return NULL;
    }

    return &arr->items[--arr->count];
}

/**
 * Get the value at the specified index
 *
 * @param arr The array to get the value from
 * @param index The index of the value to get
 * @return The value at the specified index
 */
void* get(dynamic_array* arr, unsigned int index)
{
    if (index >= arr->count)
    {
        return NULL;
    }

    return &arr->items[index];
}

/**
 * Get current capacity of the array
 *
 * @param arr The array to get the count of
 * @return The capacity of the array
 */
size_t get_capacity(dynamic_array* arr) { return arr->capacity; }

/**
 * Get current count of items inside the array
 *
 * @param arr The array to get the count of
 * @return The total count of items of the array
*/
size_t get_count(dynamic_array* arr) { return arr->count; }
