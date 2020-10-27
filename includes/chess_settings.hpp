# pragma once
# include <unordered_map>
# include <string>

struct chess_settings {
    bool debug = false;
    std::unordered_map<std::string, std::string> options;
};

extern chess_settings settings;