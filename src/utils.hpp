#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdint>

using u8 = uint8_t;
using u64 = uint64_t;

namespace Utils {
    /*
        used like Position::colours[WHITE_PIECES_INDEX] and Position::pieces[PAWNS_INDEX]
        to get current positions of white piecesin the form of a bitboard
    */
    constexpr u8 WHITE_PIECES_INDEX = 0; // Index of white pieces
    constexpr u8 BLACK_PIECES_INDEX = 1;
    constexpr u8 PAWNS_INDEX = 0;
    constexpr u8 KNIGHTS_INDEX = 1;
    constexpr u8 BISHOPS_INDEX = 2;
    constexpr u8 ROOKS_INDEX = 3;
    constexpr u8 QUEENS_INDEX = 4;
    constexpr u8 KINGS_INDEX = 5;

    u64 shift_up(u64 board); // replace bit with 0, replace bit shifted 8 left with 1
    u64 shift_down(u64 board);
    u64 get_ls1b(u64 board); // gets the least significant bit that is 1 (furthest right in bitstring)
}
#endif // #ifndef UTILS_HPP