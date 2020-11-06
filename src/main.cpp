# include <iostream>
# include <thread>
# include <string>
# include <sstream>
# include <algorithm>
# include <vector>
# include <cassert>
# include <chrono>

# include "chess_settings.hpp"
# include "chess_board.hpp"
# include "precalculated.hpp"

chess_settings settings = {};
bool should_stop = false;

void calculate() {
    while (!should_stop) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "bestmove e2e4" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "bestmove d2d4" << std::endl;
        should_stop = true;
    }
}

std::vector<std::string> split(std::string const & str, char delimiter) {
    std::vector<std::string> out;
    std::string::size_type prev_pos = 0, pos = 0;
    while ( (pos = str.find(delimiter, pos)) != std::string::npos) {
        out.push_back(str.substr(prev_pos, pos-prev_pos));
        prev_pos = ++pos;
    }
    out.push_back(str.substr(prev_pos, pos-prev_pos));
    return out;
}

void str_to_lower(std::string & str) {
    std::transform(str.begin(), str.end(), str.begin(),
        [](unsigned char c){ return std::tolower(c); });
}

template <class T>
void print(std::vector<T> vec) {
    std::stringstream temp;
    temp << "[";
    for (auto const & val : vec) {
        temp << val << ", ";
    }
    temp << "]";
    std::cout << temp.str() << std::endl;
}

int handle_uci_input() {
    std::string input_line;
    while (std::getline(std::cin, input_line)) {

        str_to_lower(input_line);

        std::vector<std::string> cmd_line = split(input_line, ' ');

        if (cmd_line[0] == "uci") {
            std::cout << "id name Gecko Engine" << std::endl;
            std::cout << "id author Greg Zborovsky" << std::endl;
            std::cout << "uciok" << std::endl;
        } else if (cmd_line[0] == "debug") {
            if (cmd_line[1] == "on") {
                settings.debug = true;
            } else {
                settings.debug = false;
            }
        } else if (cmd_line[0] == "isready") {
            std::cout << "readyok" << std::endl;
        } else if (cmd_line[0] == "setoption") {
            std::string name;
            std::string value;
            for (int i = 1; i < cmd_line.size(); i += 2) {
                if (cmd_line[i] == "name") {
                    name = cmd_line[i+1];
                } else if (cmd_line[i] == "value") {
                    value = cmd_line[i+1];
                }
            }

            settings.options.insert({name, value});
        } else if (cmd_line[0] == "register") {
            // bonk ig
        } else if (cmd_line[0] == "ucinewgame") {
            // bonk ig
        } else if (cmd_line[0] == "position") {
            std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

            int moves_idx;
            
            if (cmd_line[1] == "fen") {
                fen = cmd_line[2]; // this aint gonna work bc of string splitting :C
                moves_idx = 3;
            } else if (cmd_line[1] == "startpos") {
                moves_idx = 2;
            } else {
                assert(false);
            }

            if (moves_idx < cmd_line.size() && cmd_line[moves_idx] == "moves") {
                for (int i = moves_idx+1; i < cmd_line.size(); i++) {
                    std::string move = cmd_line[i];
                }
            }
        } else if (cmd_line[0] == "go") {
            should_stop = false;
            std::cout << "bestmove e2e4" << std::endl;
            // auto t = std::thread(calculate);
            // t.detach();
        } else if (cmd_line[0] == "stop") {
            should_stop = true;
        } else if (cmd_line[0] == "ponderhit") {
            // bonk ig
        } else if (cmd_line[0] == "quit") {
            should_stop = true;
            return 0;

        }
    }
}

int main(int argc, char **argv)
{

    precalculate();

    std::cout << "The chess board uses " << sizeof(chess_board) << " bytes" << std::endl;
    
    // std::string fen = "";

    std::string fen = "r3kb1r/ppp1pppp/3q1n2/3PN3/3QP3/2N5/PPP2PPP/R3K2R b KQkq - 2 11";

    // std::cout << "1 " << std::endl;

    chess_board chess;
    // std::cout << "2 " << std::endl;
    chess.print();

    int times_to_gen = 1'000'000;

    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < times_to_gen; i++) {
        chess.iterate_over_moves();
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "took: " << elapsed_seconds.count() << "s to generate moves " << times_to_gen << " times" << std::endl;

    // chess.iterate_over_moves();

    return 0;
}