#ifndef GAME_STRAP_SCENESTRAP_HPP
#define GAME_STRAP_SCENESTRAP_HPP

#include <revolution/types.h>

#include "ExScene.hpp"

#include "CellAnim.hpp"

namespace Strap {

class CMyFlow; // Forward-declaration
class CMyChecker; // Forward-declaration

extern s32 sceneVer;

} // namespace Strap

class CSceneStrap : public CExScene {
    friend class Strap::CMyFlow;
    friend class Strap::CMyChecker;
    
public:
    virtual ~CSceneStrap(void) {}
    virtual void _10(void);
    virtual void _14(void);
    virtual void _18(void);
    virtual void _1C(void);
    virtual void _20(void);
    virtual bool _24(void);
    virtual void _28(void);

    CSceneStrap(void) {}

    SCENE_DECL_CREATE_FN()

    void fn_8007B6C4(u32, u32);

    static void fn_8007B270(u32);
    
    static void fn_8007B2D8(void);

private:
    CCellAnim *mStrapAnim;
    s32 mFrameCounter;
    s32 mFadeOpacity;
};

#endif