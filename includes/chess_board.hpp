# pragma once

# include <array>
# include <string>
# include <vector>
# include <string>
# include <functional>

# include "utils.hpp"

extern bitboard CASTLE[2][2];
extern bitboard CASTLE_SIDE[2];
extern bitboard CASTLE_CHECK[2][2];
extern bitboard CASTLE_IN_BETWEEN[2][2];
extern bitboard BLACK_TURN;

struct chess_move {
    enum_square from;
    enum_square to;
};

struct move_info {
    chess_move move;
    int evaluation;
};

class chess_board {
    public:
    bitboard piece_colors[2];
    bitboard pieces[6];
    bitboard en_passant;

    unsigned char data;
    unsigned char ply_ctr;

    std::string info = "";

    void init();

    public:
    chess_board();
    chess_board(std::string const & fen);
    void add_piece(enum_color color, enum_piece piece, enum_square location);
    void remove_piece(enum_color color, enum_piece piece, enum_square location);
    void clear_loc(enum_square location);
    void print();

    bitboard generate_protected_board(enum_color side) const;
    bool king_in_check(enum_color side);

    void try_attacks(enum_piece pt, enum_square loc, bitboard attacks, std::vector<chess_board> & boards, std::function<void(chess_board &, chess_move & move)> action);
    void try_promotions(enum_square loc, bitboard attacks, std::vector<chess_board> & boards);
    void try_attack(enum_piece pt, chess_move move, std::vector<chess_board> & boards, std::function<void(chess_board &, chess_move & move)> action);
    void try_castle(int dir, std::vector<chess_board> & boards);

    void generate_legal_moves(std::vector<chess_board> & boards);

    float evaluate();

    float minimax(int depth, float alpha, float beta);
    ull perft(int depth);

    // move_info find_best_move(int remaining_plies);
    // //if remaining is 0, static eval
    // //else, recursion

    // make another function to play move from one position to another for more general piece playing, and accept PGN/FEN
    // ull play_move(enum_piece piece_type, chess_move move, bool special, bool in_check, char promo);

    void play_move(std::string move);
};