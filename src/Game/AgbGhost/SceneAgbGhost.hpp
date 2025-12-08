#ifndef GAME_AGBGHOST_SCENEAGBGHOST_HPP
#define GAME_AGBGHOST_SCENEAGBGHOST_HPP

#include <revolution/types.h>

#include "ExScene.hpp"

#include "CellAnim.hpp"

#include "TimeRatio.hpp"

#include "SNDHandle.hpp"

namespace AgbGhost {

class CMyFlow; // Forward-declaration
class CMyChecker; // Forward-declaration

extern s32 sceneVer;

} // namespace AgbGhost

class CSceneAgbGhost : public CExScene {
    friend class AgbGhost::CMyFlow;
    friend class AgbGhost::CMyChecker;
    
public:
    virtual ~CSceneAgbGhost(void) {}
    virtual void _10(void);
    virtual void _14(void);
    virtual void _18(void);
    virtual void _1C(void);
    virtual void _20(void);
    virtual bool _24(void);
    virtual void _28(void);

    CSceneAgbGhost(void) {}

    SCENE_DECL_CREATE_FN()

    void fn_800A7FA8(u32 button, u32 type);
    void fn_800A8080(void);
    void fn_800A80EC(f32 start, f32 time);
    bool fn_800A815C(void);
    void fn_800A81A8(u32 pos);
    void fn_800A8348(void);
    void fn_800A84A8(bool through);
    void fn_800A854C(bool late);
    void fn_800A860C(void);
    void fn_800A8658(void);
    void fn_800A86E0(void);
    void fn_800A8730(void);
    void fn_800A8788(bool);

    static void fn_800A75DC(u32);

    static void fn_800A772C(void);

private:
    CCellAnim *mBowArrowAnim;
    CCellAnim *mGhostAnim;

    CCellAnim *mBgTreeAnim;

    CCellAnim *mDoorAnim;
    CCellAnim *mDoorMaskAnim;

    CCellAnim *mRainAnim[30];
    CCellAnim *mRainRippleAnim[20];

    CTRParabola mUnk110;
    CTRFloat mUnk168;

    SNDHandle mUnk19C;

    s32 mGhostHeight;
    
    s32 mRainNext;
    s32 mRainRippleNext;

    bool mIsRainSlow;
    bool mUnk1B1;
    bool mUnk1B2;
};

#endif