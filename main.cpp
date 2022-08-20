#include <iostream>
#include <array>
#include <unordered_set>
#include "PawnBoard.h"
#include "TranspositionTable.h"

PawnBoard board;

std::array<std::array<uint32_t , 25>, 100> moves_per_depth;

std::vector<std::array<uint64_t, 4>> fake_hash_table;

TranspositionTable tt;

int temp_best_move;

void find_hash(uint64_t white, uint64_t black, uint64_t depth) {
    for (std::array<uint64_t, 4>& entry : fake_hash_table) {
        if (entry[0] == white && entry[1] == black && entry[2] == depth) {
            std::cout << entry[3] << std::endl;
            break;
        }
    }
}

template<bool WHITE>
void hashPerft(int depth) {
    if (!tt.isPresent(board.hashkey(), WHITE, depth)) {
        board.generate_moves<WHITE>(moves_per_depth[depth]);
        for (int i = 1; i <= moves_per_depth[depth][0]; i++) {
            uint32_t move = moves_per_depth[depth][i];
            board.make_move<WHITE>(move);
            if (depth > 1) {
                hashPerft<!WHITE>(depth - 1);
            }
            tt.put(board.hashkey(), !WHITE, depth - 1);
            board.unmake_move<WHITE>(move);
        }
    }
}

template<bool WHITE>
uint64_t perft(int depth) {
    if (depth == 0) {
        return 1;
    }
    uint64_t nodes = 0;
    board.generate_moves<WHITE>(moves_per_depth[depth]);
    for (int i = 1; i <= moves_per_depth[depth][0]; i++) {
        uint32_t move = moves_per_depth[depth][i];
        board.make_move<WHITE>(move);
        nodes += perft<!WHITE>(depth - 1);
        board.unmake_move<WHITE>(move);
    }
    return nodes;
}

template<bool WHITE, uint32_t DEPTH>
int negaMax(int alpha, int beta) {
    board.generate_moves<WHITE>(moves_per_depth[DEPTH]);
    if (moves_per_depth[DEPTH][0] == 0) { // we have no legal moves, i.e. got stalemated
        return board.eval<WHITE>() - 8; // -8 as eval term for stalemated
    } else {
        uint32_t best_move = 255; // -1 byte as non move
        for (int i = 1; i <= moves_per_depth[DEPTH][0]; i++) {
            uint32_t move = moves_per_depth[DEPTH][i];
            board.make_move<WHITE>(move);
            int eval = board.template eval<WHITE>();
            if (eval > 50) {
                // yeah, maybe templating depth wasn't a good idea, requiring this...
            } else if constexpr (DEPTH > 1) {
                eval = -negaMax<!WHITE, DEPTH - 1>(-beta, -alpha);
            }
            board.unmake_move<WHITE>(move);
            if (eval > alpha) {
                alpha = eval;
                best_move = move;
                if (alpha >= beta) {
                    alpha = beta;
                    break;
                }
            }
        }
        if (DEPTH > 10) {
            fake_hash_table.template emplace_back(
                    std::array<uint64_t, 4>{board.white_pieces, board.black_pieces, DEPTH + (WHITE ? 0 : 100),
                                            best_move});
        }
    }
    return alpha;
}

template<bool WHITE>
void setup();

