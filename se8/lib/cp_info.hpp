#pragma once

#include <string>
#include <iostream>
#include <map>
#include <assert.h>
#include "BinaryFileReader.hpp"
#include "classfile_constants.h"

// constant pool info

struct cp_info {

    uint8_t tag;

    union {

        struct {
            u2 name_index;
        } CONSTANT_Class_info;

        struct {
            u2 class_index;
            u2 name_and_type_index;
        } CONSTANT_Fieldref_info;

        struct {
            u2 class_index;
            u2 name_and_type_index;
        } CONSTANT_Methodref_info;

        struct {
            u2 class_index;
            u2 name_and_type_index;
        } CONSTANT_InterfaceMethodref_info;

        struct {
            u2 string_index;
        } CONSTANT_String_info;

        struct {
            union {
                int val;                
                char bytes[4];
            };
        } CONSTANT_Integer_info;

        struct {
            union {
                float val;
                char bytes[4];
            };
        } CONSTANT_Float_info;

        struct {
            union {
                long int val;
                struct {
                    char low_bytes[4];
                    char high_bytes[4];
                };
            };
        } CONSTANT_Long_info;

        struct {
            union {
                double val;
                struct {
                    char low_bytes[4];
                    char high_bytes[4];
                };
            };
        } CONSTANT_Double_info;

        struct {
            u2 name_index;
            u2 descriptor_index;
        } CONSTANT_NameAndType_info;

        struct {
            u2 length;
            char* bytes;
        } CONSTANT_Utf8_info;

        struct {
            u2 reference_kind;
            u2 reference_index;
        } CONSTANT_MethodHandle_info;

        struct {
            u2 descriptor_index;
        } CONSTANT_MethodType_info;

        struct {
            u2 bootstrap_method_attr_index;
            u2 name_and_type_index;
        } CONSTANT_InvokeDynamic_info;

    };

    std::string string_value;

    // initialize the cp_info with data from a file reader. the tag value has already been set
    void initialize(BinaryFileReader& bfr);

    // return a printable type specifier with constant type information
    const char* type(void);

    // create a string for every entry after the constants are placed in memory
    std::string gen_string(std::map<int, cp_info>& cp_map);

    // get the string value. assumes the value has already been generated
    std::string str(void);

    // iterate through all available constants and create strings
    static void finalize(std::map<int, cp_info>& cp_map);

};

void cp_info::initialize(BinaryFileReader& bfr) {

    switch(this->tag) {
        case CONSTANT_Utf8:
            {
                u2 len = bfr.read_u16();
                this->CONSTANT_Utf8_info.bytes = new char[len];
                bfr.read_buffer(this->CONSTANT_Utf8_info.bytes, len);
                this->CONSTANT_Utf8_info.length = len;
            }
            break;
        case CONSTANT_Integer:
            this->CONSTANT_Integer_info.val = bfr.read_i32();
            break;
        case CONSTANT_Float:
            this->CONSTANT_Float_info.val = bfr.read_f32();
            break;
        case CONSTANT_Long:
            this->CONSTANT_Long_info.val = bfr.read_i64();
            break;
        case CONSTANT_Double:
            this->CONSTANT_Double_info.val = bfr.read_f64();
            break;
        case CONSTANT_Class:
            this->CONSTANT_Class_info.name_index = bfr.read_u16();
            break;
        case CONSTANT_String:
            this->CONSTANT_String_info.string_index = bfr.read_u16();
            break;
        case CONSTANT_FieldRef:
            this->CONSTANT_Fieldref_info.class_index = bfr.read_u16();
            this->CONSTANT_Fieldref_info.name_and_type_index = bfr.read_u16();
            break;
        case CONSTANT_MethodRef:
            this->CONSTANT_Methodref_info.class_index = bfr.read_u16();
            this->CONSTANT_Methodref_info.name_and_type_index = bfr.read_u16();
            break;
        case CONSTANT_InterfaceMethodref:
            this->CONSTANT_InterfaceMethodref_info.class_index = bfr.read_u16();
            this->CONSTANT_InterfaceMethodref_info.name_and_type_index = bfr.read_u16();
            break;
        case CONSTANT_NameAndType:
            this->CONSTANT_NameAndType_info.name_index = bfr.read_u16();
            this->CONSTANT_NameAndType_info.descriptor_index = bfr.read_u16();
            break;
        case CONSTANT_MethodHandle:
            this->CONSTANT_MethodHandle_info.reference_kind = bfr.read_u16();
            this->CONSTANT_MethodHandle_info.reference_index = bfr.read_u16();
            break;
        case CONSTANT_MethodType:
            this->CONSTANT_MethodType_info.descriptor_index = bfr.read_u16();
            break;
        case CONSTANT_InvokeDynamic:
            this->CONSTANT_InvokeDynamic_info.bootstrap_method_attr_index = bfr.read_u16();
            this->CONSTANT_InvokeDynamic_info.name_and_type_index = bfr.read_u16();
            break;
        default:
            throw std::runtime_error("Unknown tag in constant pool: " + std::to_string((int)this->tag));
    }
}

