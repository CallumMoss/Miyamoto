#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdint>

using u8 = uint8_t;
using u64 = uint64_t;

namespace Utils {
    u64 shift_up(u64 board); // replace bit with 0, replace bit shifted 8 left with 1
    u64 shift_down(u64 board);
    u64 get_ls1b(u64 board); // gets the least significant bit that is 1 (furthest right in bitstring)
}
#endif // #ifndef UTILS_HPP