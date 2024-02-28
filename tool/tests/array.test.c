#include "../src/array.h"
#include "../unity/unity.h"

void check_create_array(void)
{
    dynamic_array* array = create_array();
    TEST_ASSERT_NOT_NULL(array);
    release_array(array);
}

void check_push_null(void)
{
    dynamic_array* array = create_array();
    TEST_ASSERT_NULL(array->items);
    release_array(array);
}

void check_get_capacity_after_push(void)
{
    dynamic_array* array = create_array();
    int* item = malloc(sizeof(int));
    *item = 1;
    push(array, item);
    TEST_ASSERT_EQUAL(256, get_capacity(array));
    release_array(array);
}

void check_get_count(void)
{
    dynamic_array* array = create_array();
    int* item = malloc(sizeof(int));
    *item = 1;
    push(array, item);
    TEST_ASSERT_EQUAL(1, get_count(array));
    release_array(array);
}

void check_get_capacity(void)
{
    dynamic_array* array = create_array();
    TEST_ASSERT_EQUAL(0, get_capacity(array));
    release_array(array);
}

void check_get_after_push(void)
{
    dynamic_array* array = create_array();

    // insert element
    int* item = malloc(sizeof(int));
    *item = 1;
    push(array, item);

    // assert equality
    int* result = get(array, 0);
    TEST_ASSERT_EQUAL(1, *result);
    release_array(array);
}

void check_push(void)
{
    dynamic_array* array = create_array();

    // insert item into array
    int* item = malloc(sizeof(int));
    *item = 1;
    push(array, item);

    TEST_ASSERT_EQUAL(1, get_count(array));
    release_array(array);
}

void check_pop(void)
{
    dynamic_array* array = create_array();

    // insert element into array
    int* item = malloc(sizeof(int));
    *item = 1;
    push(array, item);

    // return value from pop
    int* value = pop(array);

    // assert equality
    TEST_ASSERT_EQUAL(1, *value);
    release_array(array);
}

void check_pop_by_reference(void)
{
    dynamic_array* array = create_array();
    int* item = malloc(sizeof(int));
    *item = 1;
    push(array, item);

    int* value = pop(array);
    TEST_ASSERT_EQUAL(value, array->items[0]);
    release_array(array);
}

void check_simple_use_case(void) {
    dynamic_array* array = create_array();
    int* item = malloc(sizeof(int));
    *item = 1;
    push(array, item);
    int* value = pop(array);
    TEST_ASSERT_EQUAL(1, *value);
    release_array(array);
}

void check_simple_use_case_with_char(void) {
    dynamic_array* array = create_array();
    char* item = malloc(sizeof(char));
    *item = 'a';
    push(array, item);
    char* value = pop(array);
    TEST_ASSERT_EQUAL('a', *value);
    release_array(array);
}
