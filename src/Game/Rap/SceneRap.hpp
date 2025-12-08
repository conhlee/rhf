#ifndef GAME_RAP_SCENERAP_HPP
#define GAME_RAP_SCENERAP_HPP

#include <revolution/types.h>

#include "ExScene.hpp"

#include "CellAnim.hpp"

#include "Pseudo3D.hpp"

#include "TimeRatio.hpp"

namespace Rap {

class CMyFlow; // Forward-declaration
class CMyChecker; // Forward-declaration
class CMyLayout; // Forward-declaration

extern s32 sceneVer;

} // namespace Rap

class CSceneRap : public CExScene {
    friend class Rap::CMyFlow;
    friend class Rap::CMyChecker;

public:
    virtual ~CSceneRap(void) {}
    virtual void _10(void);
    virtual void _14(void);
    virtual void _18(void);
    virtual void _1C(void);
    virtual void _20(void);
    virtual bool _24(void);
    virtual void _28(void);

    CSceneRap(void) {}

    SCENE_DECL_CREATE_FN()

    void fn_8005128C(u32 button, s32 type, f32 ticks);

    void fn_800516C0(void);

    static void fn_80050284(u32 button);

    static void fn_80050340(void);

    static void fn_800504DC(void);

private:
    struct CharaRapper {
        CSceneRap *scene;
        CCellAnim *footAnim;
        CCellAnim *bodyAnim;
        CCellAnim *headAnim;
        CCellAnim *eyeAnim;
        CCellAnim *mouthAnim;
        bool amIPlayer;
        u8 throughStareTimer; // The CPU rapper is staring at the player.

        void fn_80051770(bool isPlayer, CSceneRap *scene);
        void fn_8005193C(void);
        void fn_800519D4(void);
        void fn_80051A2C(s32 type, s32 unk);
        void fn_80051BB8(s32 type);
        void fn_80051D00(void);
        void fn_80051D34(void);
    };

    struct CharaGirl {
        CCellAnim *footAnim;
        CCellAnim *bodyAnim;
        CCellAnim *eyeAnim;
        CCellAnim *mouthAnim;

        void fn_80051DAC(CCellAnim *carAnim);
        void fn_80051ED0(void);
        void fn_80051F0C(s32 type, bool unk);
        void fn_80052070(s32 type);
        void fn_800521A8(void);
    };

private:
    s32 mUnk34;

    CharaRapper mPlayerChara;
    CharaRapper mOtherChara;
    CharaGirl mGirlChara;

    CCellAnim *mBGAnim;
    CCellAnim *mFenceAnim;
    CCellAnim *mGroundDryAnim;
    CCellAnim *mGroundWetAnim;

    CCellAnim *mCarAnim;
    CCellAnim *mCarShadowAnim;

    CCellAnim *mAirplaneAnim;
    CCellAnim *mAirplaneShadowAnim;
    CCellAnim *mAirplaneTireAnim;

    CCellAnim *mRoomAnim;
    CCellAnim *mTVAnim;
    CCellAnim *mSofaAnim;

    CCellAnim *mChandelierAnim; // NOTE: exclusively for ver1

    CPseudo3D *mPlane3D;
    CTRFloat mPlaneY;
    CTRFloat mPlaneZ;

    CTRFloat mCharaDarken;
    CTRFloat mFenceDarken;
    CTRFloat mAirplaneShadowPos;

    bool mUnk1BC;
    bool mUnk1BD;
    bool mUnk1BE;
    bool mUnk1BF;

    CTRFloat mGroundWetness;

    CTRFloat mUnk1F4;
    CTRFloat mUnk228;

    Rap::CMyLayout *mMyLayout;
};

#endif