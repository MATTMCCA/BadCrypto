/*
    xor file encryption, NOT cryptographically safe.
     !!! Small period in sudo random number generator, hash algorithm has collision !!!
     (this is why rolling your own encryption is not a good idea lol!)

     MJM - 2021, 100 lines
*/

#include <cstdio>
#include <cstdint>
#include <iostream>
#include <fstream>

#define buffer_size 1024
static uint8_t seed = 123;

static const unsigned char T[256] = {
    153, 184, 222, 195, 134, 152, 154, 124,  53, 140,  17,  97,  25, 158,  32, 112,
    239, 127,   2,  12, 146, 119,   3, 103, 179, 133,  83, 148,  10, 106,  19, 181,
    159,  72,  31, 122, 198, 131, 168, 234, 172, 151, 205, 191, 170,  33, 201, 100,
    116, 171,  55, 128,  44, 182, 149,  71, 126,   1, 102,  62, 242, 114,  52, 187,
     81, 189, 166,  38,  82,  49,  64,  70,  29,  85,  57, 113, 231, 145,  18,  65,
     30,  58, 233, 147, 206, 236,  67,  89,  68, 240,  94, 129,   8, 215, 101, 115,
    232, 175, 185,  78, 173, 164,  46,  73, 107, 137, 243,  23,  90, 204,  84,  61,
    207, 157,  40, 123, 245, 163, 186,   9, 178,  24, 203, 110,  37, 192,  48,  92,
    144, 196,  11, 228, 167, 118, 161,  59, 199, 226, 254, 109, 120, 241, 238,  42,
    249,   6, 194, 165,  36, 160,  76, 229, 142,  88, 132,  86, 227,  77, 248, 250,
     50, 210, 141,  16, 197, 139,  56, 176,  14, 125, 208, 188, 104, 105,  43, 138,
    251, 224,  15, 193, 180, 108,   4,  80,  41,  27, 136, 183, 200, 216, 177, 218,
    235,  60,  87, 237, 214, 253,  35,  96,  91,  13,  21, 246,  95, 212, 150, 130,
    111,  22, 143, 247,  75,   5, 156, 169,  45, 117,  93,  69,  63, 220, 213, 174,
    209,  79,   7,  26, 162, 244,  34, 217,  28, 230, 252,  98, 221, 121,  74, 155,
    255, 211,  66,  51,  47,  99,  54, 190,  20,  39, 202, 135, 223,   0, 225, 219,
};

uint8_t pearson8(const unsigned char* x, size_t len);
uint8_t LFSR(void);

int main(int argc, char* argv[]) {
    char* file_name = nullptr;
    char* password = nullptr;
    if (argc >= 3) {
        file_name = argv[1];
        password = argv[2];
    }
    else {
        printf("%s FILENAME PASSWORD\n", argv[0]);
        return 1;
    }
    seed = pearson8((const unsigned char*)password, strlen(password));
    //printf("FileName:\t%s\nHash:0x%.2X\t[%s]\n", file_name, seed, password); //debuging statement
    /* File IO needs more error handeling */
    std::fstream file(file_name, std::fstream::in | std::fstream::out | std::ios::binary | std::ios::ate);
    if (file.is_open()) {
        std::streamsize index = 0;
        std::streampos length = file.tellg();
        file.seekg(index);
        char* buf = nullptr;
        while (index != length) {
            uint32_t y = buffer_size;
            if (index + buffer_size > length)
                y = (uint32_t)(length - index);
            buf = new char[y] {0};
            if (buf != nullptr) {
                file.seekg(index); file.read(buf, y);
                for (uint32_t i = 0; i < y; i++)
                    buf[i] = buf[i] ^ LFSR();
                file.seekp(index); file.write(buf, y);
            }
            else { break; }
            index += y;
            delete[] buf;
            buf = nullptr;
        }
        file.close();
    }
    else {
        printf("FILE ERR\n");
    }
    return 0;
}
/* 8-bit LFSR, period: 255 (see wikipedia) */
uint8_t LFSR(void) {
    static uint8_t lfsr = seed;
    uint8_t bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 4)) & 1u;
    lfsr = (lfsr >> 1) | (bit << 7);
    return lfsr;
}
/* 8-bit pearson hash (see wikipedia) */
uint8_t pearson8(const unsigned char* x, size_t len) {
    size_t i, j;
    uint8_t retval = 0;
    unsigned char h;
    for (j = 0; j < sizeof(retval); ++j) {
        h = T[(x[0] + j) % 256];
        for (i = 1; i < len; ++i)
            h = T[h ^ x[i]];
        retval |= h;
    }
    return retval;
}
//LINE 100