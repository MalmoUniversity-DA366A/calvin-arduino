#include "../../include/test/test_exampel.h"
#include "../../include/unity/unity.h"

#ifdef __cplusplus
 extern "C" {
#endif

void test_ExampelCode(void)
{
	TEST_ASSERT_EQUAL(1, 1);
}

void test_ExampelCode2(void)
{
	TEST_ASSERT_EQUAL(1, 0);
}

#ifdef __cplusplus
}
#endif
