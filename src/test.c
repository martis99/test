#include "test.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	#define T_WIN
	#if defined(_WIN64)
		#define T_WIN64
	#else
		#define T_WIN32
	#endif
#elif __linux__
	#define T_LINUX
#else
	#error "Platform not supported"
#endif

#include <assert.h>
#include <fcntl.h>
#if defined(T_WIN)
	#include <io.h>
#endif
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(T_WIN)
	#define BYTE_TO_BIN_PATTERN L"%c%c%c%c%c%c%c%c"
#else
	#define BYTE_TO_BIN_PATTERN "%c%c%c%c%c%c%c%c"
#endif

// clang-format off
#define BYTE_TO_BIN(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')
// clang-format on

struct tdata {
	int width;
	long long passed;
	long long failed;
	int depth;
};

static struct tdata s_data;

static void *s_priv;
static setup_fn s_setup;
static setup_fn s_teardown;

void t_set_priv(void *priv)
{
	s_priv = priv;
}

void t_setup(setup_fn setup)
{
	s_setup = setup;
}

void t_teardown(teardown_fn teardown)
{
	s_teardown = teardown;
}

void *t_get_priv()
{
	return s_priv;
}

static inline void t_sprint()
{
#if defined(T_WIN)
	int r = _setmode(_fileno(stdout), _O_U16TEXT);
#endif
}

static inline void t_eprint()
{
#if defined(T_WIN)
	int r = _setmode(_fileno(stdout), _O_TEXT);
#endif
}

static inline void pur()
{
#if defined(T_WIN)
	wprintf_s(L"└─");
#else
	printf("└─");
#endif
}

static inline void pv()
{
#if defined(T_WIN)
	wprintf_s(L"│ ");
#else
	printf("│ ");
#endif
}

static inline void pvr()
{
#if defined(T_WIN)
	wprintf_s(L"├─");
#else
	printf("├─");
#endif
}

void t_init(int width)
{
	t_sprint();

	s_data.width  = width;
	s_data.passed = 0;
	s_data.failed = 0;
	s_data.depth  = -1;
}

void t_free()
{
	t_eprint();
}

void t_start()
{
	t_eprint();
	if (s_setup) {
		s_setup(s_priv);
	}
}

int t_end(int passed, const char *func)
{
	if (s_teardown) {
		s_teardown(s_priv);
	}
	t_sprint();
	if (passed) {
		for (int i = 0; i < s_data.depth; i++) {
			pv();
		}
		pvr();
#if defined(T_WIN)
		wprintf_s(L"\033[0;32m%-75hs %*hsOK\033[0m\n", func, (s_data.width - s_data.depth) * 2, " ");
#else
		printf("\033[0;32m%-75s %*sOK\033[0m\n", func, (s_data.width - s_data.depth) * 2, " ");
#endif

		s_data.passed++;
		return 0;
	}

	s_data.failed++;
	return 1;
}

void t_sstart(const char *func)
{
	for (int i = 0; i < s_data.depth; i++) {
		pv();
	}
	if (s_data.depth >= 0) {
		pvr();
	}
#if defined(T_WIN)
	wprintf_s(L"%hs\n", func);
#else
	printf("%s\n", func);
#endif
	s_data.depth++;
}

int t_send(int passed, int failed)
{
	for (int i = 0; i < s_data.depth; i++) {
		pv();
	}
	pur();
	if (failed == 0) {
#if defined(T_WIN)
		wprintf_s(L"\033[0;32mPASSED %d %hs\033[0m\n", passed, passed == 1 ? "TEST" : "TESTS");
#else
		printf("\033[0;32mPASSED %d %s\033[0m\n", passed, passed == 1 ? "TEST" : "TESTS");
#endif
	} else {
#if defined(T_WIN)
		wprintf_s(L"\033[0;31mFAILED %d/%d %hs\033[0m\n", failed, failed + passed, failed == 1 ? "TEST" : "TESTS");
#else
		printf("\033[0;31mFAILED %d/%d %s\033[0m\n", failed, failed + passed, failed == 1 ? "TEST" : "TESTS");
#endif
	}
	s_data.depth--;
	return failed > 0;
}

static void print_header(int passed, const char *func)
{
	t_sprint();
	if (passed) {
		for (int i = 0; i < s_data.depth; i++) {
			pv();
		}
		pvr();
#if defined(T_WIN)
		wprintf_s(L"\033[0;31m%-75hs %*hsFAILED\033[0m\n", func, (s_data.width - s_data.depth) * 2, " ");
#else
		printf("\033[0;31m%-75s %*sFAILED\033[0m\n", func, (s_data.width - s_data.depth) * 2, " ");
#endif
	}
	for (int i = 0; i < s_data.depth + 1; i++) {
		pv();
	}
	pvr();
}

