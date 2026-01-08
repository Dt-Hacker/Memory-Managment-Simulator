# 🧠 Memory Management Simulator <br>

A comprehensive **Memory Management Simulator** implemented in **C++**, covering key **Operating System** and **Computer Architecture** concepts such as memory allocation, caching, and virtual memory. <br>
The project provides an **interactive CLI-based simulation environment**. <br><br>

🎥 **Watch the demo:** <br>
https://www.youtube.com/watch?v=xxLkp1VCjtI <br>

---

## 📌 Table of Contents <br>

🧠 Overview <br>
🧱 Features <br>
🧩 Modules <br>
  • Contiguous Memory Allocation <br>
  • Cache Hierarchy <br>
  • Buddy Allocator <br>
  • Virtual Memory <br>
🚀 Build & Run <br>
🧪 Testing <br>
📂 Project Structure <br>
📌 Notes & Troubleshooting <br>
🙍‍♂️ Author <br>

---

## 🧠 Overview <br>

This simulator demonstrates how an operating system manages memory at multiple levels: <br><br>

• Dynamic allocation (**First Fit, Best Fit, Worst Fit**) <br>
• **Buddy system allocator** <br>
• **Multi-level CPU cache simulator** <br>
• **Paging-based virtual memory** with page replacement <br><br>

This project is ideal for **OS courses, academic assignments, and viva demonstrations**. <br>

---

## 🧱 Features <br>

✔ Contiguous memory allocation with statistics <br>
✔ Buddy memory allocation <br>
✔ Configurable multi-level cache (**L1 / L2 / L3**) <br>
✔ Replacement policies: **FIFO, LRU, LFU** <br>
✔ Virtual memory with **FIFO, LRU, CLOCK** <br>
✔ Interactive CLI + test script support <br>

---

## 🧩 Modules <br>

### 💾 Contiguous Memory Allocation <br>

**Supports:** <br>
• First Fit <br>
• Best Fit <br>
• Worst Fit <br><br>

**Tracks:** <br>
• Memory utilization <br>
• External fragmentation <br>
• Allocation success / failure <br>

---

### ⚡ CPU Cache Simulation <br>

Set-associative cache with configurable: <br>
• Cache size <br>
• Block size <br>
• Associativity <br>
• Replacement policy (**FIFO, LRU, LFU**) <br><br>

**Simulates:** <br>
• Cache hits <br>
• Cache misses <br>
• Total access cycles <br>

---

### 🧠 Buddy Allocator <br>

• Memory partitioned into **power-of-two blocks** <br>
• Fast allocation and deallocation <br>
• Recursive merging using the **buddy strategy** <br>

---

### 🌐 Virtual Memory <br>

• Paging support with **page tables per process** <br>
• Page replacement policies: <br>
  • FIFO <br>
  • LRU <br>
  • CLOCK <br><br>

**Simulates:** <br>
• Page hits <br>
• Page faults <br>

---

## 🚀 Build & Run <br>

### 🧰 Using Makefile (Preferred) <br>

If you have a Unix-like environment (**Linux / WSL / MSYS2**): <br>

```bash
make
./simulator
🧱 Using g++ Directly <br>

If make is not available, clone the repository into a folder that has g++ installed and run: <br>

g++ -std=c++17 -Wall -Wextra -O2 \
main.cpp memory.cpp cache.cpp buddy.cpp virtual_memory.cpp \
-o simulator


Then run: <br>

./simulator

📌 On Windows (PowerShell / CMD), use WSL or Git Bash for Linux-style syntax: <br>

wsl ./simulator

🧪 Testing <br>

Automated testing is supported using stdin redirection. <br><br>

/simulator < tests/test_contiguous.txt
./simulator < tests/test_cache.txt
./simulator < tests/test_buddy.txt
./simulator < tests/test_vm.txt
📂 Project Structure <br>
text
Copy code
memory_simulator/
│
├── main.cpp
├── memory.h
├── memory.cpp
├── cache.h
├── cache.cpp
├── buddy.h
├── buddy.cpp
├── virtual_memory.h
├── virtual_memory.cpp
├── tests/
│   ├── test_contiguous.txt
│   ├── test_cache.txt
│   ├── test_buddy.txt
│   └── test_vm.txt
├── Makefile
└── README.md
📌 Notes & Troubleshooting <br>
❓ make: command not found <br><br>
Use: <br>
• WSL (Ubuntu) <br>

bash
Copy code
sudo apt update
sudo apt install build-essential
• OR MSYS2 / Git Bash on Windows <br><br>

❓ ./simulator < tests/... not working <br>
Ensure you are using a Bash-like shell (WSL / Git Bash), not native PowerShell. <br>

🙍‍♂️ Author <br>
Dakshit Tanay <br>

🎥 Video Demo <br>
👉 https://www.youtube.com/watch?v=xxLkp1VCjtI <br>