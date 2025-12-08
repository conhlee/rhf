#ifndef NW4R_LYT_PANE_H
#define NW4R_LYT_PANE_H

#include <revolution/types.h>
#include <nw4r/ut/RuntimeTypeInfo.h>
#include <nw4r/lyt/types.h>
#include <nw4r/math/types.h>
#include <nw4r/lyt/drawInfo.h>
#include <nw4r/ut/Color.h>
#include <nw4r/lyt/material.h>

namespace nw4r {
namespace lyt {

namespace detail {

class PaneBase {
public:
    PaneBase();
    virtual ~PaneBase();

public:
    ut::LinkListNode mLink;
};

} // namespace detail

class Pane : public detail::PaneBase {
public:
    NW4R_UT_RUNTIME_TYPEINFO;

private:
    enum {
        BIT_VISIBLE,
        BIT_INFLUENCED_ALPHA,
        BIT_LOCATION_ADJUST,
    };

public:
    Pane();
    virtual ~Pane();
    virtual void CalculateMtx(const DrawInfo &drawInfo);
    virtual void Draw(const DrawInfo &drawInfo);
    virtual void DrawSelf(const DrawInfo &drawInfo);
    virtual void Animate(u32 option);
    virtual void AnimateSelf(u32 option);

    virtual const ut::Color GetVtxColor(u32 idx) const;
    virtual void SetVtxColor(u32 idx, ut::Color value);
    virtual u8 GetColorElement(u32 idx) const;
    virtual void SetColorElement(u32 idx, u8 value);
    virtual u8 GetVtxColorElement(u32 idx) const;
    virtual void SetVtxColorElement(u32 idx, u8 value);
    virtual Pane *FindPaneByName(const char *findName, bool bRecursive = true);
    virtual Material *FindMaterialByName(const char *findName, bool bRecursive = true);
    virtual void BindAnimation(AnimTransform *pAnimTrans, bool bRecursive = true, bool bDisable = false);
    virtual void UnbindAnimation(AnimTransform *pAnimTrans,  bool bRecursive = true);
    virtual void UnbindAllAnimation(bool bRecursive = true);
    virtual void UnbindAnimationSelf(AnimTransform *pAnimTrans);
    virtual AnimationLink *FindAnimationLinkSelf(AnimTransform *pAnimTrans);
    virtual AnimationLink *FindAnimationLinkSelf(const void *animRes); // TODO: const AnimResource &
    virtual void SetAnimationEnable(AnimTransform *pAnimTrans, bool bEnable, bool bRecursive = true);
    virtual void SetAnimationEnable(const void *animRes, bool bEnable, bool bRecursive = true); // TODO: const AnimResource &
    virtual u8 GetMaterialNum() const;
    virtual Material *GetMaterial() const;
    virtual Material *GetMaterial(u32 idx) const;

    bool IsVisible() const {
        return detail::TestBit(mFlag, 0);
    }
    void SetVisible(bool bVisible) {
        detail::SetBit(&mFlag, 0, bVisible);
    }

    bool IsLocationAdjust() const {
        return detail::TestBit(mFlag, BIT_LOCATION_ADJUST);
    }
    void SetLocationAdjust(bool bAdjust) {
        detail::SetBit(&mFlag, BIT_LOCATION_ADJUST, bAdjust);
    }

    const math::VEC3 &GetTranslate() const {
        return mTranslate;
    }
    void SetTranslate(const math::VEC3 &value) {
        mTranslate = value;
    }
    void SetTranslate(const math::VEC2 &value) {
        mTranslate = nw4r::math::VEC3(value.x, value.y, 0.0f);
    }

    void SetScale(const math::VEC2 &value) {
        mScale = value;
    }

    void SetSize(const Size &value) {
        mSize = value;
    }

    Pane *GetParent() const {
        return mpParent;
    }
    
protected:
    virtual void LoadMtx(const DrawInfo &drawInfo);

protected:
    Pane *mpParent;
    ut::LinkList<Pane, offsetof(detail::PaneBase, mLink)> mChildList;
    ut::LinkList<AnimationLink, offsetof(AnimationLink, mLink)> mAnimList;
    Material *mpMaterial;
    math::VEC3 mTranslate;
    math::VEC3 mRotate;
    math::VEC2 mScale;
    Size mSize;
    math::MTX34 mMtx;
    math::MTX34 mGlbMtx;
    const void *mpExtUserDataList; // TODO: struct ExtUserDataList *
    u8 mAlpha;
    u8 mGlbAlpha;
    u8 mBasePosition;
    u8 mFlag;
    char mName[16 + 1];
    char mUserData[8 + 1];
    u8 mbUserAllocated;
    u8 mPadding;
};

} // namespace lyt
} // namespace nw4r

#endif
