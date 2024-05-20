#include "Position.hpp"
#include <vector>
#include <sstream>
#include <iostream>

Position::Position(const std::string& fen) {
    // Initialising values
    for(int i = 0; i < colours.size(); i++) {
        colours[i] = 0b0;
    }
    for(int i = 0; i < pieces.size(); i++) {
        pieces[i] = 0b0;
    }
		turn = Turn::WHITE; // 0 for white, 1 for black
        castling_rights = 0b0; // XXXX-BL-BS-WL-WS, last 4 bits, 0 if cannot castle
		en_passant_mask = 0b0;
	    half_move_clock = 0; // number of half moves, to test for 50 move rule
		full_move_counter = 0; // how many moves have been played

    // Create a stringstream from the input string
    std::istringstream iss(fen);
    std::vector<std::string> words;
    // Parse the string based on space (' ')
    std::string word;
    while (std::getline(iss, word, ' ')) {
        words.push_back(word); // pushes word to the back
    }

    int rank = 7, file = 0;
    for (char c : words[0]) {

        if (c == '/') {
            // this splits each rank
            rank--;
            file = 0;
        }
        else if (isdigit(c)) {
            file += (c - '0'); // skips empty files of this amount
        }
        else {
            uint64_t square = 1ULL << (rank * 8 + file);
            // shifts 1 bit, to represent there is a piece on the square, across the bitboard's bitstring
            // For example, c4 in LERF is 26. So you want to push a 1 onto the 26th bit of the bitstring
            switch (c) {
                // Bit boards are all 0s, by using or, the 1 is added in.
                // Need to use or instead of equals as after the first pawn is detected,
                // square wont remember where the 1 was originally for the first pawn
                case 'P': pieces[Utils::PAWNS_INDEX] |= square; colours[Utils::WHITE_PIECES_INDEX] |= square; break;
                case 'N': pieces[Utils::KNIGHTS_INDEX] |= square; colours[Utils::WHITE_PIECES_INDEX] |= square; break;
                case 'B': pieces[Utils::BISHOPS_INDEX] |= square; colours[Utils::WHITE_PIECES_INDEX] |= square; break;
                case 'R': pieces[Utils::ROOKS_INDEX] |= square; colours[Utils::WHITE_PIECES_INDEX] |= square; break;
                case 'Q': pieces[Utils::QUEENS_INDEX] |= square; colours[Utils::WHITE_PIECES_INDEX] |= square; break;
                case 'K': pieces[Utils::KINGS_INDEX] |= square; colours[Utils::WHITE_PIECES_INDEX] |= square; break;
                case 'p': pieces[Utils::PAWNS_INDEX] |= square; colours[Utils::BLACK_PIECES_INDEX] |= square; break;
                case 'n': pieces[Utils::KNIGHTS_INDEX] |= square; colours[Utils::BLACK_PIECES_INDEX] |= square; break;
                case 'b': pieces[Utils::BISHOPS_INDEX] |= square; colours[Utils::BLACK_PIECES_INDEX] |= square; break;
                case 'r': pieces[Utils::ROOKS_INDEX] |= square; colours[Utils::BLACK_PIECES_INDEX] |= square; break;
                case 'q': pieces[Utils::QUEENS_INDEX] |= square; colours[Utils::BLACK_PIECES_INDEX] |= square; break;
                case 'k': pieces[Utils::KINGS_INDEX] |= square; colours[Utils::BLACK_PIECES_INDEX] |= square; break;
            }
            file++;
        }
    }

    char file_letter; // declared out of switch statement
    int file_number;

    for (int i = 1; i < 6; i++) {
        std::string word = words[i];
        switch (i) {
            case 1: // which colour to move
                if (word[0] == 'w') { turn = Turn::WHITE; }
                else { turn = Turn::BLACK; }
                break;
            case 2: // which colours have castling rights
                for (char c : word) {
                    switch (c) {
                        case('-'):
                            castling_rights = 0b0;
                            break;
                        case('K'):
                            castling_rights |= 0b1;
                            break;
                        case('Q'):
                            castling_rights |= 0b1 << 1;
                            break;
                        case('k'):
                            castling_rights |= 0b1 << 2;
                            break;
                        case('q'):
                            castling_rights |= 0b1 << 3;
                            break;
                    }

                }
                break;
            case 3: // which squares are being targetted with en passant
                file_letter = word[0];
                if (file_letter == '-') { en_passant_mask = 0ULL; break; }
                else if (file_letter == 'a') { file_number = 0; }
                else if (file_letter == 'b') { file_number = 1; }
                else if (file_letter == 'c') { file_number = 2; }
                else if (file_letter == 'd') { file_number = 3; }
                else if (file_letter == 'e') { file_number = 4; }
                else if (file_letter == 'f') { file_number = 5; }
                else if (file_letter == 'g') { file_number = 6; }
                else { file_number = 7; } // if == 'h'

                en_passant_mask = 1ULL << ((word[1] - 1) * 8 + file_number);
                break;
            case 4: // how many half moves have been played
                half_move_clock = std::stoi(word); // string to integer
                break;
            case 5: // how many moves have been played
                full_move_counter = std::stoi(word);
                break;
        }
    }
}

