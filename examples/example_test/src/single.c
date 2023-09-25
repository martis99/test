#include "test.h"

TEST(test)
{
	START;
	EXPECT_EQ(4 + 4, 9);
	END;
}

TEST(tests)
{
	SSTART;
	RUN(test);
	SEND;
}

int single()
{
	t_init(80);
	tests();
	t_finish();
}
