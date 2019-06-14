#pragma once

#include "BinaryFileReader.h"
#include "Enums.h"
#include "ConstantPool.h"
#include "Fields.h"
#include <vector>

class ClassFile {
private:

    // used to read properly formatted binary data from class files
    BinaryFileReader bfr;

    uint32_t  magic;
    uint16_t  minor_version;
    uint16_t  major_version;
    uint16_t  constant_pool_count;
    cp_info** constant_pool;
    uint16_t  access_flags;
    uint16_t  this_class;
    uint16_t  super_class;
    uint16_t  interfaces_count;
    uint16_t* interfaces;
    uint16_t  fields_count;


    void load_preface(void) {
        this->magic = this->bfr.read_u32();

        if(this->magic != 0xCAFEBABE) {
            throw std::runtime_error("Wrong magic number. Expecting 0xCAFEBABE.");
        }

        this->minor_version = this->bfr.read_u16();
        this->major_version = this->bfr.read_u16();
    }

    void load_constant_pool(void) {
        constant_pool_count = this->bfr.read_u16();
    
        // allocate pointers for each constant
        this->constant_pool = new cp_info*[this->constant_pool_count-1];

        for(int i = 0; i < constant_pool_count-1; i++) {

            //std::cout << "#" << i+1 << std::endl << std::flush;

            CONSTANT c = static_cast<CONSTANT>(this->bfr.read_u8());
            switch(c) {
                case CONSTANT::Utf8:                this->constant_pool[i] = new CONSTANT_Utf8_info(this->bfr); break;
                case CONSTANT::Integer:             this->constant_pool[i] = new CONSTANT_Integer_info(this->bfr); break;
                case CONSTANT::Float:               this->constant_pool[i] = new CONSTANT_Float_info(this->bfr); break;
                case CONSTANT::Long:                this->constant_pool[i] = new CONSTANT_Long_info(this->bfr);   i++; break;
                case CONSTANT::Double:              this->constant_pool[i] = new CONSTANT_Double_info(this->bfr); i++; break;
                case CONSTANT::Class:               this->constant_pool[i] = new CONSTANT_Class_info(this->bfr); break;
                case CONSTANT::String:              this->constant_pool[i] = new CONSTANT_String_info(this->bfr); break;
                case CONSTANT::FieldRef:            this->constant_pool[i] = new CONSTANT_Fieldref_info(this->bfr); break;
                case CONSTANT::MethodRef:           this->constant_pool[i] = new CONSTANT_Methodref_info(this->bfr); break;
                case CONSTANT::InterfaceMethodRef:  this->constant_pool[i] = new CONSTANT_InterfaceMethodref_info(this->bfr); break;
                case CONSTANT::NameAndType:         this->constant_pool[i] = new CONSTANT_NameAndType_info(this->bfr); break;
                case CONSTANT::MethodHandle:        this->constant_pool[i] = new CONSTANT_MethodHandle_info(this->bfr); break;
                case CONSTANT::MethodType:          this->constant_pool[i] = new CONSTANT_MethodType_info(this->bfr); break;
                case CONSTANT::InvokeDynamic:       this->constant_pool[i] = new CONSTANT_InvokeDynamic_info(this->bfr); break;
                default: throw std::runtime_error("Unknown tag for constant pool entry: " 
                                        + std::to_string(static_cast<int>(c))); break;
            };

        }

    }

    // loads 'access_flags', 'this_class', and 'super_class'
    void load_misc(void) {
        this->access_flags = this->bfr.read_u16();
        this->this_class   = this->bfr.read_u16();
        this->super_class  = this->bfr.read_u16();
    }

    void load_interfaces(void) {
        this->interfaces_count = this->bfr.read_u16();
        if(this->interfaces_count) {
            this->interfaces = new uint16_t[this->interfaces_count];

            for(int i = 0; i < this->interfaces_count; i++)
                this->interfaces[i] = this->bfr.read_u16();

        } else {
            this->interfaces = NULL;
        }
    }

    void load_fields(void) {
        this->fields_count = this->bfr.read_u16();
    }

    // ====================================
    // PRINT functions
    // ====================================

    void print_preface(void) {
        std::cout << "Magic number: "
            << std::hex << this->magic << "\nClass File version: " 
            << std::dec << this->major_version << "." << this->minor_version 
            << std::endl << std::flush;

        std::cout << "Java Version:       ";
        switch(this->major_version) {
            case 48: std::cout << "Java 1.4\n"; break;
            case 49: std::cout << "Java 5\n"; break;
            case 50: std::cout << "Java 6\n"; break;
            case 51: std::cout << "Java 7\n"; break;
            case 52: std::cout << "Java 8\n"; break;
            case 53: std::cout << "Java 9\n"; break;
            default:
                std::cout << "UNKNOWN\n" << std::flush;
                throw std::runtime_error("Unknown java version used");
        }
        std::cout << std::flush;
    }

