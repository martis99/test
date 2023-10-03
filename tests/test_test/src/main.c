#include "test_test.h"

#include "test.h"

int main(int argc, char **argv)
{
	t_init(80);
	test_test_print();
	return t_finish();
}
