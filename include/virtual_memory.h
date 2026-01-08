#ifndef VIRTUAL_MEMORY_H
#define VIRTUAL_MEMORY_H

#include <vector>
#include <map>
#include <cstdint>

enum class PageReplacement {
    FIFO,
    LRU,
    CLOCK
};

struct PageTableEntry {
    bool valid;
    int frame;
    uint64_t arrival;
    uint64_t last_used;
    bool ref_bit;
};

struct FrameInfo {
    int pid; // process id
    int vpn; // virtual page number
};

class VirtualMemory {
private:
    size_t page_size; // page size
    size_t num_frames; // frame size
    uint64_t time; // time for fifo
    PageReplacement policy; // replacement policy

    std::map<int, std::vector<PageTableEntry>> page_tables; // page table
    std::vector<FrameInfo> frame_table; // frame table contains the data of frame

    std::vector<std::vector<uint8_t>> disk; // disk just for simulation
    std::vector<std::vector<uint8_t>> physical_memory; // physical memory 

    size_t clock_hand; // clock hand shows which page to evict according to clock page replacement policy 

    int find_victim_frame();
    void page_out(int pid, int vpn, int frame);
    void page_in(int pid, int vpn, int frame);

public:
    size_t page_hits;
    size_t page_faults;

    VirtualMemory(size_t num_pages,
                  size_t num_frames,
                  size_t page_size,
                  PageReplacement policy);

    void create_process(int pid, size_t num_pages);
    uint64_t translate(int pid, uint64_t virtual_address);
};

#endif
