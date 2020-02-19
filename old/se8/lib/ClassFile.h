#pragma once

#include "BinaryFileReader.h"
#include "Enums.h"
#include "ConstantPool.h"
#include "Fields.h"
#include "Methods.h"
#include "Attributes.h"
#include <vector>

#define PRINTFLAG(fvalue, enumname, os) if(flags & fvalue) os << #enumname << ' '

class ClassFile {
private:

    // used to read properly formatted binary data from class files
    BinaryFileReader bfr;

    uint32_t         magic;
    uint16_t         minor_version;
    uint16_t         major_version;
    uint16_t         constant_pool_count;
    cp_info**        constant_pool;
    uint16_t         access_flags;
    uint16_t         this_class;
    uint16_t         super_class;
    uint16_t         interfaces_count;
    uint16_t*        interfaces;
    uint16_t         fields_count;
    field_info*      fields;
    uint16_t         methods_count;
    method_info*     methods;
    uint16_t         attributes_count;
    attribute_info** attributes;

    void load_preface(void) {

        std::cout << "Loading preface..." << std::flush;

        this->magic = this->bfr.read_u32();

        if(this->magic != 0xCAFEBABE) {
            throw std::runtime_error("Wrong magic number. Expecting 0xCAFEBABE.");
        }

        this->minor_version = this->bfr.read_u16();
        this->major_version = this->bfr.read_u16();

        std::cout << "DONE\n" << std::flush;

    }

    void load_constant_pool(void) {
        std::cout << "Loading constant pool..." << std::flush;

        constant_pool_count = this->bfr.read_u16();
    
        // allocate pointers for each constant
        // after this allocation, this never changes 
        // for the life of the class file
        this->constant_pool = new cp_info*[this->constant_pool_count-1];

        for(int i = 0; i < constant_pool_count-1;) {

            //std::cout << "#" << i+1 << std::endl << std::flush;

            uint8_t tag = this->bfr.read_u8();
            //std::cout << "  Constant: " << static_cast<int>(tag) << std::endl << std::flush;
            CONSTANT c = static_cast<CONSTANT>(tag);

            switch(c) {
                case CONSTANT::Utf8:                
                    this->constant_pool[i] = new CONSTANT_Utf8_info(this->bfr); 
                    i += 1; break;

                case CONSTANT::Integer:             
                    this->constant_pool[i] = new CONSTANT_Integer_info(this->bfr); 
                    i += 1; break;
                
                case CONSTANT::Float:               
                    this->constant_pool[i] = new CONSTANT_Float_info(this->bfr); 
                    i += 1; break;
                
                case CONSTANT::Long:                
                    this->constant_pool[i]   = new CONSTANT_Long_info(this->bfr);
                    this->constant_pool[i+1] = this->constant_pool[i];   
                    i += 2; break;
                
                case CONSTANT::Double:              
                    this->constant_pool[i]   = new CONSTANT_Double_info(this->bfr); 
                    this->constant_pool[i+1] = this->constant_pool[i];
                    i += 2; break;
                
                case CONSTANT::Class:               
                    this->constant_pool[i] = new CONSTANT_Class_info(this->bfr); 
                    i += 1; break;
                
                case CONSTANT::String:              
                    this->constant_pool[i] = new CONSTANT_String_info(this->bfr); 
                    i += 1; break;
                
                case CONSTANT::FieldRef:            
                    this->constant_pool[i] = new CONSTANT_Fieldref_info(this->bfr); 
                    i += 1; break;
                
                case CONSTANT::MethodRef:           
                    this->constant_pool[i] = new CONSTANT_Methodref_info(this->bfr); 
                    i += 1; break;
                
                case CONSTANT::InterfaceMethodRef:  
                    this->constant_pool[i] = new CONSTANT_InterfaceMethodref_info(this->bfr); 
                    i += 1; break;
                
                case CONSTANT::NameAndType:         
                    this->constant_pool[i] = new CONSTANT_NameAndType_info(this->bfr); 
                    i += 1; break;
                
                case CONSTANT::MethodHandle:        
                    this->constant_pool[i] = new CONSTANT_MethodHandle_info(this->bfr); 
                    i += 1; break;
                
                case CONSTANT::MethodType:          
                    this->constant_pool[i] = new CONSTANT_MethodType_info(this->bfr); 
                    i += 1; break;
                
                case CONSTANT::InvokeDynamic:       
                    this->constant_pool[i] = new CONSTANT_InvokeDynamic_info(this->bfr); 
                    i += 1; break;
                
                default: throw std::runtime_error("Unknown tag for constant pool entry: " 
                                        + std::to_string(static_cast<int>(c))); break;
            };

            // every constant needs a reference to its pool
            //this->constant_pool[i]->cp_ptr = this->constant_pool;

        }

        for(int i = 0; i < constant_pool_count-1; i++)
            this->constant_pool[i]->cp_ptr = this->constant_pool;

        std::cout << "DONE\n" << std::flush;

    }

