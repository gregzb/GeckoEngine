#include <limits>
#include <cmath>
#include "chess_board.hpp"
#include "precalculated.hpp"

bitboard CASTLE[2][2] = {{0b1, 0b10}, {0b100, 0b1000}}; // CASTLE[SIDE][KING/QUEEN]
bitboard CASTLE_SIDE[2] = {0b0011, 0b1100};
bitboard CASTLE_CHECK[2][2] = {{0, 0}, {0, 0}};         // CASTLE[SIDE][KING/QUEEN]
bitboard CASTLE_IN_BETWEEN[2][2] = {{0, 0}, {0, 0}};    // CASTLE[SIDE][KING/QUEEN]
bitboard BLACK_TURN = 0b10000;

void chess_board::init()
{
    piece_colors[WHITE] = 0;
    piece_colors[BLACK] = 0;

    pieces[PAWN] = 0;
    pieces[ROOK] = 0;
    pieces[KNIGHT] = 0;
    pieces[BISHOP] = 0;
    pieces[QUEEN] = 0;
    pieces[KING] = 0;

    en_passant = 0;

    data = 0;

    ply_ctr = 0;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            data |= CASTLE[i][j];
        }
    }

    set(CASTLE_CHECK[WHITE][1], 1, 0, 2);
    set(CASTLE_CHECK[WHITE][1], 1, 0, 3);
    set(CASTLE_CHECK[WHITE][1], 1, 0, 4);

    set(CASTLE_CHECK[WHITE][0], 1, 0, 6);
    set(CASTLE_CHECK[WHITE][0], 1, 0, 5);
    set(CASTLE_CHECK[WHITE][0], 1, 0, 4);

    set(CASTLE_CHECK[BLACK][1], 1, 7, 2);
    set(CASTLE_CHECK[BLACK][1], 1, 7, 3);
    set(CASTLE_CHECK[BLACK][1], 1, 7, 4);

    set(CASTLE_CHECK[BLACK][0], 1, 7, 6);
    set(CASTLE_CHECK[BLACK][0], 1, 7, 5);
    set(CASTLE_CHECK[BLACK][0], 1, 7, 4);

    set(CASTLE_IN_BETWEEN[WHITE][1], 1, 0, 1);
    set(CASTLE_IN_BETWEEN[WHITE][1], 1, 0, 2);
    set(CASTLE_IN_BETWEEN[WHITE][1], 1, 0, 3);

    set(CASTLE_IN_BETWEEN[WHITE][0], 1, 0, 6);
    set(CASTLE_IN_BETWEEN[WHITE][0], 1, 0, 5);

    set(CASTLE_IN_BETWEEN[BLACK][1], 1, 7, 1);
    set(CASTLE_IN_BETWEEN[BLACK][1], 1, 7, 2);
    set(CASTLE_IN_BETWEEN[BLACK][1], 1, 7, 3);

    set(CASTLE_IN_BETWEEN[BLACK][0], 1, 7, 6);
    set(CASTLE_IN_BETWEEN[BLACK][0], 1, 7, 5);
}

chess_board::chess_board()
{
    init();

    add_piece(WHITE, ROOK, a1);
    add_piece(WHITE, KNIGHT, b1);
    add_piece(WHITE, BISHOP, c1);
    add_piece(WHITE, QUEEN, d1);
    add_piece(WHITE, KING, e1);
    add_piece(WHITE, BISHOP, f1);
    add_piece(WHITE, KNIGHT, g1);
    add_piece(WHITE, ROOK, h1);

    add_piece(WHITE, PAWN, a2);
    add_piece(WHITE, PAWN, b2);
    add_piece(WHITE, PAWN, c2);
    add_piece(WHITE, PAWN, d2);
    add_piece(WHITE, PAWN, e2);
    // add_piece(BLACK, ROOK, e4);
    add_piece(WHITE, PAWN, f2);
    add_piece(WHITE, PAWN, g2);
    add_piece(WHITE, PAWN, h2);

    // add_piece(BLACK, BISHOP, h4);

    // add_piece(BLACK, QUEEN, h4);

    add_piece(BLACK, ROOK, a8);
    add_piece(BLACK, KNIGHT, b8);
    add_piece(BLACK, BISHOP, c8);
    add_piece(BLACK, QUEEN, d8);
    add_piece(BLACK, KING, e8);
    add_piece(BLACK, BISHOP, f8);
    add_piece(BLACK, KNIGHT, g8);
    add_piece(BLACK, ROOK, h8);

    add_piece(BLACK, PAWN, a7);
    add_piece(BLACK, PAWN, b7);
    add_piece(BLACK, PAWN, c7);
    add_piece(BLACK, PAWN, d7);
    add_piece(BLACK, PAWN, e7);
    add_piece(BLACK, PAWN, f7);
    add_piece(BLACK, PAWN, g7);
    add_piece(BLACK, PAWN, h7);
}

