#pragma once

#include <iostream>
#include "BinaryFileReader.h"
#include "Enums.h"

auto pad_left(std::string str, int len) -> std::string {
    while(str.size() < len)
        str = " " + str;
    return str;
}

auto pad_right(std::string str, int len) -> std::string {
    while(str.size() < len)
        str = str + " ";
    return str;
}

#define PR_SPC 20

struct cp_info {
    CONSTANT tag;

    cp_info(void) { }
    ~cp_info(void) { }

};

cp_info** global_constant_pool = NULL;

// helper function to get strings
std::string string_of(int index);

struct CONSTANT_Class_info : public cp_info {

    uint16_t name_index;

    CONSTANT_Class_info(BinaryFileReader& bfr) : cp_info() {
        this->tag = CONSTANT::Class;
        this->name_index = bfr.read_u16();
    }

    friend std::ostream& operator<<(std::ostream& os, CONSTANT_Class_info& cinfo) {

        os << pad_right("Class", PR_SPC);
        os << pad_right("#" + std::to_string(cinfo.name_index), 12) << "// ";
        os << ::string_of(cinfo.name_index) << std::endl << std::flush;

        return os;
    }

};

struct CONSTANT_Fieldref_info : public cp_info {

    uint16_t class_index;
    uint16_t name_and_type_index;

    CONSTANT_Fieldref_info(BinaryFileReader& bfr) : cp_info() {
        this->tag = CONSTANT::FieldRef;
        this->class_index = bfr.read_u16();
        this->name_and_type_index = bfr.read_u16();
    }

    friend std::ostream& operator<<(std::ostream& os, CONSTANT_Fieldref_info& cinfo) {

        os << pad_right("Fieldref", PR_SPC);
        os << pad_right(
                "#" + 
                std::to_string(cinfo.class_index) + 
                ".#" + 
                std::to_string(cinfo.name_and_type_index), 12) << std::flush;
        os << "// " << ::string_of(cinfo.class_index) << '.' << ::string_of(cinfo.name_and_type_index) << "\n" << std::flush;


        return os;
    }

};

struct CONSTANT_Methodref_info : public cp_info {

    uint16_t class_index;
    uint16_t name_and_type_index;

    CONSTANT_Methodref_info(BinaryFileReader& bfr) : cp_info() {
        this->tag = CONSTANT::MethodRef;
        this->class_index = bfr.read_u16();
        this->name_and_type_index = bfr.read_u16();
    }

    friend std::ostream& operator<<(std::ostream& os, CONSTANT_Methodref_info& cinfo) {

        os << pad_right("Methodref", PR_SPC);
        os << pad_right(
                "#" + 
                std::to_string(cinfo.class_index) + 
                ".#" + 
                std::to_string(cinfo.name_and_type_index), 12) << std::flush;
        os << "// " << ::string_of(cinfo.class_index) << '.' << ::string_of(cinfo.name_and_type_index) << "\n" << std::flush;

        return os;
    }

};

struct CONSTANT_InterfaceMethodref_info : public cp_info {

    uint16_t class_index;
    uint16_t name_and_type_index;

    CONSTANT_InterfaceMethodref_info(BinaryFileReader& bfr) : cp_info() {
        this->tag = CONSTANT::InterfaceMethodRef;
        this->class_index = bfr.read_u16();
        this->name_and_type_index = bfr.read_u16();
    }

    friend std::ostream& operator<<(std::ostream& os, CONSTANT_InterfaceMethodref_info& cinfo) {

        // os << pad_right("InterfaceMethodref", PR_SPC);
        // os << "#" << cinfo.class_index << ".#" << cinfo.name_and_type_index << "\n" << std::flush;

        os << pad_right("InterfaceMethodref", PR_SPC);
        os << pad_right(
                "#" + 
                std::to_string(cinfo.class_index) + 
                ".#" + 
                std::to_string(cinfo.name_and_type_index), 12) << std::flush;
        os << "// " << ::string_of(cinfo.class_index) << '.' << ::string_of(cinfo.name_and_type_index) << "\n" << std::flush;


        return os;
    }

};

struct CONSTANT_String_info : public cp_info {

    uint16_t string_index;

    CONSTANT_String_info(BinaryFileReader& bfr) : cp_info() {
        this->tag = CONSTANT::String;
        this->string_index = bfr.read_u16();
    }

    friend std::ostream& operator<<(std::ostream& os, CONSTANT_String_info& cinfo) {

        os << pad_right("String", PR_SPC);
        os 
            << pad_right("#" + std::to_string(cinfo.string_index), 12) 
            << "// \"" << ::string_of(cinfo.string_index) << "\"\n" << std::flush;

        return os;
    }

};

struct CONSTANT_Integer_info : public cp_info {

    int bytes;

