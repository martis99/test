#include "test.h"

#include "mem.h"
#include "platform.h"

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(C_WIN)
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

static tdata_t s_data;

tdata_t t_get_data()
{
	return s_data;
}

void t_set_data(tdata_t data)
{
	s_data = data;
}

static int t_printv(const char *fmt, va_list args)
{
	return dprintv(s_data.print, fmt, args);
}

static int t_printf(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int ret = t_printv(fmt, args);
	va_end(args);
	return ret;
}

static int t_wprintv(const wchar *fmt, va_list args)
{
	return dwprintv(s_data.wprint, fmt, args);
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
	s_data.priv = priv;
}

void t_setup(setup_fn setup)
{
	s_data.setup = setup;
}

void t_teardown(teardown_fn teardown)
{
	s_data.teardown = teardown;
}

print_dst_t t_set_print(print_dst_t print)
{
	print_dst_t cur = s_data.print;
	s_data.print	= print;
	return cur;
}

wprint_dst_t t_set_wprint(wprint_dst_t wprint)
{
	wprint_dst_t cur = s_data.wprint;
	s_data.wprint	 = wprint;
	return cur;
}

void *t_get_priv()
{
	return s_data.priv;
}

static inline int pur()
{
	t_printf("└─");
	return 2;
}

static inline int pv()
{
	t_printf("│ ");
	return 2;
}

static inline int pvr()
{
	t_printf("├─");
	return 2;
}

void t_init(int width)
{
	s_data.print  = PRINT_DST_STD();
	s_data.wprint = PRINT_DST_WSTD();

	s_data.width  = width;
	s_data.passed = 0;
	s_data.failed = 0;
	s_data.depth  = -1;

	s_data.buf_size = 256;
	s_data.buf_len	= 0;

	s_data.buf = malloc(s_data.buf_size);
}

int t_finish()
{
	if (s_data.failed == 0) {
		t_printf("\033[0;32mPASSED %llu %s\033[0m\n", s_data.passed, s_data.passed == 1 ? "TEST" : "TESTS");
	} else {
		t_printf("\033[0;31mFAILED %llu/%llu %s\033[0m\n", s_data.failed, s_data.failed + s_data.passed, s_data.failed == 1 ? "TEST" : "TESTS");
	}

	free(s_data.buf);

	return (int)s_data.failed;
}

int t_run(test_fn fn, int print)
{
	print_dst_t printfn   = { 0 };
	wprint_dst_t wprintfn = { 0 };

	if (print == 0) {
		printfn	 = t_set_print(PRINT_DST_NONE());
		wprintfn = t_set_wprint(PRINT_DST_WNONE());
	}

	int ret = fn();

	if (print == 0) {
		t_set_print(printfn);
		t_set_wprint(wprintfn);
	}

	return ret;
}

void t_start()
{
	s_data.mem = mem_get()->mem;

	if (s_data.setup) {
		s_data.setup(s_data.priv);
	}
}

int t_end(int passed, const char *func)
{
	if (s_data.teardown) {
		s_data.teardown(s_data.priv);
	}

	if (!passed) {
		s_data.failed++;
		return 1;
	}

	int len = 0;
	for (int i = 0; i < s_data.depth; i++) {
		len += pv();
	}
	len += pvr();

	size_t mem = mem_get()->mem;
	if (s_data.mem != mem) {
		t_printf("\033[0;31m%-*s          LEAKED %d B\033[0m\n", s_data.width - len, func, mem - s_data.mem);
		s_data.failed++;
		return 1;
	}

	t_printf("\033[0;32m%-*s          OK\033[0m\n", s_data.width - len, func);

	s_data.passed++;
	return 0;
}

void t_sstart(const char *func)
{
	for (int i = 0; i < s_data.depth; i++) {
		pv();
	}
	if (s_data.depth >= 0) {
		pvr();
	}

	t_printf("%s\n", func);
	s_data.depth++;
}

