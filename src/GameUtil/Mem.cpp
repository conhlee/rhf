#include "Mem.hpp"

MEMiHeapHead *gHeapMEM1;
MEMiHeapHead *gHeapMEM2;

static bool sHeapInitialized = false;

static u8 sHeapGroupIDStackPos = 0;
static u16 sHeapGroupIDStack[16];

static void *doAlloc(size_t size, EHeapMEM heap, s32 align);

void *operator new(size_t size) {
    if (!sHeapInitialized) {
        memInitHeap();
    }

    BOOL prevInterrupts = OSDisableInterrupts();
    void *alloc = MEMAllocFromExpHeap(gHeapMEM2, size);
    OSRestoreInterrupts(prevInterrupts);

    if (alloc == NULL) {
        OSReport("Can't Alloc Heap\n");
        OSReport(" Required : %d B\n", size);
        OSReport(" Free     : %d B\n", MEMGetTotalFreeSizeForExpHeap(gHeapMEM2));
        OSReport(" Max Size : %d B\n", MEMGetAllocatableSizeForExpHeap(gHeapMEM2));
        OSReport(" Total    : %d B\n", MEMGetHeapTotalSize(gHeapMEM2));
    }

    return alloc;
}

void *operator new(size_t size, EHeapMEM heap, s32 align) {
    return doAlloc(size, heap, align);
}

void *operator new[](size_t size) {
    if (!sHeapInitialized) {
        memInitHeap();
    }

    BOOL prevInterrupts = OSDisableInterrupts();
    void *alloc = MEMAllocFromExpHeap(gHeapMEM2, size);
    OSRestoreInterrupts(prevInterrupts);

    if (alloc == NULL) {
        OSReport("Can't Alloc Heap\n");
        OSReport(" Required : %d B\n", size);
        OSReport(" Free     : %d B\n", MEMGetTotalFreeSizeForExpHeap(gHeapMEM2));
        OSReport(" Max Size : %d B\n", MEMGetAllocatableSizeForExpHeap(gHeapMEM2));
        OSReport(" Total    : %d B\n", MEMGetHeapTotalSize(gHeapMEM2));
    }

    return alloc;
}

void *operator new[](size_t size, EHeapMEM heap, s32 align) {
    return doAlloc(size, heap, align);
}

void operator delete(void *ptr) {
    BOOL prevInterrupts = OSDisableInterrupts();

    if (ptr != NULL) {
        MEMiHeapHead *found = MEMFindContainHeap(ptr);
        if (found == gHeapMEM1) {
            MEMFreeToExpHeap(gHeapMEM1, ptr);
        }
        else if (found == gHeapMEM2) {
            MEMFreeToExpHeap(gHeapMEM2, ptr);
        }
    }

    OSRestoreInterrupts(prevInterrupts);
}

void operator delete[](void *ptr) {
    BOOL prevInterrupts = OSDisableInterrupts();

    if (ptr != NULL) {
        MEMiHeapHead *found = MEMFindContainHeap(ptr);
        if (found == gHeapMEM1) {
            MEMFreeToExpHeap(gHeapMEM1, ptr);
        }
        else if (found == gHeapMEM2) {
            MEMFreeToExpHeap(gHeapMEM2, ptr);
        }
    }

    OSRestoreInterrupts(prevInterrupts);
}

void fn_801D3564(void) {}

