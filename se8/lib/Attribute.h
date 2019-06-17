#pragma once

#include "ConstantPool.h"
#include "BinaryFileReader.h"

struct attribute_info {

    uint16_t attribute_name_index;
    uint32_t attribute_length;

    attribute_info(uint16_t index, uint32_t len) {
        this->attribute_name_index = index;
        this->attribute_length = len;
    }

};

// convenience function
void place_attribute_info(BinaryFileReader& bfr, attribute_info*&);

// ============================================================================
// ConstantValue
// ============================================================================

struct ConstantValue_attribute : public attribute_info {
    
    uint16_t constantvalue_index;

    ConstantValue_attribute(BinaryFileReader& bfr, uint16_t attribute_name_index, uint32_t attribute_length) 
            : attribute_info(attribute_name_index, attribute_length) {

        this->constantvalue_index = bfr.read_u16();
    }

};

// ============================================================================
// Code
// ============================================================================

struct exception_table_entry {
    uint16_t start_pc;
    uint16_t end_pc;
    uint16_t handler_pc;
    uint16_t catch_type;
};

struct Code_attribute : public attribute_info {

    uint16_t max_stack;
    uint16_t max_locals;
    uint32_t code_length;
    uint8_t* code;
    uint16_t exception_table_length;
    exception_table_entry* exception_table; 
    uint16_t attributes_count;
    attribute_info** attributes;

    Code_attribute(BinaryFileReader& bfr, uint16_t attribute_name_index, uint32_t attribute_length) 
            : attribute_info(attribute_name_index, attribute_length) {
    
        this->max_stack = bfr.read_u16();
        this->max_locals = bfr.read_u16();

        // read correct amount of code
        this->code_length = bfr.read_u32();
        this->code = new uint8_t[this->code_length];
        bfr.read_buffer(reinterpret_cast<char*>(this->code), this->code_length);

        this->exception_table_length = bfr.read_u16();
        this->exception_table = new exception_table_entry[this->exception_table_length];
        for(int i = 0; i < this->exception_table_length; i++) {
            this->exception_table[i].start_pc = bfr.read_u16();
            this->exception_table[i].end_pc = bfr.read_u16();
            this->exception_table[i].handler_pc = bfr.read_u16();
            this->exception_table[i].catch_type = bfr.read_u16();
        }

        this->attributes_count = bfr.read_u16();
        this->attributes = new attribute_info*[attributes_count];

        for(int i = 0; i < this->attributes_count; i++)
            ::place_attribute_info(bfr, this->attributes[i]);

    }

};

// ============================================================================
// StackMapTable
// ============================================================================

struct verification_type_info {

    ITEM tag;

    union {
        struct {
            uint16_t cpool_index;
        } Object_variable_info;

        struct {
            uint16_t offset;
        } Uninitialized_variable_info;
    };

    void init(BinaryFileReader& bfr) {
        this->tag = static_cast<ITEM>(bfr.read_u8());

        switch(this->tag) {
            case ITEM::Top:
            case ITEM::Integer:
            case ITEM::Float:
            case ITEM::Double:
            case ITEM::Long:
            case ITEM::Null:
            case ITEM::UninitializedThis:
                break; // nothing needs to be done for these
            case ITEM::Object:
                this->Object_variable_info.cpool_index = bfr.read_u16();
                break;
            case ITEM::Uninitialized:
                this->Uninitialized_variable_info.offset = bfr.read_u16();
                break;
            default:
                throw std::runtime_error("");
        }
    }

};

struct stack_map_frame {

    // the offical JVM spec doesnt call for both of these members but i want 
    // them so here they are. use frame_type when the enum value is important. 
    // use tag when the numerical value is important
    FRAME_TYPE frame_type;
    uint8_t tag;

    union {

        struct {
            verification_type_info stack[1];
        } same_locals_1_stack_item_frame;

        struct {
            uint16_t offset_delta;
            verification_type_info stack[1];
        } same_locals_1_stack_item_frame_extended;

        struct {
            uint16_t offset_delta;
        } chop_frame;

