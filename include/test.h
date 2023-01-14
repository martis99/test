#ifndef TEST_H
#define TEST_H

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

void t_expect_ch(int passed, const char *func, const char *check, int line);
void t_expect_eq(int passed, const char *func, const char *actual, const char *expected, int line, ...);
void t_expect_eqm(int passed, const char *func, const char *actual, const char *expected, const char *mask_str, unsigned char mask, int line, ...);
void t_expect_ne(int passed, const char *func, const char *actual, const char *expected, int line, ...);

void t_results();

//Declare test
#define TEST(_name, ...) static inline int _name##(__VA_ARGS__)

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
			t_expect_ch(_passed, __func__, #_check, __LINE__); \
			_passed = 0;                                       \
		}                                                          \
	} while (0)

#define EXPECT_EQ(_actual, _expected)                                                            \
	do {                                                                                     \
		if (_actual != _expected) {                                                      \
			t_expect_eq(_passed, __func__, #_actual, #_expected, __LINE__, _actual); \
			_passed = 0;                                                             \
		}                                                                                \
	} while (0)

#define EXPECT_EQM(_actual, _expected, _mask)                                                                    \
	do {                                                                                                     \
		if (((_actual) & (_mask)) != ((_expected) & (_mask))) {                                          \
			t_expect_eqm(_passed, __func__, #_actual, #_expected, #_mask, _mask, __LINE__, _actual); \
			_passed = 0;                                                                             \
		}                                                                                                \
	} while (0)

#define EXPECT_NE(_actual, _expected)                                                            \
	do {                                                                                     \
		if (_actual == _expected) {                                                      \
			t_expect_ne(_passed, __func__, #_actual, #_expected, __LINE__, _actual); \
			_passed = 0;                                                             \
		}                                                                                \
	} while (0)

#endif
