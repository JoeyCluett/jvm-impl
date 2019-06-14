#pragma once

#include "ConstantPool.h"
#include "BinaryFileReader.h"

struct attribute_info {

    uint16_t attribute_name_index;
    uint32_t attribute_length;

    attribute_info(uint16_t index, uint32_t len) {
        this->attribute_name_index = index;
        this->attribute_length = len;
    }

};

// convenience function
void place_attribute_info(BinaryFileReader& bfr, attribute_info*&);

struct ConstantValue_attribute : public attribute_info {
    
    uint16_t constantvalue_index;

    ConstantValue_attribute(BinaryFileReader& bfr, uint16_t attribute_name_index, uint32_t attribute_length) 
            : attribute_info(attribute_name_index, attribute_length) {

        this->constantvalue_index = bfr.read_u16();
    }

};

struct exception_table_entry {
    uint16_t start_pc;
    uint16_t end_pc;
    uint16_t handler_pc;
    uint16_t catch_type;
};

struct Code_attribute : public attribute_info {

    uint16_t max_stack;
    uint16_t max_locals;
    uint32_t code_length;
    uint8_t* code;
    uint16_t exception_table_length;
    exception_table_entry* exception_table; 
    uint16_t attributes_count;
    attribute_info** attributes;

    Code_attribute(BinaryFileReader& bfr, uint16_t attribute_name_index, uint32_t attribute_length) 
            : attribute_info(attribute_name_index, attribute_length) {
    
        this->max_stack = bfr.read_u16();
        this->max_locals = bfr.read_u16();

        this->code_length = bfr.read_u32();
        this->code = new uint8_t[this->code_length];
        bfr.read_buffer(reinterpret_cast<char*>(this->code), this->code_length);

        this->exception_table_length = bfr.read_u16();
        this->exception_table = new exception_table_entry[this->exception_table_length];
        for(int i = 0; i < this->exception_table_length; i++) {
            this->exception_table[i].start_pc = bfr.read_u16();
            this->exception_table[i].end_pc = bfr.read_u16();
            this->exception_table[i].handler_pc = bfr.read_u16();
            this->exception_table[i].catch_type = bfr.read_u16();
        }

        this->attributes_count = bfr.read_u16();
        this->attributes = new attribute_info*[attributes_count];

        for(int i = 0; i < this->attributes_count; i++)
            ::place_attribute_info(bfr, this->attributes[i]);

    }

};

struct verification_type_info {

    uint8_t tag;

    union {
        struct {
            uint16_t cpool_index;
        } Object_variable_info;

        struct {
            uint16_t offset;
        } Uninitialized_variable_info;

    };

};



struct StackMapTable_attribute : public attribute_info {



    StackMapTable_attribute(BinaryFileReader& bfr, uint16_t attribute_name_index, uint32_t attribute_length) 
            : attribute_info(attribute_name_index, attribute_length) {

    }
};

void place_attribute_info(BinaryFileReader& bfr, attribute_info*& ptr_ref) {
    uint16_t tmp_name_index = bfr.read_u16();
    uint32_t tmp_length = bfr.read_u32();

    auto str = ::string_of(tmp_name_index);

    if(str == "ConstantValue") {
        ptr_ref = new ConstantValue_attribute(bfr, tmp_name_index, tmp_length);
    }
    else if(str == "Code") {
        ptr_ref = new Code_attribute(bfr, tmp_name_index, tmp_length);
    }
    else {
        throw std::runtime_error("error placing attribute info: unknown string constant: " + str);
    }

}

