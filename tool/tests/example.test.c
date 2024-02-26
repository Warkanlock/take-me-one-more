#include "../unity/unity.h"

void example_test_pass(void) { TEST_ASSERT_EQUAL(1, 1); }

void example_test_fail(void) { TEST_ASSERT_FALSE(1 == 2); }
