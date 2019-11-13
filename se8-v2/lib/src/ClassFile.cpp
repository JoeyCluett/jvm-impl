#include <ClassFile.h>
#include <BinaryFileReader.h>
#include <ConstantInfo.h>
#include <functional>
#include <stdio.h>

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

    for(int i = 0; i < this->cp_info.size(); i++) {

//        std::cout << this->getConstantAt(i+1) << std::endl;

        auto s = this->getConstantAt(i+1);
        printf("%-5d : %s %s\n", i, ConstantInfo::nameOfTag(std::get<0>(this->cp_info[i])).c_str(), s.c_str());

    }

}

void ClassFile::load_constant_pool(BinaryFileReader& bfr) {

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
                            ci.utf8_info.buf,
                            ci.utf8_info.buf + ci.utf8_info.length
                        );
                    }
                    break;
                case 15: // CONSTANT_MethodHandle
                    {
                        return 
                            std::to_string(int(ci.methodhandle_info.reference_kind)) + " " +
                            eval_constant(ci.methodhandle_info.reference_index);
                    }
                    break;
                case 16: // CONSTANT_MethodType
                    {
                        return eval_constant(ci.methodtype_info.descriptor_index);
                    }
                    break;
                case 18: // CONSTANT_InvokeDynamic
                    {
                        return 
                            std::to_string(int(ci.invokedynamic_info.bootstrap_method_attr_index)) + " " +
                            eval_constant(ci.invokedynamic_info.name_and_type_index);
                    }
                    break;
                default:
                    throw std::runtime_error(
                            "Unknown tag value in constant pool: " +
                            std::to_string(int(ci.tag)));
            }
        };

    for(int i = 0; i < cvec.size(); i++) {
        auto& c = cvec[i];

        // string representation
        std::string str_rep = eval_constant(i+1);
        //std::cout << str_rep << std::endl << std::flush;

        std::tuple<uint8_t, int, int> t;
        std::get<0>(t) = c.tag;
        std::get<1>(t) = this->cp_buffer.size();
        this->cp_buffer.insert(this->cp_buffer.end(), str_rep.begin(), str_rep.end());
        std::get<2>(t) = this->cp_buffer.size();

        this->cp_info.push_back(t);

        //std::cout << std::get<1>(t) << "  " << std::get<2>(t) << "\n";

    }

}

std::string ClassFile::getConstantAt(int index) { // uses the 1-based indexing used by Java classs files

    auto& t = this->cp_info.at(index-1);

    return std::string(
            this->cp_buffer.begin() + std::get<1>(t), 
            this->cp_buffer.begin() + std::get<2>(t));

}