    CONSTANT_Integer_info(BinaryFileReader& bfr) : cp_info() {
        this->tag = CONSTANT::Integer;
        this->bytes = bfr.read_i32();
    }

    std::string to_str(void) {
        return std::to_string(this->bytes);
    }

    friend std::ostream& operator<<(std::ostream& os, CONSTANT_Integer_info& cinfo) {

        os << pad_right("Integer", PR_SPC);
        os  << cinfo.bytes << "\n" << std::flush;

        return os;
    }

};

struct CONSTANT_Float_info : public cp_info {

    float bytes;

    CONSTANT_Float_info(BinaryFileReader& bfr) : cp_info() {
        this->tag = CONSTANT::Float;
        this->bytes = bfr.read_f32();
    }

    std::string to_str(void) {
        return std::to_string(this->bytes);
    }

    friend std::ostream& operator<<(std::ostream& os, CONSTANT_Float_info& cinfo) {

        os << pad_right("Float", PR_SPC);
        os << cinfo.bytes << "f\n" << std::flush;

        return os;
    }

};

struct CONSTANT_Long_info : public cp_info {

    long int bytes;

    CONSTANT_Long_info(BinaryFileReader& bfr) : cp_info() {
        this->tag = CONSTANT::Long;
        this->bytes = bfr.read_i64();
    }

    std::string to_str(void) {
        return std::to_string(this->bytes);
    }

    friend std::ostream& operator<<(std::ostream& os, CONSTANT_Long_info& cinfo) {

        os << pad_right("Long", PR_SPC) << cinfo.bytes << "L\n" << std::flush;

        return os;
    }

};

struct CONSTANT_Double_info : public cp_info {

    double bytes;

    CONSTANT_Double_info(BinaryFileReader& bfr) : cp_info() {
        this->tag = CONSTANT::Double;
        this->bytes = bfr.read_f64();
    }

    std::string to_str(void) {
        return std::to_string(bytes);
    }

    friend std::ostream& operator<<(std::ostream& os, CONSTANT_Double_info& cinfo) {
        os << pad_right("Double", PR_SPC) << cinfo.bytes << "\n" << std::flush;
        return os;
    }

};

struct CONSTANT_NameAndType_info : public cp_info {

    uint16_t name_index;
    uint16_t descriptor_index;

    CONSTANT_NameAndType_info(BinaryFileReader& bfr) : cp_info() {
        this->tag = CONSTANT::NameAndType;
        this->name_index = bfr.read_u16();
        this->descriptor_index = bfr.read_u16();
    }

    friend std::ostream& operator<<(std::ostream& os, CONSTANT_NameAndType_info& cinfo) {

        //os << pad_right("NameAndType", PR_SPC); 
        //os << "#" << cinfo.name_index << ".#" << cinfo.descriptor_index << "\n" << std::flush;

        os << pad_right("NameAndType", PR_SPC);
        os << pad_right(
                "#" + 
                std::to_string(cinfo.name_index) + 
                ":#" + 
                std::to_string(cinfo.descriptor_index), 12) << std::flush;
        os << "// " << ::string_of(cinfo.name_index) << ':' << ::string_of(cinfo.descriptor_index) << "\n" << std::flush;

        return os;
    }

};

struct CONSTANT_Utf8_info : public cp_info {

    uint16_t length;
    uint8_t* bytes = NULL;

    CONSTANT_Utf8_info(BinaryFileReader& bfr) : cp_info() {
        this->tag = CONSTANT::Utf8;

        this->length = bfr.read_u16();
        bytes = new uint8_t[this->length];
        bfr.read_buffer(reinterpret_cast<char*>(this->bytes), this->length);
    }

    ~CONSTANT_Utf8_info(void) {
        delete[] bytes; // remember to delete dynamically allocated memory
    }

    std::string to_str(void) {
        return std::string{bytes, bytes + length};
    }

    friend std::ostream& operator<<(std::ostream& os, CONSTANT_Utf8_info& cinfo) {

        os << pad_right("Utf8", PR_SPC) << cinfo.to_str() << "\n" << std::flush;

        return os;
    }

};

struct CONSTANT_MethodHandle_info : public cp_info {

    uint8_t reference_kind;
    uint16_t reference_index;

    CONSTANT_MethodHandle_info(BinaryFileReader& bfr) : cp_info() {
        this->tag = CONSTANT::MethodHandle;
        this->reference_kind = bfr.read_u8();
        this->reference_index = bfr.read_u16();
    }

    friend std::ostream& operator<<(std::ostream& os, CONSTANT_MethodHandle_info& cinfo) {

        os 
            << pad_right("MethodHandle", PR_SPC) 
            << pad_right(
                "#" + std::to_string(cinfo.reference_kind) + 
                ".#" + std::to_string(cinfo.reference_index), 12) 
            << "// " <<  pad_right(REF_flag_string(static_cast<REF>(cinfo.reference_kind)), 19) << "   " << ::string_of(cinfo.reference_index) << "\n" << std::flush;

        return os;
    }

};

