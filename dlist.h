#ifndef _DLIST_H
#define _DLIST_H

#include "typedef.h"

struct _DList;
typedef struct _DList DList;

DList* dlist_create(DataDestroyFunc destroy, void* ctx);

Ret dlist_insert(DList* thiz, size_t index, void* data);
Ret dlist_prepend(DList* thiz, void* data);
Ret dlist_append(DList* thiz, void* data);
Ret dlist_delete(DList* thiz, size_t index);
Ret dlist_get_by_index(DList* thiz, size_t index, void** data);
Ret dlist_set_by_index(DList* thiz, size_t index, void* data);
size_t dlist_length(DList* thiz);
int dlist_find(DList *thiz, CompareFunc cmp, void* ctx);
Ret dlist_foreach(DList* thiz, VisitFunc visit, void* ctx);

void dlist_destroy(DList *thiz);
#endif
