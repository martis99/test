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
#include <errno.h>
#include <fcntl.h>
#if defined(T_WIN)
	#include <io.h>
#endif
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#if defined(T_WIN)
	#define vprintf vprintf_s
	#define vsscanf vsscanf_s
#endif

#define BYTE_TO_BIN_PATTERN "%c%c%c%c%c%c%c%c"

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

static FILE *t_freopen(const char *path, const char *mode, FILE *file)
{
	FILE *new_file = NULL;

#if defined(T_WIN)
	freopen_s(&file, path, mode, file);
#else
	file = freopen(path, mode, file);
#endif
	return file;
}

static int t_printv(const char *fmt, va_list args)
{
	va_list copy;
	va_copy(copy, args);
	int ret = vprintf(fmt, copy);
	if (ret < 0 && errno == 0) {
		t_freopen(NULL, "w", stdout);
		ret = vprintf(fmt, copy);
	}
	va_end(copy);
	return ret;
}

static int t_printf(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int ret = t_printv(fmt, args);
	va_end(args);
	return ret;
}

static int t_wprintv(const wchar_t *fmt, va_list args)
{
	va_list copy;
	va_copy(copy, args);
	errno	= 0;
	int ret = vwprintf(fmt, copy);
	if (ret < 0 && errno == 0) {
		t_freopen(NULL, "w", stdout);
		ret = vwprintf(fmt, copy);
	}
	va_end(copy);
	return ret;
}

static int t_wprintf(const wchar_t *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int ret = t_wprintv(fmt, args);
	va_end(args);
	return ret;
}

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
	fflush(stdout);
	int r = _setmode(_fileno(stdout), _O_WTEXT);
#endif
}

static inline void t_eprint()
{
#if defined(T_WIN)
	fflush(stdout);
	int r = _setmode(_fileno(stdout), _O_TEXT);
#endif
}

static inline int pur()
{
#if defined(T_WIN)
	return t_wprintf(L"└─");
#else
	return t_printf("└─");
#endif
}

static inline int pv()
{
#if defined(T_WIN)
	return t_wprintf(L"│ ");
#else
	return t_printf("│ ");
#endif
}

