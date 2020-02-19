#pragma once

// Big Ol' list of opcodes

#define INST(mnemonic, hexvalue) const unsigned char mnemonic = hexvalue

namespace instruction {

    INST( nop,         0x00); // 0 : do nothing
    INST( aconst_null, 0x01); // 1 : push null reference
    INST( iconst_m1,   0x02); // 2 : push int constant (implicitly -1)
    INST( iconst_0,    0x03); // 3 : push int constant (implicitly 0)
    INST( iconst_1,    0x04); // 4 : push int constant (implicitly 1)
    INST( iconst_2,    0x05); // 5 : push int constant (implicitly 2)
    INST( iconst_3,    0x06); // 6 : push int constant (implicitly 3)
    INST( iconst_4,    0x07); // 7 : push int constant (implicitly 4)
    INST( iconst_5,    0x08); // 8 : push int constant (implicitly 5)
    INST( lconst_0,    0x09); // 9 :  push long constant (implicitly 0)
    INST( lconst_1,    0x0a); // 10 : push long constant (implicitly 1)
    INST( fconst_0,    0x0b); // 11 : push float (impicitly 0)
    INST( fconst_1,    0x0c); // 12 : push float (impicitly 1)
    INST( fconst_2,    0x0d); // 13 : push float (impicitly 2)
    INST( dconst_0,    0x0e); // 14 : push double (implicitly 0)
    INST( dconst_1,    0x0f); // 15 : push double (implicitly 1)

    INST( bipush,      0x10); // 16 : push byte
    INST( sipush,      0x11); // 17 : push short
    INST( ldc,         0x12); // 18 : push item from run-time constant pool
    INST( ldc_w,       0x13); // 19 : push item from run-time constant pool (wide)
    INST( ldc2_w,      0x14); // 20 : push long or double from run-time constant pool (wide)
    INST( iload,       0x15); // 21 : load int from local var
    INST( lload,       0x16); // 22 : load long from local var
    INST( fload,       0x17); // 23 : load float from local var
    INST( dload,       0x18); // 24 : load double from local var
    INST( aload,       0x19); // 25 : load ref from local var
    INST( iload_0,     0x1a); // 26 : load int from local var (implicitly 0)
    INST( iload_1,     0x1b); // 27 : load int from local var (implicitly 1)
    INST( iload_2,     0x1c); // 28 : load int from local var (implicitly 2)
    INST( iload_3,     0x1d); // 29 : load int from local var (implicitly 3)
    INST( lload_0,     0x1e); // 30 : load long from local var (implicitly 0)
    INST( lload_1,     0x1f); // 31 : load long from local var (implicitly 1)

    INST( lload_2,     0x20); // 32 : load long from local var (implicitly 2)
    INST( lload_3,     0x21); // 33 : load long from local var (implicitly 3)
    INST( fload_0,     0x22); // 34 : load float from local var (impicitly 0)
    INST( fload_1,     0x23); // 35 : load float from local var (impicitly 1)
    INST( fload_2,     0x24); // 36 : load float from local var (impicitly 2)
    INST( fload_3,     0x25); // 37 : load float from local var (impicitly 3)
    INST( dload_0,     0x26); // 38 : load double from local var (implicitly 0)
    INST( dload_1,     0x27); // 39 : load double from local var (implicitly 1)
    INST( dload_2,     0x28); // 40 : load double from local var (implicitly 2)
    INST( dload_3,     0x29); // 41 : load double from local var (implicitly 3)
    INST( aload_0,     0x2a); // 42 : load ref from local var (implicitly 0)
    INST( aload_1,     0x2b); // 43 : load ref from local var (implicitly 1)
    INST( aload_2,     0x2c); // 44 : load ref from local var (implicitly 2)
    INST( aload_3,     0x2d); // 45 : load ref from local var (implicitly 3)
    INST( iaload,      0x2e); // 46 : load int from array
    INST( laload,      0x2f); // 47 : load long from array

    INST( faload,      0x30); // 48 : load float from array
    INST( daload,      0x31); // 49 : load double from array
    INST( aaload,      0x32); // 50 : load ref from array
    INST( baload,      0x33); // 51 : load byte/bool from array
    INST( caload,      0x34); // 52 : load char from array
    INST( saload,      0x35); // 53 : load short from array
    INST( istore,      0x36); // 54 : store int into local var
    INST( lstore,      0x37); // 55 : store long into local var
    INST( fstore,      0x38); // 56 : store float into local var
    INST( dstore,      0x39); // 57 : store double into local var
    INST( astore,      0x3a); // 58 : store ref into local var
    INST( istore_0,    0x3b); // 59 : store int into local var (implicitly 0)
    INST( istore_1,    0x3c); // 60 : store int into local var (implicitly 1)
    INST( istore_2,    0x3d); // 61 : store int into local var (implicitly 2)
    INST( istore_3,    0x3e); // 62 : store int into local var (implicitly 3)
    INST( lstore_0,    0x3f); // 63 : store long into local var (implicitly 0)