bool is_alpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

chess_board::chess_board(std::string const &fen)
{
    init();
    data = 0;

    std::vector<std::string> fen_parts = split_string(fen, " ");
    std::string pieces = fen_parts[0];

    data |= BLACK_TURN * (fen_parts[1] == "b");

    std::string castling = fen_parts[2];

    for (char ch : castling) {
        if (ch == '-') continue;
        int is_black = !(ch >= 'A' && ch <= 'Z');
        int is_queen = ch == 'q' || ch == 'Q';

        data |= CASTLE[is_black][is_queen];
    }

    std::string en_passant_square = fen_parts[3];
    if (en_passant_square != "-") {
        int file = en_passant_square[0] - 'a';
        int rank = en_passant_square[1] - '1';
        en_passant = 0;
        set(en_passant, 1, (enum_square) (rank*8+file));
    }

    int half_move_clock = std::stoi(fen_parts[4]);
    ply_ctr = half_move_clock;

    int location = 0;
    for (int i = 0; i < pieces.size(); i++)
    {
        char ch = pieces[i];

        if (!is_alpha(ch))
        {
            if (ch == '/')
                continue;
            int val = ch - '0';
            location += val;
            continue;
        }

        enum_color is_black = (enum_color)(!(ch >= 'A' && ch <= 'Z'));

        if (ch >= 'A' && ch <= 'Z')
            ch += 32;

        int rank = 7 - (location >> 3);
        int file = location & 7;
        enum_square loc = (enum_square)(rank * 8 + file);
        // std::cout << rank << " " << file << std::endl;
        // std::cout << ch << " " << (rank * 8 + file) << std::endl;

        if (ch == 'r')
        {
            add_piece(is_black, ROOK, loc);
        }
        else if (ch == 'n')
        {
            add_piece(is_black, KNIGHT, loc);
        }
        else if (ch == 'b')
        {
            add_piece(is_black, BISHOP, loc);
        }
        else if (ch == 'q')
        {
            add_piece(is_black, QUEEN, loc);
        }
        else if (ch == 'k')
        {
            add_piece(is_black, KING, loc);
        }
        else if (ch == 'p')
        {
            add_piece(is_black, PAWN, loc);
        }
        location++;
    }
}

void chess_board::add_piece(enum_color color, enum_piece piece, enum_square location)
{
    set(pieces[piece], 1, location);
    set(piece_colors[color], 1, location);
}

void chess_board::remove_piece(enum_color color, enum_piece piece, enum_square location)
{
    set(pieces[piece], 0, location);
    set(piece_colors[color], 0, location);
}

void chess_board::clear_loc(enum_square location)
{
    // set(pieces[piece], 0, location);
    set(piece_colors[WHITE], 0, location);
    set(piece_colors[BLACK], 0, location);

    set(pieces[PAWN], 0, location);
    set(pieces[KNIGHT], 0, location);
    set(pieces[KING], 0, location);
    set(pieces[QUEEN], 0, location);
    set(pieces[ROOK], 0, location);
    set(pieces[BISHOP], 0, location);
}

void chess_board::print()
{
    std::string out;

    for (int i = 0; i < 64; i++)
    {
        out += '-';
    }

    for (int rank = 0; rank < 8; rank++)
    {
        for (int file = 0; file < 8; file++)
        {
            int str_idx = (7 - rank) * 8 + file;
            int board_idx = rank * 8 + file;

            bool white = get(piece_colors[WHITE], (enum_square)board_idx);

            if (get(pieces[PAWN], (enum_square)board_idx))
            {
                out[str_idx] = white ? 'P' : 'p';
            }
            else if (get(pieces[ROOK], (enum_square)board_idx))
            {
                out[str_idx] = white ? 'R' : 'r';
            }
            else if (get(pieces[KNIGHT], (enum_square)board_idx))
            {
                out[str_idx] = white ? 'N' : 'n';
            }
            else if (get(pieces[BISHOP], (enum_square)board_idx))
            {
                out[str_idx] = white ? 'B' : 'b';
            }
            else if (get(pieces[QUEEN], (enum_square)board_idx))
            {
                out[str_idx] = white ? 'Q' : 'q';
            }
            else if (get(pieces[KING], (enum_square)board_idx))
            {
                out[str_idx] = white ? 'K' : 'k';
            }
        }
    }

    std::string tmp;
    int counter = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            std::string piece_char;
            piece_char += out[counter];
            tmp += piece_char + " ";
            counter++;
        }
        tmp += '\n';
    }
    std::cout << tmp << std::endl;
}

