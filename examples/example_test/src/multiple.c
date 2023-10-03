#include "test.h"

TEST(test1)
{
	START;
	EXPECT_EQ(4 + 5, 9);
	END;
}

TEST(test2)
{
	START;
	int a = 5 * 4;
	EXPECT_EQ(a, 30);
	END;
}

TEST(test3)
{
	START;
	int a = 8;
	int b = 1;
	EXPECT_EQ(a / b, 4);
	END;
}

TEST(tests)
{
	SSTART;
	RUN(test1);
	RUN(test2);
	RUN(test3);
	SEND;
}

void multiple()
{
	t_init(80);
	tests();
	t_finish();
}
