#pragma once

#include <iostream>
#include "ClassFileConstants.h"
#include "AttributeInfoEntry.h"

struct FieldInfoEntry {
    
    uint16_t access_flags;
    uint16_t name_index;
    uint16_t descriptor_index;
    
    uint16_t attributes_count;
    attribute_info* attributes = NULL;

    ~FieldInfoEntry(void) {
        if(attributes)
            delete[] attributes;
    }

};

typedef FieldInfoEntry field_info;
