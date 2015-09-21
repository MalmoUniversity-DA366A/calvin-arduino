/*
 * test_stdout.c
 *
 *  Created on: 21 sep. 2015
 *      Author: Daniel
 */

#include "../../include/test/test_exampel.h"
#include "../../include/unity/unity.h"
#include "test_stdout.h"
#include "std_out.h"

#ifdef __cplusplus
 extern "C" {
#endif

void testStdPrint(){
	TEST_ASSERT_EQUAL(4,actorStdout("hej"));

}


#ifdef __cplusplus
}
#endif
