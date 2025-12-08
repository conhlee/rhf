#ifndef GAME_CREDIT_SCENECREDIT_HPP
#define GAME_CREDIT_SCENECREDIT_HPP

#include <revolution/types.h>

#include "ExScene.hpp"

#include "CellAnim.hpp"

#include "TimeRatio.hpp"

namespace Credit {

class CMyFlow; // Forward-declaration
class CMyChecker; // Forward-declaration

extern s32 sceneVer;

} // namespace Credit

class CSceneCredit : public CExScene {
    friend class Credit::CMyFlow;
    friend class Credit::CMyChecker;
    
public:
    virtual ~CSceneCredit(void) {}
    virtual void _10(void);
    virtual void _14(void);
    virtual void _18(void);
    virtual void _1C(void);
    virtual void _20(void);
    virtual bool _24(void);
    virtual void _28(void);

    CSceneCredit(void) {}

    SCENE_DECL_CREATE_FN()

    void fn_800C4C70(u32, u32);

    void fn_800C4C74(s32);

    void fn_800C4D2C(s32, f32, f32, f32);

    void fn_800C4DB4(s32, s32);

    void fn_800C4E38(s32);

    static void fn_800C421C(u32);

    static void fn_800C42AC(void);

private:
    enum {
        CHARA_COUNT = 28, // Including master.
        MASCOT_COUNT = 3,
    };

    enum {
        CELL_INDEX_CHARA_START,
        CELL_INDEX_CHARA_END = CELL_INDEX_CHARA_START + CHARA_COUNT,

        CELL_INDEX_WIPE = CELL_INDEX_CHARA_END,
        CELL_INDEX_TITLE,
        CELL_INDEX_NAVI,

        CELL_INDICES_END,
    };

    CCellAnim *mCharaAnim[CHARA_COUNT];
    CCellAnim *mMasterAnim;

    CCellAnim *mWipeAnim;
    CCellAnim *mTitleAnim;

    CCellAnim *mMascotBodyAnim[MASCOT_COUNT];
    CCellAnim *mMascotFaceAnim[MASCOT_COUNT];
    CTRParabola mMascotPosition[MASCOT_COUNT];

    CTRFloat mScreenScroll;

    s32 mCharaIndex; // -1 for master.
};

#endif