        struct {
            uint16_t offset_delta;
        } same_frame_extended;

        struct {
            uint16_t offset_delta;
            verification_type_info* locals; // locals[frame_type - 251]
        } append_frame;

        struct {
            uint16_t offset_delta;
            uint16_t number_of_locals;
            verification_type_info* locals; // locals[number_of_locals]
            uint16_t number_of_stack_items;
            verification_type_info* stack;  // stack[number_of_stack_items]
        } full_frame;

    };

    void init(BinaryFileReader& bfr) {
        this->tag = bfr.read_u8(); // this value may be important later
        this->frame_type = get_frame_type_from_tag(this->tag);
    
        switch(this->frame_type) {
            case FRAME_TYPE::SAME:                               // 0-63
                break;
            case FRAME_TYPE::SAME_LOCALS_1_STACK_ITEM:           // 64-127
                this->same_locals_1_stack_item_frame.stack[0].init(bfr);
                break;
            case FRAME_TYPE::SAME_LOCALS_1_STACK_ITEM_EXTENDED:  // 247
                this->same_locals_1_stack_item_frame_extended.offset_delta = bfr.read_u16();
                this->same_locals_1_stack_item_frame_extended.stack[0].init(bfr);
                break;
            case FRAME_TYPE::CHOP:                               // 249-250
                this->chop_frame.offset_delta = bfr.read_u16();
                break;
            case FRAME_TYPE::SAME_FRAME_EXTENDED:                // 251
                this->same_frame_extended.offset_delta = bfr.read_u16();
                break;
            case FRAME_TYPE::APPEND:                             // 252-254
                this->append_frame.offset_delta = bfr.read_u16();
                this->append_frame.locals = new verification_type_info[this->tag - 251];
                {
                    for(int i = 0; i < (this->tag-251); i++)
                        this->append_frame.locals[i].init(bfr);
                }
                break;
            case FRAME_TYPE::FULL_FRAME:                         // 255
                this->full_frame.offset_delta = bfr.read_u16();
                this->full_frame.number_of_locals = bfr.read_u16();
                this->full_frame.locals = new verification_type_info[this->full_frame.number_of_locals];
                {
                    for(int i = 0; i < this->full_frame.number_of_locals; i++)
                        this->full_frame.locals[i].init(bfr);
                }
                this->full_frame.number_of_stack_items = bfr.read_u16();
                this->full_frame.stack = new verification_type_info[this->full_frame.number_of_stack_items];
                {
                    for(int i = 0; i < this->full_frame.number_of_stack_items; i++)
                        this->full_frame.stack[i].init(bfr);
                }
                break;
            default:
                throw std::runtime_error("Invalid frame type tag in stack_map_frame");
        };

    }

    ~stack_map_frame(void) {
        // take care of the dynamic memory used
        if(this->frame_type == FRAME_TYPE::APPEND)
            delete[] this->append_frame.locals;
        else if(this->frame_type == FRAME_TYPE::FULL_FRAME) {
            delete[] this->full_frame.locals;
            delete[] this->full_frame.stack;
        }

    }

};

struct StackMapTable_attribute : public attribute_info {

    uint16_t number_of_entries;
    stack_map_frame* entries;


    StackMapTable_attribute(BinaryFileReader& bfr, uint16_t attribute_name_index, uint32_t attribute_length) 
            : attribute_info(attribute_name_index, attribute_length) {

        this->number_of_entries = bfr.read_u16();
        this->entries = new stack_map_frame[this->number_of_entries];

        for(int i = 0; i < this->number_of_entries; i++)
            this->entries[i].init(bfr);

    }

    ~StackMapTable_attribute(void) {
        delete[] this->entries;
    }

};

// ============================================================================
// Exceptions
// ============================================================================

struct Exceptions_attribute : public attribute_info {

    uint16_t number_of_exceptions;
    uint16_t* exception_index_table;