int next_bit(bitboard &board)
{
    int add_to_pos = __builtin_ctzll(board);
    board >>= add_to_pos;
    board >>= 1;
    return add_to_pos + 1;
}

bitboard chess_board::generate_protected_board(enum_color side) const
{
    bitboard out = 0;

    bitboard all_pieces = piece_colors[0] | piece_colors[1];

    bitboard pawns = pieces[PAWN] & piece_colors[side];
    for (int current_pos = -1; pawns;)
    {
        current_pos += next_bit(pawns);
        enum_square loc = (enum_square)current_pos;

        bitboard attacks = 0;
        attacks |= pawn_diag_attacks_lookup[loc];
        attacks &= pawn_rank_lookup[side][loc];

        out |= attacks;
    }

    bitboard rooks = pieces[ROOK] & piece_colors[side];
    for (int current_pos = -1; rooks;)
    {
        current_pos += next_bit(rooks);
        enum_square loc = (enum_square)current_pos;

        bitboard attacks = 0;
        attacks |= rank_attacks(all_pieces, loc);
        attacks |= file_attacks(all_pieces, loc);

        out |= attacks;
    }

    bitboard bishops = pieces[BISHOP] & piece_colors[side];
    for (int current_pos = -1; bishops;)
    {
        current_pos += next_bit(bishops);
        enum_square loc = (enum_square)current_pos;

        bitboard attacks = 0;
        attacks |= diagonal_attacks(all_pieces, loc);
        attacks |= anti_diagonal_attacks(all_pieces, loc);

        out |= attacks;
    }

    bitboard knights = pieces[KNIGHT] & piece_colors[side];
    for (int current_pos = -1; knights;)
    {
        current_pos += next_bit(knights);
        enum_square loc = (enum_square)current_pos;

        bitboard attacks = 0;
        attacks |= knight_attacks_lookup[loc];

        out |= attacks;
    }

    bitboard queens = pieces[QUEEN] & piece_colors[side];
    for (int current_pos = -1; queens;)
    {
        current_pos += next_bit(queens);
        enum_square loc = (enum_square)current_pos;

        bitboard attacks = 0;
        attacks |= diagonal_attacks(all_pieces, loc);
        attacks |= anti_diagonal_attacks(all_pieces, loc);
        attacks |= rank_attacks(all_pieces, loc);
        attacks |= file_attacks(all_pieces, loc);

        out |= attacks;
    }

    bitboard kings = pieces[KING] & piece_colors[side];
    for (int current_pos = -1; kings;)
    {
        current_pos += next_bit(kings);
        enum_square loc = (enum_square)current_pos;

        bitboard attacks = 0;
        attacks |= king_attacks_lookup[loc];

        out |= attacks;
    }
    return out;
}

bool chess_board::king_in_check(enum_color side) {
    enum_color other_side = (enum_color) (side ^ 1);
    bitboard king_piece = piece_colors[side] & pieces[KING];
    enum_square king_square = (enum_square) __builtin_ctzll(king_piece);

    bitboard all_pieces = piece_colors[0] | piece_colors[1];
    bitboard rook_attacks = rank_attacks(all_pieces, king_square) | file_attacks(all_pieces, king_square);
    bitboard bishop_attacks = diagonal_attacks(all_pieces, king_square) | anti_diagonal_attacks(all_pieces, king_square);

    return (pawn_diag_attacks_lookup[king_square] & pawn_rank_lookup[other_side][king_square] & piece_colors[other_side] & pieces[PAWN]) | 
    (rook_attacks & piece_colors[other_side] & pieces[ROOK]) | 
    (bishop_attacks & piece_colors[other_side] & pieces[BISHOP]) | 
    ((rook_attacks | bishop_attacks) & piece_colors[other_side] & pieces[QUEEN]) |
    (knight_attacks_lookup[king_square] & piece_colors[other_side] & pieces[KNIGHT]);
}

ull chess_board::play_regular_attacks(enum_piece pt, enum_square loc, bitboard attacks, bool in_check) {
    ull ret = 0;
    for (int atk_pos = -1; attacks;)
    {
        atk_pos += next_bit(attacks);
        enum_square atk_loc = (enum_square)atk_pos;

        ret += play_regular_move(pt, {loc, atk_loc}, in_check);
    }
    return ret;
}

ull chess_board::play_regular_move(enum_piece pt, chess_move move, bool in_check) {
    chess_board copy = *this;

    enum_color side = (enum_color)(!!(copy.data & BLACK_TURN));

    copy.en_passant = 0;
    copy.clear_loc(move.to);
    copy.remove_piece(side, pt, move.from);
    copy.add_piece(side, pt, move.to);

    // copy.print();

    // bitboard attacked = copy.generate_protected_board((enum_color) (1-(int)side));
    // if (attacked & copy.pieces[KING] & copy.piece_colors[side]) return 0;
    if (copy.king_in_check(side)) return 0;

    return 1;
}

