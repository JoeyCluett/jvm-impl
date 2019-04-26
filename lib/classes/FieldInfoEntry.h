#pragma once

#include <iostream>
#include "ClassFileConstants.h"
#include "AttributeInfoEntry.h"

struct FieldInfoEntry {
    
    uint16_t access_flags;
    uint16_t name_index;
    uint16_t descriptor_index;
    uint16_t attributes_count;



};

typedef FieldInfoEntry field_info;
