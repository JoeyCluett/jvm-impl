#pragma once

#include <iostream>
#include "AttributeInfoEntry.h"

struct MethodInfoEntry {

    uint16_t access_flags;
    uint16_t name_index;
    uint16_t descriptor_index;
    
    uint16_t attributes_count;
    attribute_info* attributes = NULL;

};

typedef MethodInfoEntry method_info;

