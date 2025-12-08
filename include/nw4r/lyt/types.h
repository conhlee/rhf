#ifndef NW4R_LYT_TYPES_H
#define NW4R_LYT_TYPES_H

#include <revolution/types.h>
#include <nw4r/ut/LinkList.h>
#include <nw4r/lyt/animation.h>

namespace nw4r {
namespace lyt {

struct Size {
    Size() :
        width(0), height(0)
    {}
    Size(f32 aWidth, f32 aHeight) :
        width(aWidth), height(aHeight)
    {}
    Size(const Size &other) :
        width(other.width), height(other.height)
    {}
    
    f32 width;
    f32 height;
};

class AnimationLink {
public:
    ut::LinkListNode mLink;

private:
    AnimTransform *mAnimTrans;
    u16 mIdx;
    bool mbDisable;
};

namespace detail {

template <typename T>
inline void SetBit(T *pBits, int pos, bool val) {
    const T mask = T(~(1 << pos));
    *pBits &= mask;
    *pBits |= (val ? 1 : 0) << pos;
}

template <typename T>
inline bool TestBit(T bits, int pos) {
    const T mask = T(1 << pos);
    return 0 != (bits & mask);
}

} // namespace detail

} // namespace lyt
} // namespace nw4r

#endif