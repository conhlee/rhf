#ifndef GAME_FORK_SCENEFORK_HPP
#define GAME_FORK_SCENEFORK_HPP

#include <revolution/types.h>

#include "ExScene.hpp"

#include "CellAnim.hpp"

#include "Pseudo3D.hpp"

namespace Fork {

class CMyFlow; // Forward-declaration
class CMyChecker; // Forward-declaration

extern s32 sceneVer;

} // namespace Fork

class CSceneFork : public CExScene {
    friend class Fork::CMyFlow;
    friend class Fork::CMyChecker;
    
public:
    virtual ~CSceneFork(void) {}
    virtual void _10(void);
    virtual void _14(void);
    virtual void _18(void);
    virtual void _1C(void);
    virtual void _20(void);
    virtual bool _24(void);
    virtual void _28(void);

    CSceneFork(void) {}

    SCENE_DECL_CREATE_FN()

    void fn_8004C91C(u32, s32);

    void fn_8004CDE4(void);

    static void fn_8004BE3C(u32);

    static void fn_8004BEB4(void);
    static void fn_8004BFD0(void);

private:
    class CFork {
    public:
        CCellAnim *mAnimBase;
        CCellAnim *mForkAnim;
        CCellAnim *mObjStingAnim[4];
        s32 mObjStingCount;
        bool mForkIsOut;
        u8 mUnk1D;
    };

private:
    CFork mFork[2];

    CCellAnim *mBalloonAnim;
    CCellAnim *mHandAnim;
    CCellAnim *mHandFarAnim;

    CCellAnim *mObjAnim[2];
    CPseudo3D *mObj3D;

    s32 mNextHandSet;

    bool mIsTwoPlay;
    s32 mForkCount;
    s32 mActiveFork; // 0 = nobody, 1 = P1, 2 = P2

    s32 mObjDirection;
};

#endif