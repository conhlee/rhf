#ifndef GAMEUTIL_CODE_801ED6A8_HPP
#define GAMEUTIL_CODE_801ED6A8_HPP

#include <revolution/types.h>

struct struct_801ED6A8 {
    u32 unk00; // Not referenced
    s32 *deltaBuffer;
    u32 unk08; // Not referenced
    u32 unk0C; // Not referenced
    u32 curDeltaIdx;
    u32 unk14;
    u32 unk18;
};

s32 fn_801ED6A8(struct_801ED6A8 *ctx);
s32 fn_801ED704(struct_801ED6A8 *ctx, u32 pos);
bool fn_801ED7B8(struct_801ED6A8 *ctx);

#endif
