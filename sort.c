#include "sort.h"

Ret bubble_sort(void** data, size_t nr, CompareFunc cmp)
{
	size_t i, j, max;
	void* e;

	return_val_if_fail((data != NULL) && (cmp != NULL), RET_INVALID_PARAMS);

	if (nr < 2)
	{
		return RET_OK;
	}

	for(i = nr - 1; i > 0; i--)
	{
		max = 0;
		for(j = 1; j < i; j++)
		{
			if (cmp(data[j], data[max]) > 0)
			{
				max = j;
			}
		}

		if (cmp(data[max], data[i]) > 0)
		{
			void* e = data[max];
			data[max] = data[i];
			data[i] = e;
		}
	}

	return RET_OK;
}

#ifdef SORT_TEST 

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static int int_cmp_asc(void* a, void* b)
{
	return (int)a - (int)b;
}

static int int_cmp_desc(void* a, void* b)
{
	return (int)b - (int)a;
}

static void** array_create(size_t nr)
{
	size_t i;
	int* data = (int*)malloc(sizeof(int) * nr);

	for(i = 0; i < nr; i++)
	{
		data[i] = rand();
	}

	return (void**)data;
}

static void sort_test_asc(SortFunc sort, size_t nr)
{
	size_t i;
	void** data = array_create(nr);

	if (data != NULL)
	{
		assert(sort(data, nr, int_cmp_asc) == RET_OK);

		for(i = 0; (i + 2) < nr; i++)
		{
			assert(data[i] <= data[i+1]);
		}

		free(data);
	}

}

static void sort_test_desc(SortFunc sort, size_t nr)
{
	size_t i;
	void** data = array_create(nr);

	if (data != NULL)
	{
		assert(sort(data, nr, int_cmp_desc) == RET_OK);
	
		for(i = 0; (i + 2) < nr; i++)
		{
			assert(data[i] >= data[i+1]);
		}

		free(data);
	}
}
	

static void sort_test(SortFunc sort)
{
	size_t i;

	for(i = 0; i < 1000; i++)
	{
		sort_test_asc(sort, i);
		sort_test_desc(sort, i);
	}
}

int main(void)
{
	printf("sort test begin.\n");

	sort_test(bubble_sort);

	printf("sort test successful.\n");

	return 0;
}

#endif
