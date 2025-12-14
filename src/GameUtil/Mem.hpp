#ifndef GAMEUTIL_MEM_HPP
#define GAMEUTIL_MEM_HPP

#include <revolution/types.h>
#include <revolution/OS.h>
#include <revolution/MEM.h>

#include <new>

enum EHeapMEM {
    eHeap_MEM1 = 0,
    eHeap_MEM2 = 1,
};

enum EHeapGroup {
    eHeapGroup_Default = 0,
    eHeapGroup_CommonAsset = 1, // allocate common assets (font, mesg, common cellanim & layout, etc.)
    eHeapGroup_SceneAsset = 2,  // allocate scene assets (archives, etc.)
    eHeapGroup_Scene = 3,       // allocate scene object
};

extern MEMiHeapHead *lbl_80320F80; // gHeapMEM1
extern MEMiHeapHead *lbl_80320F84; // gHeapMEM2

void *operator new(size_t size);
void *operator new(size_t size, EHeapMEM heap, s32 align);
void *operator new[](size_t size);
void *operator new[](size_t size, EHeapMEM heap, s32 align);
void operator delete(void *ptr);
void operator delete[](void *ptr);

void fn_801D3564(void);
void fn_801D3568(void);
void fn_801D3634(void);
void fn_801D3638(u32);
u16 fn_801D363C(void);

void fn_801D3644(void);
void fn_801D369C(u16 groupID);
void fn_801D375C(u16 groupID);
void fn_801D3770(u16 groupID);

#endif