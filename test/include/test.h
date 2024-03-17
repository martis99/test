#ifndef TEST_H
#define TEST_H

#include "print.h"

#include <stdarg.h>
#include <stddef.h>
#include <wchar.h>

void t_init(int width);
int t_finish();

typedef int (*test_fn)();
int t_run(test_fn fn, int print);

typedef int (*setup_fn)(void *priv);
typedef int (*teardown_fn)(void *priv);

void t_set_priv(void *priv);
void t_setup(setup_fn setup);
void t_teardown(teardown_fn teardown);

void t_set_print(c_printv_fn print);
void t_set_wprint(c_wprintv_fn wprint);

void *t_get_priv();

void t_start();
int t_end(int passed, const char *func);

void t_sstart(const char *func);
int t_send(int passed, int failed);

int t_scan(const char *str, const char *fmt, ...);
int t_strcmp(const char *act, const char *exp);
int t_strncmp(const char *act, const char *exp, size_t len);

int t_wstrcmp(const wchar_t *act, const wchar_t *exp);
int t_wstrncmp(const wchar_t *act, const wchar_t *exp, size_t len);

void t_expect_ch(int passed, const char *func, int line, const char *check);

void t_expect_g(int passed, const char *func, int line, const char *act, size_t act_size, const char *exp, size_t exp_size, const char *cond, ...);
void t_expect_m(int passed, const char *func, int line, const char *act, size_t act_size, const char *exp, size_t exp_size, const char *cond, unsigned char mask, ...);

void t_expect_fmt(int passed, const char *func, int line, const char *act, unsigned int cnt, ...);

void t_expect_str(int passed, const char *func, int line, const char *act, const char *exp);
void t_expect_strn(int passed, const char *func, int line, const char *act, const char *exp, size_t len);

void t_expect_wstr(int passed, const char *func, int line, const wchar_t *act, const wchar_t *exp);
void t_expect_wstrn(int passed, const char *func, int line, const wchar_t *act, const wchar_t *exp, size_t len);

void t_expect_fail(int passed, const char *func, int line, const char *fmt, ...);

int t_fprintf(void *priv, const char *fmt, ...);
void t_expect_fstr_start(const char *exp, size_t len);
int t_expect_fstr_end(int passed, const char *func, int line);

//Declare test
#define TEST(_name)	  static inline int _name()
#define TESTP(_name, ...) static inline int _name(__VA_ARGS__)

//Test start
#define START            \
	int _passed = 1; \
	t_start()

//Test result
#define RES t_end(_passed, __func__)

//Test end
#define END return RES

//Subtests start
#define SSTART            \
	int _spassed = 0; \
	int _sfailed = 0; \
	t_sstart(__func__)

//Run test
#define RUN(_fn)                    \
	do {                        \
		if (_fn()) {        \
			_sfailed++; \
		} else {            \
			_spassed++; \
		}                   \
	} while (0)

#define RUNP(_fn, ...)                  \
	do {                            \
		if (_fn(__VA_ARGS__)) { \
			_sfailed++;     \
		} else {                \
			_spassed++;     \
		}                       \
	} while (0)

//Subtests end
#define SEND return t_send(_spassed, _sfailed)

