#ifndef GAME_INTERVIEW_SCENEINTERVIEW_HPP
#define GAME_INTERVIEW_SCENEINTERVIEW_HPP

#include <revolution/types.h>

#include "ExScene.hpp"

#include "CellAnim.hpp"

#include "TimeRatio.hpp"

#include "Sound.hpp"

#include "CellAnimRenderEff.hpp"

namespace Interview {

class CMyFlow; // Forward-declaration
class CMyChecker; // Forward-declaration

extern s32 sceneVer;

} // namespace Interview

class CSceneInterview : public CExScene {
    friend class Interview::CMyFlow;
    friend class Interview::CMyChecker;
    
public:
    virtual ~CSceneInterview(void) {}
    virtual void _10(void);
    virtual void _14(void);
    virtual void _18(void);
    virtual void _1C(void);
    virtual void _20(void);
    virtual bool _24(void);
    virtual void _28(void);

    CSceneInterview(void) {}

    void fn_800B6860(u32 button, s32, s32, f32 ticks);

    SCENE_DECL_CREATE_FN()

    static void fn_800B5444(u32);

    static void fn_800B5508(void);
    static void fn_800B5624(void);

private:
    void fn_800B70F4(s32 wrestlerID);

    void fn_800B7AA8(f32 ticks);
    void fn_800B7BE4(f32 ticks);

private:
    class CWrestler {
    public:
        CCellAnim *bodyAnim;
        CCellAnim *faceAnim;
        CCellAnim *faceMark; // Mark on the head signifying the player number (hidden in Single Play).
        CCellAnim *shadowAnim;
        CCellAnim *sweatBase;

        CCellAnim *bodyAnimPaper;
        CCellAnim *faceAnimPaper;
        CCellAnim *faceMarkPaper;
        CCellAnim *shadowAnimPaper;
        CCellAnim *sweatBasePaper;

        s32 unk28;
        s32 unk2C;
        s32 unk30;
        s32 unk34;
        bool unk38;
    };

private:
    CWrestler mWrestler[2];

    CCellAnim *mBGFlashAnim;
    CCellAnim *mPanelAnim;

    CCellAnim *mGirlBodyAnim;
    CCellAnim *mGirlFaceAnim;
    CCellAnim *mGirlShadowAnim;
    CCellAnim *mCameramenAnim;

    CCellAnim *mPaperAnim;
    CCellAnim *mPaperBackAnim;

    CCellAnim *mPaperPanelAnim;

    CCellAnim *mPaperGirlBodyAnim;
    CCellAnim *mPaperGirlFaceAnim;
    CCellAnim *mPaperGirlShadowAnim;
    CCellAnim *mPaperCameramenAnim;

    CCellAnim *mFarViewAnim;

    CCellAnim *mStadiumAnim;

    CCellAnim *mTelopAnimBegin;
    CCellAnim *mTelopAnimEnd;

    CTRParabola mCameramenJumping;
    CTRFloat mCameramenScaling;

    CTRFloat mPaperScaling;
    CTRFloat mPaperRotation;

    CTRFloat mStadiumZooming;
    CTRFloat mStadiumOpacity;

    bool mPaperRotateCounterclock;

    SNDHandle mUnk250;

    ECellAnimRenderEff mUnk258;

    bool mIsTwoPlay;

    s32 mWrestlerCount;
    s32 mUnk264; // mActiveWrestler?
};

#endif