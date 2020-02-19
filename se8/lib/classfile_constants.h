#pragma once

#include <iostream>

// some useful typedefs
typedef uint8_t  u1;
typedef uint16_t u2;

// class file access flags
const uint16_t CLASS_ACC_PUBLIC     = 0x0001;
const uint16_t CLASS_ACC_FINAL      = 0x0010;
const uint16_t CLASS_ACC_SUPER      = 0x0020;
const uint16_t CLASS_ACC_INTERFACE  = 0x0200;
const uint16_t CLASS_ACC_ABSTRACT   = 0x0400;
const uint16_t CLASS_ACC_SYNTHETIC  = 0x1000;
const uint16_t CLASS_ACC_ANNOTATION = 0x2000;
const uint16_t CLASS_ACC_ENUM       = 0x4000;

// every constant field entry needs one of these
const uint8_t CONSTANT_Utf8               = 1;
const uint8_t CONSTANT_Integer            = 3;
const uint8_t CONSTANT_Float              = 4;
const uint8_t CONSTANT_Long               = 5;
const uint8_t CONSTANT_Double             = 6;
const uint8_t CONSTANT_Class              = 7;
const uint8_t CONSTANT_String             = 8;
const uint8_t CONSTANT_FieldRef           = 9;
const uint8_t CONSTANT_MethodRef          = 10;
const uint8_t CONSTANT_InterfaceMethodref = 11;
const uint8_t CONSTANT_NameAndType        = 12;
const uint8_t CONSTANT_MethodHandle       = 15;
const uint8_t CONSTANT_MethodType         = 16;
const uint8_t CONSTANT_InvokeDynamic      = 18;

// additional access flags for fields (some are already defined for classes)
const uint16_t FIELD_ACC_PUBLIC    = 0x0001;
const uint16_t FIELD_ACC_PRIVATE   = 0x0002;
const uint16_t FIELD_ACC_PROTECTED = 0x0004;
const uint16_t FIELD_ACC_STATIC    = 0x0008;
const uint16_t FIELD_ACC_FINAL     = 0x0010;
const uint16_t FIELD_ACC_VOLATILE  = 0x0040;
const uint16_t FIELD_ACC_TRANSIENT = 0x0080;
const uint16_t FIELD_ACC_SYNTHETIC = 0x1000;
const uint16_t FIELD_ACC_ENUM      = 0x4000;

const uint16_t METHOD_ACC_PUBLIC       = 0x0001;
const uint16_t METHOD_ACC_PRIVATE      = 0x0002;
const uint16_t METHOD_ACC_PROTECTED    = 0x0004;
const uint16_t METHOD_ACC_STATIC       = 0x0008;
const uint16_t METHOD_ACC_FINAL        = 0x0010;
const uint16_t METHOD_ACC_SYNCHRONIZED = 0x0020;
const uint16_t METHOD_ACC_BRIDGE       = 0x0040;
const uint16_t METHOD_ACC_VARARGS      = 0x0080;
const uint16_t METHOD_ACC_NATIVE       = 0x0100;
const uint16_t METHOD_ACC_ABSTRACT     = 0x0400;
const uint16_t METHOD_ACC_STRICT       = 0x0800;
const uint16_t METHOD_ACC_SYNTHETIC    = 0x1000;