    // loads 'access_flags', 'this_class', and 'super_class'
    void load_misc(void) {
        this->access_flags = this->bfr.read_u16();
        this->this_class   = this->bfr.read_u16();
        this->super_class  = this->bfr.read_u16();
    }

    void load_interfaces(void) {

        std::cout << "Loading interfaces..." << std::flush;

        this->interfaces_count = this->bfr.read_u16();
        if(this->interfaces_count) {
            this->interfaces = new uint16_t[this->interfaces_count];

            for(int i = 0; i < this->interfaces_count; i++)
                this->interfaces[i] = this->bfr.read_u16();

        } else {
            this->interfaces = NULL;
        }

        std::cout << "DONE\n" << std::flush;
    }

    void load_fields(void) {

        std::cout << "Loading fields..." << std::flush;

        this->fields_count = this->bfr.read_u16();
        std::cout << "(" << this->fields_count << ")\n" << std::flush;
        
        this->fields = new field_info[this->fields_count];

        for(int i = 0; i < this->fields_count; i++) {
            std::cout << '[' << i+1 << std::flush;
            this->fields[i].init(this->bfr, this->constant_pool);

        }

        std::cout << "DONE\n" << std::flush;
    }

    void load_methods(void) {
        this->methods_count = this->bfr.read_u16();
        this->methods = new method_info[this->methods_count];

        for(int i = 0; i < this->methods_count; i++)
            this->methods[i].init(this->bfr, this->constant_pool);
    }

    void load_attributes(void) {
        this->attributes_count = this->bfr.read_u16();
        this->attributes = new attribute_info*[this->attributes_count];
    
        for(int i = 0; i < this->attributes_count; i++) {
            auto s = ::place_attribute_info(this->bfr, this->attributes[i], this->constant_pool);
        }

    }

    // ====================================
    // PRINT functions
    // ====================================

    void print_preface(std::ostream& os = std::cout) {
        os << "Magic number: "
            << std::hex << this->magic << "\nClass File version: " 
            << std::dec << this->major_version << "." << this->minor_version 
            << std::endl << std::flush;

        os << "Java Version:       ";
        switch(this->major_version) {
            case 48: os << "Java 1.4\n"; break;
            case 49: os << "Java 5\n";   break;
            case 50: os << "Java 6\n";   break;
            case 51: os << "Java 7\n";   break;
            case 52: os << "Java 8\n";   break;
            case 53: os << "Java 9\n";   break;
            default:
                os << "UNKNOWN\n" << std::flush;
                throw std::runtime_error("Unknown java version used");
        }
        os << std::flush;

        // this JVM implementation only supports SE8
        if(this->major_version != 52)
            throw std::runtime_error("JVM implementation only supports SE8");

    }