    INST( lstore_1,    0x40); // 64 : store long into local var (implicitly 1)
    INST( lstore_2,    0x41); // 65 : store long into local var (implicitly 2)
    INST( lstore_3,    0x42); // 66 : store long into local var (implicitly 3)
    INST( fstore_0,    0x43); // 67 : store float into local var (implicitly 0)
    INST( fstore_1,    0x44); // 68 : store float into local var (implicitly 1)
    INST( fstore_2,    0x45); // 69 : store float into local var (implicitly 2)
    INST( fstore_3,    0x46); // 70 : store float into local var (implicitly 3)
    INST( dstore_0,    0x47); // 71 : store double into local var (implicitly 0)
    INST( dstore_1,    0x48); // 72 : store double into local var (implicitly 1)
    INST( dstore_2,    0x49); // 73 : store double into local var (implicitly 2)
    INST( dstore_3,    0x4a); // 74 : store double into local var (implicitly 3)
    INST( astore_0,    0x4b); // 75 : store ref into local var (implicitly 0)
    INST( astore_1,    0x4c); // 76 : store ref into local var (implicitly 1)
    INST( astore_2,    0x4d); // 77 : store ref into local var (implicitly 2)
    INST( astore_3,    0x4e); // 78 : store ref into local var (implicitly 3)
    INST( iastore,     0x4f); // 79 : store into int array

    INST( lastore,     0x50); // 80 : store into long array
    INST( fastore,     0x51); // 81 : store float into array
    INST( dastore,     0x52); // 82 : store double into array
    INST( aastore,     0x53); // 83 : store ref into array
    INST( bastore,     0x54); // 84 : store byte/bool into array
    INST( castore,     0x55); // 85 : store char into array
    INST( sastore,     0x56); // 86 : store into short array
    INST( pop,         0x57); // 87 : pop top operand stack value
    INST( pop2,        0x58); // 88 : pop the top one or two operand stack values
    INST( dup,         0x59); // 89 : duplicate top of operand stack
    INST( dup_x1,      0x5a); // 90 : duplicate top operand stack value and insert two values down
    INST( dup_x2,      0x5b); // 91 : duplicate top operand stack value and insert two or three values down
    INST( dup2,        0x5c); // 92 : duplicate the top one or two operand stack values
    INST( dup2_x1,     0x5d); // 93 : duplicate the top one or two operand stack values and insert two or three values down
    INST( dup2_x2,     0x5e); // 94 : duplicate the top one or two operand stack values and insert two, three, or four values down
    INST( _swap,       0x5f); // 95 : swap the top two operand stack values

    INST( iadd,        0x60); // 96 : add int
    INST( ladd,        0x61); // 97 : add long
    INST( fadd,        0x62); // 98 : add float
    INST( dadd,        0x63); // 99 : add double
    INST( isub,        0x64); // 100 : subtract int
    INST( lsub,        0x65); // 101 : subtract long
    INST( fsub,        0x66); // 102 : subtract float
    INST( dsub,        0x67); // 103 : subtract double
    INST( imul,        0x68); // 104 : multiply int
    INST( lmul,        0x69); // 105 : multiply long
    INST( fmul,        0x6a); // 106 : multiply float
    INST( dmul,        0x6b); // 107 : multiply double
    INST( idiv,        0x6c); // 108 : divide int
    INST( _ldiv,       0x6d); // 109 : divide long ## ldiv is part of the c stdlib
    INST( fdiv,        0x6e); // 110 : divide float
    INST( ddiv,        0x6f); // 111 : divide double

    INST( irem,        0x70); // 112 : remainder int
    INST( lrem,        0x71); // 113 : remainder long
    INST( frem,        0x72); // 114 : remainder float
    INST( drem,        0x73); // 115 : remainder double
    INST( ineg,        0x74); // 116 : negate int
    INST( lneg,        0x75); // 117 : negate long
    INST( fneg,        0x76); // 118 : negate float
    INST( dneg,        0x77); // 119 : negate double
    INST( ishl,        0x78); // 120 : shift left int
    INST( lshl,        0x79); // 121 : shift left long
    INST( ishr,        0x7a); // 122 : arithmetic shift right int
    INST( lshr,        0x7b); // 123 : arithmetic shift right long
    INST( iushr,       0x7c); // 124 : logical shift right int
    INST( lushr,       0x7d); // 125 : logical shift right long
    INST( iand,        0x7e); // 126 : boolean and int
    INST( land,        0x7f); // 127 : boolean and long

    INST( ior,         0x80); // 128 : boolean or int
    INST( lor,         0x81); // 129 : boolean or long
    INST( ixor,        0x82); // 130 : boolean xor int
    INST( lxor,        0x83); // 131 : boolean xor long
    INST( iinc,        0x84); // 132 : increment local var by constant
    INST( i2l,         0x85); // 133 : convert int to long
    INST( i2f,         0x86); // 134 : convert int to float
    INST( i2d,         0x87); // 135 : convert int to double
    INST( l2i,         0x88); // 136 : convert long to int
    INST( l2f,         0x89); // 137 : convert long to float
    INST( l2d,         0x8a); // 138 : convert long to double
    INST( f2i,         0x8b); // 139 : convert float to int
    INST( f2l,         0x8c); // 140 : convert float to long
    INST( f2d,         0x8d); // 141 : convert float to double
    INST( d2i,         0x8e); // 142 : convert double to int
    INST( d2l,         0x8f); // 143 : convert double to long

