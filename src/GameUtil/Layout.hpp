#ifndef GAMEUTIL_LAYOUT_HPP
#define GAMEUTIL_LAYOUT_HPP

#include <revolution/types.h>

#include <nw4r/lyt.h>

#include "LayoutAnimation.hpp"

enum EBtnState {
    eBtnState_Default,
    eBtnState_Selected,
    eBtnState_Active,
    eBtnState_Unk03
};

class CBtnPane {
public:
    CBtnPane() {}

protected:
    static bool fn_801D85BC(nw4r::lyt::Pane *pane, nw4r::lyt::DrawInfo *drawInfo);
};

class CExBtnPaneManager;

class CExBtnPane : public CBtnPane {
    virtual void _08(nw4r::lyt::Pane *pane);
    virtual EBtnState _0C(nw4r::math::VEC2 *, bool, nw4r::lyt::DrawInfo *);
    virtual void _10(EBtnState, bool);
    virtual void _14(void);
    virtual nw4r::math::VEC2 _18(void);

    CExBtnPane(void) {}

private:
    nw4r::lyt::Pane *mPane;
    EBtnState mState;

    CLayoutAnimation *mDefaultAnime;
    CLayoutAnimation *mSelectedAnime;
    CLayoutAnimation *mActiveAnime;

    s32 mUnk18;

    s32 mSelectSfxID;
    s32 mActiveSfxID;

    bool mEnabled;
    bool mUnk25;
    bool mUnk26;
    bool mUnk27;
    bool mUnk28;
    bool mVibrateOnHover;

    s32 mControllerChan;

    CExBtnPaneManager *mMyManager;

    bool mUnk34;
};

class CLayout {
public:
    virtual ~CLayout(void) {}
    virtual void _0C(void);
    virtual void _10(void);
    virtual void _14(void);
    virtual void _18(nw4r::lyt::DrawInfo *drawInfo);
    virtual void _1C(nw4r::lyt::DrawInfo *drawInfo);
    virtual void _20(nw4r::lyt::DrawInfo *drawInfo);

    CLayout(void);

    void fn_801D9E24(bool enable);

    void buildLayout(const void *binAddr, nw4r::lyt::ResourceAccessor *resAccessor) {
        getLayout()->Build(binAddr, resAccessor);
    }

    nw4r::lyt::Layout *getLayout(void) {
        return mLayout;
    }
    CLayoutAnimation *getAnimation(u32 index) {
        return &mAnimations[index];
    }

    void setAnimationCount(s32 count) {
        mAnimationCount = count;
    }

    bool getUpdateEnable(void) const { return mUpdateEnable; }
    void setUpdateEnable(bool enabled) { mUpdateEnable = enabled; }

    bool getDrawEnable(void) const { return mDrawEnable; }
    void setDrawEnable(bool enabled) { mDrawEnable = enabled; }

private:
    nw4r::lyt::Layout *mLayout;
    CLayoutAnimation *mAnimations; // CLayoutAnimation[mAnimationCount]
    s32 mAnimationCount;
    bool mUpdateEnable;
    bool mDrawEnable;
    bool mApplyManagerScale;
    f32 mScaleX;
    f32 mScaleY;
};

#endif