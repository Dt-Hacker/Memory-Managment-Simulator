#include "include/virtual_memory.h"
#include <cstring>
#include <limits>

//constructor

VirtualMemory::VirtualMemory(size_t num_pages,
                             size_t frames,
                             size_t psize,
                             PageReplacement pol)
    : page_size(psize),
      num_frames(frames),
      policy(pol),
      time(0),
      page_hits(0),
      page_faults(0),
      clock_hand(0)
{
    frame_table.resize(num_frames, {-1, -1}); // {process id , page number}
    physical_memory.resize(num_frames, std::vector<uint8_t>(page_size)); // just representational
    disk.resize(num_pages, std::vector<uint8_t>(page_size)); // just representational
}

// this is used to create a process
void VirtualMemory::create_process(int pid, size_t num_pages)
{
    page_tables[pid] = std::vector<PageTableEntry>(num_pages, {false, -1, 0, 0, false});
}

int VirtualMemory::find_victim_frame()
{
    if (policy == PageReplacement::FIFO) {
        uint64_t oldest = UINT64_MAX;
        int victim = -1;

        for (int i = 0; i < (int)num_frames; i++) {
            auto &f = frame_table[i];
            if (f.pid != -1) {
                auto &pte = page_tables[f.pid][f.vpn];
                if (pte.arrival < oldest) { // checks on arrival
                    oldest = pte.arrival; 
                    victim = i;
                }
            }
        }
        return victim;
    }

    if (policy == PageReplacement::LRU) {
        uint64_t least = UINT64_MAX;
        int victim = -1;

        for (int i = 0; i < (int)num_frames; i++) {
            auto &f = frame_table[i];
            if (f.pid != -1) {
                auto &pte = page_tables[f.pid][f.vpn];
                if (pte.last_used < least) { // checks when was it last used
                    least = pte.last_used;
                    victim = i;
                }
            }
        }
        return victim;
    }

    // CLOCK
    while (true) {
        auto &f = frame_table[clock_hand];
        if (f.pid != -1) {
            auto &pte = page_tables[f.pid][f.vpn];
            if (!pte.ref_bit) {
                int victim = clock_hand;
                clock_hand = (clock_hand + 1) % num_frames;
                return victim;
            }
            pte.ref_bit = false;
        }
        clock_hand = (clock_hand + 1) % num_frames;
    }
}

void VirtualMemory::page_out(int pid, int vpn, int frame)
{
    std::memcpy(disk[vpn].data(),physical_memory[frame].data(),page_size);
}

void VirtualMemory::page_in(int pid, int vpn, int frame)
{
    std::memcpy(physical_memory[frame].data(),disk[vpn].data(),page_size);
}

uint64_t VirtualMemory::translate(int pid, uint64_t va) // proces id , virtual memory 
{
    time++;

    auto &ptable = page_tables[pid]; // page table for a process
    size_t vpn = va / page_size; 
    size_t offset = va % page_size;
    auto &pte = ptable[vpn]; // this is page table entry of the chosen page id

    // PAGE HIT
    if (pte.valid) {
        page_hits++;
        pte.last_used = time;
        pte.ref_bit = true;
        return (uint64_t)pte.frame * page_size + offset;
    }

    // PAGE FAULT
    page_faults++;

    int frame = -1;
    for (int i = 0; i < (int)num_frames; i++) {
        if (frame_table[i].pid == -1) {
            frame = i; // checks if any frame is empty
            break;
        }
    }

    // Eviction needed
    if (frame == -1) {
        frame = find_victim_frame(); // for finding victim we just iterate over all the frames and evict according to the policy
        auto &victim = frame_table[frame];
        auto &vpte = page_tables[victim.pid][victim.vpn];
        page_out(victim.pid, victim.vpn, frame);
        vpte.valid = false;
    }

    page_in(pid, vpn, frame);

    pte.valid = true;
    pte.frame = frame;
    pte.arrival = time;
    pte.last_used = time;
    pte.ref_bit = true;

    frame_table[frame] = {pid, (int)vpn};

    return (uint64_t)frame * page_size + offset;
}