ull chess_board::play_castle(int dir) {
    chess_board copy = *this;

    enum_color side = (enum_color)(!!(copy.data & BLACK_TURN));

    copy.en_passant = 0;
    copy.data &= ~0b1111;

    int rank = side * 7;

    int a = 0;
    int b = 1;
    int c = 2;
    int d = 3;
    int e = 4;
    int f = 5;
    int g = 6;
    int h = 7;

    if (dir == 0) {
        copy.remove_piece(side, KING, (enum_square) (rank * 8 + e));
        copy.add_piece(side, KING, (enum_square) (rank * 8 + g));
        copy.remove_piece(side, ROOK, (enum_square) (rank * 8 + h));
        copy.add_piece(side, ROOK, (enum_square) (rank * 8 + f));
    } else {
        copy.remove_piece(side, KING, (enum_square) (rank * 8 + e));
        copy.add_piece(side, KING, (enum_square) (rank * 8 + c));
        copy.remove_piece(side, ROOK, (enum_square) (rank * 8 + a));
        copy.add_piece(side, ROOK, (enum_square) (rank * 8 + d));
    }

    return 1;
}

ull chess_board::play_pawn_jump(chess_move move, enum_square intermediary) {
    chess_board copy = *this;

    enum_color side = (enum_color)(!!(copy.data & BLACK_TURN));
    copy.en_passant = intermediary;
    copy.remove_piece(side, PAWN, move.from);
    copy.add_piece(side, PAWN, move.to);

    if (copy.king_in_check(side)) return 0;

    return 1;
}

ull chess_board::play_en_passant(chess_move move) {
    chess_board copy = *this;

    enum_color side = (enum_color)(!!(copy.data & BLACK_TURN));

    copy.en_passant = 0;
    int rank = move.from >> 3;
    int file = move.to & 3;
    copy.remove_piece(side, PAWN, move.from);
    copy.add_piece(side, PAWN, move.to);
    copy.remove_piece(side, PAWN, (enum_square) (rank*8+file));

    if (copy.king_in_check(side)) return 0;

    return 1;
}

ull chess_board::play_pawn_promotions(enum_square loc, bitboard attacks, bool in_check) {
    ull ret = 0;
    for (int atk_pos = -1; attacks;)
    {
        atk_pos += next_bit(attacks);
        enum_square atk_loc = (enum_square)atk_pos;

        // enum_piece promote_to = QUEEN;

        for (enum_piece promote_to : {QUEEN, ROOK, KNIGHT, BISHOP}) {
            ret += play_pawn_promotion({loc, atk_loc}, promote_to, in_check);
        }
    }
    return ret;
}

ull chess_board::play_pawn_promotion(chess_move move, enum_piece pt, bool in_check) {
    chess_board copy = *this;

    enum_color side = (enum_color)(!!(copy.data & BLACK_TURN));

    copy.en_passant = 0;
    copy.clear_loc(move.to);
    copy.remove_piece(side, PAWN, move.from);
    copy.add_piece(side, pt, move.to);

    if (copy.king_in_check(side)) return 0;

    return 1;
}

ull chess_board::play_king_attacks(enum_square loc, bitboard attacks, bool in_check) {
    ull ret = 0;
    for (int atk_pos = -1; attacks;)
    {
        atk_pos += next_bit(attacks);
        enum_square atk_loc = (enum_square)atk_pos;

        ret += play_king_attack({loc, atk_loc}, in_check);
    }
    return ret;
}

ull chess_board::play_king_attack(chess_move move, bool in_check) {
    chess_board copy = *this;

    enum_color side = (enum_color)(!!(copy.data & BLACK_TURN));

    copy.en_passant = 0;
    copy.data &= ~0b1111;
    copy.clear_loc(move.to);
    copy.remove_piece(side, KING, move.from);
    copy.add_piece(side, KING, move.to);

    // if (copy.king_in_check(side)) return 0;

    return 1;
}

ull chess_board::play_rook_attacks(enum_square loc, bitboard attacks, bool in_check) {
    ull ret = 0;
    for (int atk_pos = -1; attacks;)
    {
        atk_pos += next_bit(attacks);
        enum_square atk_loc = (enum_square)atk_pos;

        ret += play_rook_attack({loc, atk_loc}, in_check);
    }
    return ret;
}

ull chess_board::play_rook_attack(chess_move move, bool in_check) {
    chess_board copy = *this;

    enum_color side = (enum_color)(!!(copy.data & BLACK_TURN));

    int rank = side * 7;

    copy.data &= ~(CASTLE[side][0] * (move.from == (enum_square) (rank * 8 + 7)));
    copy.data &= ~(CASTLE[side][1] * (move.from == (enum_square) (rank * 8 + 0)));

    copy.en_passant = 0;
    copy.clear_loc(move.to);
    copy.remove_piece(side, ROOK, move.from);
    copy.add_piece(side, ROOK, move.to);

    if (copy.king_in_check(side)) return 0;

    return 1;
}

