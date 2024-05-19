#ifndef UTILS_CPP
#define UTILS_CPP


#include "Utils.hpp"


u64 Utils::shift_up(u64 board) { // Moving the board up a rank (can be used for seeing possible pawn moves for example)
    return board << 8;
}

u64 Utils::shift_down(u64 board) { // Moving the board down a rank
    return board >> 8;
}

#endif // #ifndef UTILS_CPP