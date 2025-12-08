#ifndef GAME_AGBTAP_SCENEAGBTAP_HPP
#define GAME_AGBTAP_SCENEAGBTAP_HPP

#include <revolution/types.h>

#include "ExScene.hpp"

#include "CellAnim.hpp"

#include "TimeRatio.hpp"

namespace AgbTap {

class CMyFlow; // Forward-declaration
class CMyChecker; // Forward-declaration

extern s32 sceneVer;

} // namespace AgbTap

enum EAgbTapMove {
    eAgbTapMove_Ready_L,
    eAgbTapMove_Ready_R,
    eAgbTapMove_Tap_1L_NG, // ?
    eAgbTapMove_Tap_1R_NG, // ?
    eAgbTapMove_Tap_0L,
    eAgbTapMove_Tap_0R,
    eAgbTapMove_Tap_1L,
    eAgbTapMove_Tap_1R,
    eAgbTapMove_Charge,
    eAgbTapMove_Crouch,
    eAgbTapMove_Jump,
    eAgbTapMove_Jump_Last,
    eAgbTapMove_Pause,
    eAgbTapMove_Round,
    eAgbTapMove_Land,
    eAgbTapMove_Ready_2, // SFX only
    eAgbTapMove_Ready_3_2, // SFX only

    eAgbTapMove_Null = 18
};

class CSceneAgbTap : public CExScene {
    friend class AgbTap::CMyFlow;
    friend class AgbTap::CMyChecker;

public:
    virtual ~CSceneAgbTap(void) {}
    virtual void _10(void);
    virtual void _14(void);
    virtual void _18(void);
    virtual void _1C(void);
    virtual void _20(void);
    virtual bool _24(void);
    virtual void _28(void);

    CSceneAgbTap(void) {}

    SCENE_DECL_CREATE_FN()

    void fn_800A9698(u32 button, s32 type);

    void fn_800A9758(void);

    void fn_800A9768(void);
    
    void fn_800A97DC(EAgbTapMove type);

    void fn_800A9924(void);

    void fn_800A9988(EAgbTapMove type);

    void fn_800A9B20(bool out);

    void fn_800A9C3C(void);

    void fn_800A9C84(u32 maxMissTapCount);

    void fn_800A9D20(void);

    void fn_800A9D90(void);

    void fn_800A9E24(void);

    static void fn_800A8FA4(u32 button);

    static void fn_800A9034(void);

private:
    CCellAnim *mGirlAnim;
    CCellAnim *mMonkeyAnim[2];
    CCellAnim *mGiraffeAnim;

    EAgbTapMove mLastAnimeType;
    u16 mGirlAnimeTimer;
    u16 mMonkeyAnimeTimer;
    u16 mGiraffeAnimeTimer;

    CTRParabola mGirlPos;
    CTRParabola mMonkeyPos;
    CTRParabola mGiraffePos;

    s32 mThroughTapCount;
    s32 mMissTapCount;
};

#endif