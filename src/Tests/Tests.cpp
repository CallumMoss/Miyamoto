#include <gtest/gtest.h>
#include "../Position.hpp"

// Testing getters and FEN parser.
TEST(FEN_And_Pos_Rep, initial_position) {
  Position pos = Position();
  EXPECT_TRUE(pos.get_wscr()) << "Expected white to have short castling rights, but it did not.";
  EXPECT_TRUE(pos.get_wlcr()) << "Expected white to have long castling rights, but it did not.";
  EXPECT_TRUE(pos.get_bscr()) << "Expected black to have short castling rights, but it did not.";
  EXPECT_TRUE(pos.get_blcr()) << "Expected black to have long castling rights, but it did not.";

  EXPECT_EQ(pos.get_pawns(), 0xff00000000ff00ULL) << "Pawns are not in the expected position.";
	EXPECT_EQ(pos.get_knights(), 0x4200000000000042ULL) << "Knights are not in the expected position.";
	EXPECT_EQ(pos.get_bishops(), 0x2400000000000024ULL) << "Bishops are not in the expected position.";
	EXPECT_EQ(pos.get_rooks(), 0x8100000000000081ULL) << "Rooks are not in the expected position.";
	EXPECT_EQ(pos.get_queens(), 0x800000000000008ULL) << "Queens are not in the expected position.";
	EXPECT_EQ(pos.get_kings(), 0x1000000000000010ULL) << "Kings are not in the expected position.";

  EXPECT_EQ(pos.get_white_pawns(), 0xff00ULL) << "The white pawns are not in the expected position.";
	EXPECT_EQ(pos.get_black_pawns(), 0xff000000000000ULL) << "The black pawns are not in the expected position.";
  EXPECT_EQ(pos.get_white_knights(), 0x42ULL) << "The white knights are not in the expected position.";
	EXPECT_EQ(pos.get_black_knights(), 0x4200000000000000ULL) << "The black knights are not in the expected position.";
  EXPECT_EQ(pos.get_white_bishops(), 0x24ULL) << "The white bishops are not in the expected position.";
	EXPECT_EQ(pos.get_black_bishops(), 0x2400000000000000ULL) << "The black bishops are not in the expected position.";
  EXPECT_EQ(pos.get_white_rooks(), 0x81ULL) << "The white rooks are not in the expected position.";
	EXPECT_EQ(pos.get_black_rooks(), 0x8100000000000000ULL) << "The black rooks are not in the expected position.";
  EXPECT_EQ(pos.get_white_queen(), 0x8ULL) << "The white queen is not in the expected position.";
	EXPECT_EQ(pos.get_black_queen(), 0x800000000000000ULL) << "The black queen is not in the expected position.";
  EXPECT_EQ(pos.get_white_king(), 0x10ULL) << "The white king is not in the expected position.";
	EXPECT_EQ(pos.get_black_king(), 0x1000000000000000ULL) << "The black king is not in the expected position.";

  EXPECT_EQ(pos.get_white_pieces(), 0xffffULL) << "The white pieces are not in the expected position.";
	EXPECT_EQ(pos.get_black_pieces(), 0xffff000000000000ULL) << "The black pieces are not in the expected position.";
  EXPECT_EQ(pos.get_board(), 0xffff00000000ffffULL) << "The pieces are not in the expected position.";

	EXPECT_EQ(pos.get_turn(), Turn::WHITE) << "Expected it to be white's turn, but it is not.";
	EXPECT_EQ(pos.get_en_passant_target(), 0ULL) << "Expected no en passant posibility, but there is.";
	EXPECT_EQ(pos.get_half_move_clock(), 0) << "Expected the half move clock to be 0, but it is not.";
	EXPECT_EQ(pos.get_full_move_counter(), 1) << "Expected the full move clock to be 1, but it is not.";
}

// We now know that getters work for getting specific coloured pieces. So only need to test pieces and colours indiviually.
TEST(FEN_And_Pos_Rep, typical_middle_game) {
  Position pos = Position("r3k2r/2pb1ppp/2pp1q2/p7/1nP1B3/1P2P3/P2N1PPP/R2QK2R w KQkq - 0 14");
  EXPECT_TRUE(pos.get_wscr()) << "Expected white to have short castling rights, but it did not.";
  EXPECT_TRUE(pos.get_wlcr()) << "Expected white to have long castling rights, but it did not.";
  EXPECT_TRUE(pos.get_bscr()) << "Expected black to have short castling rights, but it did not.";
  EXPECT_TRUE(pos.get_blcr()) << "Expected black to have long castling rights, but it did not.";

  EXPECT_EQ(pos.get_pawns(), 0xe40c010412e100ULL) << "Pawns are not in the expected position.";
	EXPECT_EQ(pos.get_knights(), 0x2000800ULL) << "Knights are not in the expected position.";
	EXPECT_EQ(pos.get_bishops(), 0x8000010000000ULL) << "Bishops are not in the expected position.";
	EXPECT_EQ(pos.get_rooks(), 0x8100000000000081ULL) << "Rooks are not in the expected position.";
	EXPECT_EQ(pos.get_queens(), 0x200000000008ULL) << "Queens are not in the expected position.";
	EXPECT_EQ(pos.get_kings(), 0x1000000000000010ULL) << "Kings are not in the expected position.";

  EXPECT_EQ(pos.get_white_pieces(), 0x1412e999ULL) << "The white pieces are not in the expected position.";
	EXPECT_EQ(pos.get_black_pieces(), 0x91ec2c0102000000ULL) << "The black pieces are not in the expected position.";

	EXPECT_EQ(pos.get_turn(), Turn::WHITE) << "Expected it to be white's turn, but it is not.";
	EXPECT_EQ(pos.get_en_passant_target(), 0ULL) << "Expected no en passant posibility, but there is.";
	EXPECT_EQ(pos.get_half_move_clock(), 0) << "Expected the half move clock to be 0, but it is not.";
	EXPECT_EQ(pos.get_full_move_counter(), 14) << "Expected the full move clock to be 14, but it is not.";
}

