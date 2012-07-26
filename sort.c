#include <stdlib.h>
#include "sort.h"

static void swap(void** a, void** b)
{
	void* c = *a;
	*a = *b;
	*b = c;
}

Ret bubble_sort(void** data, int nr, CompareFunc cmp)
{
	int i, j, max;

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
			swap(&data[max], &data[i]);
		}
	}

	return RET_OK;
}

static int partition(void** data, int low, int high, CompareFunc cmp)
{
	int index = low, cursor;
	void* pivot_key = data[low];
	swap(&data[low], &data[high]);

	for(cursor = low; cursor < high; cursor++)
	{
		if (cmp(data[cursor], pivot_key) < 0)
		{
			swap(&data[index++], &data[cursor]);	
		}
	}
	swap(&data[index], &data[high]);

	return index;

}

static void quick_sort_impl(void** data, int low, int high, CompareFunc cmp)
{
	if(low < high)
	{
		int pivot = partition(data, low, high, cmp);
		quick_sort_impl(data, low, pivot - 1, cmp);
		quick_sort_impl(data, pivot + 1, high, cmp);
	}
}

Ret quick_sort(void** data, int nr, CompareFunc cmp)
{
	return_val_if_fail((data != NULL) && (cmp != NULL), RET_INVALID_PARAMS);

	if (nr < 2)
	{
		return RET_OK;
	}

	quick_sort_impl(data, 0, nr-1, cmp);

	return RET_OK;
}

static Ret merge_sort_impl(void** storage, void** data, int low, int mid, int high, CompareFunc cmp)
{
	int i = low;
	int j = low;
	int k = mid;

	if ((low + 1) < mid)
	{
		int x = low + ((mid - low) >> 1);
		merge_sort_impl(storage, data, low, x, mid, cmp);
	}
	if ((mid + 1) < high)
	{
		int x = mid + ((high - mid) >> 1);
		merge_sort_impl(storage, data, mid, x, high, cmp);
	}

	while (j < mid && k < high)
	{
		if (cmp(data[j], data[k]) <= 0)
		{
			storage[i++] = data[j++];
		}
		else
		{
			storage[i++] = data[k++];
		}
	}

	while(j < mid)
	{
		storage[i++] = data[j++];
	}

	while(k < high)
	{
		storage[i++] = data[k++];
	}

	for(i = low; i < high; i++)
	{
		data[i] = storage[i];
	}

	return RET_OK;
}

Ret merge_sort(void** data, int nr, CompareFunc cmp)
{
	Ret ret = RET_OK;

	return_val_if_fail((data != NULL) && (cmp != NULL), RET_INVALID_PARAMS);

	if (nr > 1)
	{
		void** storage = (void**)malloc(sizeof(void*) * ((size_t)nr));

		if (storage != NULL)
		{
			ret = merge_sort_impl(storage, data, 0, nr>>1, nr, cmp);
			free(storage);
		}
		else
		{
			ret = RET_OOM;
		}
	}

	return ret;
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

static void** array_create(int nr)
{
	size_t i;
	int* data = (int*)malloc(sizeof(int) * nr);

	for(i = 0; i < nr; i++)
	{
		data[i] = rand();
	}

	return (void**)data;
}

static void sort_test_asc(SortFunc sort, int nr)
{
	int i;
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

static void sort_test_desc(SortFunc sort, int nr)
{
	int i;
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
	int i;

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
	sort_test(quick_sort);
	sort_test(merge_sort);

	printf("sort test successful.\n");

	return 0;
}

#endif
