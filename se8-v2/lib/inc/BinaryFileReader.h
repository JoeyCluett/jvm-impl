#pragma once

#include <fstream>
#include <utility>

/*
    This class assumes you are working
    on a 64-bit little-endian ISA like AMD64

    I will need to add code to verify that
    this is the case...fml
*/

class BinaryFileReader {
private:
    std::fstream fs;

public:

    // empty constructor
    BinaryFileReader();

    // prep the filereader to begin reading data.
    // file is opened as a binary stream
    void open(std::string filename);

    // finish using the file. specified with the ::open
    // method. after this operation, file can be reopened
    // elsewhere without fear of being busy
    void close(void);

    // read an unsigned 64-bit integer. the integer is
    // byte-swapped to conform with target architecture
    uint64_t read_u64(void);

    // read a signed 64-bit integer. the integer is
    // byte-swapped to conform to target architecture
    int64_t read_i64(void);

    // read a 64-bit float. the float is
    // byte-swapped to conform to target architecture
    double read_f64(void);

    // read an unsigned 32-bit integer. integer is
    // byte-swapped to conform to target architecture
    uint32_t read_u32(void);

    // read a signed 32-bit integer. integer is
    // byte-swapped to conform to target architecture
    int32_t read_i32(void);

    // read a 32-bit float. float is byte-swapped to conform
    // to target architecture
    float read_f32(void);

    // read a 16-bit unsigned integer. integer is byte-swapped
    // to conform to target architecture
    uint16_t read_u16(void);

    // read unsigned 8-bit integer. this does not need to be byte-swapped
    uint8_t read_u8(void);

    // read a raw character stream from the file
    void read_buffer(char* buf, int len);

};
