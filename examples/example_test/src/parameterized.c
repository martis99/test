#include "test.h"

TEST(test, int a, int b, int c)
{
	START;
	EXPECT_EQ(a + b, c);
	END;
}

TEST(tests)
{
	SSTART;
	RUN(test, 4, 5, 9);
	RUN(test, 1, 2, 3);
	RUN(test, 2, 4, 6);
	SEND;
}

int parameterized()
{
	t_init(80);
	tests();
	t_finish();
}
