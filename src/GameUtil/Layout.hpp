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
public:
    virtual void _08(nw4r::lyt::Pane *pane);
    virtual EBtnState _0C(nw4r::math::VEC2 *, bool, nw4r::lyt::DrawInfo *);
    virtual void _10(EBtnState, bool);
    virtual void _14(void);
    virtual nw4r::math::VEC2 _18(void);

    CExBtnPane(void) {}

    nw4r::lyt::Pane *getPane(void) const {
        return mPane;
    }

    bool getEnabled(void) const { return mEnabled; }

    bool getUnk26(void) const { return mUnk26; }
    bool getUnk27(void) const { return mUnk27; }

    void setFocusAnime(CLayoutAnimation *animation) {
        mFocusAnime = animation;
    }
    void setActionAnime(CLayoutAnimation *animation) {
        mActionAnime = animation;
    }

    void setSelectSfxID(u16 soundID) {
        mSelectSfxID = soundID;
    }
    void setActionSfxID(u16 soundID) {
        mActionSfxID = soundID;
    }

private:
    nw4r::lyt::Pane *mPane;
    EBtnState mState;

    CLayoutAnimation *mDefaultAnime;
    CLayoutAnimation *mFocusAnime;
    CLayoutAnimation *mActionAnime;

    s32 mUnk18;

    s32 mSelectSfxID;
    s32 mActionSfxID;

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

struct CExBtnPaneManager_Sub {
    u8 initialButtonIndex;
    u8 *buttonIndices0;
    u8 *buttonIndices1;
    u8 *buttonIndices2;
    u8 *buttonIndices3;
};

extern CExBtnPaneManager_Sub lbl_802ED5F0;
extern CExBtnPaneManager_Sub lbl_802ED604;

class CExBtnPaneManager {
public:
    typedef void (*UnkCallbackFn)(bool useCursor);

public:
    virtual ~CExBtnPaneManager(void);
    virtual void _0C(Vec2 *, bool, nw4r::lyt::DrawInfo *drawInfo); // TODO typing

    CExBtnPaneManager(u8 buttonCount);

    void fn_8008F65C(u8 index, CExBtnPane *button);

    void fn_8008F688(CExBtnPaneManager_Sub *);

    void fn_8008F96C(
        nw4r::lyt::Pane *, nw4r::lyt::Pane *,
        CLayoutAnimation *, CLayoutAnimation *,
        f32
    );

    void fn_8008F984(bool, nw4r::lyt::DrawInfo *drawInfo);

    void fn_8008F80C(UnkCallbackFn);

    void fn_8008F81C(bool, s8);

private:
    char _temppad[0x48 - 0x4];
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

    nw4r::lyt::Layout *getLayout(void) const {
        return mLayout;
    }
    CLayoutAnimation *getAnimation(u32 index) const {
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