struct CONSTANT_MethodType_info : public cp_info {

    uint16_t descriptor_index;

    CONSTANT_MethodType_info(BinaryFileReader& bfr) : cp_info() {
        this->tag = CONSTANT::MethodType;
        this->descriptor_index = bfr.read_u16();
    }

    friend std::ostream& operator<<(std::ostream& os, CONSTANT_MethodType_info& cinfo) {

        os 
            << pad_right("MethodType", PR_SPC) 
            << pad_right("#" + std::to_string(cinfo.descriptor_index), 12) 
            << ::string_of(cinfo.descriptor_index) << "\n" << std::flush;

        return os;
    }

};

struct CONSTANT_InvokeDynamic_info : public cp_info {

    uint16_t bootstrap_method_attr_index;
    uint16_t name_and_type_index;

    CONSTANT_InvokeDynamic_info(BinaryFileReader& bfr) : cp_info() {
        this->tag = CONSTANT::InvokeDynamic;
        this->bootstrap_method_attr_index = bfr.read_u16();
        this->name_and_type_index = bfr.read_u16();
    }

    friend std::ostream& operator<<(std::ostream& os, CONSTANT_InvokeDynamic_info& cinfo) {

        os << pad_right("InvokeDynamic", PR_SPC) << 
            pad_right(
                "#" + 
                std::to_string(cinfo.bootstrap_method_attr_index) + 
                ".#" + 
                std::to_string(cinfo.name_and_type_index), 12) << std::flush;

        os 
            << "// <bootstrap_method_array_index>." 
            << ::string_of(cinfo.name_and_type_index) << '\n' << std::flush;

        return os;
    }

};

std::string string_of(int index) {
    cp_info* tmp = global_constant_pool[index-1];

    switch(tmp->tag) {
        case CONSTANT::Utf8:
            return static_cast<CONSTANT_Utf8_info*>(tmp)->to_str();
            break;
        case CONSTANT::Integer:
            return static_cast<CONSTANT_Integer_info*>(tmp)->to_str(); 
            break;
        case CONSTANT::Float:
            return static_cast<CONSTANT_Float_info*>(tmp)->to_str();
            break;
        case CONSTANT::Long:
            return static_cast<CONSTANT_Long_info*>(tmp)->to_str();
            break;
        case CONSTANT::Double:
            return static_cast<CONSTANT_Double_info*>(tmp)->to_str();
            break;
        case CONSTANT::Class:
            // recursive function calls ftw
            return ::string_of(static_cast<CONSTANT_Class_info*>(tmp)->name_index);
            break;
        case CONSTANT::String:
            return ::string_of(static_cast<CONSTANT_String_info*>(tmp)->string_index);
            break;
        case CONSTANT::FieldRef:
            return 
                (::string_of(static_cast<CONSTANT_Fieldref_info*>(tmp)->class_index)
                + "."
                + ::string_of(static_cast<CONSTANT_Fieldref_info*>(tmp)->name_and_type_index));
            break;
        case CONSTANT::MethodRef:
            return 
                (::string_of(static_cast<CONSTANT_Methodref_info*>(tmp)->class_index)
                + "."
                + ::string_of(static_cast<CONSTANT_Methodref_info*>(tmp)->name_and_type_index));
            break;
        case CONSTANT::InterfaceMethodRef:
            return 
                (::string_of(static_cast<CONSTANT_InterfaceMethodref_info*>(tmp)->class_index)
                + "."
                + ::string_of(static_cast<CONSTANT_InterfaceMethodref_info*>(tmp)->name_and_type_index));
            break;
        case CONSTANT::NameAndType:
            return 
                (::string_of(static_cast<CONSTANT_NameAndType_info*>(tmp)->name_index)
                + ":"
                + ::string_of(static_cast<CONSTANT_NameAndType_info*>(tmp)->descriptor_index));
            break;
        case CONSTANT::MethodHandle:
            return 
                (pad_right(REF_flag_string(static_cast<REF>(static_cast<CONSTANT_MethodHandle_info*>(tmp)->reference_kind)), 19) 
                + "   " + ::string_of(static_cast<CONSTANT_MethodHandle_info*>(tmp)->reference_index));
            break;
        case CONSTANT::MethodType:
            return ::string_of(static_cast<CONSTANT_MethodType_info*>(tmp)->descriptor_index);
            break;
        case CONSTANT::InvokeDynamic:
            return 
                ("<bootstrap_method_array_index>." 
                + ::string_of(static_cast<CONSTANT_InvokeDynamic_info*>(tmp)->name_and_type_index));
            break;
        default:
            throw std::runtime_error("Unknown tag in string_of " + std::to_string(static_cast<int>(tmp->tag)));
    };

}
