#ifndef RVL_SDK_OS_ALLOC_H
#define RVL_SDK_OS_ALLOC_H
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef int OSHeapHandle;

extern volatile OSHeapHandle __OSCurrHeap;

void* OSInitAlloc(void* arenaStart, void* arenaEnd, int maxHeaps);

OSHeapHandle OSCreateHeap(void* start, void* end);
OSHeapHandle OSSetCurrentHeap(OSHeapHandle heap);

void* OSAllocFromHeap(OSHeapHandle heap, unsigned long size);
void OSFreeToHeap(OSHeapHandle heap, void* ptr);

#ifdef __cplusplus
}
#endif
#endif
