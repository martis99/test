#include "test.h"

int setup(void *priv)
{
	int *a = priv;
	*a     = 5;
	return 0;
}

int teardown(void *priv)
{
	int *a = priv;
	*a     = 0;
	return 0;
}

TEST(test, int v)
{
	START;
	int *a = t_get_priv();
	EXPECT_EQ(*a, v);
	END;
}

TEST(tests)
{
	SSTART;
	RUN(test, 5);
	RUN(test, 4);
	SEND;
}

void setup_teardown()
{
	int a;
	t_set_priv(&a);
	t_setup(setup);
	t_teardown(teardown);

	t_init(80);
	tests();
	t_finish();
}