const char* cp_info::type(void) {
    switch(this->tag) {
        case CONSTANT_Utf8: return "Utf8"; break;
        case CONSTANT_Integer: return "Integer"; break;
        case CONSTANT_Float: return "Float";
        case CONSTANT_Long: return "Long";
        case CONSTANT_Double: return "Double";
        case CONSTANT_Class: return "Class";
        case CONSTANT_String: return "String";
        case CONSTANT_FieldRef: return "FieldRef";
        case CONSTANT_MethodRef: return "MethodRef";
        case CONSTANT_InterfaceMethodref: return "InterfaceMethodref";
        case CONSTANT_NameAndType: return "NameAndType";
        case CONSTANT_MethodHandle: return "MethodHandle";
        case CONSTANT_MethodType: return "MethodType";
        case CONSTANT_InvokeDynamic: return "InvokeDymamic";
        default:
            throw std::runtime_error("Unknown tag in constant pool: " + std::to_string((int)this->tag));
    }
}

void cp_info::finalize(std::map<int, cp_info>& cp_map) {
    for(auto& p : cp_map) {
        // cache results as they come in
        p.second.string_value = p.second.gen_string(cp_map);
    }
}

std::string cp_info::gen_string(std::map<int, cp_info>& cp_map) {
    
    // results are effectively cached by cp_info::finalize
    if(this->string_value.size() > 0)
        return this->string_value;
    
    // if the result does not already exist, create it and let cp_info::finalize cache it for us
    switch(this->tag) {
        case CONSTANT_Utf8:    
            {
                // strings eventually make their way to one of these constant types
                char* b = this->CONSTANT_Utf8_info.bytes;
                return std::string(b, b + this->CONSTANT_Utf8_info.length);
            }            
            break;
        case CONSTANT_Integer:
            return 
                std::to_string(this->CONSTANT_Integer_info.val);
            break;
        case CONSTANT_Float:
            return 
                std::to_string(this->CONSTANT_Float_info.val);
            break;
        case CONSTANT_Long:
            return 
                std::to_string(this->CONSTANT_Long_info.val);
            break;
        case CONSTANT_Double:
            return 
                std::to_string(this->CONSTANT_Double_info.val);
            break;
        case CONSTANT_Class:
            return 
                cp_map.at(this->CONSTANT_Class_info.name_index).gen_string(cp_map);
            break;
        case CONSTANT_String:
            return 
                cp_map.at(this->CONSTANT_String_info.string_index).gen_string(cp_map);
            break;
        case CONSTANT_FieldRef:
            return 
                cp_map.at(this->CONSTANT_Fieldref_info.class_index).gen_string(cp_map) + " " + 
                cp_map.at(this->CONSTANT_Fieldref_info.name_and_type_index).gen_string(cp_map);
            break;
        case CONSTANT_MethodRef:
            return 
                cp_map.at(this->CONSTANT_Methodref_info.class_index).gen_string(cp_map) + " " + 
                cp_map.at(this->CONSTANT_Methodref_info.name_and_type_index).gen_string(cp_map);
            break;
        case CONSTANT_InterfaceMethodref: 
            return 
                cp_map.at(this->CONSTANT_InterfaceMethodref_info.class_index).gen_string(cp_map) + " " + 
                cp_map.at(this->CONSTANT_InterfaceMethodref_info.name_and_type_index).gen_string(cp_map);
            break;
        case CONSTANT_NameAndType:
            return
                cp_map.at(this->CONSTANT_NameAndType_info.name_index).gen_string(cp_map) + " " + 
                cp_map.at(this->CONSTANT_NameAndType_info.descriptor_index).gen_string(cp_map);
            break;
        case CONSTANT_MethodHandle:
            return
                cp_map.at(this->CONSTANT_MethodHandle_info.reference_kind).gen_string(cp_map) + " " + 
                cp_map.at(this->CONSTANT_MethodHandle_info.reference_index).gen_string(cp_map);
            break;
        case CONSTANT_MethodType:
            return 
                cp_map.at(CONSTANT_MethodType_info.descriptor_index).gen_string(cp_map);
            break;
        case CONSTANT_InvokeDynamic:
            return
                cp_map.at(CONSTANT_InvokeDynamic_info.bootstrap_method_attr_index).gen_string(cp_map) + " " +
                cp_map.at(CONSTANT_InvokeDynamic_info.name_and_type_index).gen_string(cp_map);
            break;
        default:
            throw std::runtime_error("Unknown tag in cp_info::finalize");
    }
}

std::string cp_info::str(void) {
    return this->string_value;
}
