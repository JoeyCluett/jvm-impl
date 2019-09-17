#pragma once

#include <iostream>
#include "ConstantPool.h"
#include "Attributes.h"
#include "Enums.h"
#include "BinaryFileReader.h"

struct field_info {
    
    uint16_t access_flags;
    uint16_t name_index;
    uint16_t descriptor_index;
    uint16_t attributes_count;
    attribute_info** attributes;

    field_info(void) = default;

    void init(BinaryFileReader& bfr, cp_info** cp) {
        this->access_flags     = bfr.read_u16();
        this->name_index       = bfr.read_u16();
        this->descriptor_index = bfr.read_u16();
        this->attributes_count = bfr.read_u16();
        this->attributes = new attribute_info*[this->attributes_count];

        for(int i = 0; i < this->attributes_count; i++) {

            ::place_attribute_info(bfr, attributes[i], cp);

        /*
            uint16_t attribute_name_index = bfr.read_u16();
            uint32_t attribute_length = bfr.read_u32();
        
            auto str = ::string_of(attribute_name_index, cp);

            if(str == "ConstantValue") {
                this->attributes[i] = new ConstantValue_attribute(bfr, attribute_name_index, attribute_length);
            }
            else if(str == "Code") {
                this->attributes[i] = new Code_attribute(bfr, attribute_name_index, attribute_length);
            }   
            else if(str == "StackMapTable") {
                this->attributes[i] = new StackMapTable_attribute(bfr, attribute_name_index, attribute_length);
            }
            else if(str == "Exceptions") {
                this->attributes[i] = new Exceptions_attribute(bfr, attribute_name_index, attribute_length);
            }
            else if(str == "InnerClasses") {
                this->attributes[i] = new InnerClasses_attribute(bfr, attribute_name_index, attribute_length);
            }
            else if(str == "EnclosingMethod") {
                this->attributes[i] = new EnclosingMethod_attribute(bfr, attribute_name_index, attribute_length);
            }
            else if(str == "Synthetic") {
                this->attributes[i] = new Synthetic_attribute(bfr, attribute_name_index, attribute_length);
            }
            else if(str == "SourceFile") {
                this->attributes[i] = new SourceFile_attribute(bfr, attribute_name_index, attribute_length);
            }
            else if(str == "SourceDebugExtension") {
                this->attributes[i] = new SourceDebugExtension_attribute(bfr, attribute_name_index, attribute_length);
            }
            else if(str == "LineNumberTable") {
                this->attributes[i] = new LineNumberTable_attribute(bfr, attribute_name_index, attribute_length);
            }
            else if(str == "LocalVariableTable") {
                this->attributes[i] = new LocalVariableTable_attribute(bfr, attribute_name_index, attribute_length);
            }
            else if(str == "LocalVariableTypeTable") {
                this->attributes[i] = new LocalVariableTypeTable_attribute(bfr, attribute_name_index, attribute_length);
            }
            else if(str == "Deprecated") {
                this->attributes[i] = new Deprecated_attribute(bfr, attribute_name_index, attribute_length);
            }
            else if(str == "RuntimeVisibleAnnotations") {
                this->attributes[i] = new RuntimeVisibleAnnotations_attribute(bfr, attribute_name_index, attribute_length);
            }
            else if(str == "RuntimeInvisibleAnnotations") {
                this->attributes[i] = new RuntimeInvisibleAnnotations_attribute(bfr, attribute_name_index, attribute_length);
            }
            else if(str == "RuntimeVisibleParameterAnnotations") {
                this->attributes[i] = new RuntimeVisibleParameterAnnotations_attribute(bfr, attribute_name_index, attribute_length);
            }
            else if(str == "RuntimeInvisibleParameterAnnotations") {
                this->attributes[i] = new RuntimeInvisibleParameterAnnotations_attribute(bfr, attribute_name_index, attribute_length);
            }
            else if(str == "RuntimeVisibleTypeAnnotations") {
                this->attributes[i] = new RuntimeVisibleTypeAnnotations_attribute(bfr, attribute_name_index, attribute_length);
            }
            else if(str == "RuntimeInvisibleTypeAnnotations") {
                this->attributes[i] = new RuntimeInvisibleTypeAnnotations_attribute(bfr, attribute_name_index, attribute_length);
            }
            else if(str == "AnnotationDefault") {
                this->attributes[i] = new AnnotationDefault_attribute(bfr, attribute_name_index, attribute_length);
            }
            else if(str == "BootstrapMethods") {
                this->attributes[i] = new BootstrapMethods_attribute(bfr, attribute_name_index, attribute_length);
            }
            else if(str == "MethodParameters") {
                this->attributes[i] = new MethodParameters_attribute(bfr, attribute_name_index, attribute_length);
            }
            else {
                throw std::runtime_error("Unknown attribute string value: " + str);
            }
        */
        }

    }

    ~field_info(void) {
        // delete data associated with every pointer first
        for(int i = 0; i < this->attributes_count; i++)
            delete this->attributes[i];

        delete[] this->attributes;
    }

};

