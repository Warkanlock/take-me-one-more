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
extern void check_program_information(void);
extern void check_program_constants(void);
extern void check_macros(void);

int main(void)
{
    UnityBegin("tests/example.test.c");
    RUN_TEST(example_test_pass);
    RUN_TEST(example_test_fail);
    UnityBegin("tests/globals.test.c");
    RUN_TEST(check_program_information);
    RUN_TEST(check_program_constants);
    RUN_TEST(check_macros);
    return (UnityEnd());
}
