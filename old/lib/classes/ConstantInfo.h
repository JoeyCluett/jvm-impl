#pragma once

#include <iostream>
#include <utility>
#include "ConstantPoolEntry.h"

// forward declarations ftw
struct ConstantPoolEntry;

struct CONSTANT_info {
    ~CONSTANT_info() {}
    static ConstantPoolEntry* constant_pool;
};
ConstantPoolEntry* CONSTANT_info::constant_pool = NULL;

struct ConstantPoolEntry {

    uint8_t tag;
    CONSTANT_info* info;

    ~ConstantPoolEntry(void) {
        if(info)
            delete info;
    }

};
typedef ConstantPoolEntry cp_info;

// Printable
struct CONSTANT_Utf8_info : public CONSTANT_info {
    uint16_t length;
    uint8_t* bytes = NULL;

    ~CONSTANT_Utf8_info(void) {
        if(bytes)
            delete[] bytes;
    }

    friend std::ostream& operator<<(std::ostream& os, CONSTANT_Utf8_info& cutf8) {
        os << "            ";
        os.write(reinterpret_cast<char*>(cutf8.bytes), cutf8.length);
        os << std::flush;

        return os;
    }

};

// Printable
struct CONSTANT_Class_info : public CONSTANT_info {
    uint16_t name_index;

    friend std::ostream& operator<<(std::ostream& os, CONSTANT_Class_info& cci) {

        //os << "    Name index: " << cci.name_index << std::endl << std::flush;
        auto& constant_pool_entry = cci.constant_pool[cci.name_index-1];
        //os << "    Fetched Utf8 Object Pointer\n" << std::flush;

        auto* constant_info_obj = static_cast<CONSTANT_Utf8_info*>(constant_pool_entry.info);
        os << *constant_info_obj;
        return os;
    }
};

// Printable
struct CONSTANT_NameAndType_info : public CONSTANT_info {
    uint16_t name_index;
    uint16_t descriptor_index;

    friend std::ostream& operator<<(std::ostream& os, CONSTANT_NameAndType_info& cnat) {

        //os << cnat.name_index << " " << cnat.descriptor_index << std::endl << std::flush;

        {
            auto& info_ref = cnat.constant_pool[cnat.name_index-1];
            os << "            Name: "; // << *static_cast<CONSTANT_Utf8_info*>(info_ref.info);

            auto& cutf8 = *static_cast<CONSTANT_Utf8_info*>(info_ref.info);
            os.write(reinterpret_cast<char*>(cutf8.bytes), cutf8.length);

            os << std::endl;
        }

        {
            auto& info_ref = cnat.constant_pool[cnat.descriptor_index-1];
            os << "            Desc: ";

            auto& cutf8 = *static_cast<CONSTANT_Utf8_info*>(info_ref.info);
            os.write(reinterpret_cast<char*>(cutf8.bytes), cutf8.length);
        }

        return os;
    }

};

// Printable
struct CONSTANT_Fieldref_info : public CONSTANT_info {
    uint16_t class_index;
    uint16_t name_and_type_index;

    friend std::ostream& operator<<(std::ostream& os, CONSTANT_Fieldref_info& cfri) {

        //os << "    class_index: " << cfri.class_index
        //<< "\n    name_and_type_index: " << cfri.name_and_type_index << std::endl << std::flush;

        auto* class_info = 
            static_cast<CONSTANT_Class_info*>(
                cfri.constant_pool[cfri.class_index-1].info); 

        {
            os << *class_info << std::endl << std::flush;
        }

        auto* name_and_type = 
            static_cast<CONSTANT_NameAndType_info*>(
                cfri.constant_pool[cfri.name_and_type_index-1].info);

        {
            os << *name_and_type << std::flush;
        }

        return os;
    }

};

// Printable
struct CONSTANT_Methodref_info : public CONSTANT_info {
    uint16_t class_index;
    uint16_t name_and_type_index;

    friend std::ostream& operator<<(std::ostream& os, CONSTANT_Methodref_info& cmri) {

        //os << "    class_index: " << cmri.class_index
        //<< "\n    name_and_type_index: " << cmri.name_and_type_index << std::endl << std::flush;

        auto* class_info = 
            static_cast<CONSTANT_Class_info*>(
                cmri.constant_pool[cmri.class_index-1].info); 

        {
            os << *class_info << std::endl << std::flush;
        }

        auto* name_and_type = 
            static_cast<CONSTANT_NameAndType_info*>(
                cmri.constant_pool[cmri.name_and_type_index-1].info);

        {
            os << *name_and_type << std::flush;
        }

        return os;
    }

};

struct CONSTANT_InterfaceMethodref_info : public CONSTANT_info {
    uint16_t class_index;
    uint16_t name_and_type_index;

    friend std::ostream& operator<<(std::ostream& os, CONSTANT_InterfaceMethodref_info& cirmi) {

        return os;
    }

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