// Testing slightly middle game with a half move, blacks turn and white having castled.
TEST(FEN_And_Pos_Rep, middle_game_white_castled) {
  Position pos = Position("r1bqk2r/ppp2ppp/2n1pn2/3p4/3P4/2NBPN2/PPPB1PPP/R2Q1RK1 b kq - 1 7");
  EXPECT_FALSE(pos.get_wscr()) << "Expected white to not have short castling rights, but it did.";
  EXPECT_FALSE(pos.get_wlcr()) << "Expected white to not have long castling rights, but it did.";
  EXPECT_TRUE(pos.get_bscr()) << "Expected black to have short castling rights, but it did not.";
  EXPECT_TRUE(pos.get_blcr()) << "Expected black to have long castling rights, but it did not.";

  EXPECT_EQ(pos.get_pawns(), 0xe710080810e700ULL) << "Pawns are not in the expected position.";
	EXPECT_EQ(pos.get_knights(), 0x240000240000ULL) << "Knights are not in the expected position.";
	EXPECT_EQ(pos.get_bishops(), 0x400000000080800ULL) << "Bishops are not in the expected position.";
	EXPECT_EQ(pos.get_rooks(), 0x8100000000000021ULL) << "Rooks are not in the expected position.";
	EXPECT_EQ(pos.get_queens(), 0x800000000000008ULL) << "Queens are not in the expected position.";
	EXPECT_EQ(pos.get_kings(), 0x1000000000000040ULL) << "Kings are not in the expected position.";

  EXPECT_EQ(pos.get_white_pieces(), 0x83cef69ULL) << "The white pieces are not in the expected position.";
	EXPECT_EQ(pos.get_black_pieces(), 0x9de7340800000000ULL) << "The black pieces are not in the expected position.";

	EXPECT_EQ(pos.get_turn(), Turn::BLACK) << "Expected it to be white's turn, but it is not.";
	EXPECT_EQ(pos.get_en_passant_target(), 0ULL) << "Expected no en passant posibility, but there is.";
	EXPECT_EQ(pos.get_half_move_clock(), 1) << "Expected the half move clock to be 1, but it is not.";
	EXPECT_EQ(pos.get_full_move_counter(), 7) << "Expected the full move clock to be 7, but it is not.";
}

// Tests en passant, blacks turn and white castling after moving rook
TEST(FEN_And_Pos_Rep, complex_position) {
  Position pos = Position("2kr3r/pb1pppbp/1pn5/q1p1n3/2BP2pP/PPN1BN2/2PQ1PP1/R3KR2 b Q h3 0 14");
  EXPECT_FALSE(pos.get_wscr()) << "Expected white to not have short castling rights, but it did.";
  EXPECT_TRUE(pos.get_wlcr()) << "Expected white to have long castling rights, but it did not.";
  EXPECT_FALSE(pos.get_bscr()) << "Expected black to have short castling rights, but it did.";
  EXPECT_FALSE(pos.get_blcr()) << "Expected black to have long castling rights, but it did.";

  EXPECT_EQ(pos.get_pawns(), 0xb90204c8036400ULL) << "Pawns are not in the expected position.";
	EXPECT_EQ(pos.get_knights(), 0x41000240000ULL) << "Knights are not in the expected position.";
	EXPECT_EQ(pos.get_bishops(), 0x42000004100000ULL) << "Bishops are not in the expected position.";
	EXPECT_EQ(pos.get_rooks(), 0x8800000000000021ULL) << "Rooks are not in the expected position.";
	EXPECT_EQ(pos.get_queens(), 0x100000800ULL) << "Queens are not in the expected position.";
	EXPECT_EQ(pos.get_kings(), 0x400000000000010ULL) << "Kings are not in the expected position.";

  EXPECT_EQ(pos.get_white_pieces(), 0x8c376c31ULL) << "The white pieces are not in the expected position.";
	EXPECT_EQ(pos.get_black_pieces(), 0x8cfb061540000000ULL) << "The black pieces are not in the expected position.";

	EXPECT_EQ(pos.get_turn(), Turn::BLACK) << "Expected it to be white's turn, but it is not.";
	EXPECT_EQ(pos.get_en_passant_target(), 0x800000ULL) << "Expected no en passant posibility, but there is.";
	EXPECT_EQ(pos.get_half_move_clock(), 0) << "Expected the half move clock to be 0, but it is not.";
	EXPECT_EQ(pos.get_full_move_counter(), 14) << "Expected the full move clock to be 14, but it is not.";
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}