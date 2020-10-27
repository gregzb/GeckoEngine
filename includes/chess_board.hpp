# pragma once

# include <array>
# include <string>
# include <vector>

# include "utils.hpp"

enum enum_color {
    WHITE = 0,
    BLACK = 1
};

enum enum_piece {
    PAWN = 0,
    KNIGHT = 1,
    BISHOP = 2,
    ROOK = 3,
    QUEEN = 4,
    KING = 5
};

enum enum_square {
  a1, b1, c1, d1, e1, f1, g1, h1,
  a2, b2, c2, d2, e2, f2, g2, h2,
  a3, b3, c3, d3, e3, f3, g3, h3,
  a4, b4, c4, d4, e4, f4, g4, h4,
  a5, b5, c5, d5, e5, f5, g5, h5,
  a6, b6, c6, d6, e6, f6, g6, h6,
  a7, b7, c7, d7, e7, f7, g7, h7,
  a8, b8, c8, d8, e8, f8, g8, h8
};

extern bitboard CASTLE[2][2];
extern bitboard CASTLE_CHECK[2][2];
extern bitboard CASTLE_IN_BETWEEN[2][2];
extern bitboard BLACK_TURN;

struct chess_move {
    enum_square from;
    enum_square to;
};

class chess_board {
    public:
    bitboard piece_colors[2];
    bitboard pieces[6];
    bitboard en_passant;

    unsigned char data;

    public:
    chess_board();
    void add_piece(enum_color color, enum_piece piece, enum_square location);
    void remove_piece(enum_color color, enum_piece piece, enum_square location);
    void clear_loc(enum_square location);
    void print();

    bitboard generate_protected_board(enum_color side);
    void iterate_over_moves();

    ull play_move(enum_piece piece_type, chess_move move, bool special, bool in_check);
};

bool get(bitboard board, enum_square piece);
bool get(bitboard board, int row, int col);

void set(bitboard & board, bool bit, enum_square piece);
void set(bitboard & board, bool bit, int row, int col);

void print(bitboard board);
void generate_rook_masks(std::vector<bitboard> &verticals, std::vector<bitboard> &horizontals);
void generate_bishop_masks(std::vector<bitboard> &diagonals, std::vector<bitboard> &anti_diagonals);

extern bitboard first_rank_attacks[8][64];
void calculate_first_rank_attacks();

extern bitboard a_file_attacks[8][64];
void calculate_a_file_attacks();

extern bitboard diagonal_masks[64];
extern bitboard anti_diagonal_masks[64]; 
extern bitboard rank_masks[64];
extern bitboard file_masks[64];

void populate_masks(std::vector<bitboard> & verticals, std::vector<bitboard> & horizontals, std::vector<bitboard> & diagonals, std::vector<bitboard> & anti_diagonals);

bitboard diagonal_attacks(bitboard occ, enum_square sq);
bitboard anti_diagonal_attacks(bitboard occ, enum_square sq);
bitboard rank_attacks(bitboard occ, enum_square sq);
bitboard file_attacks(bitboard occ, enum_square sq);

extern bitboard knight_attacks_lookup[64];
void populate_knight_attacks();

extern bitboard king_attacks_lookup[64];
void populate_king_attacks();

extern bitboard pawn_diag_attacks_lookup[64];
extern bitboard pawn_step_attacks_lookup[64];
extern bitboard pawn_full_step_attacks_lookup[64];
extern bitboard pawn_rank_lookup[2][64];
void populate_pawn_attacks();