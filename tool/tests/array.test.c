#include "../src/array.h"
#include "../unity/unity.h"

void check_create_array(void)
{
    int_array* array = create_array();
    TEST_ASSERT_NOT_NULL(array);
    release_array(array);
}

void check_push_null(void)
{
    int_array* array = create_array();
    TEST_ASSERT_NULL(array->items);
    release_array(array);
}

void check_push(void)
{
    int_array* array = create_array();
    push(array, 1);
    TEST_ASSERT_EQUAL(1, array->items[0]);
    release_array(array);
}

void check_pop(void)
{
    int_array* array = create_array();
    push(array, 1);
    int* value = pop(array);
    TEST_ASSERT_EQUAL(1, *value);
    release_array(array);
}

void check_pop_by_reference(void)
{
    int_array* array = create_array();
    push(array, 1);
    int* value = pop(array);
    *value = 2;
    TEST_ASSERT_EQUAL(2, array->items[0]);
    release_array(array);
}
