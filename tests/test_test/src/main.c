#include "test_test.h"

#include "test.h"

#include "cplatform.h"

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	cplatform_t cplatform = { 0 };
	cplatform_init(&cplatform);

	t_init(80);
	t_run(test_test, 1);
	int ret = t_finish();

	cplatform_free(&cplatform);

	return ret;
}
