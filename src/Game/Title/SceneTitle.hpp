#ifndef GAME_TITLE_SCENETITLE_HPP
#define GAME_TITLE_SCENETITLE_HPP

#include <revolution/types.h>

#include "ExScene.hpp"

#include "TimeRatio.hpp"

#include "CellAnim.hpp"

namespace Title {

class CMyFlow; // Forward-declaration
class CMyChecker; // Forward-declaration
class CMyLayoutF; // Forward-declaration

extern s32 sceneVer;

} // namespace Title

class CSceneTitle : public CExScene {
    friend class Title::CMyFlow;
    friend class Title::CMyChecker;
    friend class Title::CMyLayoutF;

public:
    virtual ~CSceneTitle(void) {}
    virtual void _10(void);
    virtual void _14(void);
    virtual void _18(void);
    virtual void _1C(void);
    virtual void _20(void);
    virtual void _28(void);

    CSceneTitle(void) {}

    SCENE_DECL_CREATE_FN()

    static void fn_80092094(u32 input);
    static void loadArchives();
    void fn_800925C0(u32 arg1, u32 arg2);
    void showTitleGirl();
    void playTitleGirlAnim(u16 animID);
    void setupLayouts(s32 arg);

    u32 unk34;
    CCellAnim *mTitleGirlAnim;
    CTRParabola mTitleGirlPos;
    bool unk94;
    bool unk95;
};

#endif
