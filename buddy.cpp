#include "include/buddy.h"
#include <iostream>
#include <cmath>
#include <stdexcept>

BuddyAllocator::BuddyAllocator(size_t total, size_t min_block) : total_size(total), min_block_size(min_block)
{
    max_order = get_order(total_size);
    free_lists[max_order].insert(0); // entire memory free
}

int BuddyAllocator::get_order(size_t size) const // returns order
{
    int order = 0;
    size_t block = min_block_size;
    while (block < size)
    {
        block <<= 1;
        order++;
    }
    return order;
}

size_t BuddyAllocator::get_block_size(int order) const // gives the block size
{
    return min_block_size << order;
}

size_t BuddyAllocator::get_buddy(size_t addr, int order) const
{
    size_t block_size = get_block_size(order); // buddy is the xor with block size as
    return addr ^ block_size;                  // the addresses only by a bit so taking xor with
                                               // block size gives buddy address
}

size_t BuddyAllocator::allocate(size_t size)
{
    int order = get_order(size);

    int current_order = order;
    while (current_order <= max_order && free_lists[current_order].empty())
    {
        current_order++;
    }
    if (current_order > max_order)
        throw std::runtime_error("Out of memory");
    // Split blocks until desired order
    size_t addr = *free_lists[current_order].begin();
    free_lists[current_order].erase(addr);
    // splitting the selected block untill we get the required size 
    while (current_order > order)
    {
        current_order--;
        size_t buddy = addr + get_block_size(current_order);
        free_lists[current_order].insert(buddy);
    }
    allocated[addr] = order;
    return addr;
}

void BuddyAllocator::deallocate(size_t addr)
{
    auto it = allocated.find(addr);
    if (it == allocated.end())
        throw std::runtime_error("Invalid free");
    int order = it->second;
    allocated.erase(it);
    while (order < max_order)
    {
        size_t buddy = get_buddy(addr, order);
        auto &free_set = free_lists[order];
        if (free_set.find(buddy) == free_set.end())
            break;
        // Merge with buddy
        free_set.erase(buddy);
        addr = std::min(addr, buddy);
        order++;
    }
    free_lists[order].insert(addr);
}

void BuddyAllocator::dump() const
{
    std::cout << "===== Buddy Allocator State(Free Block Addresses) =====\n";
    for (const auto &p : free_lists)
    {
        std::cout << "Order " << p.first<< " (Block size "<< get_block_size(p.first) << "): ";
        for (auto addr : p.second)
            std::cout << addr << " ";
        std::cout << "\n";
    }
}