int t_send(int passed, int failed)
{
	for (int i = 0; i < s_data.depth; i++) {
		pv();
	}
	pur();
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

static int print_header(int passed, const char *func, int child)
{
	if (passed && child == 0) {
		int len = 0;
		for (int i = 0; i < s_data.depth; i++) {
			len += pv();
		}
		len += pvr();
		t_printf("\033[0;31m%-*s          FAILED\033[0m\n", s_data.width - len, func);
	}
	int len = 0;
	for (int i = 0; i < s_data.depth; i++) {
		len += pv();
	}
	len += pv();
	return len;
}

// clang-format off
#define get_char(_size, _args) \
	(char)va_arg(_args, int)

#define get_short(_size, _args)                  \
	_size == 1 ? (short)va_arg(_args, int) : \
	_size == 2 ? (short)va_arg(_args, int) : \
		     0

#define get_int(_size, _args)                  \
	_size == 1 ? (int)va_arg(_args, int) : \
	_size == 2 ? (int)va_arg(_args, int) : \
	_size == 4 ? (int)va_arg(_args, int) : \
		     0
// clang-format on

#define get_long(_size, _args)                             \
	_size == 1 ? (long long)va_arg(_args, int) :       \
	_size == 2 ? (long long)va_arg(_args, int) :       \
	_size == 4 ? (long long)va_arg(_args, int) :       \
	_size == 8 ? (long long)va_arg(_args, long long) : \
		     0

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

static void print_values(int passed, const char *func, const char *act, size_t act_size, const char *exp, size_t exp_size, const char *cond, int inc, va_list args)
{
	int exp_width = s_data.width - print_header(passed, func, 0);

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

	int max_size = MAX((int)act_size, (int)exp_size);

	switch (max_size) {
	case 0: {
		const unsigned char a = get_char(act_size, args);
		const unsigned char b = get_char(exp_size, args);

		const int byte_len = 1;

		act_width = MAX(act_lcol + 1 + (byte_len + 4 + byte_len), exp_width);

		const int pl = MAX(exp_rlval - byte_len, 0);
		const int l  = MAX(pl - lover, 0);

		lover -= pl - l;

		const int r = MAX(exp_rrval - byte_len - lover, 0);
		t_printf("%*s%c %s %c%-*s", l, "", a ? '1' : '0', cond, b ? '1' : '0', r, "");
		break;
	}
	case 1: {
		const unsigned char a = get_char(act_size, args);
		const unsigned char b = get_char(exp_size, args);

		const int bin_len = 8;

		act_width = MAX(act_lcol + 1 + (bin_len + 4 + bin_len), exp_width);

		const int pl = MAX(exp_rlval - bin_len, 0);
		const int l  = MAX(pl - lover, 0);

		lover -= pl - l;

		const int r = MAX(exp_rrval - bin_len - lover, 0);
		t_printf("%*s" BYTE_TO_BIN_PATTERN " %s " BYTE_TO_BIN_PATTERN "%*s", l, "", BYTE_TO_BIN(a), cond, BYTE_TO_BIN(b), r, "");
		break;
	}
	case 2: {
		const unsigned short a = get_short(act_size, args);
		const unsigned short b = get_short(exp_size, args);

		const int hex_len = max_size * 2;

		act_width = MAX(act_lcol + 1 + (hex_len + 4 + hex_len), exp_width);

		const int pl = MAX(exp_rlval - hex_len, 0);
		const int l  = MAX(pl - lover, 0);

		lover -= pl - l;

		const int r = MAX(exp_rrval - hex_len - lover, 0);
		t_printf("%*s%04X %s %04X%*s", l, "", a, cond, b, r, "");
		break;
	}
	case 4: {
		const int a = get_int(act_size, args);
		const int b = get_int(exp_size, args);

		const int hex_len = max_size * 2;

		act_width = MAX(act_lcol + 1 + (hex_len + 4 + hex_len), exp_width);

		const int pl = MAX(exp_rlval - hex_len, 0);
		const int l  = MAX(pl - lover, 0);

		lover -= pl - l;

		const int r = MAX(exp_rrval - hex_len - lover, 0);
		t_printf("%*s%08X %s %08X%*s", l, "", a, cond, b, r, "");
		break;
	}
	case 8: {
		const long long a = get_long(act_size, args);
		const long long b = get_long(exp_size, args);

		const int hex_len = max_size * 2;

		act_width = MAX(act_lcol + 1 + (hex_len + 4 + hex_len), exp_width);

		const int pl = MAX(exp_rlval - hex_len, 0);
		const int l  = MAX(pl - lover, 0);

		lover -= pl - l;

		const int r = MAX(exp_rrval - hex_len - lover, 0);
		t_printf("%*s%016llX %s %016llX%*s", l, "", a, cond, b, r, "");
		break;
	}
	default: t_printf("Unsupported type of size: %zu\n", act_size); return;
	}

	const int over = MAX(act_width - exp_width, 0);
	const int add  = MAX(inc - over, 0);

	t_printf("%*s", add, "");
}

void t_expect_ch(int passed, const char *file, const char *func, int line, const char *check)
{
	int exp_width = s_data.width - print_header(passed, func, 0);

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

	t_printf("\033[0;31m%*s%s%*s %s:%d\033[0m\n", l, "", check, r + a + add, "", file, line);
}

void t_expect_g(int passed, const char *file, const char *func, int line, const char *act, size_t act_size, const char *exp, size_t exp_size, const char *cond, ...)
{
	va_list args;
	va_start(args, cond);
	print_values(passed, func, act, act_size, exp, exp_size, cond, 9, args);
	va_end(args);

	t_printf(" %s:%d\033[0m\n", file, line);
}

void t_expect_m(int passed, const char *file, const char *func, int line, const char *act, size_t act_size, const char *exp, size_t exp_size, const char *cond,
		unsigned char mask, ...)
{
	va_list args;
	va_start(args, mask);
	print_values(passed, func, act, act_size, exp, exp_size, cond, 0, args);
	va_end(args);

	t_printf(" " BYTE_TO_BIN_PATTERN " %s:%d\033[0m\n", BYTE_TO_BIN(mask), file, line);
}

static void print_str(int passed, const char *file, const char *func, int line, const char *act_str, const char *exp_str, size_t act_len, size_t exp_len)
{
	size_t ln	    = 0;
	size_t col	    = 0;
	size_t line_start   = 0;
	size_t exp_line_end = 0;
	size_t act_line_end = 0;
	int diff	    = 0;

	for (size_t i = 0; i < exp_len && i < act_len; i++) {
		const char exp = exp_str[i];
		const char act = act_str[i];

		if (act != exp) {
			diff = 1;
		}

		if (diff == 0) {
			if (exp == '\n') {
				col = 0;
				ln++;
				line_start = i + 1;
			} else {
				col++;
			}
		} else {
			if (exp == '\n' && exp_line_end == 0) {
				exp_line_end = i + 1;
			}
			if (act == '\n' && act_line_end == 0) {
				act_line_end = i + 1;
			}
			if (exp_line_end != 0 && act_line_end != 0) {
				break;
			}
		}
	}

	if (diff == 0 && exp_len == act_len) {
		return;
	}

	if (exp_line_end == 0) {
		exp_line_end = exp_len;
	}
	if (act_line_end == 0) {
		act_line_end = act_len;
	}

	const int exp_width = s_data.width - print_header(passed, func, 0);

	t_printf("\033[0;31m%*s          %s:%d\033[0m\n", MAX(exp_width, 0), "", file, line);
	print_header(passed, func, 1);
	t_printf("\033[0;31m");

	int exp_app = 0;
	t_printf("exp:%d: ", ln);
	for (size_t i = 0; i < exp_line_end - line_start; i++) {
		char c = exp_str[line_start + i];
		// clang-format off
		switch (c) {
		case '\n':t_printf("\\n"); exp_app += (i <= col ? 1 : 0); break;
		case '\r':t_printf("\\r"); exp_app += (i <= col ? 1 : 0); break;
		case '\t':t_printf("\\t"); exp_app += (i <= col ? 1 : 0); break;
		default: t_printf("%c", c); break;
		}
		// clang-format on
	}

	t_printf("\033[0m\n");
	print_header(passed, func, 1);
	t_printf("\033[0;31m");

	int act_app = 0;
	int h_len   = t_printf("act:%d: ", ln);
	for (size_t i = 0; i < act_line_end - line_start; i++) {
		char c = act_str[line_start + i];
		// clang-format off
		switch (c) {
		case '\n': t_printf("\\n"); act_app += (i <= col ? 1 : 0); break;
		case '\r': t_printf("\\r"); act_app += (i <= col ? 1 : 0); break;
		case '\t': t_printf("\\t"); act_app += (i <= col ? 1 : 0); break;
		default: t_printf("%c", c); break;
		}
		// clang-format on
	}

	t_printf("\033[0m\n");
	print_header(passed, func, 1);
	t_printf("\033[0;31m%*s^\033[0m\n", h_len + MIN(act_app, exp_app) + col, "");
}

static void print_wstr(int passed, const char *file, const char *func, int line, const wchar_t *act_str, const wchar_t *exp_str, size_t act_len, size_t exp_len)
{
	size_t ln	    = 0;
	size_t col	    = 0;
	size_t line_start   = 0;
	size_t exp_line_end = 0;
	size_t act_line_end = 0;
	int diff	    = 0;

	for (size_t i = 0; i < exp_len && i < act_len; i++) {
		const wchar_t exp = exp_str[i];
		const wchar_t act = act_str[i];

		if (act != exp) {
			diff = 1;
		}

		if (diff == 0) {
			if (exp == '\n') {
				col = 0;
				ln++;
				line_start = i + 1;
			} else {
				col++;
			}
		} else {
			if (exp == '\n' && exp_line_end == 0) {
				exp_line_end = i + 1;
			}
			if (act == '\n' && act_line_end == 0) {
				act_line_end = i + 1;
			}
			if (exp_line_end != 0 && act_line_end != 0) {
				break;
			}
		}
	}

	if (diff == 0 && exp_len == act_len) {
		return;
	}

	if (exp_line_end == 0) {
		exp_line_end = exp_len;
	}
	if (act_line_end == 0) {
		act_line_end = act_len;
	}

	const int exp_width = s_data.width - print_header(passed, func, 0);

	t_printf("\033[0;31m%*s          %s:%d\033[0m\n", MAX(exp_width, 0), "", file, line);
	print_header(passed, func, 1);
	t_printf("\033[0;31m");

	int exp_app = 0;
	t_printf("exp(%d): ", ln);
	for (size_t i = 0; i < exp_line_end - line_start; i++) {
		wchar_t c = exp_str[line_start + i];
		// clang-format off
		switch (c) {
		case '\n':t_wprintf(L"\\n"); exp_app += (i <= col ? 1 : 0); break;
		case '\r':t_wprintf(L"\\r"); exp_app += (i <= col ? 1 : 0); break;
		case '\t':t_wprintf(L"\\t"); exp_app += (i <= col ? 1 : 0); break;
		default: t_wprintf(L"%c", c); break;
		}
		// clang-format on
	}

	t_printf("\033[0m\n");
	print_header(passed, func, 1);
	t_printf("\033[0;31m");

	int act_app = 0;
	int h_len   = t_printf("act(%d): ", ln);
	for (size_t i = 0; i < act_line_end - line_start; i++) {
		wchar_t c = act_str[line_start + i];
		// clang-format off
		switch (c) {
		case '\n': t_wprintf(L"\\n"); act_app += (i <= col ? 1 : 0); break;
		case '\r': t_wprintf(L"\\r"); act_app += (i <= col ? 1 : 0); break;
		case '\t': t_wprintf(L"\\t"); act_app += (i <= col ? 1 : 0); break;
		default: t_wprintf(L"%c", c); break;
		}
		// clang-format on
	}

	t_printf("\033[0m\n");
	print_header(passed, func, 1);
	t_printf("\033[0;31m%*s^\033[0m\n", h_len + MIN(act_app, exp_app) + col, "");
}

void t_expect_fmt(int passed, const char *file, const char *func, int line, const char *act, unsigned int cnt, ...)
{
	va_list args;
	va_start(args, cnt);
	const char *exp = va_arg(args, const char *);
	va_end(args);

	print_str(passed, file, func, line, act, exp, (int)strlen(act), (int)strlen(exp));
}

void t_expect_str(int passed, const char *file, const char *func, int line, const char *act, const char *exp)
{
	print_str(passed, file, func, line, act, exp, act == NULL ? 0 : (int)strlen(act), exp == NULL ? 0 : (int)strlen(exp));
}

void t_expect_strn(int passed, const char *file, const char *func, int line, const char *act, const char *exp, size_t len)
{
	print_str(passed, file, func, line, act, exp, (int)MIN(len, act == NULL ? 0 : strlen(act)), (int)MIN(len, exp == NULL ? 0 : strlen(exp)));
}

void t_expect_wstr(int passed, const char *file, const char *func, int line, const wchar_t *act, const wchar_t *exp)
{
	print_wstr(passed, file, func, line, act, exp, act == NULL ? 0 : (int)wcslen(act), exp == NULL ? 0 : (int)wcslen(exp));
}

void t_expect_wstrn(int passed, const char *file, const char *func, int line, const wchar_t *act, const wchar_t *exp, size_t len)
{
	print_wstr(passed, file, func, line, act, exp, (int)MIN(len, act == NULL ? 0 : wcslen(act)), (int)MIN(len, exp == NULL ? 0 : wcslen(exp)));
}

void t_expect_fail(int passed, const char *file, const char *func, int line, const char *fmt, ...)
{
	int len = print_header(passed, func, 0);

	va_list args;
	va_start(args, fmt);

	t_printf("\033[0;31m");
	len += t_printv(fmt, args);
	t_printf("%*s %s:%d\033[0m\n", MAX(s_data.width - len + 9, 0), "", file, line);

	va_end(args);
}

int t_fprintf(void *priv, const char *fmt, ...)
{
	(void)priv;

	va_list args;
	va_start(args, fmt);

	int ret = vsnprintf(s_data.buf + s_data.buf_len, s_data.buf_size - s_data.buf_len, fmt, args);

	va_end(args);

	s_data.buf_len += ret;

	return ret;
}

void t_expect_fstr_start(const char *exp, size_t len)
{
	(void)exp;
	(void)len;
	s_data.exp     = exp;
	s_data.exp_len = len;

	if (s_data.buf_size < len + 1) {
		s_data.buf_size = len + 1;
		s_data.buf	= realloc(s_data.buf, s_data.buf_size);
	}

	memset(s_data.buf, 0, s_data.buf_size);
	s_data.buf_len = 0;
}

int t_expect_fstr_end(int passed, const char *file, const char *func, int line)
{
	const int ret = t_strcmp(s_data.buf, s_data.exp);

	if (ret != 0) {
		print_str(passed, file, func, line, s_data.buf, s_data.exp, s_data.buf_len, s_data.exp_len);
	}

	return ret;
}
