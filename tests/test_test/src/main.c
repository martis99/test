#include "test_test.h"

#include "test.h"

int main(int argc, char **argv)
{
	t_init(80);
	t_run(test_test, 1);
	return t_finish();
}