static char get_char(size_t size, va_list args)
{
	switch (size) {
	case 0:
	case 1: return (char)va_arg(args, int);
	case 2: return (char)va_arg(args, int);
	case 4: return (char)va_arg(args, int);
	case 8: return (char)va_arg(args, long long);
	}

#if defined(T_WIN)
	wprintf_s(L"Unsupported type of size: %zd\n", size);
#else
	printf("Unsupported type of size: %zd\n", size);
#endif
	assert(0);
	return 0;
}

static short get_short(size_t size, va_list args)
{
	switch (size) {
	case 0:
	case 1: return (short)va_arg(args, int);
	case 2: return (short)va_arg(args, int);
	case 4: return (short)va_arg(args, int);
	case 8: return (short)va_arg(args, long long);
	}

#if defined(T_WIN)
	wprintf_s(L"Unsupported type of size: %zd\n", size);
#else
	printf("Unsupported type of size: %zd\n", size);
#endif
	assert(0);
	return 0;
}

static int get_int(size_t size, va_list args)
{
	switch (size) {
	case 0:
	case 1: return (int)va_arg(args, int);
	case 2: return (int)va_arg(args, int);
	case 4: return (int)va_arg(args, int);
	case 8: return (int)va_arg(args, long long);
	}

#if defined(T_WIN)
	wprintf_s(L"Unsupported type of size: %zd\n", size);
#else
	printf("Unsupported type of size: %zd\n", size);
#endif
	assert(0);
	return 0;
}

static long long get_long(size_t size, va_list args)
{
	switch (size) {
	case 0:
	case 1: return (long long)va_arg(args, int);
	case 2: return (long long)va_arg(args, int);
	case 4: return (long long)va_arg(args, int);
	case 8: return (long long)va_arg(args, long long);
	}

#if defined(T_WIN)
	wprintf_s(L"Unsupported type: %zd\n", size);
#else
	printf("Unsupported type: %zd\n", size);
#endif
	assert(0);
	return 0;
}

static void print_values(int passed, const char *func, const char *act, size_t act_size, const char *exp, size_t exp_size, const char *cond, va_list args)
{
	print_header(passed, func);

#if defined(T_WIN)
	wprintf_s(L"\033[0;31m%20hs %hs %-20hs ", act, cond, exp);
#else
	printf("\033[0;31m%20s %s %-20s ", act, cond, exp);
#endif

	switch (act_size) {
	case 0: {
		char a = va_arg(args, int);
		char b = get_char(exp_size, args);
#if defined(T_WIN)
		wprintf_s(L"       %c %hs %c       ", a ? '1' : '0', cond, b ? '1' : '0');
#else
		printf("       %c %s %c       ", a ? '1' : '0', cond, b ? '1' : '0');
#endif
		break;
	}
	case 1: {
		unsigned char a = va_arg(args, int);
		unsigned char b = va_arg(args, int);
#if defined(T_WIN)
		wprintf_s(BYTE_TO_BIN_PATTERN L" %hs " BYTE_TO_BIN_PATTERN, BYTE_TO_BIN(a), cond, BYTE_TO_BIN(b));
#else
		printf(BYTE_TO_BIN_PATTERN " %s " BYTE_TO_BIN_PATTERN, BYTE_TO_BIN(a), cond, BYTE_TO_BIN(b));
#endif
		break;
	}
	case 2: {
		short a = (short)va_arg(args, int);
		short b = get_short(exp_size, args);
#if defined(T_WIN)
		wprintf_s(L"%08X %hs %08X", a, cond, b);
#else
		printf("%08X %s %08X", a, cond, b);
#endif
		break;
	}
	case 4: {
		int a = va_arg(args, int);
		int b = get_int(exp_size, args);
#if defined(T_WIN)
		wprintf_s(L"%08X %hs %08X", a, cond, b);
#else
		printf("%08X %s %08X", a, cond, b);
#endif
		break;
	}
	case 8: {
		long long a = va_arg(args, long long);
		long long b = get_long(exp_size, args);
#if defined(T_WIN)
		wprintf_s(L"%p %hs %p", (void *)a, cond, (void *)b);
#else
		printf("%p %s %p", (void *)a, cond, (void *)b);
#endif
		break;
	}
	default:
#if defined(T_WIN)
		wprintf_s(L"Unsupported type: %zd\n", act_size);
#else
		printf("Unsupported type: %zd\n", act_size);
#endif
		assert(0);
	}
}

void t_expect_ch(int passed, const char *func, int line, const char *check)
{
	print_header(passed, func);

	int len = (int)strlen(check);
	int l, r, m;

	if (len > 44) {
		l = 0;
		r = 1;
		m = -74;
	} else {
		l = (44 - len) / 2;
		r = l + 31;
		m = 0;
	}

#if defined(T_WIN)
	wprintf_s(L"\033[0;31m%*hs%*hs %*hsL%d\033[0m\n", l, "", m, check, r + (s_data.width - (s_data.depth + 1)) * 2, " ", line);
#else
	printf("\033[0;31m%*s%*s %*sL%d\033[0m\n", l, "", m, check, r + (s_data.width - (s_data.depth + 1)) * 2, " ", line);
#endif

	t_eprint();
}

