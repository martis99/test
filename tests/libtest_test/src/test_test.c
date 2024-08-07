#include "test_test.h"

#include "mem.h"
#include "test.h"
#include "test_cplatform.h"
#include "type.h"

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct tdata_s {
	void *priv;
	setup_fn setup;
	setup_fn teardown;
	print_dst_t print;
	wprint_dst_t wprint;
	int width;
	long long passed;
	long long failed;
	int depth;
	char *buf;
	size_t buf_size;
	size_t buf_len;
	const char *exp;
	size_t exp_len;
	size_t mem;
} tdata_t;

extern tdata_t t_get_data();
extern void t_set_data(tdata_t data);

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
	EXPECT_EQM(0x3, 0xa, 0x2);
	EXPECT_NEM(0x3, 0xa, 0xb);
	EXPECT_FMT("123", 1, "%3u", &u);
	EXPECT_STR(NULL, NULL);
	EXPECT_STR("a", "a");
	EXPECT_STRN(NULL, NULL, 1);
	EXPECT_STRN("ab", "ac", 1);
	EXPECT_WSTR(NULL, NULL);
	EXPECT_WSTR(L"a", L"a");
	EXPECT_WSTRN(NULL, NULL, 1);
	EXPECT_WSTRN(L"ab", L"ac", 1);
	EXPECT(!strcmp("a", "a"));
	char exp[] = "\tTest\n\tTest";
	EXPECT_FSTR(t_fprintf(NULL, "\tTest\n\tTest"), exp, sizeof(exp) - 1);
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
	EXPECT_EQM(0x3, 0xa, 0xe);
	EXPECT_NEM(0x3, 0xa, 0xb);

	EXPECT_FMT("123", 2, "%3u", &u);
	EXPECT_FMT("aaa", 1, "%3u", &u);
	EXPECT_FMT("aaaa", 1, "%3u", &u);
	EXPECT_FMT("aaaaaaaaa", 1, "%3u", &u);

	EXPECT_STR("", NULL);
	EXPECT_STR("a", "b");
	EXPECT_STRN("ab", NULL, 1);
	EXPECT_STRN("ab", "bc", 1);
	EXPECT_WSTR(L"", NULL);
	EXPECT_WSTR(L"a", L"b");
	EXPECT_WSTRN(L"ab", NULL, 1);
	EXPECT_WSTRN(L"ab", L"bc", 1);

	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "-----------------------------------EXPECT_EQB-----------------------------------");

	EXPECT_EQB(0, 1);
	EXPECT_EQB(1, 0);

	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "------------------------------------EXPECT_EQ-----------------------------------");

	EXPECT_EQ(U8_MIN, U8_MAX);
	EXPECT_EQ(U8_MIN, U16_MAX);
	EXPECT_EQ(U8_MIN, U32_MAX);
	EXPECT_EQ(U8_MIN, U64_MAX);
	EXPECT_EQ(U8_MAX, U8_MIN);
	EXPECT_EQ(U16_MIN, U8_MAX);
	EXPECT_EQ(U16_MIN, U16_MAX);
	EXPECT_EQ(U16_MIN, U32_MAX);
	EXPECT_EQ(U16_MIN, U64_MAX);
	EXPECT_EQ(U16_MAX, U16_MIN);

	EXPECT_EQ(U32_MIN, U8_MAX);
	EXPECT_EQ(U32_MIN, U16_MAX);
	EXPECT_EQ(U32_MIN, U32_MAX);
	EXPECT_EQ(U32_MIN, U64_MAX);
	EXPECT_EQ(U32_MAX, U32_MIN);

	EXPECT_EQ(U64_MIN, U8_MAX);
	EXPECT_EQ(U64_MIN, U16_MAX);
	EXPECT_EQ(U64_MIN, U32_MAX);
	EXPECT_EQ(U64_MIN, U64_MAX);
	EXPECT_EQ(U64_MAX, U64_MIN);

	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "-----------------------------------EXPECT_EQM-----------------------------------");

	EXPECT_EQM(U8_MIN, U8_MAX, 1);
	EXPECT_EQM(U8_MAX, U8_MIN, 1);
	EXPECT_EQM(U16_MIN, U16_MAX, 1);
	EXPECT_EQM(U16_MAX, U16_MIN, 1);
	EXPECT_EQM(U32_MIN, U32_MAX, 1);
	EXPECT_EQM(U32_MAX, U32_MIN, 1);
	EXPECT_EQM(U64_MIN, U64_MAX, 1);
	EXPECT_EQM(U64_MAX, U64_MIN, 1);

	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "-----------------------------------EXPECT_FMT-----------------------------------");

	EXPECT_FMT("\taaaaaaa", 1, "\ta", &u);
	EXPECT_FMT("\ta", 1, "\taaaaaaa", &u);

	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "-----------------------------------EXPECT_STR-----------------------------------");

	EXPECT_STR("aaaaaaa", "a");
	EXPECT_STR("a", "aaaaaaa");
	EXPECT_STR("a\n", "b\n");
	EXPECT_STR("\n\r\ta", "\n\r\tb");

	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "-----------------------------------EXPECT_STRN----------------------------------");

	EXPECT_STRN("aaaaaaa", "a", 7);
	EXPECT_STRN("a", "aaaaaaa", 7);

	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "-----------------------------------EXPECT_WSTR-----------------------------------");

	EXPECT_WSTR(L"aaaaaaa", L"a");
	EXPECT_WSTR(L"a", L"aaaaaaa");
	EXPECT_WSTR(L"a\n", L"b\n");
	EXPECT_WSTR(L"\n\r\ta", L"\n\r\tb");

	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "-----------------------------------EXPECT_WSTRN----------------------------------");

	EXPECT_WSTRN(L"aaaaaaa", L"a", 7);
	EXPECT_WSTRN(L"a", L"aaaaaaa", 7);

	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "-------------------------------------EXPECT-------------------------------------");

	EXPECT(!strcmp("a", "b"));
	EXPECT(!strcmp("aaaaaaaaaaaaaaaaaaaa", "bbbbbbbbbbbbbbbbbbbb"));

	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "-----------------------------------EXPECT_FAIL----------------------------------");

	EXPECT_FAIL("%s", "Fail");
	EXPECT_FAIL("Fail %d", 1);

	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "----------------------------------Left Overfill---------------------------------");

	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "-----------------------------------EXPECT_EQB-----------------------------------");

	EXPECT_EQB(0, 1);
	EXPECT_EQB(1, 0);
	EXPECT_EQB(0000000000000000, 1);
	EXPECT_EQB(1, 0000000000000000);
	EXPECT_EQB(00000000000000000, 1);
	EXPECT_EQB(1, 000000000000000000);

	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "------------------------------------EXPECT_EQ-----------------------------------");

	EXPECT_EQ((u8)0, (u8)1);
	EXPECT_EQ((u8)1, (u8)0);
	EXPECT_EQ((u8)0000000000000, (u8)1);
	EXPECT_EQ((u8)1, (u8)00000000000000);
	EXPECT_EQ((u16)0, (u16)1);
	EXPECT_EQ((u16)1, (u16)0);
	EXPECT_EQ((u16)000000000000, (u16)1);
	EXPECT_EQ((u16)1, (u16)0000000000000);
	EXPECT_EQ((u32)0, (u32)1);
	EXPECT_EQ((u32)1, (u32)0);
	EXPECT_EQ((u32)000000000000, (u32)1);
	EXPECT_EQ((u32)1, (u32)0000000000000);
	EXPECT_EQ((u64)0, (u64)1);
	EXPECT_EQ((u64)1, (u64)0);
	EXPECT_EQ((u64)000000000000, (u64)1);
	EXPECT_EQ((u64)1, (u64)0000000000000);

	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "-------------------------------------EXPECT-------------------------------------");

	EXPECT(!strcmp("aaaaaaaaaaaaaaaaaaaa", "a"));
	EXPECT(!strcmp("aaaaaaaaaaaaaaaaaaaaa", "a"));
	EXPECT(!strcmp("aaaaaaaaaaaaaaaaaaaaaa", "a"));
	EXPECT(!strcmp("aaaaaaaaaaaaaaaaaaaaaaa", "a"));
	EXPECT(!strcmp("aaaaaaaaaaaaaaaaaaaaaaaa", "a"));

	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "-------------------------------------Overfill-----------------------------------");

	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "-----------------------------------EXPECT_EQB-----------------------------------");

	EXPECT_EQB(000000000000000000000000000000000000000, 1);
	EXPECT_EQB(1, 000000000000000000000000000000000000000);
	EXPECT_EQB(000000000000000000000000000000000000000000000000, 1);
	EXPECT_EQB(1, 000000000000000000000000000000000000000000000000);
	EXPECT_EQB(000000000000000000000000000000000000000000000000000, 1);
	EXPECT_EQB(1, 000000000000000000000000000000000000000000000000000);
	EXPECT_EQB(000000000000000000000000000000000000000000000000000000000000000000000, 1);
	EXPECT_EQB(1, 000000000000000000000000000000000000000000000000000000000000000000000);
	EXPECT_EQB(0000000000000000000000000000000000000000000000000000000000000000000000000, 1);
	EXPECT_EQB(1, 0000000000000000000000000000000000000000000000000000000000000000000000000);
	EXPECT_EQB(0000000000000000000000000000000000000000000000000000000000000000000000000000, 1);
	EXPECT_EQB(1, 0000000000000000000000000000000000000000000000000000000000000000000000000000);
	EXPECT_EQB(00000000000000000000000000000000000000000000000000000000000000000000000000000000, 1);
	EXPECT_EQB(1, 00000000000000000000000000000000000000000000000000000000000000000000000000000000);

	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "------------------------------------EXPECT_EQ-----------------------------------");

	EXPECT_FAIL("%s", "---------------------------------------8----------------------------------------");

	EXPECT_EQ((u8)000000000000000000000000000001, (u8)0);
	EXPECT_EQ((u8)0, (u8)000000000000000000000000000001);
	EXPECT_EQ((u8)000000000000000000000000000000001, (u8)0);
	EXPECT_EQ((u8)0, (u8)000000000000000000000000000000001);
	EXPECT_EQ((u8)00000000000000000000000000000000000001, (u8)0);
	EXPECT_EQ((u8)0, (u8)00000000000000000000000000000000000001);
	EXPECT_EQ((u8)00000000000000000000000000000000000000000000001, (u8)0);
	EXPECT_EQ((u8)0, (u8)00000000000000000000000000000000000000000000001);
	EXPECT_EQ((u8)000000000000000000000000000000000000000000000000001, (u8)0);
	EXPECT_EQ((u8)0, (u8)000000000000000000000000000000000000000000000000001);
	EXPECT_EQ((u8)000000000000000000000000000000000000000000000000000001, (u8)0);
	EXPECT_EQ((u8)0, (u8)000000000000000000000000000000000000000000000000000001);
	EXPECT_EQ((u8)0000000000000000000000000000000000000000000000000000000001, (u8)0);
	EXPECT_EQ((u8)0, (u8)0000000000000000000000000000000000000000000000000000000001);

	EXPECT_FAIL("%s", "---------------------------------------16---------------------------------------");

	EXPECT_EQ((u16)0000000000000000000000000001, (u16)0);
	EXPECT_EQ((u16)0, (u16)0000000000000000000000000001);
	EXPECT_EQ((u16)00000000000000000000000000000000001, (u16)0);
	EXPECT_EQ((u16)0, (u16)00000000000000000000000000000000001);
	EXPECT_EQ((u16)00000000000000000000000000000000000001, (u16)0);
	EXPECT_EQ((u16)0, (u16)00000000000000000000000000000000000001);
	EXPECT_EQ((u16)00000000000000000000000000000000000000000000000000001, (u16)0);
	EXPECT_EQ((u16)0, (u16)00000000000000000000000000000000000000000000000000001);
	EXPECT_EQ((u16)000000000000000000000000000000000000000000000000000000001, (u16)0);
	EXPECT_EQ((u16)0, (u16)000000000000000000000000000000000000000000000000000000001);
	EXPECT_EQ((u16)000000000000000000000000000000000000000000000000000000000001, (u16)0);
	EXPECT_EQ((u16)0, (u16)000000000000000000000000000000000000000000000000000000000001);
	EXPECT_EQ((u16)0000000000000000000000000000000000000000000000000000000000000001, (u16)0);
	EXPECT_EQ((u16)0, (u16)0000000000000000000000000000000000000000000000000000000000000001);

	EXPECT_FAIL("%s", "---------------------------------------32---------------------------------------");

	EXPECT_EQ((u32)0000000000000000000000000001, (u32)0);
	EXPECT_EQ((u32)0, (u32)0000000000000000000000000001);
	EXPECT_EQ((u32)0000000000000000000000000000001, (u32)0);
	EXPECT_EQ((u32)0, (u32)0000000000000000000000000000001);
	EXPECT_EQ((u32)000000000000000000000000000000000001, (u32)0);
	EXPECT_EQ((u32)0, (u32)000000000000000000000000000000000001);
	EXPECT_EQ((u32)000000000000000000000000000000000000000000001, (u32)0);
	EXPECT_EQ((u32)0, (u32)000000000000000000000000000000000000000000001);
	EXPECT_EQ((u32)0000000000000000000000000000000000000000000000001, (u32)0);
	EXPECT_EQ((u32)0, (u32)0000000000000000000000000000000000000000000000001);
	EXPECT_EQ((u32)0000000000000000000000000000000000000000000000000001, (u32)0);
	EXPECT_EQ((u32)0, (u32)0000000000000000000000000000000000000000000000000001);
	EXPECT_EQ((u32)00000000000000000000000000000000000000000000000000000001, (u32)0);
	EXPECT_EQ((u32)0, (u32)00000000000000000000000000000000000000000000000000000001);

	EXPECT_FAIL("%s", "---------------------------------------64---------------------------------------");

	EXPECT_EQ((u64)0000000000000000001, (u64)0);
	EXPECT_EQ((u64)0, (u64)0000000000000000001);
	EXPECT_EQ((u64)00000000000000000000001, (u64)0);
	EXPECT_EQ((u64)0, (u64)00000000000000000000001);
	EXPECT_EQ((u64)0000000000000000000000001, (u64)0);
	EXPECT_EQ((u64)0, (u64)0000000000000000000000001);
	EXPECT_EQ((u64)00000000000000000000000000001, (u64)0);
	EXPECT_EQ((u64)0, (u64)00000000000000000000000000001);
	EXPECT_EQ((u64)000000000000000000000000000000001, (u64)0);
	EXPECT_EQ((u64)0, (u64)000000000000000000000000000000001);
	EXPECT_EQ((u64)000000000000000000000000000000000001, (u64)0);
	EXPECT_EQ((u64)0, (u64)000000000000000000000000000000000001);
	EXPECT_EQ((u64)0000000000000000000000000000000000000001, (u64)0);
	EXPECT_EQ((u64)0, (u64)0000000000000000000000000000000000000001);

	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "-----------------------------------EXPECT_EQM-----------------------------------");

	EXPECT_FAIL("%s", "---------------------------------------8----------------------------------------");

	EXPECT_EQM((u8)000000000000000000000000000001, (u8)0, 1);
	EXPECT_EQM((u8)0, (u8)000000000000000000000000000001, 1);
	EXPECT_EQM((u8)000000000000000000000000000000001, (u8)0, 1);
	EXPECT_EQM((u8)0, (u8)000000000000000000000000000000001, 1);
	EXPECT_EQM((u8)00000000000000000000000000000000000001, (u8)0, 1);
	EXPECT_EQM((u8)0, (u8)00000000000000000000000000000000000001, 1);
	EXPECT_EQM((u8)000000000000000000000000000000000000000001, (u8)0, 1);
	EXPECT_EQM((u8)0, (u8)000000000000000000000000000000000000000001, 1);
	EXPECT_EQM((u8)00000000000000000000000000000000000000000000001, (u8)0, 1);
	EXPECT_EQM((u8)0, (u8)00000000000000000000000000000000000000000000001, 1);
	EXPECT_EQM((u8)000000000000000000000000000000000000000000000000001, (u8)0, 1);
	EXPECT_EQM((u8)0, (u8)000000000000000000000000000000000000000000000000001, 1);
	EXPECT_EQM((u8)000000000000000000000000000000000000000000000000000001, (u8)0, 1);
	EXPECT_EQM((u8)0, (u8)000000000000000000000000000000000000000000000000000001, 1);
	EXPECT_EQM((u8)0000000000000000000000000000000000000000000000000000000001, (u8)0, 1);
	EXPECT_EQM((u8)0, (u8)0000000000000000000000000000000000000000000000000000000001, 1);

	EXPECT_FAIL("%s", "---------------------------------------16---------------------------------------");

	EXPECT_EQM((u16)0000000000000000000000000001, (u16)0, 1);
	EXPECT_EQM((u16)0, (u16)0000000000000000000000000001, 1);
	EXPECT_EQM((u16)00000000000000000000000000000000001, (u16)0, 1);
	EXPECT_EQM((u16)0, (u16)00000000000000000000000000000000001, 1);
	EXPECT_EQM((u16)00000000000000000000000000000000000001, (u16)0, 1);
	EXPECT_EQM((u16)0, (u16)00000000000000000000000000000000000001, 1);
	EXPECT_EQM((u16)000000000000000000000000000000000000000000000001, (u16)0, 1);
	EXPECT_EQM((u16)0, (u16)000000000000000000000000000000000000000000000001, 1);
	EXPECT_EQM((u16)00000000000000000000000000000000000000000000000000001, (u16)0, 1);
	EXPECT_EQM((u16)0, (u16)00000000000000000000000000000000000000000000000000001, 1);
	EXPECT_EQM((u16)000000000000000000000000000000000000000000000000000000001, (u16)0, 1);
	EXPECT_EQM((u16)0, (u16)000000000000000000000000000000000000000000000000000000001, 1);
	EXPECT_EQM((u16)000000000000000000000000000000000000000000000000000000000001, (u16)0, 1);
	EXPECT_EQM((u16)0, (u16)000000000000000000000000000000000000000000000000000000000001, 1);
	EXPECT_EQM((u16)0000000000000000000000000000000000000000000000000000000000000001, (u16)0, 1);
	EXPECT_EQM((u16)0, (u16)0000000000000000000000000000000000000000000000000000000000000001, 1);

	EXPECT_FAIL("%s", "---------------------------------------32---------------------------------------");

	EXPECT_EQM((u32)0000000000000000000000000001, (u32)0, 1);
	EXPECT_EQM((u32)0, (u32)0000000000000000000000000001, 1);
	EXPECT_EQM((u32)0000000000000000000000000000001, (u32)0, 1);
	EXPECT_EQM((u32)0, (u32)0000000000000000000000000000001, 1);
	EXPECT_EQM((u32)000000000000000000000000000000000001, (u32)0, 1);
	EXPECT_EQM((u32)0, (u32)000000000000000000000000000000000001, 1);
	EXPECT_EQM((u32)0000000000000000000000000000000000000001, (u32)0, 1);
	EXPECT_EQM((u32)0, (u32)0000000000000000000000000000000000000001, 1);
	EXPECT_EQM((u32)000000000000000000000000000000000000000000001, (u32)0, 1);
	EXPECT_EQM((u32)0, (u32)000000000000000000000000000000000000000000001, 1);
	EXPECT_EQM((u32)0000000000000000000000000000000000000000000000001, (u32)0, 1);
	EXPECT_EQM((u32)0, (u32)0000000000000000000000000000000000000000000000001, 1);
	EXPECT_EQM((u32)0000000000000000000000000000000000000000000000000001, (u32)0, 1);
	EXPECT_EQM((u32)0, (u32)0000000000000000000000000000000000000000000000000001, 1);
	EXPECT_EQM((u32)00000000000000000000000000000000000000000000000000000001, (u32)0, 1);
	EXPECT_EQM((u32)0, (u32)00000000000000000000000000000000000000000000000000000001, 1);

	EXPECT_FAIL("%s", "---------------------------------------64---------------------------------------");

	EXPECT_EQM((u64)0000000000000000001, (u64)0, 1);
	EXPECT_EQM((u64)0, (u64)0000000000000000001, 1);
	EXPECT_EQM((u64)00000000000000000000001, (u64)0, 1);
	EXPECT_EQM((u64)0, (u64)00000000000000000000001, 1);
	EXPECT_EQM((u64)000000000000000000000001, (u64)0, 1);
	EXPECT_EQM((u64)0, (u64)000000000000000000000001, 1);
	EXPECT_EQM((u64)00000000000000000000000000001, (u64)0, 1);
	EXPECT_EQM((u64)0, (u64)00000000000000000000000000001, 1);
	EXPECT_EQM((u64)000000000000000000000000000000001, (u64)0, 1);
	EXPECT_EQM((u64)0, (u64)000000000000000000000000000000001, 1);

	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "-------------------------------------EXPECT-------------------------------------");

	EXPECT(!strcmp("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "a"));
	EXPECT(!strcmp("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "a"));
	EXPECT(!strcmp("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "a"));
	EXPECT(!strcmp("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "a"));

	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "-----------------------------------EXPECT_FAIL----------------------------------");

	EXPECT_FAIL("%s", "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor in");
	EXPECT_FAIL("%s", "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incidi");
	EXPECT_FAIL("%s", "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididun");
	EXPECT_FAIL("%s", "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut l");

	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "-----------------------------------EXPECT_FSTR----------------------------------");
	EXPECT_FAIL("%s", "---------------------------------Missing actual---------------------------------");
	{
		char exp[] = "Test\r\n";
		EXPECT_FSTR(t_fprintf(NULL, "est\r\n"), exp, sizeof(exp) - 1);
	}
	EXPECT_FAIL("%s", "--------------------------------Missing expected--------------------------------");
	{
		char exp[] = "est\n";
		EXPECT_FSTR(t_fprintf(NULL, "Test\n"), exp, sizeof(exp) - 1);
	}
	EXPECT_FAIL("%s", "------------------------------------Different-----------------------------------");
	{
		char exp[] = "Test\n";
		EXPECT_FSTR(t_fprintf(NULL, "Tost\n"), exp, sizeof(exp) - 1);
	}
	EXPECT_FAIL("%s", "------------------------------Missing \\n expected------------------------------");
	{
		char exp[] = "Test";
		EXPECT_FSTR(t_fprintf(NULL, "Test\n"), exp, sizeof(exp) - 1);
	}
	EXPECT_FAIL("%s", "-------------------------------Missing \\n actual-------------------------------");
	{
		char exp[] = "Test\n";
		EXPECT_FSTR(t_fprintf(NULL, "Test"), exp, sizeof(exp) - 1);
	}
	EXPECT_FAIL("%s", "----------------------------Different in second line----------------------------");
	{
		char exp[] = "\tTest\n\tTest";
		EXPECT_FSTR(t_fprintf(NULL, "\tTest\n\tGest"), exp, sizeof(exp) - 1);
	}
	EXPECT_FAIL("%s", "---------------------------------------Long-------------------------------------");
	{
		char exp[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit,\n"
			     "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n"
			     "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n"
			     "Duis aute irure dolor in rep";
		EXPECT_FSTR(t_fprintf(NULL, "Lorem ipsum dolor sit amet, consectetur adipiscing elit,\n"
					    "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n"
					    "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n"
					    "Duis aute irure dolor in res"),
			    exp, sizeof(exp) - 1);
	}
	_passed = 1;
	END;
}

TEST(child_test)
{
	SSTART;
	RUN(success_test);
	RUN(fail_test);
	SEND;
}

TESTP(param_test, int x)
{
	START;
	EXPECT_EQ(x, 1);
	END;
}

TEST(parent_test)
{
	SSTART;
	RUN(success_test);
	RUN(fail_test);
	RUN(child_test);
	RUNP(param_test, 1);
	SEND;
}

int empty_test()
{
	return 1;
}

TEST(t_t_run)
{
	START;

	EXPECT_EQ(t_run(empty_test, 0), 1);
	EXPECT_EQ(t_run(empty_test, 1), 1);

	END;
}

TEST(t_t_end)
{
	START;

	tdata_t tdata = t_get_data();

	t_set_print(PRINT_DST_NONE());
	t_set_wprint(PRINT_DST_WNONE());

	t_start();

	void *data = mem_alloc(1);

	EXPECT_EQ(t_end(1, ""), 1);

	mem_free(data, 1);

	t_set_data(tdata);

	END;
}

TEST(t_t_set_priv)
{
	START;

	tdata_t tdata = t_get_data();

	int a = 0;
	t_set_priv(&a);
	EXPECT_EQ(t_get_priv(), &a);

	t_set_data(tdata);

	END;
}

static int setup(void *priv)
{
	(void)priv;
	return 0;
}

static int teardown(void *priv)
{
	(void)priv;
	return 0;
}

TEST(t_t_setup_teardown)
{
	START;

	tdata_t tdata = t_get_data();

	t_set_print(PRINT_DST_NONE());
	t_set_wprint(PRINT_DST_WNONE());

	t_setup(setup);
	t_teardown(teardown);

	t_start();
	t_end(0, "");

	t_set_data(tdata);

	END;
}

TEST(t_t_expect)
{
	START;

	tdata_t tdata = t_get_data();

	t_set_print(PRINT_DST_NONE());
	t_set_wprint(PRINT_DST_WNONE());

	t_expect_g(1, __FILE__, __func__, __LINE__, "", 2, "", 0, "==");
	t_expect_g(1, __FILE__, __func__, __LINE__, "", 4, "", 3, "==");
	t_expect_g(1, __FILE__, __func__, __LINE__, "", 8, "", 7, "==");
	t_expect_g(1, __FILE__, __func__, __LINE__, "", 7, "", 7, "==");

	t_set_data(tdata);

	END;
}

TEST(t_t_set_print)
{
	START;

	tdata_t tdata = t_get_data();

	t_set_print(PRINT_DST_NONE());
	t_set_wprint(PRINT_DST_WNONE());

	EXPECT_STR("a", "b");
	EXPECT_WSTR(L"a", L"b");

	t_set_data(tdata);
	_passed = 1;

	END;
}

TEST(t_t_send)
{
	START;

	tdata_t tdata = t_get_data();

	t_set_print(PRINT_DST_NONE());
	t_set_wprint(PRINT_DST_WNONE());

	t_send(0, 0);
	t_send(0, 1);

	t_set_data(tdata);

	END;
}

TEST(t_t_finish)
{
	START;

	tdata_t tdata = t_get_data();
	tdata_t tmp   = tdata;

	tmp.print  = PRINT_DST_NONE();
	tmp.wprint = PRINT_DST_WNONE();

	tmp.failed = 1;
	tmp.buf	   = malloc(tmp.buf_size);
	t_set_data(tmp);
	t_finish();

	tmp.failed = 0;
	tmp.buf	   = malloc(tmp.buf_size);
	t_set_data(tmp);
	t_finish();

	t_set_data(tdata);

	END;
}

TEST(tests)
{
	int _spassed = 0;
	int _sfailed = 0;
	t_sstart("test_test");

	RUN(success_test);
	RUN(fail_test);
	RUN(parent_test);

	RUN(t_t_run);
	RUN(t_t_end);
	RUN(t_t_set_priv);
	RUN(t_t_setup_teardown);
	RUN(t_t_expect);
	RUN(t_t_set_print);
	RUN(t_t_send);
	RUN(t_t_finish);

	SEND;
}

int test_test()
{
	test_cplatform();
	tests();

	return 0;
}
