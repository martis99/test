#include "child1.h"

#include "test.h"

TEST(grand_child1)
{
	START;
	EXPECT_EQ(5 + 5, 10);
	END;
}

STEST(child1)
{
	SSTART;
	RUN(grand_child1);
	RUN(grand_child1);
	RUN(grand_child1);
	RUN(grand_child1);
	SEND;
}
