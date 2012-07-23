#include <stdio.h>
#include "typedef.h"

#ifndef DARRAY_H
#define DARRAY_H

struct _DArray;
typedef struct _DArray DArray;

DArray* darray_create(DataDestroyFunc data_destroy, void* ctx);

Ret darray_insert(DArray* thiz, size_t index, void* data);
Ret darray_prepend(DArray* thiz, void* data);
Ret darray_append(DArray* thiz, void* data);
Ret darray_delete(DArray* thiz, size_t index);
Ret darray_get_by_index(DArray* thiz, size_t index, void** data);
Ret darray_set_by_index(DArray* thiz, size_t index, void* data);
size_t darray_length(DArray* thiz);
int darray_find(DArray* thiz, CompareFunc cmp, void* ctx);
Ret darray_foreach(DArray* thiz, VisitFunc visit, void* ctx);

void darray_destroy(DArray* thiz);

#endif
