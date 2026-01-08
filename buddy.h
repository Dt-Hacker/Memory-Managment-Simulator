    #ifndef BUDDY_ALLOCATOR_H
    #define BUDDY_ALLOCATOR_H

    #include <map>
    #include <set>
    #include <cstddef>

    class BuddyAllocator {
    private:
        size_t total_size; // size of memory 
        size_t min_block_size; // minimum allocatable block size
        int max_order; //Order is the difference in the power of 2 from min block size
        // Free list store the address in a set for all orders
        std::map<int, std::set<size_t>> free_lists;
        // allocated address -> order
        std::map<size_t, int> allocated;

        int get_order(size_t size) const;
        size_t get_block_size(int order) const;
        size_t get_buddy(size_t addr, int order) const; // buddy is the block of same size made when block
                                                        // is broken
    public:
        BuddyAllocator(size_t total_size, size_t min_block_size);
        size_t allocate(size_t size);
        void deallocate(size_t addr);
        void dump() const;
    };

    #endif
        