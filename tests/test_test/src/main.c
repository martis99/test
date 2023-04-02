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
	EXPECT_STR("a", "a");
	EXPECT_STRN("ab", "ac", 1);
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
	EXPECT_EQ((long)0, 1);
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
	EXPECT_EQM(0b00000011, 0b00001010, 0b00001110);
	EXPECT_NEM(0b00000011, 0b00001010, 0b00001011);

	EXPECT_FMT("123", 2, "%3u", &u);
	EXPECT_FMT("aaa", 1, "%3u", &u);
	EXPECT_FMT("aaaa", 1, "%3u", &u);
	EXPECT_FMT("aaaaaaaaa", 1, "%3u", &u);

	EXPECT_STR("a", "b");
	EXPECT_STRN("ab", "bc", 1);

	EXPECT_FAIL("");
	EXPECT_FAIL("-------------------------------------Column-------------------------------------");

	EXPECT_FAIL("------------------------------------EXPECT_EQ-----------------------------------");

	EXPECT_EQ((char)0b0000000001, 0);
	EXPECT_EQ(0, (char)0b0000000001);
	EXPECT_EQ((short)0b000000001, 0);
	EXPECT_EQ(0, (short)0b000000001);
	EXPECT_EQ((int)0b00000000001, 0);
	EXPECT_EQ(0, (int)0b00000000001);
	EXPECT_EQ((long)0b0000000001, 0);
	EXPECT_EQ(0, (long)0b0000000001);
	EXPECT_EQ((long long)0b00001, 0);
	EXPECT_EQ(0, (long long)0b00001);

	EXPECT_FAIL("");
	EXPECT_FAIL("-----------------------------------EXPECT_EQM-----------------------------------");

	EXPECT_EQM((char)0b0000000001, 0, 1);
	EXPECT_EQM(0, (char)0b0000000001, 1);
	EXPECT_EQM((short)0b000000001, 0, 1);
	EXPECT_EQM(0, (short)0b000000001, 1);
	EXPECT_EQM((int)0b00000000001, 0, 1);
	EXPECT_EQM(0, (int)0b00000000001, 1);
	EXPECT_EQM((long)0b0000000001, 0, 1);
	EXPECT_EQM(0, (long)0b0000000001, 1);
	EXPECT_EQM((long long)0b00001, 0, 1);
	EXPECT_EQM(0, (long long)0b00001, 1);

	EXPECT_FAIL("");
	EXPECT_FAIL("-----------------------------------EXPECT_EQB-----------------------------------");

	struct {
		unsigned char aaaaaaaaaaaaaaa : 1;
	} c1 = { 0 };

	EXPECT_EQB(c1.aaaaaaaaaaaaaaa, 1);

	EXPECT_FAIL("");
	EXPECT_FAIL("-----------------------------------EXPECT_FMT-----------------------------------");

	EXPECT_FMT("aaaaaaaaaaaaaaaaaaaaaaa", 1, "bbbbbbbbbbbbbbbbbbbbbbb", &u);
	EXPECT_FMT("", 1, "bbbbbbbbbbbbbbbbbbbbbbb", &u);
	EXPECT_FMT("aaaaaaaaaaaaaaaaaaaaaaa", 1, "", &u);

	EXPECT_FAIL("");
	EXPECT_FAIL("-----------------------------------EXPECT_STR-----------------------------------");

	EXPECT_STR("aaaaaaaaaaaaaaaaaaaaaaa", "bbbbbbbbbbbbbbbbbbbbbbb");
	EXPECT_STR("", "bbbbbbbbbbbbbbbbbbbbbbb");
	EXPECT_STR("aaaaaaaaaaaaaaaaaaaaaaa", "");

	EXPECT_FAIL("");
	EXPECT_FAIL("-----------------------------------EXPECT_STRN-----------------------------------");

	EXPECT_STRN("aaaaaaaaaaaaaaaaaaaaaaac", "bbbbbbbbbbbbbbbbbbbbbbbc", 23);
	EXPECT_STRN("", "bbbbbbbbbbbbbbbbbbbbbbbc", 23);
	EXPECT_STRN("aaaaaaaaaaaaaaaaaaaaaaac", "", 23);

	EXPECT_FAIL("");
	EXPECT_FAIL("-------------------------------------EXPECT-------------------------------------");

	EXPECT(!strcmp("a", "b"));
	EXPECT(!strcmp("aaaaaaaaaaaaaaaaaaaa", "bbbbbbbbbbbbbbbbbbbb"));

	EXPECT_FAIL("");
	EXPECT_FAIL("-----------------------------------EXPECT_FAIL----------------------------------");

	EXPECT_FAIL("Fail");
	EXPECT_FAIL("Fail %d", 1);

	EXPECT_FAIL("");
	EXPECT_FAIL("-------------------------------------Columns------------------------------------");

	EXPECT_FAIL("------------------------------------EXPECT_EQ-----------------------------------");

	EXPECT_FAIL("--------------------------------------char--------------------------------------");

	EXPECT_EQ((char)0b000000000000000000000000000001, 0);
	EXPECT_EQ((char)0, 0b000000000000000000000000000001);
	EXPECT_EQ((char)0b00000000000000000000000000000000000001, 0);
	EXPECT_EQ((char)0, 0b00000000000000000000000000000000000001);

	EXPECT_FAIL("--------------------------------------short-------------------------------------");

	EXPECT_EQ((short)0b00000000000000000000000000001, 0);
	EXPECT_EQ((short)0, 0b00000000000000000000000000001);
	EXPECT_EQ((short)0b0000000000000000000000000000000000001, 0);
	EXPECT_EQ((short)0, 0b0000000000000000000000000000000000001);

	EXPECT_FAIL("---------------------------------------int--------------------------------------");

	EXPECT_EQ((int)0b0000000000000000000000000000001, 0);
	EXPECT_EQ((int)0, 0b0000000000000000000000000000001);
	EXPECT_EQ((int)0b000000000000000000000000000000000000001, 0);
	EXPECT_EQ((int)0, 0b000000000000000000000000000000000000001);

	EXPECT_FAIL("--------------------------------------long--------------------------------------");

	EXPECT_EQ((long)0b000000000000000000000000000001, 0);
	EXPECT_EQ((long)0, 0b000000000000000000000000000001);
	EXPECT_EQ((long)0b00000000000000000000000000000000000001, 0);
	EXPECT_EQ((long)0, 0b00000000000000000000000000000000000001);

	EXPECT_FAIL("------------------------------------long long-----------------------------------");

	EXPECT_EQ((long long)0b0000000000000000001, 0);
	EXPECT_EQ((long long)0, 0b0000000000000000001);
	EXPECT_EQ((long long)0b0000000000000000000000001, 0);
	EXPECT_EQ((long long)0, 0b0000000000000000000000001);

	EXPECT_FAIL("");
	EXPECT_FAIL("-----------------------------------EXPECT_EQM-----------------------------------");

	EXPECT_FAIL("--------------------------------------char--------------------------------------");

	EXPECT_EQM((char)0b000000000000000000000000000001, 0, 1);
	EXPECT_EQM((char)0, 0b000000000000000000000000000001, 1);
	EXPECT_EQM((char)0b00000000000000000000000000000000000001, 0, 1);
	EXPECT_EQM((char)0, 0b00000000000000000000000000000000000001, 1);

	EXPECT_FAIL("--------------------------------------short-------------------------------------");

	EXPECT_EQM((short)0b00000000000000000000000000001, 0, 1);
	EXPECT_EQM((short)0, 0b00000000000000000000000000001, 1);
	EXPECT_EQM((short)0b0000000000000000000000000000000000001, 0, 1);
	EXPECT_EQM((short)0, 0b0000000000000000000000000000000000001, 1);

	EXPECT_FAIL("---------------------------------------int--------------------------------------");

	EXPECT_EQM((int)0b0000000000000000000000000000001, 0, 1);
	EXPECT_EQM((int)0, 0b0000000000000000000000000000001, 1);
	EXPECT_EQM((int)0b000000000000000000000000000000000000001, 0, 1);
	EXPECT_EQM((int)0, 0b000000000000000000000000000000000000001, 1);

	EXPECT_FAIL("--------------------------------------long--------------------------------------");

	EXPECT_EQM((long)0b000000000000000000000000000001, 0, 1);
	EXPECT_EQM((long)0, 0b000000000000000000000000000001, 1);
	EXPECT_EQM((long)0b00000000000000000000000000000000000001, 0, 1);
	EXPECT_EQM((long)0, 0b00000000000000000000000000000000000001, 1);

	EXPECT_FAIL("------------------------------------long long-----------------------------------");

	EXPECT_EQM((long long)0b0000000000000000001, 0, 1);
	EXPECT_EQM((long long)0, 0b0000000000000000001, 1);
	EXPECT_EQM((long long)0b0000000000000000000000001, 0, 1);
	EXPECT_EQM((long long)0, 0b0000000000000000000000001, 1);

	EXPECT_FAIL("");
	EXPECT_FAIL("-----------------------------------EXPECT_EQB-----------------------------------");

	struct {
		unsigned char aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa : 1;
		unsigned char aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa : 1;
	} c = { 0 };

	EXPECT_EQB(c.aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa, 1);
	EXPECT_EQB(c.aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa, 1);

	EXPECT_FAIL("");
	EXPECT_FAIL("------------------------------------Overfill------------------------------------");

	EXPECT_FAIL("------------------------------------EXPECT_EQ-----------------------------------");

	EXPECT_FAIL("--------------------------------------char--------------------------------------");

	EXPECT_EQ((char)0b000000000000000000000000000000000000000000000001, 0);
	EXPECT_EQ((char)0, 0b000000000000000000000000000000000000000000000001);
	EXPECT_EQ((char)0b000000000000000000000000000000000000000000000000000001, 0);
	EXPECT_EQ((char)0, 0b000000000000000000000000000000000000000000000000000001);
	EXPECT_EQ((char)0b000000000000000000000000000000000000000000000000000000000001, 0);
	EXPECT_EQ((char)0, 0b000000000000000000000000000000000000000000000000000000000001);

	EXPECT_FAIL("--------------------------------------short-------------------------------------");

	EXPECT_EQ((short)0b00000000000000000000000000000000000000000000001, 0);
	EXPECT_EQ((short)0, 0b00000000000000000000000000000000000000000000001);
	EXPECT_EQ((short)0b00000000000000000000000000000000000000000000000000001, 0);
	EXPECT_EQ((short)0, 0b00000000000000000000000000000000000000000000000000001);
	EXPECT_EQ((short)0b00000000000000000000000000000000000000000000000000000000001, 0);
	EXPECT_EQ((short)0, 0b00000000000000000000000000000000000000000000000000000000001);

	EXPECT_FAIL("---------------------------------------int--------------------------------------");

	EXPECT_EQ((int)0b0000000000000000000000000000000000000000000000001, 0);
	EXPECT_EQ((int)0, 0b0000000000000000000000000000000000000000000000001);
	EXPECT_EQ((int)0b0000000000000000000000000000000000000000000000000000001, 0);
	EXPECT_EQ((int)0, 0b0000000000000000000000000000000000000000000000000000001);
	EXPECT_EQ((int)0b0000000000000000000000000000000000000000000000000000000000001, 0);
	EXPECT_EQ((int)0, 0b0000000000000000000000000000000000000000000000000000000000001);

	EXPECT_FAIL("--------------------------------------long--------------------------------------");

	EXPECT_EQ((long)0b000000000000000000000000000000000000000000000001, 0);
	EXPECT_EQ((long)0, 0b000000000000000000000000000000000000000000000001);
	EXPECT_EQ((long)0b000000000000000000000000000000000000000000000000000001, 0);
	EXPECT_EQ((long)0, 0b000000000000000000000000000000000000000000000000000001);
	EXPECT_EQ((long)0b000000000000000000000000000000000000000000000000000000000001, 0);
	EXPECT_EQ((long)0, 0b000000000000000000000000000000000000000000000000000000000001);

	EXPECT_FAIL("------------------------------------long long-----------------------------------");

	EXPECT_EQ((long long)0b000000000000000000000000001, 0);
	EXPECT_EQ((long long)0, 0b000000000000000000000000001);
	EXPECT_EQ((long long)0b000000000000000000000000000000001, 0);
	EXPECT_EQ((long long)0, 0b000000000000000000000000000000001);
	EXPECT_EQ((long long)0b000000000000000000000000000000000000001, 0);
	EXPECT_EQ((long long)0, 0b000000000000000000000000000000000000001);

	EXPECT_FAIL("");
	EXPECT_FAIL("-----------------------------------EXPECT_EQM-----------------------------------");

	EXPECT_FAIL("--------------------------------------char--------------------------------------");

	EXPECT_EQM((char)0b000000000000000000000000000000000000000000000001, 0, 1);
	EXPECT_EQM((char)0, 0b000000000000000000000000000000000000000000000001, 1);
	EXPECT_EQM((char)0b000000000000000000000000000000000000000000000000000001, 0, 1);
	EXPECT_EQM((char)0, 0b000000000000000000000000000000000000000000000000000001, 1);
	EXPECT_EQM((char)0b000000000000000000000000000000000000000000000000000000000001, 0, 1);
	EXPECT_EQM((char)0, 0b000000000000000000000000000000000000000000000000000000000001, 1);

	EXPECT_FAIL("--------------------------------------short-------------------------------------");

	EXPECT_EQM((short)0b00000000000000000000000000000000000000000000001, 0, 1);
	EXPECT_EQM((short)0, 0b00000000000000000000000000000000000000000000001, 1);
	EXPECT_EQM((short)0b00000000000000000000000000000000000000000000000000001, 0, 1);
	EXPECT_EQM((short)0, 0b00000000000000000000000000000000000000000000000000001, 1);
	EXPECT_EQM((short)0b00000000000000000000000000000000000000000000000000000000001, 0, 1);
	EXPECT_EQM((short)0, 0b00000000000000000000000000000000000000000000000000000000001, 1);

	EXPECT_FAIL("---------------------------------------int--------------------------------------");

	EXPECT_EQM((int)0b0000000000000000000000000000000000000000000000001, 0, 1);
	EXPECT_EQM((int)0, 0b0000000000000000000000000000000000000000000000001, 1);
	EXPECT_EQM((int)0b0000000000000000000000000000000000000000000000000000001, 0, 1);
	EXPECT_EQM((int)0, 0b0000000000000000000000000000000000000000000000000000001, 1);
	EXPECT_EQM((int)0b0000000000000000000000000000000000000000000000000000000000001, 0, 1);
	EXPECT_EQM((int)0, 0b0000000000000000000000000000000000000000000000000000000000001, 1);

	EXPECT_FAIL("--------------------------------------long--------------------------------------");

	EXPECT_EQM((long)0b000000000000000000000000000000000000000000000001, 0, 1);
	EXPECT_EQM((long)0, 0b000000000000000000000000000000000000000000000001, 1);
	EXPECT_EQM((long)0b000000000000000000000000000000000000000000000000000001, 0, 1);
	EXPECT_EQM((long)0, 0b000000000000000000000000000000000000000000000000000001, 1);
	EXPECT_EQM((long)0b000000000000000000000000000000000000000000000000000000000001, 0, 1);
	EXPECT_EQM((long)0, 0b000000000000000000000000000000000000000000000000000000000001, 1);

	EXPECT_FAIL("------------------------------------long long-----------------------------------");

	EXPECT_EQM((long long)0b000000000000000000000000001, 0, 1);
	EXPECT_EQM((long long)0, 0b000000000000000000000000001, 1);
	EXPECT_EQM((long long)0b000000000000000000000000000000001, 0, 1);
	EXPECT_EQM((long long)0, 0b000000000000000000000000000000001, 1);
	EXPECT_EQM((long long)0b000000000000000000000000000000000000001, 0, 1);
	EXPECT_EQM((long long)0, 0b000000000000000000000000000000000000001, 1);

	EXPECT_FAIL("");
	EXPECT_FAIL("-----------------------------------EXPECT_EQB-----------------------------------");

	struct {
		unsigned char aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa : 1;
		unsigned char aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa : 1;
		unsigned char aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa : 1;
	} o = { 0 };

	EXPECT_EQB(o.aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa, 1);
	EXPECT_EQB(o.aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa, 1);
	EXPECT_EQB(o.aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa, 1);

	EXPECT_FAIL("");
	EXPECT_FAIL("-------------------------------------EXPECT-------------------------------------");

	EXPECT(!strcmp("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "a"));
	EXPECT(!strcmp("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "a"));
	EXPECT(!strcmp("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "a"));

	EXPECT_FAIL("");
	EXPECT_FAIL("-----------------------------------EXPECT_FMT-----------------------------------");

	EXPECT_FMT("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 1, "a", &u);
	EXPECT_FMT("a", 1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", &u);
	EXPECT_FMT("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 1, "a", &u);
	EXPECT_FMT("a", 1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", &u);
	EXPECT_FMT("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 1, "a", &u);
	EXPECT_FMT("a", 1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", &u);

	EXPECT_FAIL("");
	EXPECT_FAIL("-----------------------------------EXPECT_STR-----------------------------------");

	EXPECT_STR("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "a");
	EXPECT_STR("a", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	EXPECT_STR("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "a");
	EXPECT_STR("a", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	EXPECT_STR("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "a");
	EXPECT_STR("a", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

	EXPECT_FAIL("");
	EXPECT_FAIL("-----------------------------------EXPECT_STRN----------------------------------");

	EXPECT_STRN("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "a", 77);
	EXPECT_STRN("a", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 77);
	EXPECT_STRN("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "a", 83);
	EXPECT_STRN("a", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 83);
	EXPECT_STRN("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "a", 89);
	EXPECT_STRN("a", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 89);

	EXPECT_FAIL("");
	EXPECT_FAIL("-----------------------------------EXPECT_FAIL----------------------------------");

	EXPECT_FAIL("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor inc");
	EXPECT_FAIL("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididun");
	EXPECT_FAIL("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut l");
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
