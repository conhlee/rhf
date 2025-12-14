#include "Mem.hpp"

MEMiHeapHead *lbl_80320F80; // gHeapMEM1
MEMiHeapHead *lbl_80320F84; // gHeapMEM2
static bool lbl_80320F88; // sHeapInitialized

static u8 lbl_80320F89; // sHeapGroupIDStackPos
static u16 lbl_803D5C28[16]; // sHeapGroupIDStack

static void *fn_801D3784(size_t size, EHeapMEM heap, s32 align); // doAlloc

void *operator new(size_t size) {
    if (!lbl_80320F88) {
        fn_801D3568();
    }

    BOOL interrupt = OSDisableInterrupts();
    void *alloc = MEMAllocFromExpHeap(lbl_80320F84, size);
    OSRestoreInterrupts(interrupt);

    if (alloc == NULL) {
        OSReport("Can't Alloc Heap\n");
        OSReport(" Required : %d B\n", size);
        OSReport(" Free     : %d B\n", MEMGetTotalFreeSizeForExpHeap(lbl_80320F84));
        OSReport(" Max Size : %d B\n", MEMGetAllocatableSizeForExpHeap(lbl_80320F84));
        OSReport(" Total    : %d B\n", MEMGetHeapTotalSize(lbl_80320F84));
    }

    return alloc;
}

void *operator new(size_t size, EHeapMEM heap, s32 align) {
    return fn_801D3784(size, heap, align);
}

void *operator new[](size_t size) {
    if (!lbl_80320F88) {
        fn_801D3568();
    }

    BOOL interrupt = OSDisableInterrupts();
    void *alloc = MEMAllocFromExpHeap(lbl_80320F84, size);
    OSRestoreInterrupts(interrupt);

    if (alloc == NULL) {
        OSReport("Can't Alloc Heap\n");
        OSReport(" Required : %d B\n", size);
        OSReport(" Free     : %d B\n", MEMGetTotalFreeSizeForExpHeap(lbl_80320F84));
        OSReport(" Max Size : %d B\n", MEMGetAllocatableSizeForExpHeap(lbl_80320F84));
        OSReport(" Total    : %d B\n", MEMGetHeapTotalSize(lbl_80320F84));
    }

    return alloc;
}

void *operator new[](size_t size, EHeapMEM heap, s32 align) {
    return fn_801D3784(size, heap, align);
}

void operator delete(void *ptr) {
    BOOL interrupt = OSDisableInterrupts();

    if (ptr != NULL) {
        MEMiHeapHead *found = MEMFindContainHeap(ptr);
        if (found == lbl_80320F80) {
            MEMFreeToExpHeap(lbl_80320F80, ptr);
        }
        else if (found == lbl_80320F84) {
            MEMFreeToExpHeap(lbl_80320F84, ptr);
        }
    }

    OSRestoreInterrupts(interrupt);
}

void operator delete[](void *ptr) {
    BOOL interrupt = OSDisableInterrupts();

    if (ptr != NULL) {
        MEMiHeapHead *found = MEMFindContainHeap(ptr);
        if (found == lbl_80320F80) {
            MEMFreeToExpHeap(lbl_80320F80, ptr);
        }
        else if (found == lbl_80320F84) {
            MEMFreeToExpHeap(lbl_80320F84, ptr);
        }
    }

    OSRestoreInterrupts(interrupt);
}

void fn_801D3564(void) {}

