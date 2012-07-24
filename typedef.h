#ifndef _TYPEDEF_H
#define _TYPEDEF_H

#define TRUE 1
#define FALSE 0

#include <stdio.h>

typedef enum 
{
	RET_OK,
	RET_INVALID_PARAMS,
	RET_OOM,
	RET_STOP,
	RET_FAIL,
} Ret;

typedef int (*CompareFunc)(void* ctx, void* data);
typedef Ret (*VisitFunc)(void* context, void* data);
typedef void (*DataDestroyFunc)(void* ctx, void* data);
typedef Ret (*SortFunc)(void** data, size_t nr, CompareFunc cmp);

#define return_if_fail(exp) if(!(exp)) \
							{ \
								printf("%s:%d warning: " #exp " failed.\n", __func__, __LINE__); \
								return; \
							} 

#define return_val_if_fail(exp, ret) if(!(exp)) \
									 { \
										 printf("%s:%d warning:" #exp " failed.\n", __func__, __LINE__); \
										 return ret; \
									 }
#endif
 
