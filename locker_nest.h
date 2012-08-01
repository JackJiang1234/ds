#include "locker.h"

#ifndef LOCKER_NEST_H
#define LOCKER_NEST_H

typedef int (*TaskSelfFunc)(void);
Locker* locker_nest_create(Locker* real_locker, TaskSelfFunc task_self);

#endif
