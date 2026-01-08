#!/bin/bash

echo "=== Contiguous Allocation ==="
./simulator < tests/test_contiguous.txt

echo "=== Cache Simulation ==="
./simulator < tests/test_cache.txt

echo "=== Buddy Allocator ==="
./simulator < tests/test_buddy.txt

echo "=== Virtual Memory ==="
./simulator < tests/test_vm.txt
