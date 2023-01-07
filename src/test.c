﻿#include "test.h"

#include <fcntl.h>
#include <io.h>
#include <locale.h>
#include <memory.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

struct tdata {
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

void t_sprint()
{
	int r = _setmode(_fileno(stdout), _O_U16TEXT);
}

void t_print(const unsigned short *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vwprintf_s(fmt, args);
	va_end(args);
}

void t_eprint()
{
	int r = _setmode(_fileno(stdout), _O_TEXT);
}

void pur()
{
	wprintf(L"└─");
}

void pv()
{
	wprintf(L"│ ");
}

void pvr()
{
	wprintf(L"├─");
}

void t_init()
{
	t_sprint();

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
	s_setup(s_priv);
}

int t_end(int passed, const char *func)
{
	s_teardown(s_priv);
	t_sprint();
	if (passed) {
		for (int i = 0; i < s_data.depth; i++) {
			pv();
			wprintf(L"  ");
		}
		pvr();
		wprintf(L"\033[0;32m%-52hs OK\033[0m\n", func);
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
		wprintf(L"  ");
	}
	if (s_data.depth >= 0) {
		pvr();
	}
	wprintf(L"%hs\n", func);
	s_data.depth++;
}

int t_send(int passed, int failed)
{
	for (int i = 0; i < s_data.depth; i++) {
		pv();
		wprintf(L"  ");
	}
	pur();
	if (failed == 0) {
		wprintf(L"\033[0;32mPASSED %d %hs\033[0m\n", passed, passed == 1 ? "TEST" : "TESTS");
	} else {
		wprintf(L"\033[0;31mFAILED %d/%d %hs\033[0m\n", failed, failed + passed, failed == 1 ? "TEST" : "TESTS");
	}
	s_data.depth--;
	return failed > 0;
}

void t_expect(int passed, const char *func)
{
	if (passed) {
		for (int i = 0; i < s_data.depth; i++) {
			pv();
			wprintf(L"  ");
		}
		pvr();
		wprintf(L"\033[0;31m%-52hs FAILED\033[0m\n", func);
	}
	for (int i = 0; i < s_data.depth + 1; i++) {
		pv();
		wprintf(L"  ");
	}
	pvr();
}

void t_results()
{
	if (s_data.failed == 0) {
		wprintf(L"\033[0;32mPASSED %llu %hs\033[0m\n", s_data.passed, s_data.passed == 1 ? "TEST" : "TESTS");
	} else {
		wprintf(L"\033[0;31mFAILED %llu/%llu %hs\033[0m\n", s_data.failed, s_data.failed + s_data.passed, s_data.failed == 1 ? "TEST" : "TESTS");
	}
}