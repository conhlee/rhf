#ifndef GAME_ERROR_SCENEERROR_HPP
#define GAME_ERROR_SCENEERROR_HPP

#include <revolution/types.h>

#include "ExScene.hpp"

namespace Error {

class CMyFlow; // Forward-declaration
class CMyChecker; // Forward-declaration
class CMyLayout; // Forward-declaration

extern s32 sceneVer;

} // namespace Error

class CSceneError : public CExScene {
    friend class Error::CMyFlow;
    friend class Error::CMyChecker;
    friend class Error::CMyLayout;
    
public:
    virtual ~CSceneError(void) {}
    virtual void _10(void);
    virtual void _14(void);
    virtual void _18(void);
    virtual void _1C(void);
    virtual void _20(void);
    virtual void _28(void);

    CSceneError(void) {}

    SCENE_DECL_CREATE_FN()

    void fn_80079B3C(u32, u32);

    void fn_80079B40(void);
    void fn_80079B98(void);
    void fn_80079BF0(void);

    static void fn_80079568(u32);

    static void fn_8007963C(void);

private:
    bool mDoExitOnEnd;

    bool mRestart;
    bool mReturnToMenu;
    bool mReturnToDataManager;
    bool mShutdownSystem;
};

#endif