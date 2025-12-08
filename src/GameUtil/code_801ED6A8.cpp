#include "code_801ED6A8.hpp"

s32 fn_801ED6A8(struct_801ED6A8 *ctx) {
    u32 delta = ctx->deltaBuffer[ctx->curDeltaIdx++];
    if (delta == -1) {
        delta = 0;
    }

    ctx->unk18 += delta;

    return (static_cast<u64>(ctx->unk18) * ctx->unk14 + 0x80) >> 8;
}

s32 fn_801ED704(struct_801ED6A8 *ctx, u32 pos) {
    u32 prev = ctx->unk18;
    ctx->unk18 = (u32)(((u64)pos << 16) / ctx->unk14 + 0x80) >> 8;

    u32 delta = ctx->unk18 - prev;
    if (delta == 0) {
        delta = -1;
    }

    ctx->deltaBuffer[ctx->curDeltaIdx++] = delta;

    return (static_cast<u64>(ctx->unk18) * ctx->unk14 + 0x80) >> 8;
}

bool fn_801ED7B8(struct_801ED6A8 *ctx) {
    return ctx->deltaBuffer[ctx->curDeltaIdx] == 0;
}
