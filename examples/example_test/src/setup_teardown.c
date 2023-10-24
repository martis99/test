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

TESTP(test, int v)
{
	START;
	int *a = t_get_priv();
	EXPECT_EQ(*a, v);
	END;
}

TEST(tests)
{
	SSTART;
	RUNP(test, 5);
	RUNP(test, 4);
	SEND;
}

void setup_teardown()
{
	int a;
	t_set_priv(&a);
	t_setup(setup);
	t_teardown(teardown);

	t_init(80);
	t_run(tests, 1);
	t_finish();
}
