#include "test.h"

#include <string.h>

TEST(success_test)
{
	START;

	struct {
		unsigned char b : 1;
	} t = { 0 };

	unsigned int u = 0;

	EXPECT_EQ(1, 1);
	EXPECT_NE(1, 2);
	EXPECT_GT(2, 1);
	EXPECT_GE(2, 1);
	EXPECT_GE(2, 2);
	EXPECT_LT(1, 2);
	EXPECT_LE(1, 2);
	EXPECT_LE(2, 2);
	EXPECT_EQB(t.b, 0);
	EXPECT_NEB(t.b, 1);
	EXPECT_EQM(0b00000011, 0b00001010, 0b00000010);
	EXPECT_NEM(0b00000011, 0b00001010, 0b00001011);
	EXPECT_FMT("123", 1, "%3u", &u);
	EXPECT(!strcmp("a", "a"));
	END;
}

TEST(fail_test)
{
	START;

	struct {
		unsigned char b : 1;
	} t = { 0 };

	unsigned int u = 0;

	EXPECT_EQB(t.b, 1);
	EXPECT_EQ((char)0, 1);
	EXPECT_EQ((short)0, 1);
	EXPECT_EQ((int)0, 1);
	EXPECT_EQ((long long)0, 1);

	EXPECT_NEB(t.b, 0);
	EXPECT_EQ(1, 2);
	EXPECT_NE(1, 1);
	EXPECT_GT(1, 2);
	EXPECT_GT(2, 2);
	EXPECT_GE(1, 2);
	EXPECT_LT(2, 1);
	EXPECT_LT(2, 1);
	EXPECT_LE(2, 1);
	EXPECT(!strcmp("a", "b"));
	EXPECT(!strcmp("aaaaaaaaaaaaaaaaaaaa", "bbbbbbbbbbbbbbbbbbbb"));
	EXPECT_EQM(0b00000011, 0b00001010, 0b00001110);
	EXPECT_NEM(0b00000011, 0b00001010, 0b00001011);

	EXPECT_FMT("123", 2, "%3u", &u);
	EXPECT_FMT("abc", 1, "%3u", &u);
	EXPECT_FMT("abcd", 1, "%3u", &u);
	EXPECT_FMT("abssssscd", 1, "%3u", &u);
	EXPECT_FMT("aaaaaaaaaaaaaaaaaaaaaaa", 1, "bbbbbbbbbbbbbbbbbbbbbbb", &u);
	EXPECT_FMT("", 1, "bbbbbbbbbbbbbbbbbbbbbbb", &u);
	EXPECT_FMT("aaaaaaaaaaaaaaaaaaaaaaa", 1, "", &u);

	EXPECT_FAIL("Fail");
	EXPECT_FAIL("Fail %d", 1);

	EXPECT(!strcmp("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"));
	EXPECT(!strcmp("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"));
	EXPECT(!strcmp("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"));
	EXPECT(!strcmp("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"));

	EXPECT_FMT("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 1, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb", &u);
	EXPECT_FMT("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 1, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb", &u);
	EXPECT_FMT("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 1, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb", &u);
	EXPECT_FMT("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 1, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb", &u);
	EXPECT_FMT("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 1, "", &u);
	EXPECT_FMT("", 1, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb", &u);
	EXPECT_FMT("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 1, "", &u);
	EXPECT_FMT("", 1, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb", &u);

	EXPECT_FAIL("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempo");
	EXPECT_FAIL("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor in");
	EXPECT_FAIL("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor inc");
	EXPECT_FAIL("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et");
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
	RUN(success_test);
	RUN(fail_test);
	RUN(child_test);
	SEND;
}

TEST(tests)
{
	SSTART;
	RUN(success_test);
	RUN(fail_test);
	RUN(parent_test);
	SEND;
}

int main(int argc, char **argv)
{
	t_init(80);
	tests();
	t_finish();
}
