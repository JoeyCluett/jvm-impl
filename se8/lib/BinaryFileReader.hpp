#pragma once

#include <fstream>
#include <utility>

/*
    This class assumes you are working on a 64-bit little-endian ISA like AMD64
    bytes read from Class file are big-endian. this class swaps bytes as needed
*/

class BinaryFileReader {
private:
    std::fstream fs;

public:

    // empty constructor
    BinaryFileReader() {}

    void open(std::string filename);
    void close(void);

    uint64_t read_u64(void);
    int64_t  read_i64(void);
    double   read_f64(void);
    uint32_t read_u32(void);
    int32_t  read_i32(void);
    float    read_f32(void);
    uint16_t read_u16(void);
    uint8_t  read_u8(void);

    void read_buffer(char* buf, int len);

};

void BinaryFileReader::open(std::string filename) {
    this->fs.open(filename, std::ios::binary | std::ios::in);
}

void BinaryFileReader::close(void) {
    this->fs.close();
}

uint64_t BinaryFileReader::read_u64(void) {
    union {
        char buf[8];
        uint64_t u64;
    };

    fs.read(buf, 8);
    std::swap(buf[0], buf[7]);
    std::swap(buf[1], buf[6]);
    std::swap(buf[2], buf[5]);
    std::swap(buf[3], buf[4]);

    return u64;
}

int64_t BinaryFileReader::read_i64(void) {
    union {
        uint64_t u64;
        int64_t i64;
    };

    u64 = this->read_u64();
    return i64;
}

double BinaryFileReader::read_f64(void) {
    union {
        double f64;
        int64_t i64;
    };

    i64 = this->read_i64();
    return f64;
}

uint32_t BinaryFileReader::read_u32(void) {
    union {
        char buf[4];
        uint32_t u32;
    };

    fs.read(buf, 4);
    std::swap(buf[0], buf[3]);
    std::swap(buf[1], buf[2]);

    return u32;
}

int32_t BinaryFileReader::read_i32(void) {
    union {
        int32_t i32;
        uint32_t u32;
    };

    u32 = this->read_u32();
    return i32;
}

float BinaryFileReader::read_f32(void) {
    union {
        int32_t i32;
        float f32;
    };

    i32 = this->read_i32();
    return f32;
}

uint16_t BinaryFileReader::read_u16(void) {
    union {
        char buf[2];
        uint16_t u16;
    };

    fs.read(buf, 2);
    std::swap(buf[0], buf[1]);

    return u16;
}

uint8_t BinaryFileReader::read_u8(void) {
    union
    {
        char buf;
        uint8_t u8;
    };
    
    fs.read(&buf, 1);
    return u8;
}

void BinaryFileReader::read_buffer(char* buf, int len) {
    fs.read(buf, len);
}