    Exceptions_attribute(BinaryFileReader& bfr, uint16_t attribute_name_index, uint32_t attribute_length)
            : attribute_info(attribute_name_index, attribute_length) {

        this->number_of_exceptions = bfr.read_u16();
        this->exception_index_table = new uint16_t[this->number_of_exceptions];
        for(int i = 0; i < this->number_of_exceptions; i++)
            this->exception_index_table[i] = bfr.read_u16();
    
    }

    ~Exceptions_attribute(void) {
        delete[] this->exception_index_table;
    }

};

// ============================================================================
// InnerClasses
// ============================================================================

struct inner_class_info {
    uint16_t inner_class_info_index;
    uint16_t outer_class_info_index;
    uint16_t inner_name_index;
    uint16_t inner_class_access_flags;

    void init(BinaryFileReader& bfr) {
        this->inner_class_info_index = bfr.read_u16();
        this->outer_class_info_index = bfr.read_u16();
        this->inner_name_index = bfr.read_u16();
        this->inner_class_access_flags = bfr.read_u16();
    }

};

struct InnerClasses_attribute : public attribute_info {

    uint16_t number_of_classes;
    inner_class_info* classes;

    InnerClasses_attribute(BinaryFileReader& bfr, uint16_t attribute_name_index, uint32_t attribute_length)
            : attribute_info(attribute_name_index, attribute_length) {

        this->number_of_classes = bfr.read_u16();
        this->classes = new inner_class_info[this->number_of_classes];
        for(int i = 0; i < this->number_of_classes; i++)
            this->classes[i].init(bfr);

    }

};

// ============================================================================
// EnclosingMethod
// ============================================================================

struct EnclosingMethod_attribute : public attribute_info {

    uint16_t class_index;
    uint16_t method_index;

    EnclosingMethod_attribute(BinaryFileReader& bfr, uint16_t attribute_name_index, uint32_t attribute_length)
            : attribute_info(attribute_name_index, attribute_length) {

        this->class_index = bfr.read_u16();
        this->method_index = bfr.read_u16();

    }

};

// ============================================================================
// Synthetic
// ============================================================================

struct Synthetic_attribute : public attribute_info {

    Synthetic_attribute(BinaryFileReader& bfr, uint16_t attribute_name_index, uint32_t attribute_length)
            : attribute_info(attribute_name_index, attribute_length) {
    
        // check the attribute length real quick here
        if(attribute_length != 0)
            throw std::runtime_error("Synthetic_attribute error: attribute length is " 
            + std::to_string(attribute_length) + ". expecting zero");
    }

};

// ============================================================================
// Signature
// ============================================================================

struct Signature_attribute : public attribute_info {

    uint16_t signature_index;

    Signature_attribute(BinaryFileReader& bfr, uint16_t attribute_name_index, uint32_t attribute_length)
            : attribute_info(attribute_name_index, attribute_length) {

        this->signature_index = bfr.read_u16();

    }

};

// ============================================================================
// SourceFile
// ============================================================================

struct SourceFile_attribute : public attribute_info {

    uint16_t sourcefile_index;

    SourceFile_attribute(BinaryFileReader& bfr, uint16_t attribute_name_index, uint32_t attribute_length)
            : attribute_info(attribute_name_index, attribute_length) {

        this->sourcefile_index = bfr.read_u16();

    }

};

// ============================================================================
// SourceDebugExtension
// ============================================================================

struct SourceDebugExtension_attribute : public attribute_info {

    uint8_t* debug_extension;

    SourceDebugExtension_attribute(BinaryFileReader& bfr, uint16_t attribute_name_index, uint32_t attribute_length)
            : attribute_info(attribute_name_index, attribute_length) {

        this->debug_extension = new uint8_t[attribute_length];
        bfr.read_buffer(reinterpret_cast<char*>(this->debug_extension), attribute_length);

    }

    ~SourceDebugExtension_attribute(void) {
        delete[] debug_extension;
    }

};

// ============================================================================
// LineNumberTable
// ============================================================================

struct line_number_table_entry {
    uint16_t start_pc;
    uint16_t line_number;

