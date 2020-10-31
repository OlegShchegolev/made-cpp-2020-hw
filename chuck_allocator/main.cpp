#include "ChunkAllocator.h"

int main() {
    ChunkAllocator<int> alloc1;
    auto alloc2(alloc1);
    auto alloc3 = alloc2;
    return 0;
}