#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "ClassFileConstants.h"

//#include "ConstantPoolEntry.h"
#include "ConstantInfo.h"
#include "FieldInfoEntry.h"
#include "AttributeInfoEntry.h"
#include "MethodInfoEntry.h"

class BinaryFileReader {
private:
    std::fstream fs;

public:

    // empty constructor
    BinaryFileReader() {}

    void open(std::string filename) {
        this->fs.open(filename, std::ios::binary | std::ios::in);
    }

    uint64_t read_u64(void) {
        union {
            char buf[8];
            uint64_t u64;
        };

        fs.read(buf, 8);

        // byte swap begin
        {
            char c = buf[0];
            buf[0] = buf[7];
            buf[7] = c;

            c = buf[1];
            buf[1] = buf[6];
            buf[6] = c;
        
            c = buf[2];
            buf[2] = buf[5];
            buf[5] = c;

            c = buf[3];
            buf[3] = buf[4];
            buf[4] = c;

        }

        return u64;
    }

    uint32_t read_u32(void) {
        union {
            char buf[4];
            uint32_t u32;
        };

        fs.read(buf, 4);

        // byte swap because class files use big endian format
        {
            char c = buf[0];
            buf[0] = buf[3];
            buf[3] = c;

            c = buf[1];
            buf[1] = buf[2];
            buf[2] = c;
        }

        return u32;
    }

    uint16_t read_u16(void) {
        union {
            char buf[2];
            uint16_t u16;
        };

        fs.read(buf, 2);

        {
            // byte swap...class files are stupid
            char c = buf[0];
            buf[0] = buf[1];
            buf[1] = c;
        }

        return u16;
    }

    uint8_t read_u8(void) {
        union
        {
            char buf;
            uint8_t u8;
        };
        
        fs.read(&buf, 1);
        return u8;
    }

    void read_buffer(char* buf, int len) {
        fs.read(buf, len);
    }

};

class ClassFile {
private:

    BinaryFileReader bfr;
    uint16_t minor_version, major_version;

    uint16_t        constant_pool_count;
    cp_info*        constant_pool = NULL;
    uint16_t        access_flags;
    uint16_t        this_class;
    uint16_t        super_class;
    uint16_t        interfaces_count;
    uint16_t*       interfaces = NULL;
    uint16_t        fields_count;
    field_info*     fields = NULL;
    uint16_t        methods_count;
    method_info*    methods = NULL;
    uint16_t        attributes_count;
    attribute_info* attributes = NULL;

    void print_constant_pool(void) {

        auto pad_number = [](int i, int len) -> std::string {
            auto str = std::to_string(i);
            while(str.size() < len)
                str = str + " ";
            return str;
        };

        for(int i = 0; i < constant_pool_count-1; i++) {
            std::cout << pad_number(i, 5);
            cp_info& el = constant_pool[i];
            switch(el.tag) {
                case CONSTANT_Utf8:
                    std::cout << " CONSTANT_Utf8\n" 
                    << *static_cast<CONSTANT_Utf8_info*>(el.info) 
                    << std::endl << std::flush;
                    break;
                case CONSTANT_Integer:
                    std::cout << " CONSTANT_Integer\n" << std::flush; 
                    break;
                case CONSTANT_Float:
                    std::cout << " CONSTANT_Float\n" << std::flush; 
                    break;
                case CONSTANT_Long:
                    std::cout << " CONSTANT_Long\n" << std::flush; 
                    break;
                case CONSTANT_Double:
                    std::cout << " CONSTANT_Double\n" << std::flush;
                    break;
                case CONSTANT_Class:
                    std::cout << " CONSTANT_Class\n" 
                    << *static_cast<CONSTANT_Class_info*>(el.info) 
                    << std::endl << std::flush;
                    break;
                case CONSTANT_String:
                    std::cout << " CONSTANT_String\n" << std::flush;
                    break;
                case CONSTANT_FieldRef:
                    std::cout << " CONSTANT_FieldRef\n" 
                    << *static_cast<CONSTANT_Fieldref_info*>(el.info) 
                    << std::endl << std::flush;
                    break;
                case CONSTANT_MethodRef:
                    std::cout << " CONSTANT_MethodRef\n" 
                    << *static_cast<CONSTANT_Methodref_info*>(el.info) 
                    << std::endl <<  std::flush;
                    break;
                case CONSTANT_InterfaceMethodref:
                    std::cout << " CONSTANT_InterfaceMethodRef\n" 
                    << *static_cast<CONSTANT_InterfaceMethodref_info*>(el.info)
                    << std::endl << std::flush; 
                    break;
                case CONSTANT_NameAndType:
                    std::cout << " CONSTANT_NameAndType\n"
                    << *static_cast<CONSTANT_NameAndType_info*>(el.info) 
                    << std::endl << std::flush;
                    break;
                case CONSTANT_MethodHandle:
                    std::cout << " CONSTANT_MethodHandle\n" << std::flush; 
                    break;
                case CONSTANT_MethodType:
                    std::cout << " CONSTANT_MethodType\n" << std::flush; 
                    break;
                case CONSTANT_InvokeDynamic:
                    std::cout << " CONSTANT_InvokeDynamic\n" << std::flush; 
                    break;
                default:
                    throw std::runtime_error("Unknown tag in print_constant_pool");
            }
        }

        std::cout << std::endl << std::flush;

    }

