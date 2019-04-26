#pragma once

#include <iostream>
#include "ClassFileConstants.h"
#include "ConstantInfo.h"

struct ConstantPoolEntry {

    uint8_t tag;
    CONSTANT_info* info;

    ~ConstantPoolEntry(void) {
        if(info)
            delete info;
    }

};

typedef ConstantPoolEntry cp_info;
