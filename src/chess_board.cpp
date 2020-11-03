#include "chess_board.hpp"
#include "precalculated.hpp"

bitboard CASTLE[2][2] = {{0b1, 0b10}, {0b100, 0b1000}}; // CASTLE[SIDE][KING/QUEEN]
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

    add_piece(BLACK, QUEEN, h4);

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
    int add_to_pos = __builtin_ctzll(board) + 1;
    board >>= add_to_pos;
    return add_to_pos;
}

bitboard chess_board::generate_protected_board(enum_color side)
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

void chess_board::iterate_over_moves()
{
    // for castling, separate it from board position - if doable, then manually add it? even, consider before?
    enum_color side = (enum_color)(!!(data & BLACK_TURN));
    int total_moves = 0;
    bitboard all_pieces = piece_colors[0] | piece_colors[1];

    bitboard attacked = generate_protected_board((enum_color)(1 - side));
    bool in_check_before = attacked & (pieces[KING] & piece_colors[side]); // maybe check popcount == 1
    // ::print(generate_protected_board((enum_color)(1 - side)));

    if (!in_check_before)
    {
        for (int dir = 0; dir < 2; dir++)
        {

            //king is 0, queen is 1

            //if can castle on that side, and no pieces in between, and no checks in the way
            if ((data & CASTLE[side][dir]) && (!(all_pieces & CASTLE_IN_BETWEEN[side][dir])) && (!(attacked & CASTLE_CHECK[side][dir])))
            {
                total_moves += play_move(KING, {e5, (enum_square)(e5 + (dir * 2 - 1) * (-2))}, true, in_check_before, ' ');
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
        attacks &= (piece_colors[1 - side] | en_passant);

        bitboard step = (pawn_step_attacks_lookup[loc] & (~all_pieces));
        step |= (!!step) * (pawn_full_step_attacks_lookup[loc] & (~all_pieces));

        attacks |= step;

        attacks &= pawn_rank_lookup[side][loc];
        attacks &= ~piece_colors[side];

        for (int atk_pos = -1; attacks;)
        {
            atk_pos += next_bit(attacks);
            enum_square atk_loc = (enum_square)atk_pos;

            total_moves += play_move(PAWN, {loc, atk_loc}, false, in_check_before, ' ');
        }
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

        for (int atk_pos = -1; attacks;)
        {
            atk_pos += next_bit(attacks);
            enum_square atk_loc = (enum_square)atk_pos;

            total_moves += play_move(ROOK, {loc, atk_loc}, false, in_check_before, ' ');
        }

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

        for (int atk_pos = -1; attacks;)
        {
            atk_pos += next_bit(attacks);
            enum_square atk_loc = (enum_square)atk_pos;

            total_moves += play_move(BISHOP, {loc, atk_loc}, false, in_check_before, ' ');
        }
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

        for (int atk_pos = -1; attacks;)
        {
            atk_pos += next_bit(attacks);
            enum_square atk_loc = (enum_square)atk_pos;

            total_moves += play_move(KNIGHT, {loc, atk_loc}, false, in_check_before, ' ');
        }
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

        for (int atk_pos = -1; attacks;)
        {
            atk_pos += next_bit(attacks);
            enum_square atk_loc = (enum_square)atk_pos;

            total_moves += play_move(QUEEN, {loc, atk_loc}, false, in_check_before, ' ');
        }
        // int attack_cnt = __builtin_popcountll(attacks);
        // total_moves += attack_cnt;
    }

    bitboard kings = pieces[KING] & piece_colors[side];

    for (int current_pos = -1; kings;)
    {
        current_pos += next_bit(kings);
        enum_square loc = (enum_square)current_pos;

        bitboard attacks = 0;
        attacks |= king_attacks_lookup[loc];
        attacks &= ~piece_colors[side];

        for (int atk_pos = -1; attacks;)
        {
            atk_pos += next_bit(attacks);
            enum_square atk_loc = (enum_square)atk_pos;

            total_moves += play_move(KING, {loc, atk_loc}, false, in_check_before, ' ');
        }

        // int attack_cnt = __builtin_popcountll(attacks);
        // total_moves += attack_cnt;
    }

    // std::cout << "Found " << total_moves << " legal moves for side " << side << std::endl;
    // std::cout << "In Check: " << in_check_before << " | Checkmated: " << (in_check_before && total_moves == 0) << " | Stalemate: " << (!in_check_before && total_moves == 0) << std::endl;
}

// only accepts legal moves!, check if move is legal earlier, except for into check, it should deal w/ that!
ull chess_board::play_move(enum_piece piece_type, chess_move move, bool special, bool in_check, char promo)
{
    chess_board copy = *this;

    bool move_done = false;

    copy.en_passant = 0;
    enum_color side = (enum_color)(!!(copy.data & BLACK_TURN));
    int from_rank, to_rank;
    bitboard attacked;
    bool in_check_after;
    switch (piece_type)
    {
    case PAWN:
        from_rank = move.from >> 3;
        to_rank = move.to >> 3;
        if (std::max(from_rank, to_rank) - std::min(from_rank, to_rank) == 2)
            set(copy.en_passant, 1, (from_rank + to_rank) / 2, move.from & 7);
        switch (to_rank)
        {
        case 0:
        case 7:
            copy.clear_loc(move.to);
            copy.remove_piece(side, PAWN, move.from);
            copy.add_piece(side, QUEEN, move.to); // queen only for now
            move_done = true;

            attacked = copy.generate_protected_board((enum_color)(1 - side));
            in_check_after = attacked & (copy.pieces[KING] & copy.piece_colors[side]);
            return 1 - in_check_after;

            break;
        default:
            break;
        }
        break;
    case KNIGHT:
        break;
    case BISHOP:
        break;
    case ROOK:
        switch (move.from)
        {
        case a1:
            copy.data &= ~(CASTLE[0][1]);
            break;
        case h1:
            copy.data &= ~(CASTLE[0][0]);
            break;
        case a8:
            copy.data &= ~(CASTLE[1][1]);
            break;
        case h8:
            copy.data &= ~(CASTLE[1][0]);
            break;
        default:
            break;
        }
        break;
    case QUEEN:
        break;
    case KING:
        if (special)
        {
            switch (move.to)
            {
            case g1:
                copy.remove_piece(side, ROOK, h1);
                copy.add_piece(side, ROOK, f1);
                break;
            case c1:
                copy.remove_piece(side, ROOK, a1);
                copy.add_piece(side, ROOK, d1);
                break;
            case g8:
                copy.remove_piece(side, ROOK, h8);
                copy.add_piece(side, ROOK, f8);
                break;
            case c8:
                copy.remove_piece(side, ROOK, a8);
                copy.add_piece(side, ROOK, d8);
                break;
            default:
                break;
            }
        }
        copy.data &= ~(CASTLE[side][0]);
        copy.data &= ~(CASTLE[side][1]);
        break;
    default:
        break;
    }

    if (!move_done)
    {
        copy.remove_piece(side, piece_type, move.from);
        copy.add_piece(side, piece_type, move.to);
    }

    attacked = copy.generate_protected_board((enum_color)(1 - side));
    in_check_after = attacked & (copy.pieces[KING] & copy.piece_colors[side]);
    return 1 - in_check_after;

    // copy.data ^= BLACK_TURN;

    // do recursive stuff on the copy, return result back to prev function
}

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