void chess_board::iterate_over_moves() {
    enum_color side = (enum_color)(!!(data & BLACK_TURN));
    ull total_moves = 0;
    bitboard all_pieces = piece_colors[0] | piece_colors[1];

    bitboard attacked = generate_protected_board((enum_color)(1 - side));
    // bool in_check_before = attacked & (pieces[KING] & piece_colors[side]);

    bool in_check_before = king_in_check(side);
    // bool in_check_before = false;

    //CASTLING - DETECT WITH AN IF IG? OR MULTIPLY BY 0 ISH

    if (!in_check_before) {
        for (int dir = 0; dir < 2; dir++) {
            // king is 0, queen is 1

            if ((data & CASTLE[side][dir]) && (!(all_pieces & CASTLE_IN_BETWEEN[side][dir])) && (!(attacked & CASTLE_CHECK[side][dir]))) {
                std::cout << "side: " << side << ", dir: " << dir << std::endl;
                total_moves += play_castle(dir);
            }
        }
    }

    bitboard pawns = pieces[PAWN] & piece_colors[side];
    for (int current_pos = -1; pawns;)
    {
        current_pos += next_bit(pawns);
        enum_square loc = (enum_square)current_pos;

        bitboard attacks = 0;

        // do attack
        attacks |= pawn_diag_attacks_lookup[loc];

        bitboard en_passant_attack = attacks & en_passant;

        if (en_passant_attack) {
            total_moves += play_en_passant({loc, (enum_square) __builtin_ctzll(en_passant_attack)});
        }

        attacks &= (piece_colors[1 - side]);

        bitboard step = (pawn_step_attacks_lookup[loc] & (~all_pieces));
        // step |= (!!step) * (pawn_full_step_attacks_lookup[loc] & (~all_pieces)); // second step is a special case

        attacks |= step;

        attacks &= pawn_rank_lookup[side][loc];
        attacks &= ~piece_colors[side];

        bitboard reg_attacks = attacks & ~(rank_lookup(0) | rank_lookup(7));
        bitboard promotion_attacks = attacks & (rank_lookup(0) | rank_lookup(7));

        total_moves += play_regular_attacks(PAWN, loc, reg_attacks, in_check_before);

        total_moves += play_pawn_promotions(loc, promotion_attacks, in_check_before);

        bitboard second_step = ((!!step) * pawn_second_step_attacks_lookup[loc]) & (~all_pieces) & pawn_rank_lookup[side][loc];
        if (second_step) {
            total_moves += play_pawn_jump({loc, (enum_square) __builtin_ctzll(second_step)}, (enum_square) __builtin_ctzll(step));
        }

        //total_moves += play_pawn_jump({loc, __builtin_ctzll(second_step)}, step); // may need an if to determine whether possible

        //a) 
        //1. Create special move initial position lookup, still & w/ pawn rank lookup - this is only allowed if step is allowed by multiplication
        //2. if can do special move, it should mark it on en passant board, thats about it

        //b) doing the en passant
        //1. check if can attack on en passant square
        //2. if so, special func to move to that square and capture other pawn

        //c) pawn promotion
        // 1. remove all first & last rank moves from regular, use in special instead
        // 2. have it make 4 moves?

        // just need pawn promotion now?


        // FOR EACH, MASK TO SPECIAL ATTACK, AND IF SPECIAL ATTACKING, MARK EN PASSANT SQUARE
        // DETECT EN PASSANT SEPARATELY
        // NEITHER SHOULD NEED IFS

        // PROMOTING AT END RANK? - MAYBE ALSO MASK PER SIDE, UNMASK IT FOR REGULAR STEP?
    }

    bitboard rooks = pieces[ROOK] & piece_colors[side];
    // ::print(rooks);
    for (int current_pos = -1; rooks;)
    {
        current_pos += next_bit(rooks);
        enum_square loc = (enum_square)current_pos;

        bitboard attacks = 0;
        attacks |= rank_attacks(all_pieces, loc);
        attacks |= file_attacks(all_pieces, loc);
        attacks &= ~piece_colors[side];

        total_moves += play_rook_attacks(loc, attacks, in_check_before);

        // int attack_cnt = __builtin_popcountl(attacks);
        // total_moves += attack_cnt;
    }

    bitboard bishops = pieces[BISHOP] & piece_colors[side];

    for (int current_pos = -1; bishops;)
    {
        current_pos += next_bit(bishops);
        enum_square loc = (enum_square)current_pos;

        bitboard attacks = 0;
        attacks |= diagonal_attacks(all_pieces, loc);
        attacks |= anti_diagonal_attacks(all_pieces, loc);
        attacks &= ~piece_colors[side];

        total_moves += play_regular_attacks(BISHOP, loc, attacks, in_check_before);
        // int attack_cnt = __builtin_popcountll(attacks);
        // total_moves += attack_cnt;
    }

    bitboard knights = pieces[KNIGHT] & piece_colors[side];

    for (int current_pos = -1; knights;)
    {
        current_pos += next_bit(knights);
        enum_square loc = (enum_square)current_pos;

        bitboard attacks = 0;
        attacks |= knight_attacks_lookup[loc];
        attacks &= ~piece_colors[side];

        total_moves += play_regular_attacks(KNIGHT, loc, attacks, in_check_before);
        // int attack_cnt = __builtin_popcountll(attacks);
        // total_moves += attack_cnt;
    }

    bitboard queens = pieces[QUEEN] & piece_colors[side];

    for (int current_pos = -1; queens;)
    {
        current_pos += next_bit(queens);
        enum_square loc = (enum_square)current_pos;

        bitboard attacks = 0;
        attacks |= diagonal_attacks(all_pieces, loc);
        attacks |= anti_diagonal_attacks(all_pieces, loc);
        attacks |= rank_attacks(all_pieces, loc);
        attacks |= file_attacks(all_pieces, loc);
        attacks &= ~piece_colors[side];

        total_moves += play_regular_attacks(QUEEN, loc, attacks, in_check_before);
        // int attack_cnt = __builtin_popcountll(attacks);
        // total_moves += attack_cnt;
    }

    bitboard kings = pieces[KING] & piece_colors[side];
    // ::print(kings);

    for (int current_pos = -1; kings;)
    {
        current_pos += next_bit(kings);
        enum_square loc = (enum_square)current_pos;

        bitboard attacks = 0;
        attacks |= king_attacks_lookup[loc];
        attacks &= ~piece_colors[side];
        attacks &= ~attacked;

        total_moves += play_king_attacks(loc, attacks, in_check_before);

        // int attack_cnt = __builtin_popcountll(attacks);
        // total_moves += attack_cnt;
    }
    std::cout << total_moves << " available moves, in check: " << in_check_before << ", in mate: " << (in_check_before && total_moves == 0)  << std::endl;
}

