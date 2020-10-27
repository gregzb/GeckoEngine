#include "chess_board.hpp"

bitboard CASTLE[2][2] = {{0b1, 0b10}, {0b100, 0b1000}}; // CASTLE[SIDE][KING/QUEEN]
bitboard CASTLE_CHECK[2][2] = {{0, 0}, {0, 0}};         // CASTLE[SIDE][KING/QUEEN]
bitboard CASTLE_IN_BETWEEN[2][2] = {{0, 0}, {0, 0}};    // CASTLE[SIDE][KING/QUEEN]
bitboard BLACK_TURN = 0b10000;

chess_board::chess_board()
{
    piece_colors[WHITE] = 0;
    piece_colors[BLACK] = 0;

    pieces[PAWN] = 0;
    pieces[ROOK] = 0;
    pieces[KNIGHT] = 0;
    pieces[BISHOP] = 0;
    pieces[QUEEN] = 0;
    pieces[KING] = 0;

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
    add_piece(WHITE, PAWN, f3);
    add_piece(WHITE, PAWN, g4);
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

    en_passant = 0;

    data = 0;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            data |= CASTLE[i][j];
            // std::cout << (int) (CASTLE[i][j]) << std::endl;
        }
    }

    // std::cout << (int) (data) << std::endl;

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

