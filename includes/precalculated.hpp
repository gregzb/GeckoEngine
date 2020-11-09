# pragma once

# include "vector"
# include "utils.hpp"

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
extern bitboard pawn_second_step_attacks_lookup[64];
extern bitboard pawn_rank_lookup[2][64];
void populate_pawn_attacks();

void precalculate();

bitboard rank_lookup(int rank);