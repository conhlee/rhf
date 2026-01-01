#ifndef GAME_GIFTTABLE_HPP
#define GAME_GIFTTABLE_HPP

#include <revolution/types.h>

#include "ExFlowDecl.hpp"

struct GiftMusic {
    const char *titlePlain; // Message ID
    const char *titleFormatted; // Message ID
    const TickFlowCode *runTickFlow;
    u32 soundID;
};

struct GiftReadingMaterial {
    const char *titlePlain; // Message ID
    const char *titleFormatted; // Message ID
    const char *content; // Message ID
};

const GiftMusic *fn_800B4338(s32 index);
const GiftReadingMaterial *fn_800B434C(s32 index);

#endif
