#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <tuple>
#include <BinaryFileReader.h>
#include <ConstantInfo.h>

class ClassFile {

    uint32_t magic;

    uint16_t minor_version;
    uint16_t major_version;

    uint16_t constant_pool_count;
    std::vector<char> cp_buffer;
    std::vector<std::tuple<uint8_t, int,   int>> cp_info;

    uint16_t access_flags;

    uint16_t this_class;

    uint16_t super_class;

    uint16_t interfaces_count;
    //uint16_t interfaces[interfaces_count];

    uint16_t fields_count;
    // field_info fields[fields_count];

    uint16_t methods_count;
    // method_info methods[method_count]

    uint16_t attributes_count;
    // attribute_info attributes[attributes_count]

    void load_constant_pool(BinaryFileReader& bfr);

public:

    // default constructor does nothing
    ClassFile(void);

    std::string getConstantAt(int index);

    // this is where the heavy lifting happens. i do it this way so
    // that the ClassFile can be constructed in one place and initialized in another
    void init(const std::string& filename);

};
