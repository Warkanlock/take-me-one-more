#include "../unity/unity.h"

void setUp(void)
{
    // set stuff up here
}

void tearDown(void)
{
    // clean stuff up here
}

extern void example_test_pass(void);
extern void example_test_fail(void);

/** Globals tests */
extern void check_program_information(void);
extern void check_program_constants(void);
extern void check_macros(void);

/** Array tests */
extern void check_create_array(void);
extern void check_push(void);
extern void check_pop(void);
extern void check_pop_by_reference(void);
extern void check_push_null(void);
extern void check_get_capacity(void);
extern void check_get_capacity_after_push(void);
extern void check_get_after_push(void);
extern void check_get_count(void);
extern void check_simple_use_case(void);
extern void check_simple_use_case_with_char(void);

/** File tests */
extern void test_cast_file_type(void);
extern void test_get_parent_dir(void);
extern void test_read_dir(void);
extern void test_free_files_container(void);
extern void test_create_directory(void);
extern void test_get_inference_path(void);
extern void test_extract_path_components(void);
extern void test_validate_path(void);


int main(void)
{
    UnityBegin("tests/example.test.c");
    RUN_TEST(example_test_pass);
    RUN_TEST(example_test_fail);
    UnityEnd();

    UnityBegin("tests/globals.test.c");
    RUN_TEST(check_program_information);
    RUN_TEST(check_program_constants);
    RUN_TEST(check_macros);
    UnityEnd();

    UnityBegin("tests/array.test.c");
    RUN_TEST(check_create_array);
    RUN_TEST(check_push);
    RUN_TEST(check_pop);
    RUN_TEST(check_pop_by_reference);
    RUN_TEST(check_push_null);
    RUN_TEST(check_get_capacity);
    RUN_TEST(check_get_capacity_after_push);
    RUN_TEST(check_get_after_push);
    RUN_TEST(check_get_count);
    RUN_TEST(check_simple_use_case);
    RUN_TEST(check_simple_use_case_with_char);
    UnityEnd();

    UnityBegin("tests/file.test.c");
    RUN_TEST(test_cast_file_type);
    RUN_TEST(test_get_parent_dir);
    RUN_TEST(test_read_dir);
    RUN_TEST(test_free_files_container);
    RUN_TEST(test_create_directory);
    RUN_TEST(test_get_inference_path);
    RUN_TEST(test_extract_path_components);
    RUN_TEST(test_validate_path);
    UnityEnd();

    return 0;
}