    void read_constant_pool(void) {
        for(int i = 0; i < constant_pool_count-1; i++) {
            cp_info& el = constant_pool[i];
            el.tag = this->bfr.read_u8();
            switch(el.tag) {
                case CONSTANT_Utf8:
                    {
                        auto cn = new CONSTANT_Utf8_info;
                        cn->length = this->bfr.read_u16();
                        cn->bytes = new uint8_t[cn->length];
                        this->bfr.read_buffer(
                            reinterpret_cast<char*>(cn->bytes), 
                            cn->length);
                        el.info = cn;
                    }
                    break;
                case CONSTANT_Integer:
                    {
                        auto cn = new CONSTANT_Integer_info;
                        union {
                            uint32_t u32;
                            int32_t i32;
                        };
                        u32 = this->bfr.read_u32();
                        cn->bytes = i32;
                        el.info = cn;
                    }
                    break;
                case CONSTANT_Float:
                    {
                        auto cn = new CONSTANT_Float_info;
                        union {
                            uint32_t u32;
                            float f32;
                        };
                        u32 = this->bfr.read_u32();
                        cn->bytes = f32;
                        el.info = cn;
                    }
                    break;
                case CONSTANT_Long:
                    {
                        auto cn = new CONSTANT_Long_info;
                        uint64_t u64 = this->bfr.read_u64();
                        cn->high_bytes = (u64 >> 32) & 0xFFFFFFFF;
                        cn->low_bytes = u64 & 0xFFFFFFFF;
                        el.info = cn;
                    }
                    break;
                case CONSTANT_Double:
                    {
                        auto cn = new CONSTANT_Double_info;
                        uint64_t u64 = this->bfr.read_u64();
                        cn->high_bytes = (u64 >> 32) & 0xFFFFFFFF;
                        cn->low_bytes = u64 & 0xFFFFFFFF;
                        el.info = cn;
                    }
                    break;
                case CONSTANT_Class:
                    {
                        auto cn = new CONSTANT_Class_info;
                        cn->name_index = this->bfr.read_u16();
                        el.info = cn;
                    }
                    break;
                case CONSTANT_String:
                    {
                        auto cn = new CONSTANT_String_info;
                        cn->string_index = this->bfr.read_u16();
                        el.info = cn;
                    }
                    break;
                case CONSTANT_FieldRef:
                    {
                        auto cn = new CONSTANT_Fieldref_info;
                        cn->class_index = this->bfr.read_u16();
                        cn->name_and_type_index = this->bfr.read_u16();
                        el.info = cn;
                    }
                    break;
                case CONSTANT_MethodRef:
                    {
                        auto cn = new CONSTANT_Methodref_info;
                        cn->class_index = this->bfr.read_u16();
                        cn->name_and_type_index = this->bfr.read_u16();
                        el.info = cn;
                    }
                    break;
                case CONSTANT_InterfaceMethodref:
                    {
                        auto cn = new CONSTANT_InterfaceMethodref_info;
                        cn->class_index = this->bfr.read_u16();
                        cn->name_and_type_index = this->bfr.read_u16();
                        el.info = cn;
                    }
                    break;
                case CONSTANT_NameAndType:
                    {
                        auto cn = new CONSTANT_NameAndType_info;
                        cn->name_index = this->bfr.read_u16();
                        cn->descriptor_index = this->bfr.read_u16();
                        el.info = cn;
                    }
                    break;
                case CONSTANT_MethodHandle:
                    {
                        auto cn = new CONSTANT_MethodHandle_info;
                        cn->reference_kind = this->bfr.read_u8();
                        cn->reference_index = this->bfr.read_u16();
                        el.info = cn;
                    }
                    break;
                case CONSTANT_MethodType:
                    {
                        auto cn = new CONSTANT_MethodType_info;
                        cn->descriptor_index = this->bfr.read_u16();
                        el.info = cn;
                    }
                    break;
                case CONSTANT_InvokeDynamic:
                    {
                        auto cn = new CONSTANT_InvokeDynamic_info;
                        cn->bootstrap_method_attr_index = this->bfr.read_u16();
                        cn->name_and_type_index = this->bfr.read_u16();
                        el.info = cn;
                    }
                    break;
                default:
                    throw std::runtime_error("Unknown tag in constant pool: " + std::to_string((int)el.tag));
            }
        }
    }

