#pragma once

#include <iostream>
#include <string>
#include <BinaryFileReader.h>
#include <vector>

struct ConstantInfo {

    uint8_t tag;

    union {

        struct {
            uint16_t name_index;
        } class_info;

        struct {
            uint16_t class_index;
            uint16_t name_and_type_index;
        } fieldref_info;

        struct {
            uint16_t class_index;
            uint16_t name_and_type_index;
        } methodref_info;

        struct {
            uint16_t class_index;
            uint16_t name_and_type_index;
        } interfacemethodref_info;

        struct {
            uint16_t string_index;
        } string_info;

        struct {
            uint32_t bytes;
        } integer_info;

        struct {
            float bytes;
        } float_info;

        struct {
            int64_t bytes;
        } long_info;

        struct {
            double bytes;
        } double_info;

        struct {
            uint16_t name_index;
            uint16_t descriptor_index;
        } nameandtype_info;

        struct {
            uint16_t length;
            char* buf;
        } utf8_info;

        struct {
            uint8_t  reference_kind;
            uint16_t reference_index;
        } methodhandle_info;

        struct {
            uint16_t descriptor_index;
        } methodtype_info;

        struct {
            uint16_t bootstrap_method_attr_index;
            uint16_t name_and_type_index;
        } invokedynamic_info;

    };

    ConstantInfo(BinaryFileReader& bfr, uint8_t tag, int& cp_index, std::vector<char>& c_buf);

    ConstantInfo(const ConstantInfo& rhs);

    ConstantInfo& operator=(const ConstantInfo& rhs);

    static std::string nameOfTag(uint8_t tag);

    ~ConstantInfo(void);

};
