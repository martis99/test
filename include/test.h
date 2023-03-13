#ifndef TEST_H
#define TEST_H

#include <stddef.h>

void t_init(int width);
void t_free();

typedef int (*setup_fn)(void *priv);
typedef int (*teardown_fn)(void *priv);

void t_set_priv(void *priv);
void t_setup(setup_fn setup);
void t_teardown(teardown_fn teardown);

void *t_get_priv();

void t_start();
int t_end(int passed, const char *func);

void t_sstart(const char *func);
int t_send(int passed, int failed);

void t_expect_ch(int passed, const char *func, int line, const char *check);

void t_expect_eq(int passed, const char *func, int line, const char *act, size_t act_size, const char *exp, size_t exp_size, ...);
void t_expect_eqm(int passed, const char *func, int line, const char *act, size_t act_size, const char *exp, size_t exp_size, const char *mask_str, unsigned char mask,
		  ...);
void t_expect_eqb(int passed, const char *func, int line, const char *act, const char *exp, ...);

void t_expect_ne(int passed, const char *func, int line, const char *act, size_t act_size, const char *exp, size_t exp_size, ...);
void t_expect_nem(int passed, const char *func, int line, const char *act, size_t act_size, const char *exp, size_t exp_size, const char *mask_str, unsigned char mask,
		  ...);
void t_expect_neb(int passed, const char *func, int line, const char *act, const char *exp, ...);

void t_results();

//Declare test
#define TEST(_name, ...) static inline int _name(__VA_ARGS__)

//Test start
#define START            \
	int _passed = 1; \
	t_start()

//Test end
#define END return t_end(_passed, __func__)

//Subtests start
#define SSTART           \
	int _passed = 0; \
	int _failed = 0; \
	t_sstart(__func__)

//Run test
#define RUN(_fn, ...)                   \
	do {                            \
		if (_fn(__VA_ARGS__)) { \
			_failed++;      \
		} else {                \
			_passed++;      \
		}                       \
	} while (0)

//Subtests end
#define SEND return t_send(_passed, _failed)

#define EXPECT(_check)                                                     \
	do {                                                               \
		if (!(_check)) {                                           \
			t_expect_ch(_passed, __func__, __LINE__, #_check); \
			_passed = 0;                                       \
		}                                                          \
	} while (0)

#define EXPECT_EQ(_actual, _expected)                                                                                                           \
	do {                                                                                                                                    \
		if ((_actual) != (_expected)) {                                                                                                 \
			t_expect_eq(_passed, __func__, __LINE__, #_actual, sizeof(_actual), #_expected, sizeof(_expected), _actual, _expected); \
			_passed = 0;                                                                                                            \
		}                                                                                                                               \
	} while (0)

#define EXPECT_EQM(_actual, _expected, _mask)                                                                                                                   \
	do {                                                                                                                                                    \
		if (((_actual) ^ (_expected)) & (_mask)) {                                                                                                      \
			t_expect_eqm(_passed, __func__, __LINE__, #_actual, sizeof(_actual), #_expected, sizeof(_expected), #_mask, _mask, _actual, _expected); \
			_passed = 0;                                                                                                                            \
		}                                                                                                                                               \
	} while (0)

#define EXPECT_EQB(_actual, _expected)                                                                       \
	do {                                                                                                 \
		if ((_actual) != (_expected)) {                                                              \
			t_expect_eqb(_passed, __func__, __LINE__, #_actual, #_expected, _actual, _expected); \
			_passed = 0;                                                                         \
		}                                                                                            \
	} while (0)

#define EXPECT_NE(_actual, _expected)                                                                                                           \
	do {                                                                                                                                    \
		if (!((_actual) != (_expected))) {                                                                                              \
			t_expect_ne(_passed, __func__, __LINE__, #_actual, sizeof(_actual), #_expected, sizeof(_expected), _actual, _expected); \
			_passed = 0;                                                                                                            \
		}                                                                                                                               \
	} while (0)

#define EXPECT_NEM(_actual, _expected, _mask)                                                                                                                   \
	do {                                                                                                                                                    \
		if (!(((_actual) ^ (_expected)) & (_mask))) {                                                                                                   \
			t_expect_nem(_passed, __func__, __LINE__, #_actual, sizeof(_actual), #_expected, sizeof(_expected), #_mask, _mask, _actual, _expected); \
			_passed = 0;                                                                                                                            \
		}                                                                                                                                               \
	} while (0)

#define EXPECT_NEB(_actual, _expected)                                                                       \
	do {                                                                                                 \
		if (!((_actual) != (_expected))) {                                                           \
			t_expect_neb(_passed, __func__, __LINE__, #_actual, #_expected, _actual, _expected); \
			_passed = 0;                                                                         \
		}                                                                                            \
	} while (0)

#endif