bool Position::piece_is_at_square(uint64_t board, int square) {
    return (board & (1ULL << square)) != 0;
}

void Position::print_position() {
    // white_pawns = pieces[Utils::PAWNS_INDEX] & colours[Utils::WHITE_PIECES_INDEX];

    char board[64] = {};

    for (int i = 0; i < 64; i++) {
        //if(board[i].isUpper())
        if (piece_is_at_square(pieces[Utils::PAWNS_INDEX] & colours[Utils::WHITE_PIECES_INDEX], i)){
           board[i] = 'P';
        }
        else if (piece_is_at_square(pieces[Utils::KNIGHTS_INDEX] & colours[Utils::WHITE_PIECES_INDEX], i)) {
            board[i] = 'N';
        }
        else if (piece_is_at_square(pieces[Utils::BISHOPS_INDEX] & colours[Utils::WHITE_PIECES_INDEX], i)) {
            board[i] = 'B';
        }
        else if (piece_is_at_square(pieces[Utils::ROOKS_INDEX] & colours[Utils::WHITE_PIECES_INDEX], i)) {
            board[i] = 'R';
        }
        else if (piece_is_at_square(pieces[Utils::QUEENS_INDEX] & colours[Utils::WHITE_PIECES_INDEX], i)) {
            board[i] = 'Q';
        }
        else if (piece_is_at_square(pieces[Utils::KINGS_INDEX] & colours[Utils::WHITE_PIECES_INDEX], i)) {
            board[i] = 'K';
        }
        else if (piece_is_at_square(pieces[Utils::PAWNS_INDEX] & colours[Utils::BLACK_PIECES_INDEX], i)) {
            board[i] = 'p';
        }
        else if (piece_is_at_square(pieces[Utils::KNIGHTS_INDEX] & colours[Utils::BLACK_PIECES_INDEX], i)) {
            board[i] = 'n';
        }
        else if (piece_is_at_square(pieces[Utils::BISHOPS_INDEX] & colours[Utils::BLACK_PIECES_INDEX], i)) {
            board[i] = 'b';
        }
        else if (piece_is_at_square(pieces[Utils::ROOKS_INDEX] & colours[Utils::BLACK_PIECES_INDEX], i)) {
            board[i] = 'r';
        }
        else if (piece_is_at_square(pieces[Utils::QUEENS_INDEX] & colours[Utils::BLACK_PIECES_INDEX], i)) {
            board[i] = 'q';
        }
        else if (piece_is_at_square(pieces[Utils::KINGS_INDEX] & colours[Utils::BLACK_PIECES_INDEX], i)) {
            board[i] = 'k';
        }
        else {
            board[i] = ' ';
        }
    }

    // print board
    std::string rank = "";
    std::cout << "  -------------------------------" << std::endl;
    for (int j = 64; j > 0; j--) { // do i need new variables?
        if (j % 8 == 0 && j != 64) {
            std::cout << " | " + rank << (j/8) + 1 << std::endl << "  -------------------------------" << std::endl;
            rank = "";
        }
        rank = std::string(1, board[j - 1]) + " | " + rank; // - 1 used so i can use 64 instead of 63
    }
    std::cout << " | " + rank << 1 << std::endl << "  -------------------------------" << std::endl;
    std::cout << "   A   B   C   D   E   F   G   H  " << std::endl << std::endl;
}