    void print_constant_pool(std::ostream& os) {
        // copy raw pointer over
        global_constant_pool = this->constant_pool;

        os << "Constant pool:\n" << std::flush;

        for(int i = 0; i < this->constant_pool_count-1; i++) {

            os << pad_left("#" + std::to_string(i+1), 7) << " = ";

            switch(this->constant_pool[i]->tag) {
                case CONSTANT::Utf8:
                    os << *static_cast<CONSTANT_Utf8_info*>(constant_pool[i]); break;
                case CONSTANT::Integer:      
                    os << *static_cast<CONSTANT_Integer_info*>(constant_pool[i]); break;
                case CONSTANT::Float:              
                    os << *static_cast<CONSTANT_Float_info*>(constant_pool[i]); break;
                case CONSTANT::Long:        
                    os << *static_cast<CONSTANT_Long_info*>(constant_pool[i]); i++; break;
                case CONSTANT::Double:             
                    os << *static_cast<CONSTANT_Double_info*>(constant_pool[i]); i++; break;
                case CONSTANT::Class:              
                    os << *static_cast<CONSTANT_Class_info*>(constant_pool[i]); break;
                case CONSTANT::String:             
                    os << *static_cast<CONSTANT_String_info*>(constant_pool[i]); break;
                case CONSTANT::FieldRef:           
                    os << *static_cast<CONSTANT_Fieldref_info*>(constant_pool[i]); break;
                case CONSTANT::MethodRef:          
                    os << *static_cast<CONSTANT_Methodref_info*>(constant_pool[i]); break;
                case CONSTANT::InterfaceMethodRef: 
                    os << *static_cast<CONSTANT_InterfaceMethodref_info*>(constant_pool[i]); break;
                case CONSTANT::NameAndType:        
                    os << *static_cast<CONSTANT_NameAndType_info*>(constant_pool[i]); break;
                case CONSTANT::MethodHandle:       
                    os << *static_cast<CONSTANT_MethodHandle_info*>(constant_pool[i]); break;
                case CONSTANT::MethodType:         
                    os << *static_cast<CONSTANT_MethodType_info*>(constant_pool[i]); break;
                case CONSTANT::InvokeDynamic:      
                    os << *static_cast<CONSTANT_InvokeDynamic_info*>(constant_pool[i]); break;
                default: throw std::runtime_error("Unknown tag for constant pool entry in print_constant_pool: " 
                                        + std::to_string(static_cast<int>(this->constant_pool[i]->tag))); break;
            };

        }
    }

    void print_access_flags(void) {

        std::cout << "Class access flags: ";

        if(this->access_flags & 0x0001)
            std::cout << "ACC_PUBLIC ";

        if(this->access_flags & 0x0010)
            std::cout << "ACC_FINAL ";

        if(this->access_flags & 0x0020)
            std::cout << "ACC_SUPER ";

        if(this->access_flags & 0x0200)
            std::cout << "ACC_INTERFACE ";

        if(this->access_flags & 0x0400)
            std::cout << "ACC_ABSTRACT ";

        if(this->access_flags & 0x1000)
            std::cout << "ACC_SYNTHETIC ";

        if(this->access_flags & 0x2000)
            std::cout << "ACC_ANNOTATION ";

        if(this->access_flags & 0x4000)
            std::cout << "ACC_ENUMERATION ";

        std::cout << std::endl << std::flush;

    }

    void print_this_class(void) {
        global_constant_pool = this->constant_pool;
        std::cout << "This class:         " << ::string_of(this->this_class) << std::endl;
    }

    void print_super_class(void) {
        std::cout << "Super class:        ";

        if(super_class) {
            std::cout << ::string_of(this->super_class) << std::endl;
        }
        else {
            std::cout << "NONE\n";
        }

        std::cout << std::flush;
    }

    void print_interfaces(void) {
        std::cout << "Interfaces:";
        for(int i = 0; i < this->interfaces_count; i++) {
            if(this->constant_pool[this->interfaces[i]-1]->tag == CONSTANT::Class) {
                // good to go
                global_constant_pool = this->constant_pool;
                std::cout << "\n    " << ::string_of(this->interfaces[i]);
            }
            else {
                throw std::runtime_error("print_interfaces : Expected constant pool entry is not CONSTANT_Class_info");
            }
        }

        if(!this->interfaces_count)
            std::cout << "         NONE";
        std::cout << std::endl << std::flush;
    }

public:

    ClassFile(void) {}

    void load_file(std::string filename) {
        this->bfr.open(filename);

        // everything related to loading the class file goes here
        this->load_preface();
        this->load_constant_pool();
        this->load_misc();

        this->bfr.close();
    }

    friend std::ostream& operator<<(std::ostream& os, ClassFile& cf) {
        
        cf.print_preface();
        cf.print_access_flags();
        cf.print_this_class();
        cf.print_super_class();
        cf.print_interfaces();
        cf.print_constant_pool(os);

        return os;
    }

};