#define EXPECT(_check)                                                     \
	do {                                                               \
		if (!(_check)) {                                           \
			t_expect_ch(_passed, __func__, __LINE__, #_check); \
			_passed = 0;                                       \
		}                                                          \
	} while (0)

#define EXPECT_EQ(_actual, _expected)                                                                                                                \
	do {                                                                                                                                         \
		if ((_actual) != (_expected)) {                                                                                                      \
			t_expect_g(_passed, __func__, __LINE__, #_actual, sizeof(_actual), #_expected, sizeof(_expected), "==", _actual, _expected); \
			_passed = 0;                                                                                                                 \
		}                                                                                                                                    \
	} while (0)

#define EXPECT_EQM(_actual, _expected, _mask)                                                                                                               \
	do {                                                                                                                                                \
		if (((_actual) ^ (_expected)) & (_mask)) {                                                                                                  \
			t_expect_m(_passed, __func__, __LINE__, #_actual, sizeof(_actual), #_expected, sizeof(_expected), "==", _mask, _actual, _expected); \
			_passed = 0;                                                                                                                        \
		}                                                                                                                                           \
	} while (0)

#define EXPECT_EQB(_actual, _expected)                                                                                 \
	do {                                                                                                           \
		if ((_actual) != (_expected)) {                                                                        \
			t_expect_g(_passed, __func__, __LINE__, #_actual, 0, #_expected, 0, "==", _actual, _expected); \
			_passed = 0;                                                                                   \
		}                                                                                                      \
	} while (0)

#define EXPECT_NE(_actual, _expected)                                                                                                                \
	do {                                                                                                                                         \
		if ((_actual) == (_expected)) {                                                                                                      \
			t_expect_g(_passed, __func__, __LINE__, #_actual, sizeof(_actual), #_expected, sizeof(_expected), "!=", _actual, _expected); \
			_passed = 0;                                                                                                                 \
		}                                                                                                                                    \
	} while (0)

#define EXPECT_NEM(_actual, _expected, _mask)                                                                                                               \
	do {                                                                                                                                                \
		if (!(((_actual) ^ (_expected)) & (_mask))) {                                                                                               \
			t_expect_m(_passed, __func__, __LINE__, #_actual, sizeof(_actual), #_expected, sizeof(_expected), "!=", _mask, _actual, _expected); \
			_passed = 0;                                                                                                                        \
		}                                                                                                                                           \
	} while (0)

#define EXPECT_NEB(_actual, _expected)                                                                                 \
	do {                                                                                                           \
		if ((_actual) == (_expected)) {                                                                        \
			t_expect_g(_passed, __func__, __LINE__, #_actual, 0, #_expected, 0, "!=", _actual, _expected); \
			_passed = 0;                                                                                   \
		}                                                                                                      \
	} while (0)

#define EXPECT_GT(_actual, _expected)                                                                                                                \
	do {                                                                                                                                         \
		if ((_actual) <= (_expected)) {                                                                                                      \
			t_expect_g(_passed, __func__, __LINE__, #_actual, sizeof(_actual), #_expected, sizeof(_expected), "> ", _actual, _expected); \
			_passed = 0;                                                                                                                 \
		}                                                                                                                                    \
	} while (0)

#define EXPECT_GE(_actual, _expected)                                                                                                                \
	do {                                                                                                                                         \
		if ((_actual) < (_expected)) {                                                                                                       \
			t_expect_g(_passed, __func__, __LINE__, #_actual, sizeof(_actual), #_expected, sizeof(_expected), ">=", _actual, _expected); \
			_passed = 0;                                                                                                                 \
		}                                                                                                                                    \
	} while (0)

#define EXPECT_LT(_actual, _expected)                                                                                                                \
	do {                                                                                                                                         \
		if ((_actual) >= (_expected)) {                                                                                                      \
			t_expect_g(_passed, __func__, __LINE__, #_actual, sizeof(_actual), #_expected, sizeof(_expected), "< ", _actual, _expected); \
			_passed = 0;                                                                                                                 \
		}                                                                                                                                    \
	} while (0)

#define EXPECT_LE(_actual, _expected)                                                                                                                \
	do {                                                                                                                                         \
		if ((_actual) > (_expected)) {                                                                                                       \
			t_expect_g(_passed, __func__, __LINE__, #_actual, sizeof(_actual), #_expected, sizeof(_expected), "<=", _actual, _expected); \
			_passed = 0;                                                                                                                 \
		}                                                                                                                                    \
	} while (0)

#define EXPECT_FMT(_str, _cnt, ...)                                                         \
	do {                                                                                \
		if ((_cnt) != t_scan(_str, __VA_ARGS__)) {                                  \
			t_expect_fmt(_passed, __func__, __LINE__, _str, _cnt, __VA_ARGS__); \
			_passed = 0;                                                        \
		}                                                                           \
	} while (0)

#define EXPECT_STR(_actual, _expected)                                                 \
	do {                                                                           \
		if (t_strcmp(_actual, _expected) != 0) {                               \
			t_expect_str(_passed, __func__, __LINE__, _actual, _expected); \
			_passed = 0;                                                   \
		}                                                                      \
	} while (0)

#define EXPECT_STRN(_actual, _expected, _len)                                                 \
	do {                                                                                  \
		if (t_strncmp(_actual, _expected, _len) != 0) {                               \
			t_expect_strn(_passed, __func__, __LINE__, _actual, _expected, _len); \
			_passed = 0;                                                          \
		}                                                                             \
	} while (0)

#define EXPECT_WSTR(_actual, _expected)                                                 \
	do {                                                                            \
		if (t_wstrcmp(_actual, _expected) != 0) {                               \
			t_expect_wstr(_passed, __func__, __LINE__, _actual, _expected); \
			_passed = 0;                                                    \
		}                                                                       \
	} while (0)

#define EXPECT_WSTRN(_actual, _expected, _len)                                                 \
	do {                                                                                   \
		if (t_wstrncmp(_actual, _expected, _len) != 0) {                               \
			t_expect_wstrn(_passed, __func__, __LINE__, _actual, _expected, _len); \
			_passed = 0;                                                           \
		}                                                                              \
	} while (0)

#define EXPECT_FAIL(_fmt, ...)                                                 \
	do {                                                                   \
		t_expect_fail(_passed, __func__, __LINE__, _fmt, __VA_ARGS__); \
		_passed = 0;                                                   \
                                                                               \
	} while (0)

#define EXPECT_FSTR(_print, _expected, _len)                               \
	do {                                                               \
		t_expect_fstr_start(_expected, _len);                      \
		_print;                                                    \
		if (t_expect_fstr_end(_passed, __func__, __LINE__) != 0) { \
			_passed = 0;                                       \
		}                                                          \
	} while (0)

#endif
