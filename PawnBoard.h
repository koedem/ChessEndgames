#ifndef CHESSENDGAMES_PAWNBOARD_H
#define CHESSENDGAMES_PAWNBOARD_H

#include <cstdint>
#include <vector>
#include <cstring>
#include <bit>

class PawnBoard {


public:
/**
     * bit order: a2, b2, ..., h2, a3, ..., h7 from lowest to highest bit, i.e. & 1 gives a2 occupancy
     *     could in theory be changed for wider board; note en_passant code though
     */
    uint64_t white_pieces = 0xFF;
    uint64_t black_pieces = 0xFF0000000000;

private:
    uint64_t all_pieces = white_pieces | black_pieces;
    bool to_move = true;

    static constexpr int number_of_files = 8; // a to h are 8 for normal chess, note en_passant code if changed
    static constexpr uint64_t full_row = (1L << number_of_files) - 1;
    static constexpr int changes[] = { number_of_files, number_of_files * 2, number_of_files - 1, number_of_files + 1 };


public:
    uint64_t hashkey() {
        uint64_t smallKey = all_pieces;
        uint64_t white = white_pieces, black = black_pieces;
        for (int i = 0; i < 16; i++) {
            smallKey <<= 1;
            if (white != 0 && ffsl(white) < ffsl(black)) {
                smallKey |= 1;
                white &= ~(1 << (ffsl(white) - 1));
            } else {
                black &= ~(1 << (ffsl(black) - 1));
            }
        }
        return smallKey;
    }

    /**
     *
     * @tparam WHITE the side who just moved
     * @return
     */
    template<bool WHITE>
    int eval() {
        int eval;
        if constexpr (WHITE) {
            eval = std::popcount(white_pieces) - std::popcount(black_pieces); // material
            if ((white_pieces & (full_row << (number_of_files * 5))) != 0) {
                eval += 128;
            }
        } else {
            eval = std::popcount(black_pieces) - std::popcount(white_pieces); // material
            if ((black_pieces & full_row) != 0) {
                eval += 128;
            }
        }
        return eval;
    }

    /**
     *
     * @tparam WHITE who to move it is before the move
     * @param move
     */
    template<bool WHITE>
    void make_move(uint32_t move) {
        uint64_t start_square = move & 63;
        uint64_t move_type = move >> 6;
        uint64_t change = 1L << start_square;
        if constexpr (WHITE) {
            uint64_t destination_square = start_square + changes[move_type];
            white_pieces ^= change | 1L << destination_square;
            if (move_type >= 2) { // capture
                black_pieces ^= 1L << destination_square;
            } else {
                change ^= 1L << destination_square;
            }
        } else {
            uint64_t destination_square = start_square - changes[move_type];
            black_pieces ^= change | 1L << destination_square;
            if (move_type >= 2) { // capture
                white_pieces ^= 1L << destination_square;
            } else {
                change ^= 1L << destination_square;
            }
        }
        all_pieces ^= change;
        to_move = !to_move;
    }

    /**
     *
     * @tparam WHITE who to move it was before the move, i.e. whose move it will be after the unmove
     * @param move
     */
    template<bool WHITE>
    void unmake_move(uint32_t move) {
        make_move<WHITE>(move); // ye, actually I think
    }

    /**
     *
     * @tparam WHITE
     * @param moves
     * @return true if the player to move can win this move (in this case the moves array will be empty)
     *      or false otherwise, in this case the moves array will contain all legal moves
     */
    template<bool WHITE>
    bool generate_moves(std::array<uint32_t, 25>& moves) {
        moves[0] = 0;
        if constexpr (WHITE) {
            uint64_t pieces = white_pieces;
            while (pieces != 0) {
                int pos = 63 - __builtin_clzl(pieces);
                if ((all_pieces & (1L << (pos + number_of_files))) == 0) {
                    moves[++moves[0]] = pos;
                    if (pos < number_of_files && (all_pieces & (1L << (pos + number_of_files * 2))) == 0) {
                        moves[++moves[0]] = pos | (1 << 6);
                    }
                }
                if ((black_pieces & (1L << (pos + (number_of_files - 1)))) != 0) {
                    moves[++moves[0]] = pos | (2 << 6);
                }
                if ((black_pieces & (1L << (pos + (number_of_files + 1)))) != 0) {
                    moves[++moves[0]] = pos | (3 << 6);
                }
                pieces &= ~(1L << pos);
            }
        } else {
            uint64_t pieces = black_pieces;
            while (pieces != 0) {
                int pos = ffsl(pieces) - 1;
                // TODO captures
                if ((all_pieces & (1L << (pos - number_of_files))) == 0) {
                    moves[++moves[0]] = pos;
                    if (pos >= number_of_files * 5 && (all_pieces & (1L << (pos - number_of_files * 2))) == 0) {
                        moves[++moves[0]] = pos | (1 << 6);
                    }
                }
                if ((white_pieces & (1L << (pos - (number_of_files - 1)))) != 0) {
                    moves[++moves[0]] = pos | (2 << 6);
                }
                if ((white_pieces & (1L << (pos - (number_of_files + 1)))) != 0) {
                    moves[++moves[0]] = pos | (3 << 6);
                }
                pieces &= ~(1L << pos);
            }
        }
        return false;
    }
};


#endif //CHESSENDGAMES_PAWNBOARD_H
