#include "test_test.h"

#include "type.h"

#include "test.h"

#include <stdio.h>
#include <string.h>

typedef struct tdata_s {
	void *priv;
	setup_fn setup;
	setup_fn teardown;
	print_fn print;
	wprint_fn wprint;
	int width;
	long long passed;
	long long failed;
	int depth;
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
	EXPECT_EQM(0b00000011, 0b00001010, 0b00000010);
	EXPECT_NEM(0b00000011, 0b00001010, 0b00001011);
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

	EXPECT_STR("", NULL);
	EXPECT_STR("a", "b");
	EXPECT_STRN("ab", NULL, 1);
	EXPECT_STRN("ab", "bc", 1);
	EXPECT_WSTR(L"a", NULL);
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

	EXPECT_FMT("", 1, "bbbbbbbbbbbbbbbbbbbbbbb", &u);
	EXPECT_FMT("aaaaaaaaaaaaaaaaaaaaaaa", 1, "", &u);
	EXPECT_FMT("aaaaaaaaaaaaaaaaaaaaaaa", 1, "bbbbbbbbbbbbbbbbbbbbbbb", &u);

	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "-----------------------------------EXPECT_STR-----------------------------------");

	EXPECT_STR("", "bbbbbbbbbbbbbbbbbbbbbbb");
	EXPECT_STR("aaaaaaaaaaaaaaaaaaaaaaa", "");
	EXPECT_STR("aaaaaaaaaaaaaaaaaaaaaaa", "bbbbbbbbbbbbbbbbbbbbbbb");

	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "-----------------------------------EXPECT_STRN-----------------------------------");

	EXPECT_STRN("", "bbbbbbbbbbbbbbbbbbbbbbbc", 23);
	EXPECT_STRN("aaaaaaaaaaaaaaaaaaaaaaac", "", 23);
	EXPECT_STRN("aaaaaaaaaaaaaaaaaaaaaaac", "bbbbbbbbbbbbbbbbbbbbbbbc", 23);

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
	EXPECT_FAIL("%s", "-----------------------------------EXPECT_FMT-----------------------------------");

	EXPECT_FMT("aaaaaaaaaaaaaaaa", 1, "a", &u);
	EXPECT_FMT("a", 1, "aaaaaaaaaaaaaaaa", &u);
	EXPECT_FMT("aaaaaaaaaaaaaaaaa", 1, "a", &u);
	EXPECT_FMT("a", 1, "aaaaaaaaaaaaaaaaa", &u);
	EXPECT_FMT("aaaaaaaaaaaaaaaaaa", 1, "a", &u);
	EXPECT_FMT("a", 1, "aaaaaaaaaaaaaaaaaa", &u);
	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "-----------------------------------EXPECT_STR-----------------------------------");

	EXPECT_STR("aaaaaaaaaaaaaaaa", "a");
	EXPECT_STR("a", "aaaaaaaaaaaaaaaa");
	EXPECT_STR("aaaaaaaaaaaaaaaaa", "a");
	EXPECT_STR("a", "aaaaaaaaaaaaaaaaa");
	EXPECT_STR("aaaaaaaaaaaaaaaaaa", "a");
	EXPECT_STR("a", "aaaaaaaaaaaaaaaaaa");

	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "-----------------------------------EXPECT_STRN----------------------------------");

	EXPECT_STRN("aaaaaaaaaaaaaaaa", "a", 77);
	EXPECT_STRN("a", "aaaaaaaaaaaaaaaa", 77);
	EXPECT_STRN("aaaaaaaaaaaaaaaaa", "a", 83);
	EXPECT_STRN("a", "aaaaaaaaaaaaaaaaa", 83);
	EXPECT_STRN("aaaaaaaaaaaaaaaaaa", "a", 83);
	EXPECT_STRN("a", "aaaaaaaaaaaaaaaaaa", 83);

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

	EXPECT_EQ((u8)0b0000000000000000000000000001, (u8)0);
	EXPECT_EQ((u8)0, (u8)0b0000000000000000000000000001);
	EXPECT_EQ((u8)0b0000000000000000000000000000001, (u8)0);
	EXPECT_EQ((u8)0, (u8)0b0000000000000000000000000000001);
	EXPECT_EQ((u8)0b000000000000000000000000000000000001, (u8)0);
	EXPECT_EQ((u8)0, (u8)0b000000000000000000000000000000000001);
	EXPECT_EQ((u8)0b000000000000000000000000000000000000000000001, (u8)0);
	EXPECT_EQ((u8)0, (u8)0b000000000000000000000000000000000000000000001);
	EXPECT_EQ((u8)0b0000000000000000000000000000000000000000000000001, (u8)0);
	EXPECT_EQ((u8)0, (u8)0b0000000000000000000000000000000000000000000000001);
	EXPECT_EQ((u8)0b0000000000000000000000000000000000000000000000000001, (u8)0);
	EXPECT_EQ((u8)0, (u8)0b0000000000000000000000000000000000000000000000000001);
	EXPECT_EQ((u8)0b00000000000000000000000000000000000000000000000000000001, (u8)0);
	EXPECT_EQ((u8)0, (u8)0b00000000000000000000000000000000000000000000000000000001);

	EXPECT_FAIL("%s", "---------------------------------------16---------------------------------------");

	EXPECT_EQ((u16)0b00000000000000000000000001, (u16)0);
	EXPECT_EQ((u16)0, (u16)0b00000000000000000000000001);
	EXPECT_EQ((u16)0b000000000000000000000000000000001, (u16)0);
	EXPECT_EQ((u16)0, (u16)0b000000000000000000000000000000001);
	EXPECT_EQ((u16)0b000000000000000000000000000000000001, (u16)0);
	EXPECT_EQ((u16)0, (u16)0b000000000000000000000000000000000001);
	EXPECT_EQ((u16)0b000000000000000000000000000000000000000000000000001, (u16)0);
	EXPECT_EQ((u16)0, (u16)0b000000000000000000000000000000000000000000000000001);
	EXPECT_EQ((u16)0b0000000000000000000000000000000000000000000000000000001, (u16)0);
	EXPECT_EQ((u16)0, (u16)0b0000000000000000000000000000000000000000000000000000001);
	EXPECT_EQ((u16)0b0000000000000000000000000000000000000000000000000000000001, (u16)0);
	EXPECT_EQ((u16)0, (u16)0b0000000000000000000000000000000000000000000000000000000001);
	EXPECT_EQ((u16)0b00000000000000000000000000000000000000000000000000000000000001, (u16)0);
	EXPECT_EQ((u16)0, (u16)0b00000000000000000000000000000000000000000000000000000000000001);

	EXPECT_FAIL("%s", "---------------------------------------32---------------------------------------");

	EXPECT_EQ((u32)0b00000000000000000000000001, (u32)0);
	EXPECT_EQ((u32)0, (u32)0b00000000000000000000000001);
	EXPECT_EQ((u32)0b00000000000000000000000000001, (u32)0);
	EXPECT_EQ((u32)0, (u32)0b00000000000000000000000000001);
	EXPECT_EQ((u32)0b0000000000000000000000000000000001, (u32)0);
	EXPECT_EQ((u32)0, (u32)0b0000000000000000000000000000000001);
	EXPECT_EQ((u32)0b0000000000000000000000000000000000000000001, (u32)0);
	EXPECT_EQ((u32)0, (u32)0b0000000000000000000000000000000000000000001);
	EXPECT_EQ((u32)0b00000000000000000000000000000000000000000000001, (u32)0);
	EXPECT_EQ((u32)0, (u32)0b00000000000000000000000000000000000000000000001);
	EXPECT_EQ((u32)0b00000000000000000000000000000000000000000000000001, (u32)0);
	EXPECT_EQ((u32)0, (u32)0b00000000000000000000000000000000000000000000000001);
	EXPECT_EQ((u32)0b000000000000000000000000000000000000000000000000000001, (u32)0);
	EXPECT_EQ((u32)0, (u32)0b000000000000000000000000000000000000000000000000000001);

	EXPECT_FAIL("%s", "---------------------------------------64---------------------------------------");

	EXPECT_EQ((u64)0b00000000000000001, (u64)0);
	EXPECT_EQ((u64)0, (u64)0b00000000000000001);
	EXPECT_EQ((u64)0b000000000000000000001, (u64)0);
	EXPECT_EQ((u64)0, (u64)0b000000000000000000001);
	EXPECT_EQ((u64)0b00000000000000000000001, (u64)0);
	EXPECT_EQ((u64)0, (u64)0b00000000000000000000001);
	EXPECT_EQ((u64)0b000000000000000000000000001, (u64)0);
	EXPECT_EQ((u64)0, (u64)0b000000000000000000000000001);
	EXPECT_EQ((u64)0b0000000000000000000000000000001, (u64)0);
	EXPECT_EQ((u64)0, (u64)0b0000000000000000000000000000001);
	EXPECT_EQ((u64)0b0000000000000000000000000000000001, (u64)0);
	EXPECT_EQ((u64)0, (u64)0b0000000000000000000000000000000001);
	EXPECT_EQ((u64)0b00000000000000000000000000000000000001, (u64)0);
	EXPECT_EQ((u64)0, (u64)0b00000000000000000000000000000000000001);

	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "-----------------------------------EXPECT_EQM-----------------------------------");

	EXPECT_FAIL("%s", "---------------------------------------8----------------------------------------");

	EXPECT_EQM((u8)0b0000000000000000000000000001, (u8)0, 1);
	EXPECT_EQM((u8)0, (u8)0b0000000000000000000000000001, 1);
	EXPECT_EQM((u8)0b0000000000000000000000000000001, (u8)0, 1);
	EXPECT_EQM((u8)0, (u8)0b0000000000000000000000000000001, 1);
	EXPECT_EQM((u8)0b000000000000000000000000000000000001, (u8)0, 1);
	EXPECT_EQM((u8)0, (u8)0b000000000000000000000000000000000001, 1);
	EXPECT_EQM((u8)0b0000000000000000000000000000000000000001, (u8)0, 1);
	EXPECT_EQM((u8)0, (u8)0b0000000000000000000000000000000000000001, 1);
	EXPECT_EQM((u8)0b000000000000000000000000000000000000000000001, (u8)0, 1);
	EXPECT_EQM((u8)0, (u8)0b000000000000000000000000000000000000000000001, 1);
	EXPECT_EQM((u8)0b0000000000000000000000000000000000000000000000001, (u8)0, 1);
	EXPECT_EQM((u8)0, (u8)0b0000000000000000000000000000000000000000000000001, 1);
	EXPECT_EQM((u8)0b0000000000000000000000000000000000000000000000000001, (u8)0, 1);
	EXPECT_EQM((u8)0, (u8)0b0000000000000000000000000000000000000000000000000001, 1);
	EXPECT_EQM((u8)0b00000000000000000000000000000000000000000000000000000001, (u8)0, 1);
	EXPECT_EQM((u8)0, (u8)0b00000000000000000000000000000000000000000000000000000001, 1);

	EXPECT_FAIL("%s", "---------------------------------------16---------------------------------------");

	EXPECT_EQM((u16)0b00000000000000000000000001, (u16)0, 1);
	EXPECT_EQM((u16)0, (u16)0b00000000000000000000000001, 1);
	EXPECT_EQM((u16)0b000000000000000000000000000000001, (u16)0, 1);
	EXPECT_EQM((u16)0, (u16)0b000000000000000000000000000000001, 1);
	EXPECT_EQM((u16)0b000000000000000000000000000000000001, (u16)0, 1);
	EXPECT_EQM((u16)0, (u16)0b000000000000000000000000000000000001, 1);
	EXPECT_EQM((u16)0b0000000000000000000000000000000000000000000001, (u16)0, 1);
	EXPECT_EQM((u16)0, (u16)0b0000000000000000000000000000000000000000000001, 1);
	EXPECT_EQM((u16)0b000000000000000000000000000000000000000000000000001, (u16)0, 1);
	EXPECT_EQM((u16)0, (u16)0b000000000000000000000000000000000000000000000000001, 1);
	EXPECT_EQM((u16)0b0000000000000000000000000000000000000000000000000000001, (u16)0, 1);
	EXPECT_EQM((u16)0, (u16)0b0000000000000000000000000000000000000000000000000000001, 1);
	EXPECT_EQM((u16)0b0000000000000000000000000000000000000000000000000000000001, (u16)0, 1);
	EXPECT_EQM((u16)0, (u16)0b0000000000000000000000000000000000000000000000000000000001, 1);
	EXPECT_EQM((u16)0b00000000000000000000000000000000000000000000000000000000000001, (u16)0, 1);
	EXPECT_EQM((u16)0, (u16)0b00000000000000000000000000000000000000000000000000000000000001, 1);

	EXPECT_FAIL("%s", "---------------------------------------32---------------------------------------");

	EXPECT_EQM((u32)0b00000000000000000000000001, (u32)0, 1);
	EXPECT_EQM((u32)0, (u32)0b00000000000000000000000001, 1);
	EXPECT_EQM((u32)0b00000000000000000000000000001, (u32)0, 1);
	EXPECT_EQM((u32)0, (u32)0b00000000000000000000000000001, 1);
	EXPECT_EQM((u32)0b0000000000000000000000000000000001, (u32)0, 1);
	EXPECT_EQM((u32)0, (u32)0b0000000000000000000000000000000001, 1);
	EXPECT_EQM((u32)0b00000000000000000000000000000000000001, (u32)0, 1);
	EXPECT_EQM((u32)0, (u32)0b00000000000000000000000000000000000001, 1);
	EXPECT_EQM((u32)0b0000000000000000000000000000000000000000001, (u32)0, 1);
	EXPECT_EQM((u32)0, (u32)0b0000000000000000000000000000000000000000001, 1);
	EXPECT_EQM((u32)0b00000000000000000000000000000000000000000000001, (u32)0, 1);
	EXPECT_EQM((u32)0, (u32)0b00000000000000000000000000000000000000000000001, 1);
	EXPECT_EQM((u32)0b00000000000000000000000000000000000000000000000001, (u32)0, 1);
	EXPECT_EQM((u32)0, (u32)0b00000000000000000000000000000000000000000000000001, 1);
	EXPECT_EQM((u32)0b000000000000000000000000000000000000000000000000000001, (u32)0, 1);
	EXPECT_EQM((u32)0, (u32)0b000000000000000000000000000000000000000000000000000001, 1);

	EXPECT_FAIL("%s", "---------------------------------------64---------------------------------------");

	EXPECT_EQM((u64)0b00000000000000001, (u64)0, 1);
	EXPECT_EQM((u64)0, (u64)0b00000000000000001, 1);
	EXPECT_EQM((u64)0b000000000000000000001, (u64)0, 1);
	EXPECT_EQM((u64)0, (u64)0b000000000000000000001, 1);
	EXPECT_EQM((u64)0b0000000000000000000001, (u64)0, 1);
	EXPECT_EQM((u64)0, (u64)0b0000000000000000000001, 1);
	EXPECT_EQM((u64)0b000000000000000000000000001, (u64)0, 1);
	EXPECT_EQM((u64)0, (u64)0b000000000000000000000000001, 1);
	EXPECT_EQM((u64)0b0000000000000000000000000000001, (u64)0, 1);
	EXPECT_EQM((u64)0, (u64)0b0000000000000000000000000000001, 1);

	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "-------------------------------------EXPECT-------------------------------------");

	EXPECT(!strcmp("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "a"));
	EXPECT(!strcmp("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "a"));
	EXPECT(!strcmp("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "a"));
	EXPECT(!strcmp("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "a"));

	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "-----------------------------------EXPECT_FMT-----------------------------------");

	EXPECT_FMT("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 1, "a", &u);
	EXPECT_FMT("a", 1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", &u);
	EXPECT_FMT("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 1, "a", &u);
	EXPECT_FMT("a", 1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", &u);
	EXPECT_FMT("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 1, "a", &u);
	EXPECT_FMT("a", 1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", &u);
	EXPECT_FMT("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 1, "a", &u);
	EXPECT_FMT("a", 1, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", &u);

	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "-----------------------------------EXPECT_STR-----------------------------------");

	EXPECT_STR("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "a");
	EXPECT_STR("a", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	EXPECT_STR("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "a");
	EXPECT_STR("a", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	EXPECT_STR("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "a");
	EXPECT_STR("a", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	EXPECT_STR("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "a");
	EXPECT_STR("a", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "-----------------------------------EXPECT_STRN----------------------------------");

	EXPECT_STRN("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "a", 76);
	EXPECT_STRN("a", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 76);
	EXPECT_STRN("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "a", 80);
	EXPECT_STRN("a", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 80);
	EXPECT_STRN("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "a", 83);
	EXPECT_STRN("a", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 83);
	EXPECT_STRN("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "a", 89);
	EXPECT_STRN("a", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 89);

	EXPECT_FAIL("%s", "");
	EXPECT_FAIL("%s", "-----------------------------------EXPECT_FAIL----------------------------------");

	EXPECT_FAIL("%s", "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor in");
	EXPECT_FAIL("%s", "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incidi");
	EXPECT_FAIL("%s", "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididun");
	EXPECT_FAIL("%s", "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut l");
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
	return 0;
}

static int teardown(void *priv)
{
	return 0;
}

TEST(t_t_setup_teardown)
{
	START;

	tdata_t tdata = t_get_data();

	t_set_print(NULL);
	t_set_wprint(NULL);

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

	t_set_print(NULL);
	t_set_wprint(NULL);

	t_expect_g(1, __func__, __LINE__, "", 2, "", 0, "==");
	t_expect_g(1, __func__, __LINE__, "", 4, "", 3, "==");
	t_expect_g(1, __func__, __LINE__, "", 8, "", 7, "==");
	t_expect_g(1, __func__, __LINE__, "", 7, "", 7, "==");

	t_set_data(tdata);

	END;
}

int t_print_cb(const char *fmt, va_list args)
{
	return -1;
}

int t_wprint_cb(const wchar_t *fmt, va_list args)
{
	return -1;
}

TEST(t_t_set_print)
{
	START;
	tdata_t tdata = t_get_data();

	t_set_print(t_print_cb);
	t_set_wprint(t_wprint_cb);

	EXPECT_STR("a", "b");
	EXPECT_WSTR(L"a", L"b");

	t_set_data(tdata);

	END;
}

TEST(t_t_send)
{
	START;

	tdata_t tdata = t_get_data();

	t_set_print(NULL);
	t_set_wprint(NULL);

	t_send(0, 0);
	t_send(0, 1);

	t_set_data(tdata);

	END;
}

TEST(t_t_finish)
{
	START;

	tdata_t tdata = t_get_data();

	t_set_print(NULL);
	t_set_wprint(NULL);

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
	tests();

	tdata_t tdata = t_get_data();
	tdata.passed += tdata.failed;
	tdata.failed = 0;
	t_set_data(tdata);

	return 0;
}
