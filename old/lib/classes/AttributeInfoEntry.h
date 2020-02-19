#pragma once

#include <iostream>

struct AttributeInfoEntry {
    uint16_t attribute_name_index;
    uint32_t attribute_length;
    uint8_t* info = NULL;

    ~AttributeInfoEntry(void) {
        if(info)
            delete[] info;
    }

};

typedef AttributeInfoEntry attribute_info;
