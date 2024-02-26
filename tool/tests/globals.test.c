#include "../src/global.h"
#include "../unity/unity.h"
#include <stdbool.h>

void check_program_information(void)
{
    TEST_ASSERT_EQUAL(PROGRAM_NAME, "Inter-frame Supervisor");
    TEST_ASSERT_EQUAL(AUTHOR_NAME, "Ignacio Brasca");
    TEST_ASSERT_EQUAL(VERSION, 1.0f);
}

void check_program_constants(void)
{
    TEST_ASSERT_EQUAL(N_FILES, 100);
    TEST_ASSERT_EQUAL(MAX_PATH, 1024);
    TEST_ASSERT_EQUAL(AVOID_DIRS, 0);
    TEST_ASSERT_EQUAL(READ_DIRS, 1);
}

void check_macros(void)
{
    TEST_ASSERT_EQUAL_STRING(PARENT_DIR, "..");
    TEST_ASSERT_EQUAL_STRING(CURRENT_DIR, ".");
    TEST_ASSERT_EQUAL_STRING(GLOBAL_DIFFERENCE_PATH, ".inference");
    TEST_ASSERT_EQUAL_STRING(GLOBAL_DIFFERENCE_FILE_EXTENSION, "dat");
    TEST_ASSERT_EQUAL_STRING(GLOBAL_DIFFERENCE_FILE_NAME, "diff");
}
