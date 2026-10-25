#ifndef GAMEUTIL_MEM_HPP
#define GAMEUTIL_MEM_HPP

#include <revolution/types.h>
#include <revolution/OS.h>
#include <revolution/MEM.h>

#include <new>

enum EHeapMEM {
    eHeap_MEM1,
    eHeap_MEM2,
};

enum EMemGroup {
    eMemGroup_Default,
    eMemGroup_CommonAsset,  // allocate common assets (font, mesg, common cellanim & layout, etc.)
    eMemGroup_SceneAsset,   // allocate scene assets (archives, etc.)
    eMemGroup_Scene,        // allocate scene object
    eMemGroup_SceneEx,      // allocate scene object (used only by SceneError)
};

extern MEMiHeapHead *gHeapMEM1;
extern MEMiHeapHead *gHeapMEM2;

void *operator new(size_t size);
void *operator new(size_t size, EHeapMEM heap, s32 align);
void *operator new[](size_t size);
void *operator new[](size_t size, EHeapMEM heap, s32 align);
void operator delete(void *ptr);
void operator delete[](void *ptr);

void memStub_801D3564(void); // (stub)
void memInitHeap(void);
void memStub_801D3634(void); // (stub)
void memStub_801D3638(u32); // (stub)

u16 memGetCurGroup(void);
void memPopGroup(void);
void memPushGroup(u16 groupID);

void memFreeGroupInMEM1(u16 groupID);
void memFreeGroupInMEM2(u16 groupID);

#endif