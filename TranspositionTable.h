#ifndef CHESSENDGAMES_TRANSPOSITIONTABLE_H
#define CHESSENDGAMES_TRANSPOSITIONTABLE_H

#include <cstdint>
#include <iostream>
#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"

class TranspositionTable {

    absl::flat_hash_map<uint64_t , uint32_t > table;
    absl::flat_hash_set<uint64_t > final_depth_set;

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
        if (depth == 0) {
            if (!table.contains(hash)) {
                final_depth_set.insert(hash);
            }
        } else {
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
                    final_depth_set.erase(hash);
                } else {
                    table[hash] = depth << 16;
                    final_depth_set.erase(hash);
                }
            }
        }
    }

    void printInfo() {
        std::cout << table.size() + final_depth_set.size() << std::endl;
    }
};

#endif //CHESSENDGAMES_TRANSPOSITIONTABLE_H
