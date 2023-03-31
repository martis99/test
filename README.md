# test
test - simple testing library for C.

***

## Usage
### Single test
```C
#include "test.h"

TEST(test)
{
	START;
	EXPECT_EQ(4 + 4, 9);
	END;
}

TEST(tests)
{
	SSTART;
	RUN(test);
	SEND;
}

int main(int argc, char **argv)
{
	t_init(80);
	tests();
	t_finish();
}
```
#### Passed
![image](https://user-images.githubusercontent.com/61162271/229192699-9a06edc1-a115-45cc-9697-31d54d2a444a.png)
#### Failed
![image](https://user-images.githubusercontent.com/61162271/229192843-00fe4306-86c7-4762-ba3a-9e6519787afe.png)
### Multiple tests
```C
#include "test.h"

TEST(test1)
{
	START;
	EXPECT_EQ(4 + 5, 9);
	END;
}

TEST(test2)
{
	START;
	int a = 5 * 6;
	EXPECT_EQ(a, 30);
	END;
}

TEST(test3)
{
	START;
	int a = 8;
	int b = 2;
	EXPECT_EQ(a / b, 4);
	END;
}

int main(int argc, char **argv)
{
	t_init(2);
	SSTART;
	RUN(test1);
	RUN(test2);
	RUN(test3);
	SEND;
	t_free();
}
```
#### Passed
![image](https://user-images.githubusercontent.com/61162271/229193424-f606da5e-ca18-4f6b-b562-9c4d6e54c235.png)
#### Failed
![image](https://user-images.githubusercontent.com/61162271/229193785-41650229-cfcf-4beb-ac43-85e089e810ad.png)

### Parameterized Tests
```C
#include "test.h"

TEST(test, int a, int b, int c)
{
	START;
	EXPECT_EQ(a + b, c);
	END;
}

TEST(tests)
{
	SSTART;
	RUN(test, 4, 5, 9);
	RUN(test, 1, 2, 3);
	RUN(test, 2, 4, 6);
	SEND;
}

int main(int argc, char **argv)
{
	t_init(80);
	tests();
	t_finish();
}
```

### Multiple level tests
```C
#include "test.h"

TEST(grand_child1, int a, int b)
{
	START;
	EXPECT_EQ(a + 5, b);
	END;
}

TEST(child1)
{
	SSTART;
	RUN(grand_child1, 3, 8);
	RUN(grand_child1, 4, 9);
	RUN(grand_child1, 5, 11);
	RUN(grand_child1, 6, 14);
	SEND;
}

TEST(grand_child2, int a, int b)
{
	START;
	EXPECT_EQ(a / 5, b);
	EXPECT_EQ(6 - 5, 1);
	END;
}

TEST(child2)
{
	SSTART;
	RUN(grand_child2, 10, 2);
	RUN(grand_child2, 20, 3);
	RUN(grand_child2, 30, 6);
	RUN(grand_child2, 40, 4);
	SEND;
}

TEST(parent)
{
	SSTART;
	RUN(child1);
	RUN(child2);
	SEND;
}

TEST(tests)
{
	SSTART;
	RUN(parent);
	SEND;
}

int main(int argc, char **argv)
{
	t_init(80);
	tests();
	t_finish();
}
```
![image](https://user-images.githubusercontent.com/61162271/229194843-f41ace2c-17a8-44b0-b6bb-86c8fbd636b0.png)

### Tests in multiple files
child1.h
```C
#ifndef CHILD1_H
#define CHILD1_H

#include "test_h.h"

STEST(child1);

#endif
```
child1.c
```C
#include "child1.h"

#include "test.h"

TEST(grand_child1, int a, int b)
{
	START;
	EXPECT_EQ(a + 5, b);
	END;
}

STEST(child1)
{
	SSTART;
	RUN(grand_child1, 3, 8);
	RUN(grand_child1, 4, 9);
	RUN(grand_child1, 5, 11);
	RUN(grand_child1, 6, 14);
	SEND;
}
```
main.c
```C
#include "child1.h"

#include "test.h"

TEST(grand_child2, int a, int b)
{
	START;
	EXPECT_EQ(a / 5, b);
	EXPECT_EQ(6 - 5, 1);
	END;
}

TEST(child2)
{
	SSTART;
	RUN(grand_child2, 10, 2);
	RUN(grand_child2, 20, 3);
	RUN(grand_child2, 30, 6);
	RUN(grand_child2, 40, 4);
	SEND;
}

TEST(parent)
{
	SSTART;
	RUN(child1);
	RUN(child2);
	SEND;
}

TEST(tests)
{
	SSTART;
	RUN(parent);
	SEND;
}

int main(int argc, char **argv)
{
	t_init(80);
	tests();
	t_finish();
}
```
### Setup & Teardown
`setup` and `teardown` functions provided in `t_setup` and `t_teardown` are ran before and after every `TEST` function.
```C
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

int main(int argc, char **argv)
{
	int a;
	t_set_priv(&a);
	t_setup(setup);
	t_teardown(teardown);

	t_init(80);
	tests();
	t_finish();
}
```