void memInitHeap(void) {
    if (!sHeapInitialized) {
        void *arena1Lo = OSGetMEM1ArenaLo();
        void *arena1Hi = OSGetMEM1ArenaHi();
        u32 arena1Size = (uintptr_t)arena1Hi - (uintptr_t)arena1Lo;

        gHeapMEM1 = MEMCreateExpHeap(arena1Lo, arena1Size);
        OSSetMEM1ArenaLo(arena1Hi);

        void *arena2Lo = OSGetMEM2ArenaLo();
        void *arena2Hi = OSGetMEM2ArenaHi();
        u32 arena2Size = (uintptr_t)arena2Hi - (uintptr_t)arena2Lo;

        /*
         * NDEV units had a 128MiB MEM2 size, so if the size of MEM2 exceeds
         * the size of MEM2 on retail (64MiB), reduce it to match.
         */
        if (arena2Size > 0x4000000) {
            arena2Size -= 0x4000000;
            arena2Hi = (u8 *)arena2Hi - 0x4000000;
        }

        gHeapMEM2 = MEMCreateExpHeap(arena2Lo, arena2Size);
        OSSetMEM2ArenaLo(arena2Hi);

        sHeapInitialized = true;

        sHeapGroupIDStackPos = 0;
        MEMSetGroupIDForExpHeap(gHeapMEM1, eMemGroup_Default);
        MEMSetGroupIDForExpHeap(gHeapMEM2, eMemGroup_Default);
    }
}

void fn_801D3634(void) {}

void fn_801D3638(u32) {}

u16 memGetCurGroup(void) {
    return MEMGetGroupIDForExpHeap(gHeapMEM1);
}

void memPopGroup(void) {
    u16 groupID = sHeapGroupIDStack[--sHeapGroupIDStackPos];
    MEMSetGroupIDForExpHeap(gHeapMEM1, groupID);
    MEMSetGroupIDForExpHeap(gHeapMEM2, groupID);
}

void memPushGroup(u16 groupID) {
    sHeapGroupIDStack[sHeapGroupIDStackPos++] = MEMGetGroupIDForExpHeap(gHeapMEM1);
    MEMSetGroupIDForExpHeap(gHeapMEM1, groupID);
    MEMSetGroupIDForExpHeap(gHeapMEM2, groupID);
}

static void freeGroupVisitor(void *ptr, MEMiHeapHead *heap, u32 groupID) {
    if (groupID == MEMGetGroupIDForMBlockExpHeap(ptr)) {
        MEMFreeToExpHeap(heap, ptr);
    }
}

void memFreeGroupInMEM1(u16 groupID) {
    MEMVisitAllocatedForExpHeap(gHeapMEM1, freeGroupVisitor, groupID);
}

void memFreeGroupInMEM2(u16 groupID) {
    MEMVisitAllocatedForExpHeap(gHeapMEM2, freeGroupVisitor, groupID);
}

static void *doAlloc(size_t size, EHeapMEM heap, s32 align) {
    if (!sHeapInitialized) {
        memInitHeap();
    }

    BOOL prevInterrupts = OSDisableInterrupts();

    void *alloc = NULL;
    if (heap == eHeap_MEM1) {
        if (align == 0) {
            alloc = MEMAllocFromExpHeap(gHeapMEM1, size);
        }
        else {
            alloc = MEMAllocFromExpHeapEx(gHeapMEM1, size, align);
        }
    }
    else if (heap == eHeap_MEM2) {
        if (align == 0) {
            alloc = MEMAllocFromExpHeap(gHeapMEM2, size);
        }
        else {
            alloc = MEMAllocFromExpHeapEx(gHeapMEM2, size, align);
        }
    }

    OSRestoreInterrupts(prevInterrupts);

    if (alloc == NULL) {
        OSReport("Can't Alloc Heap\n");
        OSReport(" Required : %d B\n", size);
        OSReport(" Free     : %d B\n", 
            (heap == eHeap_MEM1) ?
                MEMGetTotalFreeSizeForExpHeap(gHeapMEM1) :
                MEMGetTotalFreeSizeForExpHeap(gHeapMEM2)
        );
        OSReport(" Max Size : %d B\n", 
            (heap == eHeap_MEM1) ?
                MEMGetAllocatableSizeForExpHeap(gHeapMEM1) :
                MEMGetAllocatableSizeForExpHeap(gHeapMEM2)
        );
        OSReport(" Total    : %d B\n",
            (heap == eHeap_MEM1) ?
                MEMGetHeapTotalSize(gHeapMEM1) :
                MEMGetHeapTotalSize(gHeapMEM2)
        );
    }

    return alloc;
}
