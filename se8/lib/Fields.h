#pragma once

#include <iostream>
#include "ConstantPool.h"
#include "Attributes.h"
#include "Enums.h"
#include "BinaryFileReader.h"

struct field_info {
    
    uint16_t access_flags;
    uint16_t name_index;
    uint16_t descriptor_index;
    uint16_t attributes_count;
    attribute_info** attributes;

    field_info(void) = default;

    void init(BinaryFileReader& bfr, cp_info** cp) {
        this->access_flags     = bfr.read_u16();
        this->name_index       = bfr.read_u16();

        std::cout << "\t#" << this->name_index << "{" << ::string_of(this->name_index, cp) << std::endl << std::flush;

        this->descriptor_index = bfr.read_u16();
        this->attributes_count = bfr.read_u16();

        std::cout << "\t\tdescriptor:       " << ::string_of(this->descriptor_index, cp) << std::endl << std::flush;
        std::cout << "\t\tattributes_count: " << this->attributes_count << std::endl << std::flush;

        if(this->attributes_count) {
            this->attributes = new attribute_info*[this->attributes_count];

            for(int i = 0; i < this->attributes_count; i++) {

                ::place_attribute_info(bfr, attributes[i], cp);

            }
        }

        std::cout << "\t\tDONE\n" << std::flush;

    }

    ~field_info(void) {
        // delete data associated with every pointer first
        if(this->attributes_count) {
            for(int i = 0; i < this->attributes_count; i++)
                delete this->attributes[i];

            delete[] this->attributes;
        }
    }

};