float chess_board::evaluate() const {
    enum_color side = (enum_color)(!!(data & BLACK_TURN));

    float material[2];
    bitboard attacked[2];
    for (int i = 0; i < 2; i++) {
        material[i] += 1 * __builtin_popcountll(piece_colors[i] & pieces[PAWN]);
        material[i] += 3 * __builtin_popcountll(piece_colors[i] & pieces[KNIGHT]);
        material[i] += 3.25 * __builtin_popcountll(piece_colors[i] & pieces[BISHOP]);
        material[i] += 5 * __builtin_popcountll(piece_colors[i] & pieces[ROOK]);
        material[i] += 9 * __builtin_popcountll(piece_colors[i] & pieces[QUEEN]);

        attacked[i] = generate_protected_board((enum_color) i);
    }

    float material_eval = material[WHITE] - material[BLACK];
    float attack_eval = __builtin_popcountll(attacked[WHITE]) - __builtin_popcountll(attacked[BLACK]);
    float side_eval = -(side - 0.5);
    return material_eval + attack_eval * (.15*std::abs(material_eval)) + side_eval;
}

// void chess_board::iterate_over_moves()
// {
//     // for castling, separate it from board position - if doable, then manually add it? even, consider before?
//     enum_color side = (enum_color)(!!(data & BLACK_TURN));
//     int total_moves = 0;
//     bitboard all_pieces = piece_colors[0] | piece_colors[1];

//     bitboard attacked = generate_protected_board((enum_color)(1 - side));
//     bool in_check_before = attacked & (pieces[KING] & piece_colors[side]); // maybe check popcount == 1
//     // ::print(generate_protected_board((enum_color)(1 - side)));

//     if (!in_check_before)
//     {
//         for (int dir = 0; dir < 2; dir++)
//         {

//             //king is 0, queen is 1

//             //if can castle on that side, and no pieces in between, and no checks in the way
//             if ((data & CASTLE[side][dir]) && (!(all_pieces & CASTLE_IN_BETWEEN[side][dir])) && (!(attacked & CASTLE_CHECK[side][dir])))
//             {
//                 total_moves += play_move(KING, {e5, (enum_square)(e5 + (dir * 2 - 1) * (-2))}, true, in_check_before, ' ');
//             }
//         }
//     }

