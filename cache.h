#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include <cstdint>

enum class ReplacementPolicy {
    FIFO,
    LRU,
    LFU
};

struct CacheLine {
    bool valid;
    uint64_t tag;
    uint64_t arrival_time; // FIFO
    uint64_t last_used;    // LRU
    uint64_t frequency;    // LFU
};

class Cache {
private:
    size_t cache_size; // total cache size
    size_t block_size; // block size
    size_t associativity; // number of blocks in a sdt
    size_t num_sets; // number of sets

    uint64_t global_time; // time stamp for fifo maintainenece
    uint64_t hit_latency; // total hit latency
    ReplacementPolicy policy; 

    std::vector<std::vector<CacheLine>> sets; // cache as a vector of cachelines with dimension being (number of sets) X (associativity)

    size_t get_set_index(uint64_t address) const; // function to get set index
    uint64_t get_tag(uint64_t address) const; // function to get tag
 
    int find_fifo_victim(size_t set_index); // if FIFO is used the victim to evict
    int find_lru_victim(size_t set_index);  // if LRU is used the victim to evict
    int find_lfu_victim(size_t set_index); // if LFU is used the victim to evict

public:
    uint64_t hits;
    uint64_t misses;
    uint64_t total_cycles; // total latency in terms of cycles 

    Cache(size_t cache_size,
          size_t block_size,
          size_t associativity,
          ReplacementPolicy policy,
          uint64_t hit_latency);

    bool access(uint64_t address);
    uint64_t latency() const { return hit_latency; }
};

#endif
