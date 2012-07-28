#ifndef SORT_H
#define SORT_H

#include "typedef.h"

Ret bubble_sort(void** data, int nr, CompareFunc cmp);

Ret quick_sort(void** data, int nr, CompareFunc cmp);

Ret merge_sort(void** data, int nr, CompareFunc cmp);

Ret select_sort(void** data, int nr, CompareFunc cmp);

Ret insert_sort(void** data, int nr, CompareFunc cmp);

#endif