    void init(BinaryFileReader& bfr) {
        this->start_pc = bfr.read_u16();
        this->line_number = bfr.read_u16();
    }

};

struct LineNumberTable_attribute : public attribute_info {

    uint16_t line_number_table_length;
    line_number_table_entry* line_number_table;

    LineNumberTable_attribute(BinaryFileReader& bfr, uint16_t attribute_name_index, uint32_t attribute_length)
            : attribute_info(attribute_name_index, attribute_length) {

        this->line_number_table_length = bfr.read_u16();
        this->line_number_table = new line_number_table_entry[this->line_number_table_length];
        for(int i = 0; i < this->line_number_table_length; i++)
            this->line_number_table[i].init(bfr);

    }

    ~LineNumberTable_attribute(void) {
        delete[] this->line_number_table;
    }

};

// ============================================================================
// LocalVariableTable
// ============================================================================

struct local_variable_table_entry {

    uint16_t start_pc;
    uint16_t length;
    uint16_t name_index;
    uint16_t descriptor_index;
    uint16_t index;

    void init(BinaryFileReader& bfr) {
        this->start_pc = bfr.read_u16();
        this->length = bfr.read_u16();
        this->name_index = bfr.read_u16();
        this->descriptor_index = bfr.read_u16();
        this->index = bfr.read_u16();
    }

};

struct LocalVariableTable_attribute : public attribute_info {

    uint16_t local_variable_table_length;
    local_variable_table_entry* local_variable_table;

    LocalVariableTable_attribute(BinaryFileReader& bfr, uint16_t attribute_name_index, uint32_t attribute_length)
            : attribute_info(attribute_name_index, attribute_length) {

        this->local_variable_table_length = bfr.read_u16();
        this->local_variable_table = new local_variable_table_entry[this->local_variable_table_length];
        for(int i = 0; i < this->local_variable_table_length; i++)
            this->local_variable_table[i].init(bfr);

    }
    
    ~LocalVariableTable_attribute(void) {
        delete[] this->local_variable_table;
    }

};

// ============================================================================
// LocalVariableTypeTable
// ============================================================================

struct local_variable_type_table_entry {

    uint16_t start_pc;
    uint16_t length;
    uint16_t name_index;
    uint16_t signature_index;
    uint16_t index;

    void init(BinaryFileReader& bfr) {
        this->start_pc = bfr.read_u16();
        this->length = bfr.read_u16();
        this->name_index = bfr.read_u16();
        this->signature_index = bfr.read_u16();
        this->index = bfr.read_u16();
    }

};

struct LocalVariableTypeTable_attribute : public attribute_info {

    uint16_t local_variable_type_table_length;
    local_variable_type_table_entry* local_variable_type_table;

    LocalVariableTypeTable_attribute(BinaryFileReader& bfr, uint16_t attribute_name_index, uint32_t attribute_length)
            : attribute_info(attribute_name_index, attribute_length) {

        this->local_variable_type_table_length = bfr.read_u16();
        this->local_variable_type_table = new local_variable_type_table_entry[this->local_variable_type_table_length];
        for(int i = 0; i < this->local_variable_type_table_length; i++)
            this->local_variable_type_table[i].init(bfr);

    }

    ~LocalVariableTypeTable_attribute(void) {
        delete[] this->local_variable_type_table;
    }

};

// ============================================================================
// Deprecated
// ============================================================================

struct Deprecated_attribute : public attribute_info {

    Deprecated_attribute(BinaryFileReader& bfr, uint16_t attribute_name_index, uint32_t attribute_length)
            : attribute_info(attribute_name_index, attribute_length) {

        // check the attribute length real quick here
        if(attribute_length != 0)
            throw std::runtime_error("Deprecated_attribute error: attribute length is " 
            + std::to_string(attribute_length) + ". expecting zero");

    }

};

// ============================================================================
// RuntimeVisibleAnnotations
// ============================================================================

struct element_value; // forward declaration to get this monstrosity to compile... i hate my life

struct element_value_pair_entry {

    uint16_t element_name_index;
    element_value* value;
    void init(BinaryFileReader& bfr);

};

