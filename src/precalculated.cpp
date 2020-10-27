# include "precalculated.hpp"

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

void precalculate() {
    std::vector<bitboard> verticals, horizontals; // verticles indexed by file, horizontals indexed by col
    generate_rook_masks(verticals, horizontals);

    std::vector<bitboard> diagonals, anti_diagonals; //rank + file for diag, rank - file + 7 for anti diag
    generate_bishop_masks(diagonals, anti_diagonals); 

    populate_masks(verticals, horizontals, diagonals, anti_diagonals);

    calculate_first_rank_attacks();

    calculate_a_file_attacks();

    populate_knight_attacks();
    populate_king_attacks();
    populate_pawn_attacks();
}