    void print_constant_pool(std::ostream& os = std::cout) {
        // copy raw pointer over
        //global_constant_pool = this->constant_pool;

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

    void print_class_access_flags(uint16_t flags, std::ostream& os = std::cout) {

        PRINTFLAG(0x0001, ACC_PUBLIC,     os);
        PRINTFLAG(0x0010, ACC_FINAL,      os);
        PRINTFLAG(0x0020, ACC_SUPER,      os);
        PRINTFLAG(0x0200, ACC_INTERFACE,  os);
        PRINTFLAG(0x0400, ACC_ABSTRACT,   os);
        PRINTFLAG(0x1000, ACC_SYNTHETIC,  os);
        PRINTFLAG(0x2000, ACC_ANNOTATION, os);
        PRINTFLAG(0x4000, ACC_ENUM,       os);

        os << std::endl << std::flush;

    }

    void print_field_access_flags(uint16_t flags, std::ostream& os = std::cout) {
        
        PRINTFLAG(0x0001, ACC_PUBLIC,    os);
        PRINTFLAG(0x0002, ACC_PRIVATE,   os);
        PRINTFLAG(0x0004, ACC_PROTECTED, os);
        PRINTFLAG(0x0008, ACC_STATIC,    os);
        PRINTFLAG(0x0010, ACC_FINAL,     os);
        PRINTFLAG(0x0040, ACC_VOLATILE,  os);
        PRINTFLAG(0x0080, ACC_TRANSIENT, os);
        PRINTFLAG(0x1000, ACC_SYNTHETIC, os);
        PRINTFLAG(0x4000, ACC_ENUM,      os);

        os << std::endl << std::flush;

    }

    void print_method_access_flags(uint16_t flags, std::ostream& os = std::cout) {

        PRINTFLAG(0x0001, ACC_PUBLIC,       os);	
        PRINTFLAG(0x0002, ACC_PRIVATE,      os);	
        PRINTFLAG(0x0004, ACC_PROTECTED,    os);	
        PRINTFLAG(0x0008, ACC_STATIC,       os);	
        PRINTFLAG(0x0010, ACC_FINAL,        os);	
        PRINTFLAG(0x0020, ACC_SYNCHRONIZED, os);	
        PRINTFLAG(0x0040, ACC_BRIDGE,       os);	
        PRINTFLAG(0x0080, ACC_VARARGS,      os);	
        PRINTFLAG(0x0100, ACC_NATIVE,       os);	
        PRINTFLAG(0x0400, ACC_ABSTRACT,     os);	
        PRINTFLAG(0x0800, ACC_STRICT,       os);	
        PRINTFLAG(0x1000, ACC_SYNTHETIC,    os);

        os << std::endl << std::flush;

    }

    void print_this_class(std::ostream& os = std::cout) {
        //global_constant_pool = this->constant_pool;
        os << "This class:         " << ::string_of(this->this_class, this->constant_pool) << std::endl;
    }

    void print_super_class(std::ostream& os = std::cout) {
        os << "Super class:        ";

        if(super_class) {
            os << ::string_of(this->super_class, this->constant_pool) << std::endl;
        }
        else {
            os << "NONE\n";
        }

        os << std::flush;
    }

    void print_interfaces(std::ostream& os = std::cout) {

        os << "Interfaces:";
        for(int i = 0; i < this->interfaces_count; i++) {
            if(this->constant_pool[this->interfaces[i]-1]->tag == CONSTANT::Class) {
                // good to go
                //global_constant_pool = this->constant_pool;
                os << "\n    " << ::string_of(this->interfaces[i], this->constant_pool);
            }
            else {
                throw std::runtime_error("print_interfaces : Expected constant pool entry is not CONSTANT_Class_info");
            }
        }

        if(!this->interfaces_count)
            os << "         NONE";
        os << std::endl << std::flush;
    }

    void print_fields(std::ostream& os = std::cout) {
        os << "Fields count:       " << this->fields_count << std::endl << std::flush;

        for(int i = 0; i < this->fields_count; i++) {
            os << pad_left("#" + std::to_string(i+1), 7);
        
            os << "   Flags:      ";
            this->print_field_access_flags(this->fields[i].access_flags, os);
            
            os << "          Name:       '" 
                << ::string_of(this->fields[i].name_index, this->constant_pool) 
                << "'\n" << std::flush;

            os << "          Descriptor: "
                << ::string_of(this->fields[i].descriptor_index, this->constant_pool)
                << std::endl << std::flush;

            os << "          Num attributes: " << this->fields[i].attributes_count
                << std::endl << std::flush;

            os << std::endl << std::flush;
        }
    }

    void print_methods(std::ostream& os = std::cout) {
        os << "Methods count:       " << this->methods_count << std::endl << std::flush;

        for(int i = 0; i < this->methods_count; i++) {
            os << pad_left("#" + std::to_string(i+1), 7);
            
            os << "   Flags:      ";
            this->print_method_access_flags(this->methods[i].access_flags, os);
            
            os << "          Name:       '" 
                << ::string_of(this->methods[i].name_index, this->constant_pool) 
                << "'\n" << std::flush;

            os << "          Descriptor: "
                << ::string_of(this->methods[i].descriptor_index, this->constant_pool)
                << std::endl << std::flush;

            os << "          Num attributes: " << this->methods[i].attributes_count
                << std::endl << std::flush;

            os << std::endl << std::flush;
        }
    }

    void print_class_attributes(std::ostream& os = std::cout) {

    }

public:

    ClassFile(void) {}

    std::string class_name(void) {

    }

    void load_file(std::string filename) {
        this->bfr.open(filename);

        // everything related to loading the class file goes here
        this->load_preface();
        this->print_preface();

        this->load_constant_pool();
        this->print_constant_pool();
        
        this->load_misc(); // loads 'access_flags', 'this_class', and 'super_class'
        std::cout << "Class access flags: ";
        this->print_class_access_flags(this->access_flags);
        this->print_this_class();
        this->print_super_class();

        this->load_interfaces();
        this->print_interfaces();

        this->load_fields();
        this->print_fields();

        this->load_methods();
        this->print_methods();

        this->bfr.close();
    }

    friend std::ostream& operator<<(std::ostream& os, ClassFile& cf) {
        
        cf.print_preface(os);
        cf.print_class_access_flags(cf.access_flags, os);
        cf.print_this_class(os);
        cf.print_super_class(os);
        cf.print_interfaces(os);
        cf.print_constant_pool(os);

        return os;
    }

};
