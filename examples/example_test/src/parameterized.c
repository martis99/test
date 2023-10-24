#include "test.h"

TESTP(test, int a, int b, int c)
{
	START;
	EXPECT_EQ(a + b, c);
	END;
}

TEST(tests)
{
	SSTART;
	RUNP(test, 4, 5, 9);
	RUNP(test, 1, 2, 3);
	RUNP(test, 2, 4, 6);
	SEND;
}

void parameterized()
{
	t_init(80);
	t_run(tests, 1);
	t_finish();
}