void t_expect_eq(int passed, const char *func, int line, const char *act, size_t act_size, const char *exp, size_t exp_size, ...)
{
	va_list args;
	va_start(args, exp_size);
	print_values(passed, func, act, act_size, exp, exp_size, "==", args);
	va_end(args);

#if defined(T_WIN)
	wprintf_s(L"           %*hsL%d\033[0m\n", (s_data.width - (s_data.depth + 1)) * 2, " ", line);
#else
	printf("           %*sL%d\033[0m\n", (s_data.width - (s_data.depth + 1)) * 2, " ", line);
#endif
	t_eprint();
}

void t_expect_eqm(int passed, const char *func, int line, const char *act, size_t act_size, const char *exp, size_t exp_size, const char *mask_str, unsigned char mask,
		  ...)
{
	va_list args;
	va_start(args, mask);
	print_values(passed, func, act, act_size, exp, exp_size, "==", args);
	va_end(args);

#if defined(T_WIN)
	wprintf_s(L"  " BYTE_TO_BIN_PATTERN L" %*hsL%d\033[0m\n", BYTE_TO_BIN(mask), (s_data.width - (s_data.depth + 1)) * 2, " ", line);
#else
	printf("  " BYTE_TO_BIN_PATTERN " %*sL%d\033[0m\n", BYTE_TO_BIN(mask), (s_data.width - (s_data.depth + 1)) * 2, " ", line);
#endif
	t_eprint();
}

void t_expect_eqb(int passed, const char *func, int line, const char *act, const char *exp, ...)
{
	va_list args;
	va_start(args, exp);
	print_values(passed, func, act, 0, exp, 0, "==", args);
	va_end(args);

#if defined(T_WIN)
	wprintf_s(L"           %*hsL%d\033[0m\n", (s_data.width - (s_data.depth + 1)) * 2, " ", line);
#else
	printf("           %*sL%d\033[0m\n", (s_data.width - (s_data.depth + 1)) * 2, " ", line);
#endif
	t_eprint();
}

void t_expect_ne(int passed, const char *func, int line, const char *act, size_t act_size, const char *exp, size_t exp_size, ...)
{
	va_list args;
	va_start(args, exp_size);
	print_values(passed, func, act, act_size, exp, exp_size, "!=", args);
	va_end(args);

#if defined(T_WIN)
	wprintf_s(L"           %*hsL%d\033[0m\n", (s_data.width - (s_data.depth + 1)) * 2, " ", line);
#else
	printf("           %*sL%d\033[0m\n", (s_data.width - (s_data.depth + 1)) * 2, " ", line);
#endif
	t_eprint();
}

void t_expect_nem(int passed, const char *func, int line, const char *act, size_t act_size, const char *exp, size_t exp_size, const char *mask_str, unsigned char mask,
		  ...)
{
	va_list args;
	va_start(args, mask);
	print_values(passed, func, act, act_size, exp, exp_size, "!=", args);
	va_end(args);

#if defined(T_WIN)
	wprintf_s(L"  " BYTE_TO_BIN_PATTERN L" %*hsL%d\033[0m\n", BYTE_TO_BIN(mask), (s_data.width - (s_data.depth + 1)) * 2, " ", line);
#else
	printf("  " BYTE_TO_BIN_PATTERN " %*sL%d\033[0m\n", BYTE_TO_BIN(mask), (s_data.width - (s_data.depth + 1)) * 2, " ", line);
#endif
	t_eprint();
}

void t_expect_neb(int passed, const char *func, int line, const char *act, const char *exp, ...)
{
	va_list args;
	va_start(args, exp);
	print_values(passed, func, act, 0, exp, 0, "!=", args);
	va_end(args);

#if defined(T_WIN)
	wprintf_s(L"           %*hsL%d\033[0m\n", (s_data.width - (s_data.depth + 1)) * 2, " ", line);
#else
	printf("           %*sL%d\033[0m\n", (s_data.width - (s_data.depth + 1)) * 2, " ", line);
#endif
	t_eprint();
}

void t_results()
{
	if (s_data.failed == 0) {
#if defined(T_WIN)
		wprintf_s(L"\033[0;32mPASSED %llu %hs\033[0m\n", s_data.passed, s_data.passed == 1 ? "TEST" : "TESTS");
#else
		printf("\033[0;32mPASSED %llu %s\033[0m\n", s_data.passed, s_data.passed == 1 ? "TEST" : "TESTS");
#endif
	} else {
#if defined(T_WIN)
		wprintf_s(L"\033[0;31mFAILED %llu/%llu %hs\033[0m\n", s_data.failed, s_data.failed + s_data.passed, s_data.failed == 1 ? "TEST" : "TESTS");
#else
		printf("\033[0;31mFAILED %llu/%llu %s\033[0m\n", s_data.failed, s_data.failed + s_data.passed, s_data.failed == 1 ? "TEST" : "TESTS");
#endif
	}
}
