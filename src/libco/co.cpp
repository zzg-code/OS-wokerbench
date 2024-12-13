//
// Created by 24213 on 2024/12/12.
//

#include "co.h"

struct  co{
    std::string name;
    std::function<void(void*)> fptr;
    void* args;

};
