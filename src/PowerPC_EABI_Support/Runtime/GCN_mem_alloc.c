#include <Runtime/GCN_mem_alloc.h>

#include <revolution/OS.h>

static void InitDefaultHeap(void) {
    void *arenaStart;
    void *arenaEnd;
    OSHeapHandle heap;

    OSReport("GCN_Mem_Alloc.c : InitDefaultHeap. No Heap Available\n");
    OSReport("Metrowerks CW runtime library initializing default heap\n");

    arenaStart = OSGetArenaLo();
    arenaEnd = OSGetArenaHi();

    arenaStart = OSInitAlloc(arenaStart, arenaEnd, 1);
    OSSetArenaLo(arenaStart);

    arenaStart = ROUND_UP_PTR(arenaStart, 32);
    arenaEnd = ROUND_DOWN_PTR(arenaEnd, 32);

    heap = OSCreateHeap(arenaStart, arenaEnd);
    OSSetCurrentHeap(heap);

    OSSetArenaLo(arenaEnd);
}

DECL_WEAK void __sys_alloc(unsigned long size) {
    if (__OSCurrHeap == -1) {
        InitDefaultHeap();
    }
    OSAllocFromHeap(__OSCurrHeap, size);
}

DECL_WEAK void __sys_free(void *block) {
    if (__OSCurrHeap == -1) {
        InitDefaultHeap();
    }

    OSFreeToHeap(__OSCurrHeap, block);
}