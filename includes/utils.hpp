#pragma once

#include <cassert>
#include <utility>
#include <iostream>
#include <bitset>

using u_char = unsigned char;
using u_int = unsigned int;
using ull = unsigned long long;

inline void set_style(int val) {
    std::cout << "\033[1;" << val << "m";
}

using bitboard = u_int64_t;

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