#include "SNDHeap.hpp"

#include <nw4r/snd.h>

#include "Sound.hpp"

#include "Mem.hpp"

static const u32 sSoundHeapAllocSize[eSoundHeap_Count] = {
    0x200000, // eSoundHeap_0
    0x800000, // eSoundHeap_1
    0x399999, // eSoundHeap_2
    0x380000, // eSoundHeap_3
};
static const EHeapMEM sSoundHeapAllocHeap[eSoundHeap_Count] = {
    eHeap_MEM1, // eSoundHeap_0
    eHeap_MEM1, // eSoundHeap_1
    eHeap_MEM1, // eSoundHeap_2
    eHeap_MEM1, // eSoundHeap_3
};

static nw4r::snd::SoundHeap *sSoundHeap[eSoundHeap_Count];
static void *sSoundHeapAlloc[eSoundHeap_Count];
static int sSoundHeapState[eSoundHeap_Count];

bool lbl_80320440;

void fn_8008357C(void) {
    for (u16 i = 0; i < eSoundHeap_Count; i++) {
        sSoundHeap[i] = new nw4r::snd::SoundHeap;
        sSoundHeapAlloc[i] = new (sSoundHeapAllocHeap[i], 32) u8[sSoundHeapAllocSize[i]];

        sSoundHeap[i]->Create(sSoundHeapAlloc[i], sSoundHeapAllocSize[i]);

        fn_8008367C(static_cast<ESoundHeap>(i));
    }

    lbl_80320440 = false;
}

void fn_80083630(u16 groupID, ESoundHeap heap) {
    gSoundManager->fn_801E7230(groupID, sSoundHeap[heap]);
}
void fn_8008364C(u16 groupID, ESoundHeap heap) {
    gSoundManager->fn_801E71CC(groupID, sSoundHeap[heap]);
}

void fn_80083668(ESoundHeap heap) {
    sSoundHeap[heap]->Clear();
}

void fn_8008367C(ESoundHeap heap) {
    sSoundHeapState[heap] = sSoundHeap[heap]->SaveState();
}
void fn_800836C0(ESoundHeap heap) {
    sSoundHeap[heap]->LoadState(sSoundHeapState[heap]);
}

void fn_800836E0(bool arg1) {
    lbl_80320440 = arg1;
}
