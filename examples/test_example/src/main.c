#include "test.h"

#include <string.h>

TEST(success_test)
{
	START;

	unsigned int u = 0;

	EXPECT_EQ(1, 1);
	EXPECT_NE(1, 2);
	EXPECT_GT(2, 1);
	EXPECT_GE(2, 1);
	EXPECT_GE(2, 2);
	EXPECT_LT(1, 2);
	EXPECT_LE(1, 2);
	EXPECT_LE(2, 2);
	EXPECT_EQM(0b00000011, 0b00001010, 0b00000010);
	EXPECT_NEM(0b00000011, 0b00001010, 0b00001011);
	EXPECT_FMT("123", 1, "%3u", &u);
	EXPECT(!strcmp("a", "a"));
	END;
}

TEST(fail_test)
{
	START;

	unsigned int u = 0;

	EXPECT_EQ(1, 2);

	END;
}

TEST(parameterized_test, a, b)
{
	START;
	EXPECT_EQ(a, b);
	END;
}

TEST(child_test)
{
	SSTART;
	RUN(success_test);
	RUN(fail_test);
	SEND;
}

TEST(parent_test)
{
	SSTART;
	RUN(child_test);
	RUN(parameterized_test, 1, 1);
	RUN(parameterized_test, 2, 2);
	RUN(parameterized_test, 3, 3);
	SEND;
}

TEST(test)
{
	SSTART;
	RUN(parent_test);
	SEND;
}

int main(int argc, char **argv)
{
	t_init(80);
	test();
	t_finish();
}
