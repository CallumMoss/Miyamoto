#ifndef POSITION_HPP
#define POSITION_HPP

#include <string>
#include <array>

#include "utils.hpp"

enum class Turn: u8 {
    WHITE,
    BLACK
};


class Position { // Game state class
    public:
        Position(const std::string& fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        bool piece_is_at_square(uint64_t board, int square);
        void print_position();

    private:
        // Piece-centric bitboards for storing the position of the pieces by types and colour
        std::array<u64, 6> pieces;
        std::array<u64, 2> colours;
		u64 en_passant_mask;
        int half_move_clock; // number of half moves, to test for 50 move rule
		int full_move_counter; // how many moves have been played
        u8 castling_rights; // XXXX-BL-BS-WL-WS, last 4 bits, 0 if cannot castle
        Turn turn;
};

#endif // #ifndef POSITION_HPP