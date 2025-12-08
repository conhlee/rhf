#ifndef NW4R_LYT_LAYOUT_H
#define NW4R_LYT_LAYOUT_H

#include <revolution/types.h>
#include <revolution/MEM.h>
#include <nw4r/ut/LinkList.h>
#include <nw4r/ut/Rect.h>
#include <nw4r/ut/TagProcessor.h>
#include <nw4r/lyt/animation.h>
#include <nw4r/lyt/pane.h>
#include <nw4r/lyt/group.h>
#include <nw4r/lyt/arcResourceAccessor.h>

namespace nw4r {
namespace lyt {

// TODO: BAD!
class AnimResource;

class Layout {
public:
    Layout();
    virtual ~Layout();
    virtual bool Build(const void *lytResBuf, ResourceAccessor *pResAcsr);
    virtual AnimTransform *CreateAnimTransform();
    virtual AnimTransform *CreateAnimTransform(const void *animResBuf, ResourceAccessor *pResAcsr);
    virtual AnimTransform *CreateAnimTransform(const AnimResource &animRes, ResourceAccessor *pResAcsr);
    virtual void BindAnimation(AnimTransform *pAnimTrans);
    virtual void UnbindAnimation(AnimTransform *pAnimTrans);
    virtual void UnbindAllAnimation();
    virtual bool BindAnimationAuto(const AnimResource &animRes, ResourceAccessor *pResAcsr);
    virtual void SetAnimationEnable(AnimTransform *pAnimTrans, bool bEnable);
    virtual void CalculateMtx(const DrawInfo &drawInfo);
    virtual void Draw(const DrawInfo &drawInfo);
    virtual void Animate(unsigned long option);
    virtual void SetTagProcessor(ut::WideTagProcessor *pProcessor);

    ut::Rect GetLayoutRect() const;
    Pane *GetRootPane() {
        return mpRootPane;
    }

    static MEMAllocator *GetAllocator() {
        return mspAllocator;
    }
    static void SetAllocator(MEMAllocator *pAllocator) {
        mspAllocator = pAllocator;
    }

protected:
    ut::LinkList<AnimTransform, offsetof(AnimTransform, mLink)> mAnimTransList;
    Pane *mpRootPane;
    GroupContainer *mpGroupContainer;
    Size mLayoutSize;

    static MEMAllocator *mspAllocator;
};

} // namespace lyt
} // namespace nw4r

#endif
