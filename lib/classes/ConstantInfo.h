#pragma once

#include <iostream>

struct CONSTANT_info {
    ~CONSTANT_info() {}
};

struct CONSTANT_Class_info : public CONSTANT_info {
    uint16_t name_index;
};

struct CONSTANT_Fieldref_info : public CONSTANT_info {
    uint16_t class_index;
    uint16_t name_and_type_index;
};

struct CONSTANT_Methodref_info : public CONSTANT_info {
    uint16_t class_index;
    uint16_t name_and_type_index;
};

struct CONSTANT_InterfaceMethodref_info : public CONSTANT_info{
    uint16_t class_index;
    uint16_t name_and_type_index;
};

struct CONSTANT_String_info : public CONSTANT_info{
    uint16_t string_index;
};

struct CONSTANT_Integer_info : public CONSTANT_info {
    int32_t bytes; // cheating a bit on this one
};

struct CONSTANT_Float_info : public CONSTANT_info {
    float bytes;
};

struct CONSTANT_Long_info : public CONSTANT_info {
    uint32_t high_bytes;
    uint32_t low_bytes;
};

struct CONSTANT_Double_info : public CONSTANT_info {
    uint32_t high_bytes;
    uint32_t low_bytes;
};

struct CONSTANT_NameAndType_info : public CONSTANT_info {
    uint16_t name_index;
    uint16_t descriptor_index;
};

struct CONSTANT_Utf8_info : public CONSTANT_info {
    uint16_t length;
    uint8_t* bytes = NULL;

    ~CONSTANT_Utf8_info(void) {
        if(bytes)
            delete[] bytes;
    }
};

struct CONSTANT_MethodHandle_info : public CONSTANT_info {
    uint8_t reference_kind;
    uint16_t reference_index;
};

struct CONSTANT_MethodType_info : public CONSTANT_info {
    uint16_t descriptor_index;
};

struct CONSTANT_InvokeDynamic_info : public CONSTANT_info {
    uint16_t bootstrap_method_attr_index;
    uint16_t name_and_type_index;
};



