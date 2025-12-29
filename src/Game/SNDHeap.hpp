#ifndef GAME_SNDHEAP_HPP
#define GAME_SNDHEAP_HPP

#include <revolution/types.h>

/*
    TODO:   rename these at some point to correspond to their purpose.
    NOTE:   sometimes, the game allocates to a wrong heap as a dirty hack if they
            run out of space in the correct heap .. see Endless Remix for example.
*/
enum ESoundHeap {
    eSoundHeap_0, // Common & Practice
    eSoundHeap_1, // Game BGM
    eSoundHeap_2, // Game SFX
    eSoundHeap_3, // Prologue BGM

    eSoundHeap_Count
};

extern bool lbl_80320440;

void fn_8008357C(void);

void fn_80083630(u16 groupID, ESoundHeap heap);
void fn_8008364C(u16 groupID, ESoundHeap heap);

void fn_80083668(ESoundHeap heap);

void fn_8008367C(ESoundHeap heap);
void fn_800836C0(ESoundHeap heap);

void fn_800836E0(bool);

#endif