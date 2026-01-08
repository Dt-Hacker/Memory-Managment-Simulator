#include "cache.h"
#include <limits>

Cache::Cache(size_t csize,size_t bsize,size_t assoc,ReplacementPolicy pol,uint64_t latency):
      cache_size(csize),
      block_size(bsize),
      associativity(assoc),
      policy(pol),
      hit_latency(latency),
      global_time(0),
      hits(0),
      misses(0),
      total_cycles(0)
{ 
    size_t total_blocks = cache_size / block_size;
    num_sets = total_blocks / associativity;

    sets.resize(num_sets,
        std::vector<CacheLine>(associativity, {false, 0, 0, 0, 0}));
}

size_t Cache::get_set_index(uint64_t address) const
{
    return (address / block_size) % num_sets;
}

uint64_t Cache::get_tag(uint64_t address) const
{
    return (address / block_size) / num_sets;
}

/* ---------------- FIFO ---------------- */
int Cache::find_fifo_victim(size_t set_index)
{
    int victim = 0;
    uint64_t oldest = std::numeric_limits<uint64_t>::max();

    for (int i = 0; i < (int)associativity; i++)
    {
        if (!sets[set_index][i].valid) // if the block is empty then it works 
            return i;

        if (sets[set_index][i].arrival_time < oldest)
        {
            oldest = sets[set_index][i].arrival_time;
            victim = i;
        }
    }
    return victim;
}

/* ---------------- LRU ---------------- */
int Cache::find_lru_victim(size_t set_index)
{
    int victim = 0;
    uint64_t least = std::numeric_limits<uint64_t>::max();

    for (int i = 0; i < (int)associativity; i++)
    {
        if (!sets[set_index][i].valid)
            return i;

        if (sets[set_index][i].last_used < least)
        {
            least = sets[set_index][i].last_used;
            victim = i;
        }
    }
    return victim;
}

/* ---------------- LFU ---------------- */
int Cache::find_lfu_victim(size_t set_index)
{
    int victim = 0;
    uint64_t min_freq = std::numeric_limits<uint64_t>::max();

    for (int i = 0; i < (int)associativity; i++)
    {
        if (!sets[set_index][i].valid)
            return i;

        if (sets[set_index][i].frequency < min_freq)
        {
            min_freq = sets[set_index][i].frequency;
            victim = i;
        }
    }
    return victim;
}

bool Cache::access(uint64_t address)
{
    global_time++;
    total_cycles += hit_latency;

    size_t set_index = get_set_index(address);
    uint64_t tag = get_tag(address);

    // HIT
    for (auto &line : sets[set_index])
    {
        if (line.valid && line.tag == tag)
        {
            hits++;
            line.last_used = global_time;
            line.frequency++;
            return true;
        }
    }

    // MISS
    misses++;

    int victim;
    if (policy == ReplacementPolicy::FIFO)
        victim = find_fifo_victim(set_index);
    else if (policy == ReplacementPolicy::LRU)
        victim = find_lru_victim(set_index);
    else
        victim = find_lfu_victim(set_index);

    sets[set_index][victim] = {
        true,
        tag,
        global_time,
        global_time,
        1
    };  

    return false;
}
