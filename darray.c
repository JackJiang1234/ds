#include <stdlib.h>
#include "darray.h"

struct _DArray
{
	void** data;
	size_t size;
	size_t alloc_size;

	DataDestroyFunc data_destroy;
	void* data_destroy_ctx; 
};

#define MIN_PRE_ALLOCATE_NR 10
static Ret darray_expand(DArray* thiz, size_t need)
{
	if ((thiz->size + need) > thiz->alloc_size);
	{
		size_t alloc_size = thiz->alloc_size + (thiz->alloc_size >> 1) + MIN_PRE_ALLOCATE_NR;

		void** data = (void**)realloc(thiz->data, sizeof(void*) * alloc_size);
		if (data != NULL)
		{
			thiz->data = data;
			thiz->alloc_size = alloc_size;
		}
	}

	return ((thiz->size + need) <= thiz->alloc_size) ? RET_OK : RET_FAIL;
}

static Ret darray_shrink(DArray* thiz)
{
	if (thiz->size < (thiz->alloc_size >> 1) && (thiz->alloc_size > MIN_PRE_ALLOCATE_NR))
	{
		size_t alloc_size = thiz->size + (thiz->size >> 1);

		void** data = (void**)realloc(thiz->data, sizeof(void*) * alloc_size);
		if (data != NULL)
		{
			thiz->data = data;
			thiz->alloc_size = alloc_size;
		}
	}

	return RET_OK;
}

static void darray_destroy_data(DArray* thiz, void* data)
{
	if (thiz->data_destroy != NULL)
	{
		thiz->data_destroy(thiz->data_destroy_ctx, data);
	}
}

DArray* darray_create(DataDestroyFunc data_destroy, void* ctx)
{
	DArray* thiz = (DArray*)malloc(sizeof(DArray));
	
	if (thiz != NULL)
	{
		thiz->data = NULL;
		thiz->size = 0;
		thiz->alloc_size = 0;
		thiz->data_destroy = data_destroy;
		thiz->data_destroy_ctx = ctx;
	}

	return thiz;
}

Ret darray_insert(DArray* thiz, size_t index, void* data)
{
	Ret ret = RET_OOM;
	size_t cursor = index;

	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS);

	cursor = cursor < thiz->size ? cursor : thiz->size;

	if (darray_expand(thiz, 1) == RET_OK)
	{
		size_t i = 0;
		for(i = thiz->size; i > cursor; i--)
		{
			thiz->data[i] = thiz->data[i-1];
		}
		thiz->data[cursor] = data;
		thiz->size++;

		ret = RET_OK;
	}

	return ret;
}

Ret darray_prepend(DArray* thiz, void* data)
{
	return darray_insert(thiz, 0, data);
}

Ret darray_append(DArray* thiz, void* data)
{
	return darray_insert(thiz, -1, data);
}

Ret darray_delete(DArray* thiz, size_t index)
{
	size_t i;

	return_val_if_fail((thiz != NULL) && (index < darray_length(thiz)), RET_INVALID_PARAMS);
	
	darray_destroy_data(thiz, thiz->data[index]);
	for(i = index; (i + 1) < thiz->size; i++)
	{
		thiz->data[i] = thiz->data[i+1];
	}
	thiz->size--;
	
	darray_shrink(thiz);

	return RET_OK;
}

Ret darray_get_by_index(DArray* thiz, size_t index, void** data)
{
	return_val_if_fail((thiz != NULL) && (index < darray_length(thiz)) && (data != NULL), RET_INVALID_PARAMS);

	*data = thiz->data[index];

	return RET_OK;
}

Ret darray_set_by_index(DArray* thiz, size_t index, void* data)
{
	return_val_if_fail((thiz != NULL) && (index < darray_length(thiz)), RET_INVALID_PARAMS);

	thiz->data[index] = data;

	return RET_OK;
}

size_t darray_length(DArray* thiz)
{
	return_val_if_fail(thiz != NULL, 0);
	
	return thiz->size;
}

int darray_find(DArray* thiz, CompareFunc cmp, void* ctx)
{
	int index = -1;
	size_t cursor;
	
	return_val_if_fail((thiz != NULL) && (cmp != NULL), index);

	for(cursor = 0; cursor < thiz->size; cursor++)
	{
		if(cmp(ctx, thiz->data[cursor]) == 0)
		{
			index = cursor;
			break;
		}
	}

	return index;
}

