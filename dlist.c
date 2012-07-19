#include <stdlib.h>
#include "dlist.h"

typedef struct _DListNode
{
	struct _DListNode* next;
	struct _DListNode* prev;

	void* data;

} DListNode;

struct _DList
{
	DListNode *first;

	DataDestroyFunc destroy;
	void* data_destroy_ctx;
};

static DListNode* dlist_create_node(void* data)
{
	DListNode* node = (DListNode *)malloc(sizeof(DListNode));
	if (node != NULL)
	{
		node->next = NULL;
		node->prev = NULL;
		node->data = data;
	}

	return node;
}

static void dlist_destroy_node(DList* thiz, DListNode* node)
{
	if (thiz->destroy != NULL)
	{
		thiz->destroy(thiz->data_destroy_ctx, node->data);
		free(node);
	}
}
	

static DListNode* dlist_get_node(DList* thiz, size_t index, int fail_return_last)
{
	DListNode* cursor;

	for(cursor=thiz->first; cursor!= NULL && cursor->next != NULL && index > 0; cursor=cursor->next)
	{
		index--;
	}

	if (!fail_return_last)
	{
		cursor = index > 0 ? NULL : cursor;
	}

	return cursor;
}

DList* dlist_create(DataDestroyFunc destroy, void* ctx)
{
	DList* list = (DList*)malloc(sizeof(DList));
	if (list != NULL)
	{
		list->first = NULL;
		list->destroy = destroy;
		list->data_destroy_ctx = ctx;
	}

	return list;
}

Ret dlist_insert(DList *thiz, size_t index, void* data)
{
	DListNode* new;
	DListNode* cursor;

	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS);
	return_val_if_fail(((new = dlist_create_node(data)) != NULL), RET_OOM);
	
	if (thiz->first == NULL)
	{
		thiz->first = new;
		return RET_OK;
	}

	cursor = dlist_get_node(thiz, index, 1);
	if (index < dlist_length(thiz))
	{
		if (thiz->first == cursor)
		{
			thiz->first = new;
		}
		else
		{
			cursor->prev->next = new;
			new->prev = cursor->prev;
		}

		new->next = cursor;
		cursor->prev = new;
	}
	else
	{
		new->prev = cursor;
		cursor->next = new;
	}

	return RET_OK;
}

Ret dlist_prepend(DList* thiz, void* data)
{
	return dlist_insert(thiz, 0, data);
}

Ret dlist_append(DList* thiz, void* data)
{
	return dlist_insert(thiz, -1, data);
}

Ret dlist_delete(DList* thiz, size_t index)
{
	DListNode* cursor;

	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS);
	
	cursor = dlist_get_node(thiz, index, 0);

	if (cursor != NULL)
	{
		if (cursor == thiz->first)
		{
			thiz->first = cursor->next;
		}
		if (cursor->prev != NULL)
		{
			cursor->prev->next  = cursor->next;
		}
		if (cursor->next != NULL)
		{
			cursor->next->prev = cursor->prev;
		}

		dlist_destroy_node(thiz, cursor);
	}

	return RET_OK;
}

Ret dlist_get_by_index(DList* thiz, size_t index, void** data)
{
	DListNode* cursor;

	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS);
	
	cursor = dlist_get_node(thiz, index, 0);
	if (cursor != NULL)
	{
		*data = cursor->data;	
	}

	return cursor != NULL ? RET_OK : RET_FAIL;
}

Ret dlist_set_by_index(DList* thiz, size_t index, void* data)
{
	DListNode* cursor;

	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS);

	cursor = dlist_get_node(thiz, index, 0);
	if (cursor != NULL)
	{
		cursor->data = data;
	}

	return cursor != NULL ? RET_OK : RET_FAIL;
}

size_t dlist_length(DList* thiz)
{
	DListNode* cursor;
	size_t count = 0;

	if (thiz != NULL)
	{
		for(cursor=thiz->first; cursor!=NULL; cursor=cursor->next)
		{
			count++;
		}
	}

	return count;
}

int dlist_find(DList* thiz, CompareFunc cmp, void* ctx)
{
	DListNode* cursor;
	int index = 0;
	int find = 0;
	
	if (thiz != NULL)
	{
		for(cursor=thiz->first; cursor != NULL; cursor=cursor->next)
		{
			if (cmp(ctx, cursor->data) == 0)
			{
				find = TRUE;
				break;
			}

			index++;
		}
	}

	return find ? index : -1;
}

