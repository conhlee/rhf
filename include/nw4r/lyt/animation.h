#ifndef NW4R_LYT_ANIMATION_H
#define NW4R_LYT_ANIMATION_H

#include <revolution/types.h>
#include <nw4r/ut/LinkList.h>
namespace nw4r {
namespace lyt {

namespace res {
struct AnimationBlock;

} // namespace res

class AnimTransform {
public:
    AnimTransform();
    virtual ~AnimTransform(); // at 0x8

    u16 GetFrameSize() const;

    bool IsLoopData() const;

    f32 GetFrame() const {
        return mFrame;
    }
    void SetFrame(f32 frame) {
        mFrame = frame;
    }

    f32 GetFrameMax() const {
        return static_cast<f32>(GetFrameSize());
    }

public:
    ut::LinkListNode mLink;

protected:
    res::AnimationBlock *mpRes;
    f32 mFrame;
};

} // namespace lyt
} // namespace nw4r

#endif
