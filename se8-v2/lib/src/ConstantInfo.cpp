#include <ConstantInfo.h>
#include <vector>

ConstantInfo::ConstantInfo(
        BinaryFileReader& bfr, uint8_t tag,
        int& cp_index, std::vector<char>& c_buf) {

    this->tag = tag;

    switch(tag) {
        case 7:  // CONSTANT_Class
            this->class_info.name_index = bfr.read_u16();
            cp_index += 1;
            break;
        case 9:  // CONSTANT_FieldRef
            this->fieldref_info.class_index = bfr.read_u16();
            this->fieldref_info.name_and_type_index = bfr.read_u16();
            cp_index += 1;
            break;
        case 10: // CONSTANT_MethodRef
            this->methodref_info.class_index = bfr.read_u16();
            this->methodref_info.name_and_type_index = bfr.read_u16();
            cp_index += 1;
            break;
        case 11: // CONSTANT_InterfaceMethodRef
            this->interfacemethodref_info.class_index = bfr.read_u16();
            this->interfacemethodref_info.name_and_type_index = bfr.read_u16();
            cp_index += 1;
            break;
        case 8:  // CONSTANT_String
            this->string_info.string_index = bfr.read_u16();
            cp_index += 1;
            break;
        case 3:  // CONSTANT_Integer
            this->integer_info.bytes = bfr.read_i32();
            cp_index += 1;
            break;
        case 4:  // CONSTANT_Float
            this->float_info.bytes = bfr.read_f32();
            cp_index += 1;
            break;
        case 5:  // CONSTANT_Long
            this->long_info.bytes = bfr.read_i64();
            cp_index += 2;
            break;
        case 6:  // CONSTANT_Double
            this->double_info.bytes = bfr.read_f64();
            cp_index += 2;
            break;
        case 12: // CONSTANT_NameAndType
            this->nameandtype_info.name_index = bfr.read_u16();
            this->nameandtype_info.descriptor_index = bfr.read_u16();
            cp_index += 1;
            break;
        case 1:  // CONSTANT_Utf8
            {
                // a lot of other constants use these types
                // to describe their component parts

                this->utf8_info.length = bfr.read_u16();

                std::vector<char> buf;
                buf.resize(this->utf8_info.length);
                bfr.read_buffer(buf.data(), this->utf8_info.length);

                this->utf8_info.start = c_buf.size();
                this->utf8_info.end   = c_buf.size() + buf.size();

                for(char c : buf)
                    c_buf.push_back(c);

            }
            cp_index += 1;
            break;
        case 15: // CONSTANT_MethodHandle
            this->methodhandle_info.reference_kind = bfr.read_u8();
            this->methodhandle_info.reference_index = bfr.read_u16();
            cp_index += 1;
            break;
        case 16: // CONSTANT_MethodType
            this->methodtype_info.descriptor_index = bfr.read_u16();
            cp_index += 1;
            break;
        case 18: // CONSTANT_InvokeDynamic
            this->invokedynamic_info.bootstrap_method_attr_index = bfr.read_u16();
            this->invokedynamic_info.name_and_type_index = bfr.read_u16();
            cp_index += 1;
            break;
        default:
            throw std::runtime_error(
                    "Unknown tag value in constant pool: " +
                    std::to_string(int(tag)));
    }

}
