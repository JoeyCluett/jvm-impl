#include <ClassFile.h>
#include <BinaryFileReader.h>
#include <ConstantInfo.h>
#include <functional>

ClassFile::ClassFile(void) {
    ; // do nothing
}

void ClassFile::init(const std::string& filename) {

    BinaryFileReader bfr;
    bfr.open(filename);

    this->magic = bfr.read_u32();

    if(this->magic != 0xCAFEBABE) {
        throw std::runtime_error(
                "ClassFile::init : magic number is incorrect");
    }

    this->minor_version = bfr.read_u16();
    this->major_version = bfr.read_u16();

    std::cout << "Version: "
        << this->major_version << "."
        << this->minor_version << std::endl;

    if(this->major_version != 52) {
        throw std::runtime_error(
                "JVM Implementation only supports Java SE8");
    }

    this->load_constant_pool(bfr);

}

void ClassFile::load_constant_pool(BinaryFileReader& bfr) {

    auto load_constant_into_buffer = [this](int index, std::string content) {

    };

    std::vector<ConstantInfo> cvec;

    this->constant_pool_count = bfr.read_u16();
    for(int i = 0; i < this->constant_pool_count-1;) {
        uint8_t tag = bfr.read_u8();
        cvec.emplace_back(bfr, tag, i, this->cp_buffer);
    }

    std::function<std::string(int)> eval_constant =
        [&eval_constant, &cvec, this](int index) -> std::string {

            ConstantInfo ci = cvec.at(index-1);

            switch(ci.tag) {
                case 7:  // CONSTANT_Class
                    {
                        return eval_constant(ci.class_info.name_index);
                    }
                    break;
                case 9:  // CONSTANT_FieldRef
                    {
                        return
                            eval_constant(ci.fieldref_info.class_index) + " " +
                            eval_constant(ci.fieldref_info.name_and_type_index);
                    }
                    break;
                case 10: // CONSTANT_MethodRef
                    {
                        return
                            eval_constant(ci.methodref_info.class_index) + " " +
                            eval_constant(ci.methodref_info.name_and_type_index);
                    }
                    break;
                case 11: // CONSTANT_InterfaceMethodRef
                    {
                        return
                            eval_constant(ci.interfacemethodref_info.class_index) + " " +
                            eval_constant(ci.interfacemethodref_info.name_and_type_index);
                    }
                    break;
                case 8:  // CONSTANT_String
                    {
                        return eval_constant(ci.string_info.string_index);
                    }
                    break;
                case 3:  // CONSTANT_Integer
                    {
                        return std::to_string(ci.integer_info.bytes);
                    }
                    break;
                case 4:  // CONSTANT_Float
                    {
                        return std::to_string(ci.float_info.bytes);
                    }
                    break;
                case 5:  // CONSTANT_Long
                    {
                        return std::to_string(ci.long_info.bytes);
                    }
                    break;
                case 6:  // CONSTANT_Double
                    {
                        return std::to_string(ci.double_info.bytes);
                    }
                    break;
                case 12: // CONSTANT_NameAndType
                    {
                        return
                            eval_constant(ci.nameandtype_info.name_index) + " " +
                            eval_constant(ci.nameandtype_info.descriptor_index);
                    }
                    break;
                case 1:  // CONSTANT_Utf8
                    {
                        return std::string(
                            this->cp_buffer.begin()+ci.utf8_info.start,
                            this->cp_buffer.begin()+ci.utf8_info.end);
                    }
                    break;
                case 15: // CONSTANT_MethodHandle
                    {
                        
                    }
                    break;
                case 16: // CONSTANT_MethodType
                    break;
                case 18: // CONSTANT_InvokeDynamic
                    break;
                default:
                    throw std::runtime_error(
                            "Unknown tag value in constant pool: " +
                            std::to_string(int(ci.tag)));
            }
        };

}

std::string ClassFile::getConstantAt(int index) {



}
