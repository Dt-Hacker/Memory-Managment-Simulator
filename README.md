# 🧠 Memory Management Simulator

A comprehensive **Memory Management Simulator** implemented in **C++**, designed to demonstrate key **Operating System** and **Computer Architecture** concepts such as memory allocation, caching, and virtual memory.

The project provides an **interactive CLI-based simulation environment** and is structured cleanly using header/source separation.

🎥 **Video Demo**  
👉 https://www.youtube.com/watch?v=xxLkp1VCjtI

---

## 📌 Table of Contents

- 🧠 Overview  
- 🧱 Features  
- 🧩 Modules  
  - Contiguous Memory Allocation  
  - CPU Cache Simulation  
  - Buddy Allocator  
  - Virtual Memory  
- 📖 Documentation  
- 🚀 Build & Run  
- 🧪 Testing  
- 📂 Project Structure  
- 📌 Notes & Troubleshooting  
- 🙍‍♂️ Author  

---

## 🧠 Overview

This simulator demonstrates how an operating system manages memory at multiple levels:

- Dynamic memory allocation (**First Fit, Best Fit, Worst Fit**)
- **Buddy system memory allocator**
- **Set-associative multi-level CPU cache**
- **Paging-based virtual memory** with page replacement algorithms

🎯 Ideal for **OS courses, lab evaluations, academic projects, and viva demonstrations**.

---

## 🧱 Features

- ✔ Contiguous memory allocation with fragmentation statistics  
- ✔ Buddy memory allocation with block merging  
- ✔ Configurable CPU cache (**L1 / L2 / L3**)  
- ✔ Cache replacement policies: **FIFO, LRU, LFU**  
- ✔ Virtual memory with **FIFO, LRU, CLOCK**  
- ✔ Interactive CLI with stdin-based test support  

---

## 🧩 Modules

### 💾 Contiguous Memory Allocation
- First Fit  
- Best Fit  
- Worst Fit  

Tracks:
- Memory utilization  
- External fragmentation  
- Allocation success / failure  

---

### ⚡ CPU Cache Simulation
Set-associative cache with configurable:
- Cache size  
- Block size  
- Associativity  
- Replacement policy (**FIFO, LRU, LFU**)  

Simulates:
- Cache hits  
- Cache misses  
- Total access cycles  

---

### 🧠 Buddy Allocator
- Memory divided into **power-of-two blocks**  
- Fast allocation and deallocation  
- Recursive splitting and merging using the **buddy strategy**  

---

### 🌐 Virtual Memory
- Paging-based virtual memory system  
- **Separate page table per process**  
- Page replacement policies:
  - FIFO  
  - LRU  
  - CLOCK  

Simulates:
- Page hits  
- Page faults  

---

## 📖 Documentation

Detailed theoretical explanation and design documentation is provided in:

- **`Memory_managment.docx`**

This document covers:
- Conceptual background
- Algorithm explanations
- Design decisions
- Sample scenarios and outputs

📎 Recommended for **understanding internals and exam preparation**.

---

## 🚀 Build & Run

### 🧰 Using Makefile (Recommended)

Supported on **Linux / WSL / MSYS2 / Git Bash**:

```bash
make
./simulator
```

🧱 Using g++ Directly

If ```make``` is unavailable:

```bash
g++ -std=c++17 main.cpp memory.cpp cache.cpp buddy.cpp virtual_memory.cpp -o simulator
```
Run:

```bash
./simulator
```
📌**Windows users**: Use **WSL** or **Git Bash** or **MSYS2** for correct execution.

---

## 🧪 Testing

Automated testing using stdin redirection:
```
./simulator < tests/test_contiguous.txt
./simulator < tests/test_cache.txt
./simulator < tests/test_buddy.txt
./simulator < tests/test_vm.txt
```

📂 Project Structure
```
memory_management_simulator/
│
├── include/                  # Header files
│   ├── memory.h
│   ├── cache.h
│   ├── buddy.h
│   └── virtual_memory.h
│
├── tests/                    # Test cases
│   ├── test_contiguous.txt
│   ├── test_cache.txt
│   ├── test_buddy.txt
│   └── test_vm.txt
│
├── main.cpp                  # Entry point
├── memory.cpp                # Contiguous allocation
├── cache.cpp                 # Cache simulation
├── buddy.cpp                 # Buddy allocator
├── virtual_memory.cpp        # Virtual memory system
│
├── Makefile
├── Memory_managment.docx     # Detailed documentation
└── README.md
```
---
**🙍‍♂️ Author**

Dakshit Tanay

🎥 Project Demo
👉 https://www.youtube.com/watch?v=xxLkp1VCjtI
