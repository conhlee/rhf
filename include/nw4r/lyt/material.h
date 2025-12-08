#ifndef NW4R_LYT_MATERIAL_H
#define NW4R_LYT_MATERIAL_H

#include <revolution/types.h>
#include <revolution/GX.h>
#include <nw4r/lyt/types.h>
#include <nw4r/ut/Color.h>

namespace nw4r {
namespace lyt {

namespace detail {

struct BitGXNums {
    u32 texMap:4;
    u32 texSRT:4;
    u32 texCoordGen:4;
    u32 indSRT:2;
    u32 indStage:2;
    u32 tevSwap:1;
    u32 tevStage:5;
    u32 chanCtrl:1;
    u32 matCol:1;
    u32 alpComp:1;
    u32 blendMode:1;
};

} // namespace detail

class Material {
protected:
    ut::LinkList<AnimationLink, offsetof(AnimationLink, mLink)> mAnimList;
    GXColorS10 mTevCols[3];
    ut::Color mTevKCols[4];
    detail::BitGXNums mGXMemCap;
    detail::BitGXNums mGXMemNum;
    void *mpGXMem;
    char mName[16 + 4 + 1];
    u8 mbUserAllocated;
    u8 mPadding[2];
};

} // namespace lyt
} // namespace nw4r

#endif
