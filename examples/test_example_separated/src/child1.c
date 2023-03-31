#include "child1.h"

#include "test.h"

TEST(grand_child1, int a, int b)
{
	START;
	EXPECT_EQ(a + 5, b);
	END;
}

STEST(child1)
{
	SSTART;
	RUN(grand_child1, 3, 8);
	RUN(grand_child1, 4, 9);
	RUN(grand_child1, 5, 11);
	RUN(grand_child1, 6, 14);
	SEND;
}
