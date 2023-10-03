#include "test.h"

TEST(grand_child1, int a, int b)
{
	START;
	EXPECT_EQ(a + 5, b);
	END;
}

TEST(child1)
{
	SSTART;
	RUN(grand_child1, 3, 8);
	RUN(grand_child1, 4, 9);
	RUN(grand_child1, 5, 11);
	RUN(grand_child1, 6, 14);
	SEND;
}

TEST(grand_child2, int a, int b)
{
	START;
	EXPECT_EQ(a / 5, b);
	EXPECT_EQ(6 - 5, 1);
	END;
}

TEST(child2)
{
	SSTART;
	RUN(grand_child2, 10, 2);
	RUN(grand_child2, 20, 3);
	RUN(grand_child2, 30, 6);
	RUN(grand_child2, 40, 4);
	SEND;
}

TEST(parent)
{
	SSTART;
	RUN(child1);
	RUN(child2);
	SEND;
}

TEST(tests)
{
	SSTART;
	RUN(parent);
	SEND;
}

void multilevel()
{
	t_init(80);
	tests();
	t_finish();
}
