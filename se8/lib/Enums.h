#pragma once

// every constant field entry needs one of these
enum class CONSTANT : uint8_t {

    Utf8               = 1,
    Integer            = 3,
    Float              = 4,
    Long               = 5,
    Double             = 6,
    Class              = 7,
    String             = 8,
    FieldRef           = 9,
    MethodRef          = 10,
    InterfaceMethodRef = 11,
    NameAndType        = 12,
    MethodHandle       = 15,
    MethodType         = 16,
    InvokeDynamic      = 18

};

// class file access flags
enum class CLASS_ACC : uint16_t {

    PUBLIC     = 0x0001,
    FINAL      = 0x0010,
    SUPER      = 0x0020,
    INTERFACE  = 0x0200,
    ABSTRACT   = 0x0400,
    SYNTHETIC  = 0x1000,
    ANNOTATION = 0x2000,
    ENUM       = 0x4000

};

std::string CLASS_ACC_flag_string(CLASS_ACC ca) {
    switch(ca) {
        case CLASS_ACC::PUBLIC:     return "ACC_PUBLIC";     break;
        case CLASS_ACC::FINAL:      return "ACC_FINAL";      break;
        case CLASS_ACC::SUPER:      return "ACC_SUPER";      break;
        case CLASS_ACC::INTERFACE:  return "ACC_INTERFACE";  break;
        case CLASS_ACC::ABSTRACT:   return "ACC_ABSTRACT";   break;
        case CLASS_ACC::SYNTHETIC:  return "ACC_SYNTHETIC";  break;
        case CLASS_ACC::ANNOTATION: return "ACC_ANNOTATION"; break;
        case CLASS_ACC::ENUM:       return "ACC_ENUM";       break;
        default:
            throw std::runtime_error("unknown flag in CLASS_ACC_flag_string");
    };
}

// additional access flags for fields
enum class FIELD_ACC : uint16_t {

    PUBLIC    = 0x0001,
    PRIVATE   = 0x0002,
    PROTECTED = 0x0004,
    STATIC    = 0x0008,
    FINAL     = 0x0010,
    VOLATILE  = 0x0040,
    TRANSIENT = 0x0080,
    SYNTHETIC = 0x1000,
    ENUM      = 0x4000

};

// method access flags
enum class METHOD_ACC : uint16_t {

    PUBLIC       = 0x0001,
    PRIVATE      = 0x0002,
    PROTECTED    = 0x0004,
    STATIC       = 0x0008,
    FINAL        = 0x0010,
    SYNCHRONIZED = 0x0020,
    BRIDGE       = 0x0040,
    VARARGS      = 0x0080,
    NATIVE       = 0x0100,
    ABSTRACT     = 0x0400,
    STRICT       = 0x0800,
    SYNTHETIC    = 0x1000

};

// bytecode behaviors for method handles
enum class REF : uint8_t {

    getField         = 1,
    getStatic        = 2,
    putField         = 3,
    putStatic        = 4,
    invokeVirtual    = 5,
    invokeStatic     = 6,
    invokeSpecial    = 7,
    newInvokeSpecial = 8,
    invokeInterface  = 9

};

std::string REF_flag_string(REF r) {
    switch(r) {
        case REF::getField:         return "REF_GET_FIELD";         break;
        case REF::getStatic:        return "REF_GET_STATIC";        break;
        case REF::putField:         return "REF_PUT_FIELD";         break;
        case REF::putStatic:        return "REF_PUT_STATIC";        break;
        case REF::invokeVirtual:    return "REF_INVOKE_VIRTUAL";    break;
        case REF::invokeStatic:     return "REF_INVOKE_STATIC";     break;
        case REF::invokeSpecial:    return "REF_INVOKE_SPECIAL";    break;
        case REF::newInvokeSpecial: return "REF_NEW_INVOKE_SPECIAL"; break;
        case REF::invokeInterface:  return "REF_INVOKE_INTERFACE";  break;
        default:
            throw std::runtime_error("unknown flag in REF_flag_string");
    };
}

enum class ITEM : uint8_t {

    Top               = 0,
    Integer           = 1,
    Float             = 2,
    Double            = 3,
    Long              = 4,
    Null              = 5,
    UninitializedThis = 6,
    Object            = 7,
    Uninitialized     = 8

};

std::string ITEM_flag_string(ITEM i) {
    switch(i) {
        case ITEM::Top:               return "ITEM_TOP"; break;
        case ITEM::Integer:           return "ITEM_INTEGER"; break;
        case ITEM::Float:             return "ITEM_FLOAT"; break;
        case ITEM::Double:            return "ITEM_DOUBLE"; break;
        case ITEM::Long:              return "ITEM_LONG"; break;
        case ITEM::Null:              return "ITEM_NULL"; break;
        case ITEM::UninitializedThis: return "ITEM_UNINITIALIZED_THIS"; break;
        case ITEM::Object:            return "ITEM_OBJECT"; break;
        default:
            throw std::runtime_error("ITEM_flag_string : unknown ITEM::flag type");
    }
}

enum class FRAME_TYPE : uint8_t {

    SAME,                               // 0-63
    SAME_LOCALS_1_STACK_ITEM,           // 64-127
    SAME_LOCALS_1_STACK_ITEM_EXTENDED,  // 247
    CHOP,                               // 249-250
    SAME_FRAME_EXTENDED,                // 251
    APPEND,                             // 252-254
    FULL_FRAME                          // 255

};

FRAME_TYPE get_frame_type_from_tag(uint8_t input) {
    if(input >= 0 && input <= 63)         return FRAME_TYPE::SAME;
    else if(input >= 64 && input <= 127)  return FRAME_TYPE::SAME_LOCALS_1_STACK_ITEM;
    else if(input == 247)                 return FRAME_TYPE::SAME_LOCALS_1_STACK_ITEM_EXTENDED;
    else if(input == 249 || input == 250) return FRAME_TYPE::CHOP;
    else if(input == 251)                 return FRAME_TYPE::SAME_FRAME_EXTENDED;
    else if(input >= 252 && input <= 254) return FRAME_TYPE::APPEND;
    else if(input == 255)                 return FRAME_TYPE::FULL_FRAME;
    else
        throw std::runtime_error("get_frame_type_from_tag unrecognized value: " + std::to_string(input));  
}
