#include "locker.h"

#ifndef RW_LOCKER_H
#define RW_LOCKER_H

struct _RwLocker;
typedef struct _RwLocker RwLocker;

RwLocker* rw_locker_create(Locker* rw_locker, Locker* rd_locker);

Ret rw_locker_wrlock(RwLocker* thiz);
Ret rw_locker_rdlock(RwLocker* thiz);
Ret rw_locker_unlock(RwLocker* thiz);

void rw_locker_destroy(RwLocker* thiz);

#endif
