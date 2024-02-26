#include "../unity/unity.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void example_test_pass(void) {
    TEST_ASSERT_EQUAL(1, 1);
}

void example_test_fail(void) {
    TEST_ASSERT_FALSE(1 == 2);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(example_test_pass);
    RUN_TEST(example_test_fail);
    UNITY_END(); // stop unit testing
}
