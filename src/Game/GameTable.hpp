#ifndef GAME_GAMETABLE_HPP
#define GAME_GAMETABLE_HPP

#include <revolution/types.h>

#include "ExFlowDecl.hpp"

enum EGiftType {
    eGiftType_Music,
    eGiftType_ReadingMaterial
};
struct GameTableEntry {
    const TickFlowCode *gameTickFlow;
    const char *name; // Message ID
    const char *description; // Message ID
    EGiftType giftType;
    u32 giftIndex;
};

const GameTableEntry *fn_8000EB50(s32 index);

s32 fn_80079540(s32 index);
const GameTableEntry *fn_80079554(s32 index);

u8 fn_8000EBB0(s32 index);
u8 fn_8000EBBC(s32 index);

#endif
