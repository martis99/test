#include "child1.h"

#include "test.h"

TEST(grand_child2)
{
	START;
	EXPECT_EQ(10 / 5, 2);
	EXPECT_EQ(6 - 5, 1);
	END;
}

TEST(child2)
{
	SSTART;
	RUN(grand_child2);
	RUN(grand_child2);
	RUN(grand_child2);
	RUN(grand_child2);
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

void separated()
{
	t_init(80);
	t_run(tests, 1);
	t_finish();
}
