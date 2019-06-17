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
        case CLASS_ACC::PUBLIC:     return "CLASS_ACC_PUBLIC";     break;
        case CLASS_ACC::FINAL:      return "CLASS_ACC_FINAL";      break;
        case CLASS_ACC::SUPER:      return "CLASS_ACC_SUPER";      break;
        case CLASS_ACC::INTERFACE:  return "CLASS_ACC_INTERFACE";  break;
        case CLASS_ACC::ABSTRACT:   return "CLASS_ACC_ABSTRACT";   break;
        case CLASS_ACC::SYNTHETIC:  return "CLASS_ACC_SYNTHETIC";  break;
        case CLASS_ACC::ANNOTATION: return "CLASS_ACC_ANNOTATION"; break;
        case CLASS_ACC::ENUM:       return "CLASS_ACC_ENUM";       break;
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

std::string FIELD_ACC_flag_string(FIELD_ACC fa) {
    switch(fa) {
        case FIELD_ACC::PUBLIC:    return "FIELD_ACC_PUBLIC";    break;
        case FIELD_ACC::PRIVATE:   return "FIELD_ACC_PRIVATE";   break;
        case FIELD_ACC::PROTECTED: return "FIELD_ACC_PROTECTED"; break;
        case FIELD_ACC::STATIC:    return "FIELD_ACC_STATIC";    break;
        case FIELD_ACC::FINAL:     return "FIELD_ACC_FINAL";     break;
        case FIELD_ACC::VOLATILE:  return "FIELD_ACC_VOLATILE";  break;
        case FIELD_ACC::TRANSIENT: return "FIELD_ACC_TRANSIENT"; break;
        case FIELD_ACC::SYNTHETIC: return "FIELD_ACC_SYNTHETIC"; break;
        case FIELD_ACC::ENUM:      return "FIELD_ACC_ENUM";      break;
        default:
            throw std::runtime_error("unknown flag in FIELD_ACC_flag_string");
    };
}

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

std::string METHOD_ACC_flag_string(METHOD_ACC ma) {
    switch(ma) {
        case METHOD_ACC::PUBLIC:       return "METHOD_ACC_PUBLIC";       break;
        case METHOD_ACC::PRIVATE:      return "METHOD_ACC_PRIVATE";      break;
        case METHOD_ACC::PROTECTED:    return "METHOD_ACC_PROTECTED";    break;
        case METHOD_ACC::STATIC:       return "METHOD_ACC_STATIC";       break;
        case METHOD_ACC::FINAL:        return "METHOD_ACC_FINAL";        break;
        case METHOD_ACC::SYNCHRONIZED: return "METHOD_ACC_SYNCHRONIZED"; break;
        case METHOD_ACC::BRIDGE:       return "METHOD_ACC_BRIDGE";       break;
        case METHOD_ACC::VARARGS:      return "METHOD_ACC_VARARGS";      break;
        case METHOD_ACC::NATIVE:       return "METHOD_ACC_NATIVE";       break;
        case METHOD_ACC::ABSTRACT:     return "METHOD_ACC_ABSTRACT";     break;
        case METHOD_ACC::STRICT:       return "METHOD_ACC_STRICT";       break;
        case METHOD_ACC::SYNTHETIC:    return "METHOD_ACC_SYNTHETIC";    break;
        default:
            throw std::runtime_error("unknown flag in METHOD_ACC_flag_string");
    }
}

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

std::string FRAME_TYPE_flag_string(FRAME_TYPE ft) {
    switch(ft) {
        case FRAME_TYPE::SAME:       
            return "FRAME_TYPE_SAME"; break;
        case FRAME_TYPE::SAME_LOCALS_1_STACK_ITEM:
            return "FRAME_TYPE_SAME_LOCALS_1_STACK_ITEM"; break;
        case FRAME_TYPE::SAME_LOCALS_1_STACK_ITEM_EXTENDED:
            return "FRAME_TYPE_SAME_LOCALS_1_STACK_ITEM_EXTENDED"; break;
        case FRAME_TYPE::CHOP:       
            return "FRAME_TYPE_CHOP"; break;
        case FRAME_TYPE::SAME_FRAME_EXTENDED:
            return "FRAME_TYPE_SAME_FRAME_EXTENDED"; break;
        case FRAME_TYPE::APPEND:     
            return "FRAME_TYPE_APPEND"; break;
        case FRAME_TYPE::FULL_FRAME: 
            return "FRAME_TYPE_FULL_FRAME"; break;
        default:
            throw std::runtime_error("unknown flag in FRAME_TYPE_flag_string");
    }
}

enum class ARRAY_TYPE : uint8_t {

    BOOLEAN = 4,
    CHAR    = 5,
    FLOAT   = 6,
    DOUBLE  = 7,
    BYTE    = 8,
    SHORT   = 9,
    INT     = 10,
    LONG    = 11

};

std::string ARRAY_TYPE_flag_string(ARRAY_TYPE ft) {
    switch(ft) {
        case ARRAY_TYPE::BOOLEAN: return "ARRAY_TYPE_BOOLEAN"; break;
        case ARRAY_TYPE::CHAR:    return "ARRAY_TYPE_CHAR";    break;
        case ARRAY_TYPE::FLOAT:   return "ARRAY_TYPE_FLOAT";   break;
        case ARRAY_TYPE::DOUBLE:  return "ARRAY_TYPE_DOUBLE";  break;
        case ARRAY_TYPE::BYTE:    return "ARRAY_TYPE_BYTE";    break;
        case ARRAY_TYPE::SHORT:   return "ARRAY_TYPE_SHORT";   break;
        case ARRAY_TYPE::INT:     return "ARRAY_TYPE_INT";     break;
        case ARRAY_TYPE::LONG:    return "ARRAY_TYPE_LONG";    break;
        default:
            throw std::runtime_error("unknown value in ARRAY_TYPE_flag_string");
    }
}