static inline int pvr()
{
#if defined(T_WIN)
	return t_wprintf(L"├─");
#else
	return t_printf("├─");
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

int t_finish()
{
	if (s_data.failed == 0) {
		t_printf("\033[0;32mPASSED %llu %s\033[0m\n", s_data.passed, s_data.passed == 1 ? "TEST" : "TESTS");
	} else {
		t_printf("\033[0;31mFAILED %llu/%llu %s\033[0m\n", s_data.failed, s_data.failed + s_data.passed, s_data.failed == 1 ? "TEST" : "TESTS");
	}
	return (int)s_data.failed;
}

void t_start()
{
	if (s_setup) {
		s_setup(s_priv);
	}
}

int t_end(int passed, const char *func)
{
	if (s_teardown) {
		s_teardown(s_priv);
	}
	if (passed) {
		t_sprint();
		int len = 0;
		for (int i = 0; i < s_data.depth; i++) {
			len += pv();
		}
		len += pvr();
		t_eprint();

		t_printf("\033[0;32m%-*s          OK\033[0m\n", s_data.width - len, func);

		s_data.passed++;
		return 0;
	}

	s_data.failed++;
	return 1;
}

void t_sstart(const char *func)
{
	t_sprint();
	for (int i = 0; i < s_data.depth; i++) {
		pv();
	}
	if (s_data.depth >= 0) {
		pvr();
	}
	t_eprint();

	t_printf("%s\n", func);
	s_data.depth++;
}

int t_send(int passed, int failed)
{
	t_sprint();
	for (int i = 0; i < s_data.depth; i++) {
		pv();
	}
	pur();
	t_eprint();
	if (failed == 0) {
		t_printf("\033[0;32mPASSED %d %s\033[0m\n", passed, passed == 1 ? "TEST" : "TESTS");
	} else {
		t_printf("\033[0;31mFAILED %d/%d %s\033[0m\n", failed, failed + passed, failed == 1 ? "TEST" : "TESTS");
	}
	s_data.depth--;
	return failed > 0;
}

int t_scan(const char *str, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	const int ret = vsscanf(str, fmt, args);
	va_end(args);
	return ret;
}

int t_strcmp(const char *act, const char *exp)
{
	if (act == NULL && exp == NULL) {
		return 0;
	}
	if (act == NULL || exp == NULL) {
		return 1;
	}
	return strcmp(act, exp);
}

int t_strncmp(const char *act, const char *exp, size_t len)
{
	if (act == NULL && exp == NULL) {
		return 0;
	}
	if (act == NULL || exp == NULL) {
		return 1;
	}
	return strncmp(act, exp, len);
}

int t_wstrcmp(const wchar_t *act, const wchar_t *exp)
{
	if (act == NULL && exp == NULL) {
		return 0;
	}
	if (act == NULL || exp == NULL) {
		return 1;
	}
	return wcscmp(act, exp);
}

int t_wstrncmp(const wchar_t *act, const wchar_t *exp, size_t len)
{
	if (act == NULL && exp == NULL) {
		return 0;
	}
	if (act == NULL || exp == NULL) {
		return 1;
	}
	return wcsncmp(act, exp, len);
}

static int print_header(int passed, const char *func)
{
	t_sprint();
	if (passed) {
		int len = 0;
		for (int i = 0; i < s_data.depth; i++) {
			len += pv();
		}
		len += pvr();
#if defined(T_WIN)
		t_wprintf(L"\033[0;31m%-*hs          FAILED\033[0m\n", s_data.width - len, func);
#else
		t_printf("\033[0;31m%-*s          FAILED\033[0m\n", s_data.width - len, func);
#endif
	}
	int len = 0;
	for (int i = 0; i < s_data.depth + 1; i++) {
		len += pv();
	}
	len += pvr();
	t_eprint();
	return len;
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

	t_printf("Unsupported type of size: %zu\n", size);
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

	t_printf("Unsupported type of size: %zu\n", size);
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

	t_printf("Unsupported type of size: %zu\n", size);
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

	t_printf("Unsupported type: %zu\n", size);
	assert(0);
	return 0;
}

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

static void print_values(int passed, const char *func, const char *act, size_t act_size, const char *exp, size_t exp_size, const char *cond, int inc, va_list args)
{
	int exp_width = s_data.width - print_header(passed, func);

	int exp_lcol = (exp_width - 1) / 2;
	int exp_rcol = (exp_width - 1 - 1) / 2 + 1;

	const int exp_llval = MAX((exp_lcol - 4) / 2, 0);
	const int exp_lrval = MAX((exp_lcol - 4 - 1) / 2 + 1, 0);

	const int exp_rlval = MAX((exp_rcol - 4) / 2, 0);
	const int exp_rrval = MAX((exp_rcol - 4 - 1) / 2 + 1, 0);

	exp_width = MAX(exp_width, 0);

	exp_lcol = MAX(exp_lcol, 0);
	exp_rcol = MAX(exp_rcol, 0);

	const int llen = (int)strlen(act);
	const int rlen = (int)strlen(exp);

	int act_llval = MAX(llen, exp_llval);
	int act_lrval = MAX(rlen, exp_lrval);

	const int act_llen = MAX(act_llval + 4 + act_lrval, exp_lcol);

	act_llval = act_llen > exp_lcol ? llen : exp_llval;
	act_lrval = act_llen > exp_lcol ? rlen : exp_lrval;

	int act_lcol = act_llval + 4 + act_lrval;

	const int missing = MAX(exp_lcol - act_lcol, 0);

	act_lcol += missing;

	t_printf("\033[0;31m%*s %s %-*s ", act_llval, act, cond, act_lrval + missing, exp);

	int lover = MAX(act_lcol - exp_lcol, 0);

	int act_width = 0;

	switch (act_size) {
	case 0: {
		const char a = va_arg(args, int);
		const char b = get_char(exp_size, args);

		act_width = MAX(act_lcol + 1 + (1 + 4 + 1), exp_width);

		const int pl = MAX(exp_rlval - 1, 0);
		const int l  = MAX(pl - lover, 0);

		lover -= pl - l;

		const int r = MAX(exp_rrval - 1 - lover, 0);
		t_printf("%*s%c %s %c%-*s", l, "", a ? '1' : '0', cond, b ? '1' : '0', r, "");
		break;
	}
	case 1: {
		const unsigned char a = va_arg(args, int);
		const unsigned char b = va_arg(args, int);

		act_width = MAX(act_lcol + 1 + (8 + 4 + 8), exp_width);

		const int pl = MAX(exp_rlval - 8, 0);
		const int l  = MAX(pl - lover, 0);

		lover -= pl - l;

		const int r = MAX(exp_rrval - 8 - lover, 0);
		t_printf("%*s" BYTE_TO_BIN_PATTERN " %s " BYTE_TO_BIN_PATTERN "%*s", l, "", BYTE_TO_BIN(a), cond, BYTE_TO_BIN(b), r, "");
		break;
	}
	case 2: {
		const short a = (short)va_arg(args, int);
		const short b = get_short(exp_size, args);

		act_width = MAX(act_lcol + 1 + (8 + 4 + 8), exp_width);

		const int pl = MAX(exp_rlval - 8, 0);
		const int l  = MAX(pl - lover, 0);

		lover -= pl - l;

		const int r = MAX(exp_rrval - 8 - lover, 0);
		t_printf("%*s%08X %s %08X%*s", l, "", a, cond, b, r, "");
		break;
	}
	case 4: {
		const int a = va_arg(args, int);
		const int b = get_int(exp_size, args);

		act_width = MAX(act_lcol + 1 + (8 + 4 + 8), exp_width);

		const int pl = MAX(exp_rlval - 8, 0);
		const int l  = MAX(pl - lover, 0);

		lover -= pl - l;

		const int r = MAX(exp_rrval - 8 - lover, 0);
		t_printf("%*s%08X %s %08X%*s", l, "", a, cond, b, r, "");
		break;
	}
	case 8: {
		const long long a = va_arg(args, long long);
		const long long b = get_long(exp_size, args);

		act_width = MAX(act_lcol + 1 + (16 + 4 + 16), exp_width);

		const int pl = MAX(exp_rlval - 16, 0);
		const int l  = MAX(pl - lover, 0);

		lover -= pl - l;

		const int r = MAX(exp_rrval - 16 - lover, 0);
		t_printf("%*s%p %s %p%*s", l, "", (void *)a, cond, (void *)b, r, "");
		break;
	}
	default:
		t_printf("Unsupported type: %zu\n", act_size);
		assert(0);
		break;
	}

	const int over = MAX(act_width - exp_width, 0);
	const int add  = MAX(inc - over, 0);

	t_printf("%*s", add, "");
}

void t_expect_ch(int passed, const char *func, int line, const char *check)
{
	int exp_width = s_data.width - print_header(passed, func);

	const int exp_lcol = MAX((exp_width - 1) / 2, 0);
	const int exp_rcol = MAX((exp_width - 1 - 1) / 2 + 1, 0);

	exp_width = MAX(exp_width, 0);

	const int act_val = (int)strlen(check);

	int act_width = act_val < exp_lcol ? exp_width : MAX(act_val, exp_width);

	const int width = act_val < exp_lcol ? exp_lcol : exp_width;
	const int a	= act_val < exp_lcol ? exp_rcol + 1 : 0;

	const int w = width - act_val;
	const int l = MAX(w / 2, 0);
	const int r = MAX((w - 1) / 2 + 1, 0);

	const int over = MAX(act_width - exp_width, 0);
	const int add  = MAX(9 - over, 0);

	t_printf("\033[0;31m%*s%s%*s L%d\033[0m\n", l, "", check, r + a + add, "", line);
}

void t_expect_g(int passed, const char *func, int line, const char *act, size_t act_size, const char *exp, size_t exp_size, const char *cond, ...)
{
	va_list args;
	va_start(args, cond);
	print_values(passed, func, act, act_size, exp, exp_size, cond, 9, args);
	va_end(args);

	t_printf(" L%d\033[0m\n", line);
}

void t_expect_m(int passed, const char *func, int line, const char *act, size_t act_size, const char *exp, size_t exp_size, const char *cond, unsigned char mask, ...)
{
	va_list args;
	va_start(args, mask);
	print_values(passed, func, act, act_size, exp, exp_size, cond, 0, args);
	va_end(args);

	t_printf(" " BYTE_TO_BIN_PATTERN " L%d\033[0m\n", BYTE_TO_BIN(mask), line);
}

static void print_str(int passed, const char *func, int line, const char *act, const char *exp, const char *cond, int llen, int rlen)
{
	const int exp_width = s_data.width - print_header(passed, func);

	const int exp_lcol = MAX((exp_width - 1) / 2, 0);
	const int exp_rcol = (exp_width - 1 - 1) / 2 + 1;

	const int len	  = MAX(llen, rlen) * 2 + 4;
	const int act_col = llen + 4 + rlen;

	const int width = len < exp_lcol ? exp_lcol : exp_width;
	const int a	= len < exp_lcol ? MAX(exp_rcol + 1, 0) : 0;

	const int exp_lval = MAX((width - 4) / 2, 0);
	const int exp_rval = MAX((width - 4 - 1) / 2 + 1, 0);

	int act_lval = MAX(llen, exp_lval);
	int act_rval = MAX(rlen, exp_rval);

	int act_width = len < exp_lcol ? exp_width : act_lval + 4 + act_rval;

	act_lval = act_width > exp_width ? llen : act_lval;
	act_rval = act_width > exp_width ? rlen : act_rval;

	act_width = len < exp_lcol ? exp_width : act_lval + 4 + act_rval;

	const int over = MAX(act_width - exp_width, 0);
	const int add  = MAX(9 - over, 0);

	t_printf("\033[0;31m%*s %s %-*s%*s L%d\033[0m\n", act_lval, act, cond, act_rval, exp, a + add, "", line);
}

static void print_wstr(int passed, const char *func, int line, const wchar_t *act, const wchar_t *exp, const char *cond, int llen, int rlen)
{
	const int exp_width = s_data.width - print_header(passed, func);

	const int exp_lcol = MAX((exp_width - 1) / 2, 0);
	const int exp_rcol = (exp_width - 1 - 1) / 2 + 1;

	const int len	  = MAX(llen, rlen) * 2 + 4;
	const int act_col = llen + 4 + rlen;

	const int width = len < exp_lcol ? exp_lcol : exp_width;
	const int a	= len < exp_lcol ? MAX(exp_rcol + 1, 0) : 0;

	const int exp_lval = MAX((width - 4) / 2, 0);
	const int exp_rval = MAX((width - 4 - 1) / 2 + 1, 0);

	int act_lval = MAX(llen, exp_lval);
	int act_rval = MAX(rlen, exp_rval);

	int act_width = len < exp_lcol ? exp_width : act_lval + 4 + act_rval;

	act_lval = act_width > exp_width ? llen : act_lval;
	act_rval = act_width > exp_width ? rlen : act_rval;

	act_width = len < exp_lcol ? exp_width : act_lval + 4 + act_rval;

	const int over = MAX(act_width - exp_width, 0);
	const int add  = MAX(9 - over, 0);

	t_wprintf(L"\033[0;31m%*s %hs %-*s%*hs L%d\033[0m\n", act_lval, act, cond, act_rval, exp, a + add, "", line);
}

void t_expect_fmt(int passed, const char *func, int line, const char *act, unsigned int cnt, ...)
{
	va_list args;
	va_start(args, cnt);
	const char *exp = va_arg(args, const char *);
	va_end(args);

	print_str(passed, func, line, act, exp, "~=", (int)strlen(act), (int)strlen(exp));
}

void t_expect_str(int passed, const char *func, int line, const char *act, const char *exp)
{
	print_str(passed, func, line, act, exp, "==", act == NULL ? 0 : (int)strlen(act), exp == NULL ? 0 : (int)strlen(exp));
}
void t_expect_strn(int passed, const char *func, int line, const char *act, const char *exp, size_t len)
{
	print_str(passed, func, line, act, exp, "==", (int)MIN(len, act == NULL ? 0 : strlen(act)), (int)MIN(len, exp == NULL ? 0 : strlen(exp)));
}

void t_expect_wstr(int passed, const char *func, int line, const wchar_t *act, const wchar_t *exp)
{
	print_wstr(passed, func, line, act, exp, "==", act == NULL ? 0 : (int)wcslen(act), exp == NULL ? 0 : (int)wcslen(exp));
}
void t_expect_wstrn(int passed, const char *func, int line, const wchar_t *act, const wchar_t *exp, size_t len)
{
	print_wstr(passed, func, line, act, exp, "==", (int)MIN(len, act == NULL ? 0 : wcslen(act)), (int)MIN(len, exp == NULL ? 0 : wcslen(exp)));
}

void t_expect_fail(int passed, const char *func, int line, const char *fmt, ...)
{
	int len = print_header(passed, func);

	va_list args;
	va_start(args, fmt);

	t_printf("\033[0;31m");
	len += t_printv(fmt, args);
	t_printf("%*s L%d\033[0m\n", MAX(s_data.width - len + 9, 0), "", line);

	va_end(args);
}
