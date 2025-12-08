#ifndef GAME_HBMMANAGER_HPP
#define GAME_HBMMANAGER_HPP

#include <revolution/types.h>

#include <revolution/HBM.h>

#include <revolution/TPL.h>
#include <revolution/KPAD.h>

#include "Singleton.hpp"

class CHBMManager : public TSingleton<CHBMManager> {
public:
    virtual void _08(void);
    virtual ~CHBMManager(void);
    virtual void _10(void);
    virtual void _14(void);
    virtual void _18(void);
    virtual void _1C(void);
    virtual void _20(void);

    CHBMManager(void);

    void setUnk416(bool value) {
        mUnk416 = value;
    }

    bool getUnk414(void) const { return mUnk414; }

private:
    void fn_80086B90(void);
    void fn_80086CCC(void);
    void fn_800870BC(void);
    void fn_80087314(void);

    static void powercallback(void);
    static void resetcallback(void);

private:
    enum EProhibitIconStage {
        eProhibitIconStage_FadeIn,
        eProhibitIconStage_Fixed,
        eProhibitIconStage_FadeOut,

        eProhibitIconStage_Null
    };

private:
    void *mHBMWorkMem;
    HBMDataInfo mHBMInfo;
    TPLPalette *mHBMIconPal;

    u32 mUnk4C;

    KPADStatus mKPADState[4];

    u8 mUnk260[432];

    s32 mHBMSelectedButton;

    u8 mUnk414;
    u8 mUnk415;
    u8 mUnk416;

    bool mPowerCalled;
    bool mResetCalled;

    u8 mUnk419;

    EProhibitIconStage mProhibitIconStage;
    s32 mUnk420;

    u8 mProhibitIconAlpha;

    void *mHBMSoundData;
    void *mHBMSoundMem;

    u8 mUnk430[8];
};

extern CHBMManager *gHBMManager;

#endif