struct annotation {
    uint16_t type_index;
    uint16_t num_element_value_pairs;
    element_value_pair_entry* element_value_pairs;

    void init(BinaryFileReader& bfr) {
        this->type_index = bfr.read_u16();
        this->num_element_value_pairs = bfr.read_u16();
        this->element_value_pairs = new element_value_pair_entry[this->num_element_value_pairs];
        for(int i = 0; i < this->num_element_value_pairs; i++)
            this->element_value_pairs[i].init(bfr);
    }

};

struct element_value {
    uint8_t tag;
    
    union {

        uint16_t const_value_index;

        struct {
            uint16_t type_name_index;
            uint16_t const_name_index;
        } enum_const_value;

        uint16_t class_info_index;

        annotation annotation_value;

        struct {
            uint16_t num_values;
            element_value* values;
        } array_value;

    } value;

    void init(BinaryFileReader& bfr) {
        bfr.read_buffer(reinterpret_cast<char*>(this->tag), 1);
        switch(this->tag) {
            case 'B': case 'C': case 'D':
            case 'F': case 'I': case 'J':
            case 'S': case 'Z': case 's':
                this->value.const_value_index = bfr.read_u16();
                break;
            case 'e':
                this->value.enum_const_value.type_name_index = bfr.read_u16();
                this->value.enum_const_value.const_name_index = bfr.read_u16();
                break;
            case 'c':
                this->value.class_info_index = bfr.read_u16();
                break;
            case '@':
                this->value.annotation_value.init(bfr);
                break;
            case '[':
                {
                    uint16_t tmp = bfr.read_u16();
                    this->value.array_value.num_values = tmp;
                    this->value.array_value.values = new element_value[tmp];
                    for(int i = 0; i < tmp; i++)
                        this->value.array_value.values[i].init(bfr);
                }
                break;
            default:
                throw std::runtime_error("element_value error: unknown tag value");
        }
    }

};

// this was seriously my path of least resistance. i have dishonored myself, my family, and my cow
void element_value_pair_entry::init(BinaryFileReader& bfr) {
    this->element_name_index = bfr.read_u16();
    this->value = new element_value;
    this->value->init(bfr);
}

struct RuntimeVisibleAnnotations_attribute : public attribute_info {

    uint16_t num_annotations;
    annotation* annotations;

    RuntimeVisibleAnnotations_attribute(BinaryFileReader& bfr, uint16_t attribute_name_index, uint32_t attribute_length)
            : attribute_info(attribute_name_index, attribute_length) {

        this->num_annotations = bfr.read_u16();
        this->annotations = new annotation[this->num_annotations];
        for(int i = 0; i < this->num_annotations; i++)
            this->annotations[i].init(bfr);
    }
};

// ============================================================================
// RuntimeInvisibleAnnotations
// ============================================================================

// ============================================================================
// RuntimeVisibleParameterAnnotations
// ============================================================================

// ============================================================================
// RuntimeInvisibleParameterAnnotations
// ============================================================================

// ============================================================================
// RuntimeVisibleTypeAnnotations
// ============================================================================

// ============================================================================
// RuntimeInvisibleTypeAnnotations
// ============================================================================

// ============================================================================
// AnnotationDefault
// ============================================================================

// ============================================================================
// BootstrapMethods
// ============================================================================

// ============================================================================
// MethodParameters
// ============================================================================


// convenience function definition
void place_attribute_info(BinaryFileReader& bfr, attribute_info*& ptr_ref) {
    uint16_t tmp_name_index = bfr.read_u16();
    uint32_t tmp_length = bfr.read_u32();

    auto str = ::string_of(tmp_name_index);

    if(str == "ConstantValue") {
        ptr_ref = new ConstantValue_attribute(bfr, tmp_name_index, tmp_length);
    }
    else if(str == "Code") {
        ptr_ref = new Code_attribute(bfr, tmp_name_index, tmp_length);
    }
    else {
        throw std::runtime_error("error placing attribute info: unknown string constant: " + str);
    }

}

