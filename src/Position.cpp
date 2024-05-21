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
                case 'P': pieces[0] |= square; colours[0] |= square; break;
                case 'N': pieces[1] |= square; colours[0] |= square; break;
                case 'B': pieces[2] |= square; colours[0] |= square; break;
                case 'R': pieces[3] |= square; colours[0] |= square; break;
                case 'Q': pieces[4] |= square; colours[0] |= square; break;
                case 'K': pieces[5] |= square; colours[0] |= square; break;
                case 'p': pieces[0] |= square; colours[1] |= square; break;
                case 'n': pieces[1] |= square; colours[1] |= square; break;
                case 'b': pieces[2] |= square; colours[1] |= square; break;
                case 'r': pieces[3] |= square; colours[1] |= square; break;
                case 'q': pieces[4] |= square; colours[1] |= square; break;
                case 'k': pieces[5] |= square; colours[1] |= square; break;
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
    std::array<char, 64> board;
    // Get letter values for all pieces in the position/
    for (int i = 0; i < 64; i++) {
        if (piece_is_at_square(get_white_pawns(), i)) { board[i] = 'P'; }
        else if (piece_is_at_square(get_white_knights(), i)) { board[i] = 'N'; }
        else if (piece_is_at_square(get_white_bishops(), i)) { board[i] = 'B'; }
        else if (piece_is_at_square(get_white_rooks(), i)) { board[i] = 'R'; }
        else if (piece_is_at_square(get_white_queen(), i)) { board[i] = 'Q'; }
        else if (piece_is_at_square(get_white_king(), i)) { board[i] = 'K'; }
        else if (piece_is_at_square(get_black_pawns(), i)) { board[i] = 'p'; }
        else if (piece_is_at_square(get_black_knights(), i)) { board[i] = 'n'; }
        else if (piece_is_at_square(get_black_bishops(), i)) { board[i] = 'b';}
        else if (piece_is_at_square(get_black_rooks(), i)) { board[i] = 'r'; }
        else if (piece_is_at_square(get_black_queen(), i)) { board[i] = 'q'; }
        else if (piece_is_at_square(get_black_king(), i)) { board[i] = 'k'; }
        else { board[i] = ' '; }
    }
    print_board(board);
}

std::array<char, 64> Position::board_to_char_array(u64 board) {
    std::array<char, 64> char_array;
    for (int i = 0; i < 64; i++) {
        if (piece_is_at_square(board, i)) { char_array[i] = '0'; }
        else { char_array[i] = '+'; }
    }
    return char_array;
}
void Position::print_board(std::array<char, 64> board) {
    // Can be used in conjunction with board_to_char_array to print a given board.
    std::string rank = "";
    std::cout << "  -------------------------------" << std::endl;
    for(int j = 64; j > 0; j--) {
        if(j % 8 == 0 && j != 64) {
            std::cout << " | " + rank << (j/8) + 1 << std::endl << "  -------------------------------" << std::endl;
            rank = "";
        }
        rank = std::string(1, board[j - 1]) + " | " + rank; // - 1 used so i can use 64 instead of 63
    }
    std::cout << " | " + rank << 1 << std::endl << "  -------------------------------" << std::endl;
    std::cout << "   A   B   C   D   E   F   G   H  " << std::endl << std::endl;
}

std::array<u64, 6> Position::get_pieces() { return pieces; }
std::array<u64, 2> Position::get_colours() { return colours; }
u64 Position::get_en_passant_mask() { return en_passant_mask; }
int Position::get_half_move_clock() { return half_move_clock; }
int Position::get_full_move_counter() { return full_move_counter; }
u8 Position::get_castling_rights() { return castling_rights; }
Turn Position::get_turn() { return turn; }

u64 Position::get_pawns() { return pieces[0]; }
u64 Position::get_white_pawns() { return pieces[0] & colours[0]; }
u64 Position::get_black_pawns() { return pieces[0] & colours[1]; }

u64 Position::get_knights() { return pieces[1]; }
u64 Position::get_white_knights() { return pieces[1] & colours[0]; }
u64 Position::get_black_knights() { return pieces[1] & colours[1]; }

u64 Position::get_bishops() { return pieces[2];}
u64 Position::get_white_bishops() { return pieces[2] & colours[0]; }
u64 Position::get_black_bishops() { return pieces[2] & colours[1]; }

u64 Position::get_rooks() { return pieces[3]; }
u64 Position::get_white_rooks() { return pieces[3] & colours[0]; }
u64 Position::get_black_rooks() { return pieces[3] & colours[1]; }

u64 Position::get_queens() { return pieces[4];}
u64 Position::get_white_queen() { return pieces[4] & colours[0]; }
u64 Position::get_black_queen() { return pieces[4] & colours[1]; }

u64 Position::get_kings() { return pieces[5]; }
u64 Position::get_white_king() { return pieces[5] & colours[0]; }
u64 Position::get_black_king() { return pieces[5] & colours[1]; }

u64 Position::get_white_pieces() { return colours[0]; } 
u64 Position::get_black_pieces() { return colours[1]; } 

u64 Position::get_board() { return colours[0] | colours[1]; }

bool Position::wscr() { return castling_rights & 1; }
bool Position::wlcr() { return castling_rights & (1 << 1); }
bool Position::bscr() { return castling_rights & (1 << 2); }
bool Position::blcr() { return castling_rights & (1 << 3); }