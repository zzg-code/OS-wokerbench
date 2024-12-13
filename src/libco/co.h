//
// Created by 24213 on 2024/12/12.
//

#ifndef OS_WOKERBENCH_CO_H
#define OS_WOKERBENCH_CO_H
#include "shared.h"
struct co* co_start(const char *name, void (*func)(void *), void *arg);
void co_yield();
void co_wait(struct co *co);


#endif //OS_WOKERBENCH_CO_H