// Initialize the heaps
void fn_801D3568(void) {
    if (!lbl_80320F88) {
        void *arena1Lo = OSGetMEM1ArenaLo();
        void *arena1Hi = OSGetMEM1ArenaHi();
        u32 arena1Size = (u8 *)arena1Hi - (u8 *)arena1Lo;

        lbl_80320F80 = MEMCreateExpHeap(arena1Lo, arena1Size);
        OSSetMEM1ArenaLo(arena1Hi);

        void *arena2Lo = OSGetMEM2ArenaLo();
        void *arena2Hi = OSGetMEM2ArenaHi();
        u32 arena2Size = (u8 *)arena2Hi - (u8 *)arena2Lo;

        // NDEV units had a 128MB MEM2 size, so if the size of MEM2
        // exceeds the size of MEM2 on retail, reduce it to match.
        if (arena2Size > 0x4000000) {
            arena2Size -= 0x4000000;
            arena2Hi = (u8 *)arena2Hi - 0x4000000;
        }

        lbl_80320F84 = MEMCreateExpHeap(arena2Lo, arena2Size);
        OSSetMEM2ArenaLo(arena2Hi);

        lbl_80320F88 = true;

        lbl_80320F89 = 0;
        MEMSetGroupIDForExpHeap(lbl_80320F80, 0);
        MEMSetGroupIDForExpHeap(lbl_80320F84, 0);
    }
}

void fn_801D3634(void) {}

void fn_801D3638(u32) {}

u16 fn_801D363C(void) {
    return MEMGetGroupIDForExpHeap(lbl_80320F80);
}

void fn_801D3644(void) {
    u16 groupID = lbl_803D5C28[--lbl_80320F89];
    MEMSetGroupIDForExpHeap(lbl_80320F80, groupID);
    MEMSetGroupIDForExpHeap(lbl_80320F84, groupID);
}

void fn_801D369C(u16 groupID) {
    lbl_803D5C28[lbl_80320F89++] = MEMGetGroupIDForExpHeap(lbl_80320F80);
    MEMSetGroupIDForExpHeap(lbl_80320F80, groupID);
    MEMSetGroupIDForExpHeap(lbl_80320F84, groupID);
}

static void fn_801D3700(void *ptr, MEMiHeapHead *heap, u32 groupID) {
    if (groupID == MEMGetGroupIDForMBlockExpHeap(ptr)) {
        MEMFreeToExpHeap(heap, ptr);
    }
}

void fn_801D375C(u16 groupID) {
    MEMVisitAllocatedForExpHeap(lbl_80320F80, fn_801D3700, groupID);
}

void fn_801D3770(u16 groupID) {
    MEMVisitAllocatedForExpHeap(lbl_80320F84, fn_801D3700, groupID);
}

static void *fn_801D3784(size_t size, EHeapMEM heap, s32 align) {
    if (!lbl_80320F88) {
        fn_801D3568();
    }

    BOOL interrupt = OSDisableInterrupts();

    void *alloc = NULL;
    if (heap == eHeap_MEM1) {
        if (align == 0) {
            alloc = MEMAllocFromExpHeap(lbl_80320F80, size);
        }
        else {
            alloc = MEMAllocFromExpHeapEx(lbl_80320F80, size, align);
        }
    }
    else if (heap == eHeap_MEM2) {
        if (align == 0) {
            alloc = MEMAllocFromExpHeap(lbl_80320F84, size);
        }
        else {
            alloc = MEMAllocFromExpHeapEx(lbl_80320F84, size, align);
        }
    }

    OSRestoreInterrupts(interrupt);

    if (alloc == NULL) {
        OSReport("Can't Alloc Heap\n");
        OSReport(" Required : %d B\n", size);
        OSReport(" Free     : %d B\n", 
            (heap == eHeap_MEM1) ?
                MEMGetTotalFreeSizeForExpHeap(lbl_80320F80) :
                MEMGetTotalFreeSizeForExpHeap(lbl_80320F84)
        );
        OSReport(" Max Size : %d B\n", 
            (heap == eHeap_MEM1) ?
                MEMGetAllocatableSizeForExpHeap(lbl_80320F80) :
                MEMGetAllocatableSizeForExpHeap(lbl_80320F84)
        );
        OSReport(" Total    : %d B\n",
            (heap == eHeap_MEM1) ?
                MEMGetHeapTotalSize(lbl_80320F80) :
                MEMGetHeapTotalSize(lbl_80320F84)
        );
    }

    return alloc;
}