//     bitboard pawns = pieces[PAWN] & piece_colors[side];
//     for (int current_pos = -1; pawns;)
//     {
//         current_pos += next_bit(pawns);
//         enum_square loc = (enum_square)current_pos;

//         bitboard attacks = 0;

//         // do attack
//         attacks |= pawn_diag_attacks_lookup[loc];
//         attacks &= (piece_colors[1 - side] | en_passant);

//         bitboard step = (pawn_step_attacks_lookup[loc] & (~all_pieces));
//         step |= (!!step) * (pawn_full_step_attacks_lookup[loc] & (~all_pieces));

//         attacks |= step;

//         attacks &= pawn_rank_lookup[side][loc];
//         attacks &= ~piece_colors[side];

//         for (int atk_pos = -1; attacks;)
//         {
//             atk_pos += next_bit(attacks);
//             enum_square atk_loc = (enum_square)atk_pos;

//             total_moves += play_move(PAWN, {loc, atk_loc}, false, in_check_before, ' ');
//         }
//     }

//     bitboard rooks = pieces[ROOK] & piece_colors[side];
//     // ::print(rooks);
//     for (int current_pos = -1; rooks;)
//     {
//         current_pos += next_bit(rooks);
//         enum_square loc = (enum_square)current_pos;

//         bitboard attacks = 0;
//         attacks |= rank_attacks(all_pieces, loc);
//         attacks |= file_attacks(all_pieces, loc);
//         attacks &= ~piece_colors[side];

//         for (int atk_pos = -1; attacks;)
//         {
//             atk_pos += next_bit(attacks);
//             enum_square atk_loc = (enum_square)atk_pos;

//             total_moves += play_move(ROOK, {loc, atk_loc}, false, in_check_before, ' ');
//         }

//         // int attack_cnt = __builtin_popcountl(attacks);
//         // total_moves += attack_cnt;
//     }

//     bitboard bishops = pieces[BISHOP] & piece_colors[side];

//     for (int current_pos = -1; bishops;)
//     {
//         current_pos += next_bit(bishops);
//         enum_square loc = (enum_square)current_pos;

//         bitboard attacks = 0;
//         attacks |= diagonal_attacks(all_pieces, loc);
//         attacks |= anti_diagonal_attacks(all_pieces, loc);
//         attacks &= ~piece_colors[side];

//         for (int atk_pos = -1; attacks;)
//         {
//             atk_pos += next_bit(attacks);
//             enum_square atk_loc = (enum_square)atk_pos;

//             total_moves += play_move(BISHOP, {loc, atk_loc}, false, in_check_before, ' ');
//         }
//         // int attack_cnt = __builtin_popcountll(attacks);
//         // total_moves += attack_cnt;
//     }

//     bitboard knights = pieces[KNIGHT] & piece_colors[side];

//     for (int current_pos = -1; knights;)
//     {
//         current_pos += next_bit(knights);
//         enum_square loc = (enum_square)current_pos;

//         bitboard attacks = 0;
//         attacks |= knight_attacks_lookup[loc];
//         attacks &= ~piece_colors[side];

//         for (int atk_pos = -1; attacks;)
//         {
//             atk_pos += next_bit(attacks);
//             enum_square atk_loc = (enum_square)atk_pos;

//             total_moves += play_move(KNIGHT, {loc, atk_loc}, false, in_check_before, ' ');
//         }
//         // int attack_cnt = __builtin_popcountll(attacks);
//         // total_moves += attack_cnt;
//     }

//     bitboard queens = pieces[QUEEN] & piece_colors[side];

//     for (int current_pos = -1; queens;)
//     {
//         current_pos += next_bit(queens);
//         enum_square loc = (enum_square)current_pos;

//         bitboard attacks = 0;
//         attacks |= diagonal_attacks(all_pieces, loc);
//         attacks |= anti_diagonal_attacks(all_pieces, loc);
//         attacks |= rank_attacks(all_pieces, loc);
//         attacks |= file_attacks(all_pieces, loc);
//         attacks &= ~piece_colors[side];

//         for (int atk_pos = -1; attacks;)
//         {
//             atk_pos += next_bit(attacks);
//             enum_square atk_loc = (enum_square)atk_pos;

//             total_moves += play_move(QUEEN, {loc, atk_loc}, false, in_check_before, ' ');
//         }
//         // int attack_cnt = __builtin_popcountll(attacks);
//         // total_moves += attack_cnt;
//     }

//     bitboard kings = pieces[KING] & piece_colors[side];

//     for (int current_pos = -1; kings;)
//     {
//         current_pos += next_bit(kings);
//         enum_square loc = (enum_square)current_pos;

//         bitboard attacks = 0;
//         attacks |= king_attacks_lookup[loc];
//         attacks &= ~piece_colors[side];

