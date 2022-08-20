#ifndef CHESSENDGAMES_TRANSPOSITIONTABLE_H
#define CHESSENDGAMES_TRANSPOSITIONTABLE_H

#include <cstdint>
#include <iostream>
#include "absl/container/flat_hash_map.h"

class TranspositionTable {

    absl::flat_hash_map<uint64_t , uint32_t > table;

public:
    bool isPresent(uint64_t hash, bool white, uint32_t depth) {
        if (table.contains(hash)) {
            uint32_t value = table[hash];
            if (white && (value & 0xFFFF) >= depth) {
                return true;
            } else if (!white && ((value >> 16) & 0xFFFF) >= depth) {
                return true;
            }
        }
        return false;
    }

    void put(uint64_t hash, bool white, uint32_t depth) {
        if (table.contains(hash)) {
            uint32_t value = table[hash];
            if (white) {
                if ((value & 0xFFFF) > depth) {
                    return;
                } else {
                    table[hash] = (value & (~0xFFFF)) | depth;
                }
            } else {
                if (((value >> 16) & 0xFFFF) > depth) {
                    return;
                } else {
                    table[hash] = (value & (~0xFFFF0000)) | (depth << 16);
                }
            }
        } else {
            if (white) {
                table[hash] = depth;
            } else {
                table[hash] = depth << 16;
            }
        }
    }

    void printInfo() {
        std::cout << table.size() << std::endl;
    }
};

#endif //CHESSENDGAMES_TRANSPOSITIONTABLE_H
