#pragma once

#include <map>

#include "BinaryFileReader.hpp"
#include "cp_info.hpp"
#include "field_info.hpp"
#include "method_info.hpp"
#include "attribute_info.hpp"

class ClassFile {
private:

    BinaryFileReader bfr;
    uint16_t minor_version, major_version;

    uint16_t        constant_pool_count;
    //cp_info*        constant_pool = NULL;
    std::map<int, cp_info> constant_pool;
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


    void read_constant_pool(void) {
        for(int i = 1; i < constant_pool_count; i++) {
            cp_info el;
            el.tag = this->bfr.read_u8();
            el.initialize(this->bfr);

            this->constant_pool.insert({ i, el });

            if(el.tag == CONSTANT_Long || el.tag == CONSTANT_Double)
                i++; // need two spaces each
        }

        cp_info::finalize(this->constant_pool);
    }

public:

    ClassFile(std::string filename) {
        this->bfr.open(filename);
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
        this->read_constant_pool();

        for(auto& p : this->constant_pool)
            std::cout << p.first << " : " << p.second.type() << " : " << p.second.str() << std::endl;

/*
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
*/
    }

};

