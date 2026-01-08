#include <iostream>
#include <cstdint>
#include <string>
#include <sstream>

// your already-written modules
#include "include/memory.h"
#include "include/cache.h"
#include "include/virtual_memory.h"
#include "include/buddy.h"

using namespace std;

int main()
{
    cout << "========================================\n";
    cout << " Welcome to the Memory Simulator\n";
    cout << "========================================\n";

    while (true)
    {
        cout << "\nWhat would you like to simulate?\n";
        cout << "1. Contiguous Memory Allocation\n";
        cout << "2. Cache Simulation\n";
        cout << "3. Buddy Allocation Simulation\n";
        cout << "4. Virtual Memory Simulation\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";

        string choice;
        cin >> choice;

        // ================= CONTIGUOUS MEMORY =================
        if (choice == "1")
        {
            PhysicalMemory *mem = nullptr;
            string allocator = "first_fit";

            cin.ignore();
            while (true)
            {
                cout << "> ";
                string line;
                getline(cin, line);
                stringstream ss(line);

                string cmd;
                ss >> cmd;

                if (cmd == "init")
                {
                    string memword;
                    size_t size;
                    ss >> memword >> size;

                    delete mem;
                    mem = new PhysicalMemory(size);
                    cout << "Memory initialized with size " << size << "\n";
                }
                else if (cmd == "set")
                {
                    string what, policy;
                    ss >> what >> policy;
                    allocator = policy;
                    if (policy == "best_fit" || policy == "first_fit" || policy == "worst_fit")
                        cout << "Allocator set to " << policy << "\n";
                    else
                        cout << "Invalid policy" << "\n";
                }
                else if (cmd == "malloc")
                {
                    size_t s;
                    ss >> s;
                    int id = -1;
                    if (allocator == "first_fit")
                        id = mem->allocate_first_fit(s);
                    else if (allocator == "best_fit")
                        id = mem->allocate_best_fit(s);
                    else if (allocator == "worst_fit")
                        id = mem->allocate_worst_fit(s);

                    if (id != -1)
                        cout << "Allocated block id=" << id << "\n";
                    else
                        cout << "Allocation failed\n";
                }
                else if (cmd == "free")
                {
                    int id;
                    ss >> id;
                    mem->deallocate(id);
                    cout << "Block " << id << " freed and merged\n";
                }
                else if (cmd == "dump")
                {
                    mem->dump();
                }
                else if (cmd == "stats")
                {
                    mem->dump(); // stats already printed inside dump
                }
                else if (cmd == "exit")
                {
                    delete mem;
                    break;
                }
                else if (cmd == "help")
                {
                    cout << "init memory <size>      : Initialize memory (e.g., init memory 1024)\n";
                    cout << "set allocator <type>    : Set allocator (first_fit | best_fit | worst_fit)\n";
                    cout << "malloc <size>           : Allocate memory (e.g., malloc 200)\n";
                    cout << "free <id>               : Free allocated block by ID\n";
                    cout << "dump                    : Show memory status\n";
                    cout << "help                    : Show this help menu\n";
                    cout << "exit                    : Exit simulator\n\n";

                    cout << "NOTE: Always type 'memory' in init command (stringstream parsing).\n";
                }
                else
                {
                    cout << "Unknown command\n";
                }
            }
        }

        // ================= MULTI-LEVEL CACHE SIMULATION =================
        else if (choice == "2")
        {
            Cache *L1 = nullptr;
            Cache *L2 = nullptr;
            Cache *L3 = nullptr;

            const uint64_t RAM_LATENCY = 100;
            uint64_t total_cycles = 0;

            cin.ignore();
            while (true)
            {
                cout << "> ";
                string line;
                getline(cin, line);
                stringstream ss(line);

                string cmd;
                ss >> cmd;

                // -------- INIT CACHE --------
                if (cmd == "init")
                {
                    size_t c1, c2, c3;
                    size_t bsize, a1, a2, a3;
                    uint64_t l1_lat, l2_lat, l3_lat;

                    cout << "--- L1 Cache ---\n";
                    cout << "Size(in bytes): ";
                    cin >> c1;
                    cout << "Block size: ";
                    cin >> bsize;
                    cout << "Associativity: ";
                    cin >> a1;
                    cout << "Latency: ";
                    cin >> l1_lat;

                    cout << "--- L2 Cache ---\n";
                    cout << "Size(in bytes): ";
                    cin >> c2;
                    cout << "Associativity: ";
                    cin >> a2;
                    cout << "Latency: ";
                    cin >> l2_lat;

                    cout << "--- L3 Cache ---\n";
                    cout << "Size(in bytes): ";
                    cin >> c3;
                    cout << "Associativity: ";
                    cin >> a3;
                    cout << "Latency: ";
                    cin >> l3_lat;

                    cout << "Replacement Policy (fifo / lru / lfu): ";
                    string pol;
                    cin >> pol;
                    if (pol != "fifo" && pol != "lru" && pol != "lfu")
                    {
                        cout << "Invalid policy , Policy set to LFU" << "\n";
                    }
                    ReplacementPolicy rp =
                        (pol == "fifo") ? ReplacementPolicy::FIFO : (pol == "lru") ? ReplacementPolicy::LRU
                                                                                   : ReplacementPolicy::LFU;

                    delete L1;
                    delete L2;
                    delete L3;

                    L1 = new Cache(c1, bsize, a1, rp, l1_lat);
                    L2 = new Cache(c2, bsize, a2, rp, l2_lat);
                    L3 = new Cache(c3, bsize, a3, rp, l3_lat);

                    total_cycles = 0;
                    cout << "Caches initialized successfully\n";

                    cin.ignore();
                }

                // -------- ACCESS ADDRESS --------
                else if (cmd == "access")
                {
                    if (!L1 || !L2 || !L3)
                    {
                        cout << "Cache not initialized\n";
                        continue;
                    }

                    uint64_t addr;
                    ss >> addr;

                    if (L1->access(addr))
                    {
                        total_cycles += L1->latency();
                        cout << "L1 HIT\n";
                    }
                    else if (L2->access(addr))
                    {
                        total_cycles += L1->latency() + L2->latency();
                        L1->access(addr);
                        cout << "L2 HIT\n";
                    }
                    else if (L3->access(addr))
                    {
                        total_cycles += L1->latency() + L2->latency() + L3->latency();
                        L2->access(addr);
                        L1->access(addr);
                        cout << "L3 HIT\n";
                    }
                    else
                    {
                        total_cycles += L1->latency() + L2->latency() +
                                        L3->latency() + RAM_LATENCY;
                        L3->access(addr);
                        L2->access(addr);
                        L1->access(addr);
                        cout << "MISS -> RAM ACCESS\n";
                    }
                }

                // -------- STATS --------
                else if (cmd == "stats")
                {
                    if (!L1)
                    {
                        cout << "Cache not initialized\n";
                        continue;
                    }

                    cout << "\n--- CACHE STATS ---\n";
                    cout << "L1 Hits: " << L1->hits << " Misses: " << L1->misses << "\n";
                    cout << "L2 Hits: " << L2->hits << " Misses: " << L2->misses << "\n";
                    cout << "L3 Hits: " << L3->hits << " Misses: " << L3->misses << "\n";
                    cout << "Total cycles: " << total_cycles << "\n";
                }

                // -------- EXIT --------
                else if (cmd == "exit")
                {
                    delete L1;
                    delete L2;
                    delete L3;
                    break;
                }
                else if (cmd == "help")
                {
                    cout << "init -> initialize L1, L2, L3 caches" << "\n";
                    cout << "access <addr> -> access a physical address" << "\n";
                    cout << "stats ->show cache statistics" << "\n";
                    cout << "exit ->go back to main menu" << "\n";
                }
                else
                {
                    cout << "Unknown command\n";
                }
            }
        }
        // ================= BUDDY ALLOCATOR =================
        else if (choice == "3")
        {
            size_t total, min_block;
            cout << "Total memory size: ";
            cin >> total;
            cout << "Minimum block size: ";
            cin >> min_block;

            BuddyAllocator buddy(total, min_block);
            cin.ignore();
            while (true)
            {
                cout << "> ";
                string line;
                getline(cin, line);
                stringstream ss(line);
                string cmd;
                ss >> cmd;
                if (cmd == "alloc")
                {
                    size_t s;
                    if (!(ss >> s))
                    {
                        cout << "Usage: alloc <size>\n";
                        continue;
                    }

                    try
                    {
                        cout << "Address = " << buddy.allocate(s) << "\n";
                    }
                    catch (...)
                    {
                        cout << "Allocation failed\n";
                    }
                }
                else if (cmd == "free")
                {
                    size_t addr;
                    if (!(ss >> addr))
                    {
                        cout << "Usage: free <address>\n";
                        continue;
                    }

                    try
                    {
                        buddy.deallocate(addr);
                        cout << "Block freed and merged if possible\n";
                    }
                    catch (...)
                    {
                        cout << "Invalid free\n";
                    }
                }
                else if (cmd == "dump")
                {
                    buddy.dump();
                }
                else if (cmd == "help")
                {
                    cout << "alloc <size>     : Allocate memory\n";
                    cout << "free <address>   : Free memory block\n";
                    cout << "dump             : Show allocator state\n";
                    cout << "back             : Return to previous menu\n";
                }
                else if (cmd == "back")
                {
                    break;
                }
                else
                {
                    cout << "Unknown command\n";
                }
            }
        }

        // ================= VIRTUAL MEMORY =================
        else if (choice == "4")
        {
            cout << "========================================\n";
            cout << " Virtual Memory + Cache Simulator\n";
            cout << "========================================\n";

            size_t num_processes;
            size_t pages_per_process;
            size_t num_frames;
            size_t page_size;
            int policy;

            cout << "Number of processes: ";
            cin >> num_processes;

            cout << "Pages per process: ";
            cin >> pages_per_process;

            cout << "Number of frames: ";
            cin >> num_frames;

            cout << "Page size (bytes): ";
            cin >> page_size;

            cout << "Page Replacement Policy (0=FIFO, 1=LRU, 2=CLOCK): ";
            cin >> policy;

            PageReplacement pr =
                (policy == 0) ? PageReplacement::FIFO : (policy == 1) ? PageReplacement::LRU
                                                                      : PageReplacement::CLOCK;

            VirtualMemory vm(
                pages_per_process,
                num_frames,
                page_size,
                pr);

            // Create processes
            for (int pid = 0; pid < (int)num_processes; pid++)
                vm.create_process(pid, pages_per_process);

            cout << "\nVirtual Memory initialized successfully\n";

            /* ---------- STANDARD CACHE CONFIG ---------- */
            const uint64_t RAM_LATENCY = 100;

            Cache L1(
                32 * 1024, // 32 KB
                64,        // block size
                4,         // associativity
                ReplacementPolicy::LRU,
                1 // latency
            );

            Cache L2(
                256 * 1024, // 256 KB
                64,
                8,
                ReplacementPolicy::LRU,
                5);

            Cache L3(
                2 * 1024 * 1024, // 2 MB
                64,
                16,
                ReplacementPolicy::LRU,
                20);

            uint64_t total_cycles = 0;

            cin.ignore();

            while (true)
            {
                cout << "\n> ";
                string line;
                getline(cin, line);
                stringstream ss(line);

                string cmd;
                ss >> cmd;

                // -------- ACCESS --------
                if (cmd == "access")
                {
                    int pid;
                    uint64_t va;
                    ss >> pid >> va;

                    if (pid < 0 || pid >= (int)num_processes)
                    {
                        cout << "Invalid PID\n";
                        continue;
                    }

                    uint64_t pa = vm.translate(pid, va);
                    cout << "Physical Address = " << pa << "\n";

                    if (L1.access(pa))
                    {
                        total_cycles += L1.latency();
                        cout << "L1 HIT\n";
                    }
                    else if (L2.access(pa))
                    {
                        total_cycles += L1.latency() + L2.latency();
                        L1.access(pa);
                        cout << "L2 HIT\n";
                    }
                    else if (L3.access(pa))
                    {
                        total_cycles += L1.latency() + L2.latency() + L3.latency();
                        L2.access(pa);
                        L1.access(pa);
                        cout << "L3 HIT\n";
                    }
                    else
                    {
                        total_cycles += L1.latency() + L2.latency() +
                                        L3.latency() + RAM_LATENCY;
                        L3.access(pa);
                        L2.access(pa);
                        L1.access(pa);
                        cout << "MISS -> RAM ACCESS\n";
                    }
                }

                // -------- STATS --------
                else if (cmd == "stats")
                {
                    cout << "\n--- VIRTUAL MEMORY STATS ---\n";
                    cout << "Page Hits: " << vm.page_hits << "\n";
                    cout << "Page Faults: " << vm.page_faults << "\n";

                    cout << "\n--- CACHE STATS ---\n";
                    cout << "L1 Hits: " << L1.hits << " Misses: " << L1.misses << "\n";
                    cout << "L2 Hits: " << L2.hits << " Misses: " << L2.misses << "\n";
                    cout << "L3 Hits: " << L3.hits << " Misses: " << L3.misses << "\n";
                    cout << "Total Cycles: " << total_cycles << "\n";
                }

                // -------- HELP --------
                else if (cmd == "help")
                {
                    cout << "access <pid> <va>  : Access virtual address\n";
                    cout << "stats              : Show VM and cache stats\n";
                    cout << "exit               : Exit simulator\n";
                }

                // -------- EXIT --------
                else if (cmd == "exit")
                {
                    break;
                }
                else
                {
                    cout << "Unknown command\n";
                }
            }
        }

        // ================= EXIT =================
        else if (choice == "5")
        {
            cout << "Exiting simulator...\n";
            break;
        }
        else
        {
            cout << "Invalid choice\n";
        }
    }

    return 0;
}