template <bool WHITE>
void search() {
    std::cout << "Eval " << 1 << " is: " << negaMax<WHITE, 1>(-200, 200) << ", move " << temp_best_move << std::endl;
    std::cout << "Eval " << 2 << " is: " << negaMax<WHITE, 2>(-200, 200) << ", move " << temp_best_move << std::endl;
    std::cout << "Eval " << 3 << " is: " << negaMax<WHITE, 3>(-200, 200) << ", move " << temp_best_move << std::endl;
    std::cout << "Eval " << 4 << " is: " << negaMax<WHITE, 4>(-200, 200) << ", move " << temp_best_move << std::endl;
    std::cout << "Eval " << 5 << " is: " << negaMax<WHITE, 5>(-200, 200) << ", move " << temp_best_move << std::endl;
    std::cout << "Eval " << 6 << " is: " << negaMax<WHITE, 6>(-200, 200) << ", move " << temp_best_move << std::endl;
    std::cout << "Eval " << 7 << " is: " << negaMax<WHITE, 7>(-200, 200) << ", move " << temp_best_move << std::endl;
    std::cout << "Eval " << 8 << " is: " << negaMax<WHITE, 8>(-200, 200) << ", move " << temp_best_move << std::endl;
    std::cout << "Eval " << 9 << " is: " << negaMax<WHITE, 9>(-200, 200) << ", move " << temp_best_move << std::endl;
    std::cout << "Eval " << 10 << " is: " << negaMax<WHITE, 10>(-200, 200) << ", move " << temp_best_move << std::endl;
    std::cout << "Eval " << 11 << " is: " << negaMax<WHITE, 11>(-200, 200) << ", move " << temp_best_move << std::endl;
    std::cout << "Eval " << 12 << " is: " << negaMax<WHITE, 12>(-200, 200) << ", move " << temp_best_move << std::endl;
    std::cout << "Eval " << 13 << " is: " << negaMax<WHITE, 13>(-200, 200) << ", move " << temp_best_move << std::endl;
    std::cout << "Eval " << 14 << " is: " << negaMax<WHITE, 14>(-200, 200) << ", move " << temp_best_move << std::endl;
    std::cout << "Eval " << 15 << " is: " << negaMax<WHITE, 15>(-200, 200) << ", move " << temp_best_move << std::endl;
    std::cout << "Eval " << 16 << " is: " << negaMax<WHITE, 16>(-200, 200) << ", move " << temp_best_move << std::endl;
    std::cout << "Eval " << 17 << " is: " << negaMax<WHITE, 17>(-200, 200) << ", move " << temp_best_move << std::endl;
    std::cout << "Eval " << 18 << " is: " << negaMax<WHITE, 18>(-200, 200) << ", move " << temp_best_move << std::endl;
    std::cout << "Eval " << 19 << " is: " << negaMax<WHITE, 19>(-200, 200) << ", move " << temp_best_move << std::endl;
    std::cout << "Eval " << 20 << " is: " << negaMax<WHITE, 20>(-200, 200) << ", move " << temp_best_move << std::endl;
    std::cout << "Eval " << 21 << " is: " << negaMax<WHITE, 21>(-200, 200) << ", move " << temp_best_move << std::endl;
    std::cout << "Eval " << 22 << " is: " << negaMax<WHITE, 22>(-200, 200) << ", move " << temp_best_move << std::endl;
    std::cout << "Eval " << 23 << " is: " << negaMax<WHITE, 23>(-200, 200) << ", move " << temp_best_move << std::endl;
    std::cout << "Eval " << 24 << " is: " << negaMax<WHITE, 24>(-200, 200) << ", move " << temp_best_move << std::endl;
    std::cout << "Eval " << 25 << " is: " << negaMax<WHITE, 25>(-200, 200) << ", move " << temp_best_move << std::endl;
    std::cout << "Eval " << 26 << " is: " << negaMax<WHITE, 26>(-200, 200) << ", move " << temp_best_move << std::endl;
    std::cout << "Eval " << 27 << " is: " << negaMax<WHITE, 27>(-200, 200) << ", move " << temp_best_move << std::endl;
    std::cout << "Eval " << 28 << " is: " << negaMax<WHITE, 28>(-200, 200) << ", move " << temp_best_move << std::endl;
    std::cout << "Eval " << 29 << " is: " << negaMax<WHITE, 29>(-200, 200) << ", move " << temp_best_move << std::endl;
    std::cout << "Table size " << fake_hash_table.size() << std::endl;
    setup<WHITE>();
}

template <bool WHITE>
void setup() {
    std::string command;
    std::cin >> command;
    if (command == "go") {
        search<WHITE>();
    } else {
        if (command == "hash") {
            std::string depth;
            std::cin >> depth;
            find_hash(board.white_pieces, board.black_pieces, std::stoul(depth));
            setup<WHITE>();
        } else {
            board.template make_move<WHITE>(std::stoi(command));
            setup<!WHITE>();
        }
    }
}

int main() {
    //setup<true>();
    for (int i = 1; i < 20; i++) {
        hashPerft<true>(i);
        std::cout << "Position-perft " << i << " is: ";
        tt.printInfo();
    }

    for (int i = 0; i < 20; i++) {
        std::cout << "Perft " << i << " is: " << perft<true>(i) << std::endl;
    }
    return 0;
}