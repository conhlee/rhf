#ifndef GAME_GAMETABLE_HPP
#define GAME_GAMETABLE_HPP

#include <revolution/types.h>

#include "ExFlowDecl.hpp"

struct GameTableEntry {
    enum EGiftType {
        eGiftType_Music,
        eGiftType_ReadingMaterial
    };

    const TickFlowCode *gameTickFlow;
    const char *name; // Message ID
    const char *description; // Message ID
    EGiftType giftType;
    u32 giftIndex;
};

const GameTableEntry *fn_8000EB50(s32 index);

#endif
