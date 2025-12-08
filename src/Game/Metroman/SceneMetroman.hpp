#ifndef GAME_METROMAN_SCENEMETROMAN_HPP
#define GAME_METROMAN_SCENEMETROMAN_HPP

#include <revolution/types.h>

#include "ExScene.hpp"

#include "CellAnim.hpp"

#include "TimeRatio.hpp"

#include "EndlessCounter.hpp"

namespace Metroman {

class CMyFlow; // Forward-declaration
class CMyChecker; // Forward-declaration

extern s32 sceneVer;

} // namespace Metroman

class CSceneMetroman : public CExScene {
    friend class Metroman::CMyFlow;
    friend class Metroman::CMyChecker;
    
public:
    virtual ~CSceneMetroman(void) {}
    virtual void _10(void);
    virtual void _14(void);
    virtual void _18(void);
    virtual void _1C(void);
    virtual void _20(void);
    virtual bool _24(void);
    virtual void _28(void);

    CSceneMetroman(void) {}

    SCENE_DECL_CREATE_FN()

    
    void fn_800D8890(u32 button);
    void fn_800D8940(s32);
    void fn_800D8A78(s32, s32);
    void fn_800D8B78(void);
    void fn_800D8B8C(f32 ticks);
    void fn_800D8C18(s32 add);
    u16 fn_800D8CA4(u16 lampSize);
    void fn_800D8D1C(void);

    static void fn_800D7EBC(u32);

    static void fn_800D7FDC(void);

    static void fn_800D80A4(void);

private:
    CCellAnim *mManAnim;
    CCellAnim *mManShadowAnim;
    s32 mManNextDirection; // 1 for left, 2 for right
    BOOL mManIsLeft;
    s32 mUnk44;

    CCellAnim *mHandAnim;
    CTRFloat mHandRotation;
    s32 mHandDirection;

    bool mUnk84;
    
    CCellAnim *mLampAnim;

    CCellAnim *mHiScoreRecordAnim;
    CCellAnim *mSecretCodeAnim;

    CEndlessCounter *mScoreCounter;
    CEndlessCounter *mHiScoreCounter;

    bool mUnk9C;
    bool mUnk9D;
    bool mUnk9E;

    s32 mUnkA0;

    u16 mLampSize;

    s32 mUnkA8;
};

#endif