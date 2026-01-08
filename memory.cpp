#include "memory.h"
#include <iostream>
#include <sstream>

static size_t alloc_requests = 0;
static size_t alloc_success = 0;
static size_t alloc_failure = 0;

PhysicalMemory::PhysicalMemory(size_t size)
{
    total_size = size;
    next_id = 1;
    // Initially one big free block
    blocks.push_back({0, size, true, -1});
}

int PhysicalMemory::allocate_first_fit(size_t req_size)
{
    alloc_requests++;
    for (auto it = blocks.begin(); it != blocks.end(); ++it)
    {
        if (it->free && it->size >= req_size)
        {
            int id = next_id++;
            alloc_success++;
            if (it->size == req_size)
            {
                // Perfect fit
                it->free = false;
                it->id = id;
            }
            else
            {
                // Split block
                Block allocated = {it->start, req_size, false, id};
                Block remaining = {it->start + req_size, it->size - req_size, true, -1};
                it = blocks.erase(it);        // erase the original block
                blocks.insert(it, remaining); // inserts the block
                blocks.insert(it, allocated); // insert the block
            }
            return id;
        }
    }
    alloc_failure++;
    return -1; // Allocation failed
}

int PhysicalMemory::allocate_best_fit(size_t req_size)
{
    alloc_requests++;
    auto best = blocks.end();
    for (auto it = blocks.begin(); it != blocks.end(); ++it)
    {
        if (it->free && it->size >= req_size)
        {
            if (best == blocks.end() || it->size < best->size)
            {
                best = it;
            }
        }
    }
    if (best == blocks.end())
    {
        alloc_failure++;
        return -1; // no suitable block
    }
    int id = next_id++;
    alloc_success++;
    if (best->size == req_size)
    {
        best->free = false;
        best->id = id;
    }
    else
    {
        // Same process as above
        Block allocated = {best->start, req_size, false, id};
        Block remaining = {best->start + req_size, best->size - req_size, true, -1};
        best = blocks.erase(best);
        blocks.insert(best, remaining);
        blocks.insert(best, allocated);
    }

    return id;
}

int PhysicalMemory::allocate_worst_fit(size_t req_size)
{
    alloc_requests++;
    auto worst = blocks.end();

    for (auto it = blocks.begin(); it != blocks.end(); ++it)
    {
        if (it->free && it->size >= req_size)
        {
            if (worst == blocks.end() || it->size > worst->size)
            {
                worst = it;
            }
        }
    }

    if (worst == blocks.end())
    {
        alloc_failure++;
        return -1;
    }
    int id = next_id++;
    alloc_success++;
    if (worst->size == req_size)
    {
        worst->free = false;
        worst->id = id;
    }
    else
    {
        // same process as above
        Block allocated = {worst->start, req_size, false, id};
        Block remaining = {worst->start + req_size, worst->size - req_size, true, -1};
        worst = blocks.erase(worst);
        blocks.insert(worst, remaining);
        blocks.insert(worst, allocated);
    }
    return id;
}

void PhysicalMemory::deallocate(int id)
{
    for (auto it = blocks.begin(); it != blocks.end(); ++it)
    {
        if (!it->free && it->id == id)
        {
            it->free = true;
            it->id = -1;
            // Merge with next block if free
            auto next = it;
            ++next;
            if (next != blocks.end() && next->free)
            {
                it->size += next->size;
                blocks.erase(next);
            }
            // Merge with previous block if free
            if (it != blocks.begin())
            {
                auto prev = it;
                --prev;
                if (prev->free)
                {
                    prev->size += it->size;
                    blocks.erase(it);
                }
            }
            return;
        }
    }
}

// Values requuired to print
void PhysicalMemory::dump() const
{
    size_t used_memory = 0;
    size_t free_memory = 0;
    size_t largest_free = 0;

    for (const auto &b : blocks)
    {
        std::cout << "[0x" << std::hex << b.start << " - 0x" << (b.start + b.size - 1) << std::dec << "] ";
        if (b.free)
        {
            std::cout << "FREE\n";
            free_memory += b.size;
            if (b.size > largest_free)
                largest_free = b.size;
        }
        else
        {
            std::cout << "USED (id=" << b.id << ")\n";
            used_memory += b.size;
        }
    }
    double utilization = (double)used_memory / (double)total_size * 100.0;
    double external_fragmentation = (free_memory == 0) ? 0.0 : (1.0 - (double)largest_free / (double)free_memory) * 100.0;
    double success_rate = (alloc_requests == 0) ? 0.0 : (double)alloc_success / alloc_requests * 100.0;
    std::cout << "\n--- Memory Statistics ---\n";
    std::cout << "Memory utilization: " << utilization << "%\n";
    std::cout << "External fragmentation: " << external_fragmentation << "%\n";
    std::cout << "Allocation success rate: " << success_rate << "%\n";
    std::cout << "Allocation failures: " << alloc_failure << "\n";
}
