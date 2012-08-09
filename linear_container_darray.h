#ifndef LINEAR_CONTAINER_DARRAY_H
#define LINEAR_CONTAINER_DARRAY_H

#include "linear_container.h"

LinearContainer* linear_container_darray_create(DataDestroyFunc destroy, void* ctx);

#endif