//         for (int atk_pos = -1; attacks;)
//         {
//             atk_pos += next_bit(attacks);
//             enum_square atk_loc = (enum_square)atk_pos;

//             total_moves += play_move(KING, {loc, atk_loc}, false, in_check_before, ' ');
//         }

//         // int attack_cnt = __builtin_popcountll(attacks);
//         // total_moves += attack_cnt;
//     }

//     // std::cout << "Found " << total_moves << " legal moves for side " << side << std::endl;
//     // std::cout << "In Check: " << in_check_before << " | Checkmated: " << (in_check_before && total_moves == 0) << " | Stalemate: " << (!in_check_before && total_moves == 0) << std::endl;
// }

// // only accepts legal moves!, check if move is legal earlier, except for into check, it should deal w/ that!
// ull chess_board::play_move(enum_piece piece_type, chess_move move, bool special, bool in_check, char promo)
// {
//     chess_board copy = *this;

//     bool move_done = false;

//     copy.en_passant = 0;
//     enum_color side = (enum_color)(!!(copy.data & BLACK_TURN));
//     int from_rank, to_rank;
//     bitboard attacked;
//     bool in_check_after;
//     switch (piece_type)
//     {
//     case PAWN:
//         from_rank = move.from >> 3;
//         to_rank = move.to >> 3;
//         if (std::max(from_rank, to_rank) - std::min(from_rank, to_rank) == 2)
//             set(copy.en_passant, 1, (from_rank + to_rank) / 2, move.from & 7);
//         switch (to_rank)
//         {
//         case 0:
//         case 7:
//             copy.clear_loc(move.to);
//             copy.remove_piece(side, PAWN, move.from);
//             copy.add_piece(side, QUEEN, move.to); // queen only for now
//             move_done = true;

//             attacked = copy.generate_protected_board((enum_color)(1 - side));
//             in_check_after = attacked & (copy.pieces[KING] & copy.piece_colors[side]);
//             return 1 - in_check_after;

//             break;
//         default:
//             break;
//         }
//         break;
//     case KNIGHT:
//         break;
//     case BISHOP:
//         break;
//     case ROOK:
//         switch (move.from)
//         {
//         case a1:
//             copy.data &= ~(CASTLE[0][1]);
//             break;
//         case h1:
//             copy.data &= ~(CASTLE[0][0]);
//             break;
//         case a8:
//             copy.data &= ~(CASTLE[1][1]);
//             break;
//         case h8:
//             copy.data &= ~(CASTLE[1][0]);
//             break;
//         default:
//             break;
//         }
//         break;
//     case QUEEN:
//         break;
//     case KING:
//         if (special)
//         {
//             switch (move.to)
//             {
//             case g1:
//                 copy.remove_piece(side, ROOK, h1);
//                 copy.add_piece(side, ROOK, f1);
//                 break;
//             case c1:
//                 copy.remove_piece(side, ROOK, a1);
//                 copy.add_piece(side, ROOK, d1);
//                 break;
//             case g8:
//                 copy.remove_piece(side, ROOK, h8);
//                 copy.add_piece(side, ROOK, f8);
//                 break;
//             case c8:
//                 copy.remove_piece(side, ROOK, a8);
//                 copy.add_piece(side, ROOK, d8);
//                 break;
//             default:
//                 break;
//             }
//         }
//         copy.data &= ~(CASTLE[side][0]);
//         copy.data &= ~(CASTLE[side][1]);
//         break;
//     default:
//         break;
//     }

//     if (!move_done)
//     {
//         copy.remove_piece(side, piece_type, move.from);
//         copy.add_piece(side, piece_type, move.to);
//     }

//     attacked = copy.generate_protected_board((enum_color)(1 - side));
//     in_check_after = attacked & (copy.pieces[KING] & copy.piece_colors[side]);
//     return 1 - in_check_after;

//     // copy.data ^= BLACK_TURN;

//     // do recursive stuff on the copy, return result back to prev function
// }

void chess_board::play_move(std::string move) {
    assert(move.size() >= 4);

    int file0 = move[0] - 'a';
    int rank0 = move[1] - '1';
    enum_square from = (enum_square) (rank0*8+file0);

    int file1 = move[2] - 'a';
    int rank1 = move[3] - '1';
    enum_square to = (enum_square) (rank1*8+file1);

    char piece = ' ';
    if (move.size() == 5) piece = move[4];

    enum_piece type;
    enum_color color;

    //get method
    for (int pt = 0; pt < 6; pt++) {
        if (get(pieces[pt], from)) {
            type = (enum_piece) pt;
            break;
        }
    }

    //get method
    for (int col = 0; col < 2; col++) {
        if (get(piece_colors[col], from)) {
            color = (enum_color) col;
            break;
        }
    }

    // need to refactor the move system!!!

}