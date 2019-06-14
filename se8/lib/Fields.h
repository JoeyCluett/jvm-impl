#pragma once

#include <iostream>
#include "ConstantPool.h"
#include "Attribute.h"
#include "Enums.h"

struct field_info {
    
    uint16_t access_flags;
    uint16_t name_index;
    uint16_t descriptor_index;
    uint16_t attributes_count;
    attribute_info** attributes;

};

