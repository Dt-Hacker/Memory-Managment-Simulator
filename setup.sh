#!/usr/bin/env bash

# ==========================================
# Memory Management Simulator - Setup Script
# ==========================================

set -e

echo " Setting up Memory Management Simulator..."

# Check for g++
if ! command -v g++ &> /dev/null; then
    echo "g++ compiler not found."
    echo "➡ Please install g++ and try again."
    echo ""
    echo "Ubuntu / WSL:"
    echo "  sudo apt update && sudo apt install build-essential"
    exit 1
fi

echo "g++ found"

# Compile source files
echo "  Building project..."

g++ -std=c++17 \
    main.cpp \
    memory.cpp \
    cache.cpp \
    buddy.cpp \
    virtual_memory.cpp \
    -o simulator

echo ""
echo "Build successful!"
echo " Run the simulator using:"
echo " ./simulator"
