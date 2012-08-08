#ifndef LINEAR_CONTAINER_H
#define LINEAR_CONTAINER_H

#include "typedef.h"

struct _LinearContainer;
typedef struct _LinearContainer LinearContainer;

typedef Ret (*LinearContainerInsert)(LinearContainer* thiz, size_t index, void* data);
typedef Ret (*LinearContainerPrepend)(LinearContainer* thiz, void* data);
typedef Ret (*LinearContainerAppend)(LinearContainer* thiz, void* data);
typedef Ret (*LinearContainerDelete)(LinearContainer* thiz, size_t index);
typedef Ret (*LinearContainerGetByIndex)(LinearContainer* thiz, size_t index, void** data);
typedef Ret (*LinearContainerSetByIndex)(LinearContainer* thiz, size_t index, void* data);
typedef size_t (*LinearContainerLength)(LinearContainer* thiz);
typedef int (*LinearContainerFind)(LinearContainer* thiz, CompareFunc cmp, void* ctx);
typedef Ret (*LinearContainerForeach)(LinearContainer* thiz, VisitFunc visit, void* ctx);
typedef void (*LinearContainerDestroy)(LinearContainer* thiz);

struct _LinearContainer
{
    LinearContainerInsert insert;
    LinearContainerPrepend prepend;
    LinearContainerAppend append;
    LinearContainerDelete del;
    LinearContainerGetByIndex get_by_index;
    LinearContainerSetByIndex set_by_index;
    LinearContainerLength length;
    LinearContainerFind find;
    LinearContainerForeach foreach;
    LinearContainerDestroy destroy;

    char priv[0];
};

static inline Ret linear_container_insert(LinearContainer* thiz, size_t index, void* data)
{
    return_val_if_fail(thiz != NULL && thiz->insert != NULL, RET_INVALID_PARAMS);

    return thiz->insert(thiz, index, data);
}

static inline Ret linear_container_prepend(LinearContainer* thiz, void* data)
{
    return_val_if_fail(thiz != NULL && thiz->prepend != NULL, RET_INVALID_PARAMS);

    return thiz->prepend(thiz, data);
}

static inline Ret linear_container_append(LinearContainer* thiz, void* data)
{
    return_val_if_fail(thiz != NULL && thiz->append != NULL, RET_INVALID_PARAMS);

    return thiz->append(thiz, data);
}

static inline Ret linear_container_delete(LinearConatiner* thiz, size_t index)
{
    return_val_if_fail(thiz != NULL && thiz->del != NULL, RET_INVALID_PARAMS);

    return thiz->del(thiz, index);
}

static inline Ret linear_container_get_by_index(LinearContainer* thiz, size_t index, void** data)
{
    return_val_if_fail(thiz != NULL && thiz->get_by_index != NULL && data != NULL, RET_INVALID_PARAMS);

    return thiz->get_by_index(thiz, index, data);
}

static inline Ret linear_container_set_by_index(LinearContainer* thiz, size_t index, void* data)
{
    return_val_if_fail(thiz != NULL && thiz->set_by_index != NULL, RET_INVALID_PARAMS);

    return thiz->set_by_index(thiz, index, data);
}

static inline size_t linear_container_length(LinearConatiner* thiz)
{
    return_val_if_fail(thiz != NULL && thiz->length != NULL, 0);

    return thiz->length(thiz);
}

static inline int linear_contaner_find(LinearContainer* thiz, CompareFunc cmp, void* ctx)
{
    return_val_if_fail(thiz != NULL && thiz->find != NULL && cmp != NULL, -1);

    return thiz->find(thiz, cmp, ctx);
}

static inline Ret linear_container_foreach(LinearContainer* thiz, VisitFunc visit, void* ctx)
{
    return_val_if_fail(thiz != NULL && thiz->foreach != NULL && visit != NULL, RET_INVALID_PARAMS);
    
    return thiz->foreach(thiz, visit, ctx);
}

static inline void linear_container_destroy(LinearContainer* thiz)
{
    return_if_fail(thiz != NULL && thiz->destory != NULL);

    thiz->destory(thiz);

    return;
}

#endif