    void read_attribute_info_entry(attribute_info* ai) {
        ai->attribute_name_index = this->bfr.read_u16();
        ai->attribute_length     = this->bfr.read_u32();
        ai->info = new uint8_t[ai->attribute_length];
        this->bfr.read_buffer(reinterpret_cast<char*>(ai->info), ai->attribute_length);
    }

    void read_fields(void) {
        for(int i = 0; i < fields_count; i++) {
            auto& el = fields[i];

            el.access_flags     = this->bfr.read_u16();
            el.name_index       = this->bfr.read_u16();
            el.descriptor_index = this->bfr.read_u16();

            el.attributes_count = this->bfr.read_u16();
            el.attributes = new attribute_info[el.attributes_count];
            for(int j = 0; j < el.attributes_count; j++)
                this->read_attribute_info_entry(el.attributes + j);
        }
    }

    void read_methods(void) {

    }

public:

    ClassFile(std::string filename) {
        this->bfr.open(filename);
    }

    // release all of the dynamically released memory
    ~ClassFile(void) {
        if(constant_pool)
            delete[] constant_pool;

        if(interfaces)
            delete[] interfaces;

        if(fields)
            delete[] fields;

        if(methods)
            delete[] methods;

        if(attributes)
            delete[] attributes;

    }

    void load_class(void) {
        auto magic = this->bfr.read_u32();
        if(magic != 0xCAFEBABE) {
            std::cerr << "Magic number: " << std::hex << magic << std::endl << std::flush;
            throw std::runtime_error("Error: Magic number is not 0xCAFEBABE");
        }

        minor_version = this->bfr.read_u16();
        major_version = this->bfr.read_u16();
        std::cout << "Version: " << major_version << "." << minor_version << std::endl;

        constant_pool_count = this->bfr.read_u16();
        std::cout << "Allocating space for " << constant_pool_count-1 << " constant entries\n";
        constant_pool = new cp_info[constant_pool_count-1];

        this->read_constant_pool();
        CONSTANT_info::constant_pool = this->constant_pool;
        this->print_constant_pool();

        access_flags     = this->bfr.read_u16();
        this_class       = this->bfr.read_u16();
        super_class      = this->bfr.read_u16();
        interfaces_count = this->bfr.read_u16();

        std::cout << "Allocating space for " << interfaces_count << " interface entries\n";
        interfaces       = new uint16_t[interfaces_count];

        // need to grab the interfaces from the class file
        for(int i = 0; i < interfaces_count; i++)
            interfaces[i] = this->bfr.read_u16();

        {
            std::cout << "Interfaces: \n";
            for(int i = 0; i < interfaces_count; i++) {
                std::cout << " >> " << interfaces[i] << std::endl;
            }
        }

        fields_count     = this->bfr.read_u16();
        std::cout << "Allocating space for " << fields_count << " field entries\n";
        fields           = new field_info[fields_count];

        // need to grab the fields from the class file
        this->read_fields();

        {
            std::cout << "Fields:\n";
            for(int i = 0; i < fields_count; i++) {

            }
        }

        methods_count    = this->bfr.read_u16();
        std::cout << "Allocating space for " << methods_count << " method entries\n";
        methods          = new method_info[methods_count];

        return;

        // need to grab the methods from the class file


        attributes_count = this->bfr.read_u16();
        attributes       = new attribute_info[attributes_count];

        // need to grab the attributes from the class file
        for(int i = 0; i < attributes_count; i++)
            this->read_attribute_info_entry(this->attributes + i);

    }

};

