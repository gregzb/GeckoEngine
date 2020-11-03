#pragma once

#include <cassert>
#include <utility>
#include <iostream>
#include <bitset>
#include <vector>
#include <string>

using u_char = unsigned char;
using u_int = unsigned int;
using ull = unsigned long long;

inline void set_style(int val) {
    std::cout << "\033[1;" << val << "m";
}

using bitboard = u_int64_t;

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

inline bool get(bitboard board, enum_square piece)
{
    bitboard one = 1;
    return board & (one << piece);
}

inline bool get(bitboard board, int rank, int file)
{
    if (rank < 0 || rank >= 8 || file < 0 || file >= 8)
        assert(false);
    return get(board, (enum_square)(rank * 8 + file));
}

inline void set(bitboard &board, bool bit, enum_square piece)
{
    bitboard one = 1;
    bitboard tmp = bit;
    board = (board & (~(one << piece))) | (tmp << piece);
}

inline void set(bitboard &board, bool bit, int rank, int file)
{
    if (rank < 0 || rank >= 8 || file < 0 || file >= 8)
        return;
    set(board, bit, (enum_square)(rank * 8 + file));
}

inline void print(bitboard board)
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

inline std::vector<std::string> split_string(std::string str, std::string delimiter) {
    std::vector<std::string> out;

    auto start = 0U;
    auto end = str.find(delimiter);
    while (end != std::string::npos)
    {
        out.push_back(str.substr(start, end - start) );
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }

    out.push_back(str.substr(start, end));
    return out;
}


// using pos = std::pair<char, char>;
// inline int get_idx(int rank, int file) {
//     return rank * 8 + file;
// }
// inline int get_idx(pos pos) {
//     return get_idx(pos.first, pos.second);
// }
// inline pos get_pos(int idx) {
//     return {idx / 8, idx % 8};
// }