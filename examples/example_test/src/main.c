#include "test.h"

#include <stdio.h>
#include <string.h>

#define SEP "\n------%s------\n\n"

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
	EXPECT_EQM(0x3, 0xa, 0x2);
	EXPECT_NEM(0x3, 0xa, 0xb);
	EXPECT_FMT("123", 1, "%3u", &u);
	EXPECT(!strcmp("a", "a"));
	END;
}

TEST(fail_test)
{
	START;
	EXPECT_EQ(1, 2);
	END;
}

TESTP(parameterized_test, int a, int b)
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
	RUNP(parameterized_test, 1, 1);
	RUNP(parameterized_test, 2, 2);
	RUNP(parameterized_test, 3, 3);
	SEND;
}

TEST(tests)
{
	SSTART;
	RUN(parent_test);
	SEND;
}

void single();
void multiple();
void parameterized();
void multilevel();
void separated();
void setup_teardown();

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	printf(SEP, "main");
	t_init(80);
	t_run(tests, 1);
	t_finish();

	printf(SEP, "single");
	single();

	printf(SEP, "multiple");
	multiple();

	printf(SEP, "parameterized");
	parameterized();

	printf(SEP, "multilevel");
	multilevel();

	printf(SEP, "separated");
	separated();

	printf(SEP, "setup_teardown");
	setup_teardown();

	return 0;
}