Ret dlist_foreach(DList* thiz, VisitFunc visit, void* ctx)
{
	DListNode* cursor;
	Ret ret = RET_OK;

	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS);
	
	for(cursor=thiz->first; cursor!=NULL; cursor=cursor->next)
	{
		ret = visit(ctx, cursor->data);
		if (ret == RET_STOP)
		{
			break;
		}
	}

	return ret;
}

void dlist_destroy(DList* thiz)
{
	DListNode* cursor;
	DListNode* next;

	if (thiz != NULL)
	{
		cursor = thiz->first;
		while (cursor != NULL)
		{
			next = cursor->next;
			dlist_destroy_node(thiz, cursor);
			cursor = next;
		}

		thiz->first = NULL;
		free(thiz);
	}
}

#ifdef DLIST_TEST
#include <assert.h>
#include <stdio.h>

static int int_compare(void* a, void* b)
{
	return (int)a - (int)b;
}

static void int_destroy(void* ctx, void* data)
{
	printf("destroy: %d\n", data);
}

static Ret int_visit(void* ctx, void* data)
{
	printf("visit:%d\n", data);

	return RET_OK;
}

static void dlist_uninit_test()
{
	assert(dlist_length(NULL) == 0);
	assert(dlist_insert(NULL, 0, (void*)0) == RET_INVALID_PARAMS);
	assert(dlist_append(NULL, (void*)0) == RET_INVALID_PARAMS);
	assert(dlist_prepend(NULL, (void*)0) == RET_INVALID_PARAMS);
	assert(dlist_delete(NULL, 0) == RET_INVALID_PARAMS);
	assert(dlist_get_by_index(NULL, 0, (void**)0) == RET_INVALID_PARAMS);
	assert(dlist_set_by_index(NULL, 0, (void*)0) == RET_INVALID_PARAMS);
	assert(dlist_find(NULL, NULL, NULL) == -1);
	assert(dlist_foreach(NULL, NULL, NULL) == RET_INVALID_PARAMS);

	dlist_destroy(NULL);
}

static void dlist_init_test()
{
	void **data;

	DList* list = dlist_create(NULL, NULL);
	assert(dlist_length(list) == 0);
	assert(dlist_get_by_index(list, 0, data) == RET_FAIL);
}

static void dlist_add_one_test()
{
	DList* list = dlist_create(NULL, NULL);
	int data;
	
	assert(dlist_insert(list, 0, (void*)0) == RET_OK);
	assert(dlist_length(list) == 1);
	assert(dlist_get_by_index(list, 0, (void**)&data) == RET_OK);
	assert(data == 0);
	assert(dlist_delete(list, 0) == RET_OK);
	assert(dlist_length(list) == 0);
}

static void dlist_normal_test()
{
	DList* list = dlist_create(int_destroy, NULL);
	int i, data;
	size_t length;

	for(i=0; i<100; i++)
	{
		length = dlist_length(list);
		assert(dlist_insert(list, 0, (void*)i) == RET_OK);
		assert(length + 1 == dlist_length(list));
		
		assert(dlist_prepend(list, (void*)i) == RET_OK);
		assert(length + 2 == dlist_length(list));
		assert(dlist_get_by_index(list, 0, (void**)&data) == RET_OK);
		assert(data == i);

		assert(dlist_append(list, (void*)i) == RET_OK);
		assert(length + 3 == dlist_length(list));
		assert(dlist_get_by_index(list, length + 2, (void**)&data) == RET_OK);
		assert(data == i);

		assert(dlist_set_by_index(list, length + 2, (void*)(i+2)) == RET_OK);
		assert(dlist_get_by_index(list, length + 2, (void**)&data) == RET_OK);
		assert(data == i + 2);

		assert(dlist_find(list, int_compare, (void*)(i+2)) > 0);
	}

	dlist_foreach(list, int_visit, (void*)0);

	dlist_destroy(list);
}

int main(void)
{
	printf("--dlist test beginning--\n");
	
	dlist_uninit_test();
	dlist_init_test();
	dlist_add_one_test();
	dlist_normal_test();

	printf("--dlist test successful--\n");

	return 0;
}

#endif