Ret darray_foreach(DArray* thiz, VisitFunc visit, void* ctx)
{
	size_t cursor; 
	Ret ret = RET_OK;
	
	return_val_if_fail((thiz != NULL) && (visit != NULL), RET_INVALID_PARAMS);

	for(cursor = 0; cursor < thiz->size; cursor++)
	{
		if ((ret = visit(ctx, thiz->data[cursor])) == RET_STOP)
		{
			break;
		}
	}

	return ret;
}

void darray_destroy(DArray* thiz)
{
	size_t cursor;

	if (thiz != NULL)
	{
		for(cursor = 0; cursor < thiz->size; cursor++)
		{
			darray_destroy_data(thiz, thiz->data[cursor]);
		}

		free(thiz->data);
		thiz->data = NULL;

		free(thiz);
	}
}

#ifdef DARRAY_TEST
#include <stdio.h>
#include <assert.h>

static darray_invalid_parameter_test()
{
	DArray* thiz = darray_create(NULL, NULL);

	assert(thiz != NULL);
	assert(darray_insert(NULL, 0, NULL) == RET_INVALID_PARAMS);
	assert(darray_prepend(NULL, NULL) == RET_INVALID_PARAMS);
	assert(darray_append(NULL, NULL) == RET_INVALID_PARAMS);
	assert(darray_delete(NULL, 0) == RET_INVALID_PARAMS);
	assert(darray_get_by_index(NULL, 0, NULL) == RET_INVALID_PARAMS);
	assert(darray_set_by_index(NULL, 0, NULL) == RET_INVALID_PARAMS);
	assert(darray_length(NULL) == 0);
	assert(darray_length(thiz) == 0);
	assert(darray_find(NULL, NULL, NULL) == -1);
	assert(darray_foreach(NULL, NULL, NULL) == RET_INVALID_PARAMS);

	assert(darray_insert(thiz, 0, (void*)0) == RET_OK);
	assert(darray_length(thiz) == 1);
	assert(darray_get_by_index(thiz, 1, NULL) == RET_INVALID_PARAMS);
	assert(darray_get_by_index(thiz, 0, NULL) == RET_INVALID_PARAMS);
	assert(darray_set_by_index(thiz, 1, NULL) == RET_INVALID_PARAMS);
	assert(darray_find(thiz, NULL, NULL) == -1);
	assert(darray_foreach(thiz, NULL, NULL) == RET_INVALID_PARAMS);
	
	darray_destroy(thiz);
}

static int int_cmp(void* ctx, void* data)
{
	return (int)ctx - (int)data;
}

static Ret int_visit(void* ctx, void* data)
{
	printf("visit %d\n", (int)data);

	return RET_OK;
}

static darray_normal_test()
{
	size_t i, data;
	size_t length;
	DArray* thiz = darray_create(NULL, NULL);

	assert(thiz != NULL);
	for(i = 0; i < 100; i++)
	{
		length = darray_length(thiz);

		assert(darray_insert(thiz, i, (void*)i) == RET_OK);
		assert(darray_length(thiz) == length + 1);
		assert(darray_get_by_index(thiz, i, (void**)&data) == RET_OK);
		assert(data == i);

		assert(darray_prepend(thiz, (void*)(i+1)) == RET_OK);
		assert(darray_length(thiz) == length + 2);
		assert(darray_get_by_index(thiz, 0, (void**)&data) == RET_OK);
		assert(data == i+1);

		assert(darray_append(thiz, (void*)(i+2)) == RET_OK);
		assert(darray_length(thiz) == length + 3);
		assert(darray_get_by_index(thiz, length + 2, (void**)&data) == RET_OK);
		assert(data == i+2);
	}

	length = darray_length(thiz);
	for(i = 0; i < length; i++)
	{
		assert(darray_set_by_index(thiz, i, (void*)i) == RET_OK);
		assert(darray_find(thiz, int_cmp, (void*)i) == i);
	}

	darray_foreach(thiz, int_visit, NULL);

	for(i = 0; i < length; i++)
	{
		assert(darray_delete(thiz, 0) == RET_OK);
	}

	darray_destroy(thiz);
}

int main(void)
{
	printf("darray test begin...\n");

	darray_invalid_parameter_test();
	darray_normal_test();

	printf("darray test successful.\n");

	return 0;
}

#endif
