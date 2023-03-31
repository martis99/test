#include "child1.h"

#include "test.h"

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

int main(int argc, char **argv)
{
	t_init(80);
	tests();
	t_finish();
}