bitboard chess_board::generate_protected_board(enum_color side)
{
    bitboard out = 0;

    bitboard curr_board = piece_colors[0] | piece_colors[1];

    bitboard pawns = pieces[PAWN] & piece_colors[side];

    int current_pos = -1;

    while (pawns)
    {
        int empty_in_front = __builtin_ctzll(pawns) + 1;
        current_pos += empty_in_front;
        pawns >>= empty_in_front;

        enum_square loc = (enum_square)current_pos;

        bitboard attacks = 0;

        // do attack
        attacks |= pawn_diag_attacks_lookup[loc];
        // attacks &= (piece_colors[1 - side] | en_passant);

        attacks &= pawn_rank_lookup[side][loc];
        out |= attacks;
    }

    bitboard rooks = pieces[ROOK] & piece_colors[side];
    // ::print(rooks);
    current_pos = -1;

    while (rooks)
    {
        int empty_in_front = __builtin_ctzll(rooks) + 1;
        current_pos += empty_in_front;
        rooks >>= empty_in_front;

        enum_square loc = (enum_square)current_pos;

        bitboard attacks = 0;
        attacks |= rank_attacks(curr_board, loc);
        attacks |= file_attacks(curr_board, loc);

        out |= attacks;
    }

    bitboard bishops = pieces[BISHOP] & piece_colors[side];

    current_pos = -1;

    while (bishops)
    {
        int empty_in_front = __builtin_ctzll(bishops) + 1;
        current_pos += empty_in_front;
        bishops >>= empty_in_front;

        enum_square loc = (enum_square)current_pos;

        bitboard attacks = 0;
        attacks |= diagonal_attacks(curr_board, loc);
        attacks |= anti_diagonal_attacks(curr_board, loc);

        out |= attacks;
    }

    bitboard knights = pieces[KNIGHT] & piece_colors[side];

    current_pos = -1;

    while (knights)
    {
        int empty_in_front = __builtin_ctzll(knights) + 1;
        current_pos += empty_in_front;
        knights >>= empty_in_front;

        enum_square loc = (enum_square)current_pos;

        bitboard attacks = 0;
        attacks |= knight_attacks_lookup[loc];

        out |= attacks;
    }

    bitboard queens = pieces[QUEEN] & piece_colors[side];

    current_pos = -1;

    while (queens)
    {
        int empty_in_front = __builtin_ctzll(queens) + 1;
        current_pos += empty_in_front;
        queens >>= empty_in_front;

        enum_square loc = (enum_square)current_pos;

        bitboard attacks = 0;
        attacks |= diagonal_attacks(curr_board, loc);
        attacks |= anti_diagonal_attacks(curr_board, loc);
        attacks |= rank_attacks(curr_board, loc);
        attacks |= file_attacks(curr_board, loc);

        out |= attacks;
    }

    bitboard kings = pieces[KING] & piece_colors[side];

    current_pos = -1;

    while (kings)
    {
        int empty_in_front = __builtin_ctzll(kings) + 1;
        current_pos += empty_in_front;
        kings >>= empty_in_front;

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
    bitboard curr_board = piece_colors[0] | piece_colors[1];
    bitboard pawns = pieces[PAWN] & piece_colors[side];

    bitboard attacked = generate_protected_board((enum_color)(1 - side));
    bool in_check_before = attacked & (pieces[KING] & piece_colors[side]); // maybe check popcount == 1
    // ::print(generate_protected_board((enum_color)(1 - side)));

    if (!in_check_before)
    {
        for (int dir = 0; dir < 2; dir++)
        {
            // std::cout << "dir: " << dir << std::endl;
            // std::cout << (data & CASTLE[side][dir]) << " " << (!(curr_board & CASTLE_IN_BETWEEN[side][dir])) << " " << (!(attacked & CASTLE_CHECK[side][dir])) << std::endl;
            // ::print(CASTLE_IN_BETWEEN)
            if ((data & CASTLE[side][dir]) && (!(curr_board & CASTLE_IN_BETWEEN[side][dir])) && (!(attacked & CASTLE_CHECK[side][dir])) )
            {
                // switch (dir) {
                //     case 0: //king
                //     break;
                //     case 1: //queen
                //     break;
                //     default: //never?
                //     break;
                // }
                total_moves += play_move(KING, {e5, (enum_square)(e5 + (dir * 2 - 1) * (-2))}, true, in_check_before);
            }
        }
    }

    int current_pos = -1;

    while (pawns)
    {
        int empty_in_front = __builtin_ctzll(pawns) + 1;
        current_pos += empty_in_front;
        pawns >>= empty_in_front;

        enum_square loc = (enum_square)current_pos;

        bitboard attacks = 0;

        // do attack
        attacks |= pawn_diag_attacks_lookup[loc];
        attacks &= (piece_colors[1 - side] | en_passant);
        
        bitboard step = (pawn_step_attacks_lookup[loc] & (~curr_board) );
        step |= (!!step) * (pawn_full_step_attacks_lookup[loc] & (~curr_board));

        attacks |= step;

        attacks &= pawn_rank_lookup[side][loc];
        attacks &= ~piece_colors[side];

        int c1_pos = -1;
        while (attacks)
        {
            int empty_in_front1 = __builtin_ctzll(attacks) + 1;
            c1_pos += empty_in_front1;
            attacks >>= empty_in_front1;

            enum_square loc1 = (enum_square)c1_pos;
            total_moves += play_move(PAWN, {loc, loc1}, false, in_check_before);
        }

        //loop thru attacks & perform each!

        // int attack_cnt = __builtin_popcountl(attacks);
        // total_moves += attack_cnt;
    }

    bitboard rooks = pieces[ROOK] & piece_colors[side];
    // ::print(rooks);
    current_pos = -1;

    while (rooks)
    {
        int empty_in_front = __builtin_ctzll(rooks) + 1;
        current_pos += empty_in_front;
        rooks >>= empty_in_front;

        enum_square loc = (enum_square)current_pos;

        bitboard attacks = 0;
        attacks |= rank_attacks(curr_board, loc);
        attacks |= file_attacks(curr_board, loc);

        attacks &= ~piece_colors[side];

        int c1_pos = -1;
        while (attacks)
        {
            int empty_in_front1 = __builtin_ctzll(attacks) + 1;
            c1_pos += empty_in_front1;
            attacks >>= empty_in_front1;

            enum_square loc1 = (enum_square)c1_pos;
            total_moves += play_move(ROOK, {loc, loc1}, false, in_check_before);
        }

        // int attack_cnt = __builtin_popcountl(attacks);
        // total_moves += attack_cnt;
    }

    bitboard bishops = pieces[BISHOP] & piece_colors[side];

    current_pos = -1;

    while (bishops)
    {
        int empty_in_front = __builtin_ctzll(bishops) + 1;
        current_pos += empty_in_front;
        bishops >>= empty_in_front;

        enum_square loc = (enum_square)current_pos;

        bitboard attacks = 0;
        attacks |= diagonal_attacks(curr_board, loc);
        attacks |= anti_diagonal_attacks(curr_board, loc);

        attacks &= ~piece_colors[side];

        int c1_pos = -1;
        while (attacks)
        {
            int empty_in_front1 = __builtin_ctzll(attacks) + 1;
            c1_pos += empty_in_front1;
            attacks >>= empty_in_front1;

            enum_square loc1 = (enum_square)c1_pos;
            total_moves += play_move(BISHOP, {loc, loc1}, false, in_check_before);
        }
        // int attack_cnt = __builtin_popcountll(attacks);
        // total_moves += attack_cnt;
    }

    bitboard knights = pieces[KNIGHT] & piece_colors[side];

    current_pos = -1;

    while (knights)
    {
        int empty_in_front = __builtin_ctzll(knights) + 1;
        current_pos += empty_in_front;
        knights >>= empty_in_front;

        enum_square loc = (enum_square)current_pos;

        bitboard attacks = 0;
        attacks |= knight_attacks_lookup[loc];

        attacks &= ~piece_colors[side];

        int c1_pos = -1;
        while (attacks)
        {
            int empty_in_front1 = __builtin_ctzll(attacks) + 1;
            c1_pos += empty_in_front1;
            attacks >>= empty_in_front1;

            enum_square loc1 = (enum_square)c1_pos;
            total_moves += play_move(KNIGHT, {loc, loc1}, false, in_check_before);
        }
        // int attack_cnt = __builtin_popcountll(attacks);
        // total_moves += attack_cnt;
    }

    bitboard queens = pieces[QUEEN] & piece_colors[side];

    current_pos = -1;

    while (queens)
    {
        int empty_in_front = __builtin_ctzll(queens) + 1;
        current_pos += empty_in_front;
        queens >>= empty_in_front;

        enum_square loc = (enum_square)current_pos;

        bitboard attacks = 0;
        attacks |= diagonal_attacks(curr_board, loc);
        attacks |= anti_diagonal_attacks(curr_board, loc);
        attacks |= rank_attacks(curr_board, loc);
        attacks |= file_attacks(curr_board, loc);

        attacks &= ~piece_colors[side];

        int c1_pos = -1;
        while (attacks)
        {
            int empty_in_front1 = __builtin_ctzll(attacks) + 1;
            c1_pos += empty_in_front1;
            attacks >>= empty_in_front1;

            enum_square loc1 = (enum_square)c1_pos;
            total_moves += play_move(QUEEN, {loc, loc1}, false, in_check_before);
        }
        // int attack_cnt = __builtin_popcountll(attacks);
        // total_moves += attack_cnt;
    }

    bitboard kings = pieces[KING] & piece_colors[side];

    current_pos = -1;

    while (kings)
    {
        int empty_in_front = __builtin_ctzll(kings) + 1;
        current_pos += empty_in_front;
        kings >>= empty_in_front;

        enum_square loc = (enum_square)current_pos;

        bitboard attacks = 0;
        attacks |= king_attacks_lookup[loc];

        attacks &= ~piece_colors[side];
        int c1_pos = -1;
        while (attacks)
        {
            int empty_in_front1 = __builtin_ctzll(attacks) + 1;
            c1_pos += empty_in_front1;
            attacks >>= empty_in_front1;

            enum_square loc1 = (enum_square)c1_pos;
            total_moves += play_move(KING, {loc, loc1}, false, in_check_before);
        }

        // int attack_cnt = __builtin_popcountll(attacks);
        // total_moves += attack_cnt;
    }

    // bool in_check_after = generate_protected_board((enum_color) (1-side)) & (pieces[KING] & piece_colors[side]); // maybe check popcount == 1

    std::cout << "Found " << total_moves << " legal moves for side " << side << std::endl;
    std::cout << "Currently in check: " << in_check_before << std::endl;
}

// only accepts legal moves!, check if move is legal earlier, except for into check, it should deal w/ that!
ull chess_board::play_move(enum_piece piece_type, chess_move move, bool special, bool in_check)
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

bool get(bitboard board, enum_square piece)
{
    bitboard one = 1;
    return board & (one << piece);
}

bool get(bitboard board, int rank, int file)
{
    if (rank < 0 || rank >= 8 || file < 0 || file >= 8)
        assert(false);
    return get(board, (enum_square)(rank * 8 + file));
}

void set(bitboard &board, bool bit, enum_square piece)
{
    bitboard one = 1;
    bitboard tmp = bit;
    board = (board & (~(one << piece))) | (tmp << piece);
}

void set(bitboard &board, bool bit, int rank, int file)
{
    if (rank < 0 || rank >= 8 || file < 0 || file >= 8)
        return;
    set(board, bit, (enum_square)(rank * 8 + file));
}

void print(bitboard board)
{
    std::string out;
    uint64_t last = 1;
    // last <<= 63;
    for (int i = 0; i < 64; i++)
    {
        int val = !!(board & last);
        out += std::to_string(val);
        board >>= 1;
    }

    std::string tmp;

    for (int i = 7; i >= 0; i--)
    {
        for (int j = 0; j < 8; j++)
        {
            tmp += out[i * 8 + j];
        }
        tmp += "\n";
    }

    std::cout << tmp << std::endl;
}

void generate_rook_masks(std::vector<bitboard> &verticals, std::vector<bitboard> &horizontals)
{
    for (int i = 0; i < 8; i++)
    {
        bitboard vertical = 0, horizontal = 0;

        for (int j = 0; j < 8; j++)
        {
            set(vertical, 1, j, i);
            set(horizontal, 1, i, j);
        }
        verticals.push_back(vertical);
        horizontals.push_back(horizontal);
    }
}

void generate_bishop_masks(std::vector<bitboard> &diagonals, std::vector<bitboard> &anti_diagonals)
{
    for (int i = 0; i < 15; i++)
    {
        // i = rank + file
        bitboard diagonal = 0, anti_diagonal = 0;
        for (int file = 0; file < 8; file++)
        {
            int diag_rank = i - file;
            int anti_diag_rank = i - 7 + file;
            set(diagonal, 1, diag_rank, file);
            set(anti_diagonal, 1, anti_diag_rank, file);
        }

        diagonals.push_back(diagonal);
        anti_diagonals.push_back(anti_diagonal);
    }
}

bitboard first_rank_attacks[8][64] = {{0}};

void calculate_first_rank_attacks()
{
    for (int file = 0; file < 8; file++)
    {
        for (int permuation = 0; permuation < 64; permuation++)
        {
            bitboard theoretical_board = permuation << 1;
            set(theoretical_board, 0, 0, file);

            for (int f = file; f >= 0; f--)
            {
                set(first_rank_attacks[file][permuation], 1, 0, f);
                if (get(theoretical_board, 0, f))
                    break;
            }

            for (int f = file; f < 8; f++)
            {
                set(first_rank_attacks[file][permuation], 1, 0, f);
                if (get(theoretical_board, 0, f) == 1)
                    break;
            }

            set(first_rank_attacks[file][permuation], 0, 0, file);

            for (int i = 0; i < 3; i++)
            {
                first_rank_attacks[file][permuation] |= first_rank_attacks[file][permuation] << ((1 << i) << 3);
            }
        }
    }
}

bitboard a_file_attacks[8][64] = {{0}};

void calculate_a_file_attacks()
{
    for (int rank = 0; rank < 8; rank++)
    {
        for (int permuation = 0; permuation < 64; permuation++)
        {
            bitboard theoretical_board = permuation << 1;
            set(theoretical_board, 0, 0, rank);

            for (int r = rank; r >= 0; r--)
            {
                set(a_file_attacks[rank][permuation], 1, r, 0);
                if (get(theoretical_board, 0, r) == 1)
                    break;
            }

            for (int r = rank; r < 8; r++)
            {
                set(a_file_attacks[rank][permuation], 1, r, 0);
                if (get(theoretical_board, 0, r) == 1)
                    break;
            }

            set(a_file_attacks[rank][permuation], 0, rank, 0);
        }
    }
}

void populate_masks(std::vector<bitboard> &verticals, std::vector<bitboard> &horizontals, std::vector<bitboard> &diagonals, std::vector<bitboard> &anti_diagonals)
{
    for (int rank = 0; rank < 8; rank++)
    {
        for (int file = 0; file < 8; file++)
        {
            enum_square loc = (enum_square)(rank * 8 + file);
            file_masks[loc] = verticals[file];
            rank_masks[loc] = horizontals[rank];
            diagonal_masks[loc] = diagonals[rank + file];
            anti_diagonal_masks[loc] = anti_diagonals[rank - file + 7];
        }
    }
}

bitboard diagonal_masks[64] = {0};
bitboard anti_diagonal_masks[64] = {0};
bitboard rank_masks[64] = {0};
bitboard file_masks[64] = {0};

bitboard diagonal_attacks(bitboard occ, enum_square sq)
{
    const bitboard bFile = 0x0202020202020202ULL;
    occ = (diagonal_masks[sq] & occ) * bFile >> 58;
    return diagonal_masks[sq] & first_rank_attacks[sq & 7][occ];
}

bitboard anti_diagonal_attacks(bitboard occ, enum_square sq)
{
    const bitboard bFile = 0x0202020202020202ULL;
    // bitboard tmp = anti_diagonal_masks[sq] & occ;
    occ = (anti_diagonal_masks[sq] & occ) * bFile >> 58;
    return anti_diagonal_masks[sq] & first_rank_attacks[sq & 7][occ];
}

bitboard rank_attacks(bitboard occ, enum_square sq)
{
    const bitboard bFile = 0x0202020202020202ULL;
    occ = (rank_masks[sq] & occ) * bFile >> 58;
    return rank_masks[sq] & first_rank_attacks[sq & 7][occ];
}

bitboard file_attacks(bitboard occ, enum_square sq)
{
    const bitboard aFile = 0x0101010101010101ULL;
    // const bitboard diac2h7 = 0x0080402010080400ULL;
    // ::print(diac2h7);
    const bitboard diac7h2 = 0b00000000'00000100'00001000'00010000'00100000'01000000'10000000'00000000ULL;
    // ::print(diac7h2);
    // std::cout << "e:" << std::endl;
    // ::print(occ);
    // ::print(occ >> (sq & 7));
    occ = aFile & (occ >> (sq & 7));
    // ::print(occ);
    // std::cout << "d: " << std::endl;
    occ = (diac7h2 * occ) >> 58;
    // ::print(occ);
    // std::cout << (sq >> 3) << std::endl;
    // ::print(a_file_attacks[sq >> 3][occ]);
    return a_file_attacks[sq >> 3][occ] << (sq & 7);
}

bitboard knight_attacks_lookup[64] = {0};

void populate_knight_attacks()
{
    for (int i = 0; i < 64; i++)
    {
        enum_square loc = (enum_square)(i);

        int rank = loc >> 3;
        int file = loc & 7;

        bitboard tmp = 0;
        int rank_offsets[8] = {1, -1, 2, -2, 1, -1, 2, -2};
        int file_offsets[8] = {2, -2, 1, -1, -2, 2, -1, 1};

        for (int j = 0; j < 8; j++)
        {
            set(tmp, 1, rank + rank_offsets[j], file + file_offsets[j]);
        }

        knight_attacks_lookup[i] = tmp;
    }
}

bitboard king_attacks_lookup[64];

void populate_king_attacks()
{
    for (int i = 0; i < 64; i++)
    {
        enum_square loc = (enum_square)(i);

        int rank = loc >> 3;
        int file = loc & 7;

        bitboard tmp = 0;
        int rank_offsets[8] = {0, 0, 1, -1, 1, 1, -1, -1};
        int file_offsets[8] = {1, -1, 0, 0, 1, -1, 1, -1};

        for (int j = 0; j < 8; j++)
        {
            set(tmp, 1, rank + rank_offsets[j], file + file_offsets[j]);
        }

        king_attacks_lookup[i] = tmp;
    }
}

bitboard pawn_diag_attacks_lookup[64];
bitboard pawn_step_attacks_lookup[64];
bitboard pawn_full_step_attacks_lookup[64];
bitboard pawn_rank_lookup[2][64];

void populate_pawn_attacks()
{
    for (int i = 0; i < 64; i++)
    {
        enum_square loc = (enum_square)(i);

        int rank = loc >> 3;
        int file = loc & 7;

        bitboard diag_tmp = 0;
        int rank_offsets[4] = {1, 1, -1, -1};
        int file_offsets[4] = {1, -1, 1, -1};

        for (int j = 0; j < 4; j++)
        {
            set(diag_tmp, 1, rank + rank_offsets[j], file + file_offsets[j]);
        }

        pawn_diag_attacks_lookup[i] = diag_tmp;

        bitboard step_tmp = 0;
        set(step_tmp, 1, rank + 1, file);
        set(step_tmp, 1, rank - 1, file);
        pawn_step_attacks_lookup[i] = step_tmp;
        if (rank == 1 || rank == 6)
        {
            set(step_tmp, 1, rank + 2, file);
            set(step_tmp, 1, rank - 2, file);
        }
        pawn_full_step_attacks_lookup[i] = step_tmp;

        bitboard rank_white = 0;
        for (int i = 0; i < 8; i++)
        {
            set(rank_white, 1, rank + 1, i);
            set(rank_white, 1, rank + 2, i);
        }
        pawn_rank_lookup[WHITE][i] = rank_white;

        bitboard rank_black = 0;
        for (int i = 0; i < 8; i++)
        {
            set(rank_black, 1, rank - 1, i);
            set(rank_black, 1, rank - 2, i);
        }
        pawn_rank_lookup[BLACK][i] = rank_black;
    }
}