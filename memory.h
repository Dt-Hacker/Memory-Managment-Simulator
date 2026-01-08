#ifndef MEMORY_H
#define MEMORY_H

#include <list>
#include <cstddef>

struct Block
{
    size_t start; // Start address
    size_t size;  // allocated size
    bool free;    // is free or occupied
    int id;       // id of the memory
};

class PhysicalMemory
{
private:
    size_t total_size;
    std::list<Block> blocks; // Double linked lists of block to implement code
    int next_id;             // it basicallly gives new id to the block

public:
    PhysicalMemory(size_t size);
    int allocate_first_fit(size_t req_size);
    int allocate_best_fit(size_t req_size);
    int allocate_worst_fit(size_t req_size);
    void deallocate(int id);
    void dump() const;
};

#endif