    INST( d2f,         0x90); // 144 : convert double to float
    INST( i2b,         0x91); // 145 : convert int to byte
    INST( i2c,         0x92); // 146 : convert int to char
    INST( i2s,         0x93); // 147 : convert int to short
    INST( lcmp,        0x94); // 148 : compare long
    INST( fcmpl,       0x95); // 149 : compare float
    INST( fcmpg,       0x96); // 150 : compare float
    INST( dcmpl,       0x97); // 151 : compare double
    INST( dcmpg,       0x98); // 152 : compare double
    INST( ifeq,        0x99); // 153 : branch if compare with zero succeeds (equal)
    INST( ifne,        0x9a); // 154 : branch if compare with zero succeeds (not equal)
    INST( iflt,        0x9b); // 155 : branch if compare with zero succeeds (less than)
    INST( ifge,        0x9c); // 156 : branch if compare with zero succeeds (greater than or equal)
    INST( ifgt,        0x9d); // 157 : branch if compare with zero succeeds (greater than)
    INST( ifle,        0x9e); // 153 : branch if compare with zero succeeds (less than or equal)
    INST( if_icmpeq,   0x9f); // 159 : branch if int compare succeeds (equal)

    INST( if_icmpne,    0xa0); // 160 : branch if int compare succeeds (not equal)
    INST( if_icmplt,    0xa1); // 161 : branch if int compare succeeds (less than)
    INST( if_icmpge,    0xa2); // 162 : branch if int compare succeeds (greater than or equal)
    INST( if_icmpgt,    0xa3); // 163 : branch if int compare succeeds (greater than)
    INST( if_icmple,    0xa4); // 164 : branch if int compare succeeds (less than or equal)
    INST( if_acmpeq,    0xa5); // 165 : branch if refs equal
    INST( if_acmpne,    0xa6); // 166 : branch if refs not equal
    INST( _goto,        0xa7); // 167 : branch always
    INST( jsr,          0xa8); // 168 : jump subroutine
    INST( ret,          0xa9); // 169 : return from subroutine
    INST( tableswitch,  0xaa); // 170 : access jump table by index and jump
    INST( lookupswitch, 0xab); // 171 : access jump table by key match and jump
    INST( ireturn,      0xac); // 172 : return int from method
    INST( lreturn,      0xad); // 173 : return long from method
    INST( freturn,      0xae); // 174 : return float from method
    INST( dreturn,      0xaf); // 175 : return double from method

    INST( areturn,         0xb0); // 176 : return reference from method
    INST( _return,         0xb1); // 177 : return void from method
    INST( getstatic,       0xb2); // 178 : get static field from class 
    INST( putstatic,       0xb3); // 179 : set static field in class
    INST( getfield,        0xb4); // 180 : fetch field from object
    INST( putfield,        0xb5); // 181 : set field in object
    INST( invokevirtual,   0xb6); // 182 : invoke instance method, dispatch based on class
    INST( invokespecial,   0xb7); // 183 : invoke instance method
    INST( invokestatic,    0xb8); // 184 : invoke a class/static method
    INST( invokeinterface, 0xb9); // 185 : invoke interface method
    INST( invokedynamic,   0xba); // 186 : invoke dynamic method
    INST( _new,            0xbb); // 187 : create new object
    INST( newarray,        0xbc); // 188 : create new array
    INST( anewarray,       0xbd); // 189 : create a new array with specified number of elements
    INST( arraylength,     0xbe); // 190 : get length of array
    INST( athrow,          0xbf); // 191 : throw exception or error

    INST( checkcast,      0xc0); // 192 : check whether object is of given type
    INST( instanceof,     0xc1); // 193 : determine whether object is of given type
    INST( monitorenter,   0xc2); // 194 : enter monitor for object
    INST( monitorexit,    0xc3); // 195 : exit monitor for object
    INST( _wide,          0xc4); // 196 : extend local var index by additional bytes
    INST( multianewarray, 0xc5); // 197 : create a new multidimensional array
    INST( ifnull,         0xc6); // 198 : branch if reference null
    INST( ifnonnull,      0xc7); // 199 : branch if reference not null
    INST( goto_w,         0xc8); // 200 : branch always (wide)
    INST( jsr_w,          0xc9); // 201 : jump subroutine (wide)
    INST( breakpoint,     0xca); // 202 : implementation defined, for debugging

    INST( impdep1,     0xfe); // 254 : implementation dependent
    INST( impdep2,     0xff); // 255 : implementation dependent

} // end of namespace instruction
