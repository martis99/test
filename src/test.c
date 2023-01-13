#include "test.h"

#include <fcntl.h>
#include <io.h>
#include <locale.h>
#include <memory.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

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
	int r = _setmode(_fileno(stdout), _O_U16TEXT);
}

static inline void t_eprint()
{
	int r = _setmode(_fileno(stdout), _O_TEXT);
}

static inline void pur()
{
	wprintf_s(L"└─");
}

static inline void pv()
{
	wprintf_s(L"│ ");
}

static inline void pvr()
{
	wprintf_s(L"├─");
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
			wprintf_s(L"  ");
		}
		pvr();
		wprintf_s(L"\033[0;32m%-64hs %*hsOK\033[0m\n", func, (s_data.width - s_data.depth) * 4, " ");

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
		wprintf_s(L"  ");
	}
	if (s_data.depth >= 0) {
		pvr();
	}
	wprintf_s(L"%hs\n", func);
	s_data.depth++;
}

int t_send(int passed, int failed)
{
	for (int i = 0; i < s_data.depth; i++) {
		pv();
		wprintf_s(L"  ");
	}
	pur();
	if (failed == 0) {
		wprintf_s(L"\033[0;32mPASSED %d %hs\033[0m\n", passed, passed == 1 ? "TEST" : "TESTS");
	} else {
		wprintf_s(L"\033[0;31mFAILED %d/%d %hs\033[0m\n", failed, failed + passed, failed == 1 ? "TEST" : "TESTS");
	}
	s_data.depth--;
	return failed > 0;
}

void t_expect_ch(int passed, const char *func, const char *check, int line)
{
	t_sprint();
	if (passed) {
		for (int i = 0; i < s_data.depth; i++) {
			pv();
			wprintf_s(L"  ");
		}
		pvr();
		wprintf_s(L"\033[0;31m%-64hs %*hsFAILED\033[0m\n", func, (s_data.width - s_data.depth) * 4, " ");
	}
	for (int i = 0; i < s_data.depth + 1; i++) {
		pv();
		wprintf_s(L"  ");
	}
	pvr();

	wprintf_s(L"\033[0;31m%-64hs %*hsL%d\033[0m\n", check, (s_data.width - (s_data.depth + 1)) * 4, " ", line);

	t_eprint();
}

void t_expect_eq(int passed, const char *func, const char *actual, const char *expected, int line, ...)
{
	t_sprint();
	if (passed) {
		for (int i = 0; i < s_data.depth; i++) {
			pv();
			wprintf_s(L"  ");
		}
		pvr();
		wprintf_s(L"\033[0;31m%-64hs %*hsFAILED\033[0m\n", func, (s_data.width - s_data.depth) * 4, " ");
	}
	for (int i = 0; i < s_data.depth + 1; i++) {
		pv();
		wprintf_s(L"  ");
	}
	pvr();

	wprintf_s(L"\033[0;31m%-19hs (", actual);

	va_list args;
	va_start(args, line);
	vwprintf_s(L"0x%08X", args);
	va_end(args);

	wprintf_s(L") == %-28hs %*hsL%d\033[0m\n", expected, (s_data.width - (s_data.depth + 1)) * 4, " ", line);
	t_eprint();
}

void t_expect_ne(int passed, const char *func, const char *actual, const char *expected, int line, ...)
{
	t_sprint();
	if (passed) {
		for (int i = 0; i < s_data.depth; i++) {
			pv();
			wprintf_s(L"  ");
		}
		pvr();
		wprintf_s(L"\033[0;31m%-64hs %*hsFAILED\033[0m\n", func, (s_data.width - s_data.depth) * 4, " ");
	}
	for (int i = 0; i < s_data.depth + 1; i++) {
		pv();
		wprintf_s(L"  ");
	}
	pvr();

	wprintf_s(L"\033[0;31m%-19hs (", actual);

	va_list args;
	va_start(args, line);
	vwprintf_s(L"0x%08X", args);
	va_end(args);

	wprintf_s(L") != %-28hs %*hsL%d\033[0m\n", expected, (s_data.width - (s_data.depth + 1)) * 4, " ", line);
	t_eprint();
}

void t_results()
{
	if (s_data.failed == 0) {
		wprintf_s(L"\033[0;32mPASSED %llu %hs\033[0m\n", s_data.passed, s_data.passed == 1 ? "TEST" : "TESTS");
	} else {
		wprintf_s(L"\033[0;31mFAILED %llu/%llu %hs\033[0m\n", s_data.failed, s_data.failed + s_data.passed, s_data.failed == 1 ? "TEST" : "TESTS");
	}
}
