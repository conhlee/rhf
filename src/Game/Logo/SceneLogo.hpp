#ifndef GAME_LOGO_SCENELOGO_HPP
#define GAME_LOGO_SCENELOGO_HPP

#include <revolution/types.h>

#include "ExScene.hpp"

#include "CellAnim.hpp"

namespace Logo {

class CMyFlow; // Forward-declaration
class CMyChecker; // Forward-declaration

extern s32 sceneVer;

} // namespace Logo

class CSceneLogo : public CExScene {
    friend class Logo::CMyFlow;
    friend class Logo::CMyChecker;
    
public:
    virtual ~CSceneLogo(void) {}
    virtual void _10(void);
    virtual void _14(void);
    virtual void _18(void);
    virtual void _1C(void);
    virtual void _20(void);
    virtual bool _24(void);
    virtual void _28(void);

    CSceneLogo(void) {}

    SCENE_DECL_CREATE_FN()

    void fn_800A0D64(u32, u32);

    static void fn_800A0B5C(u32);

    static void fn_800A0BC4(void);

private:
    u8 mUnk04[64];
    s32 mTimer;
    s32 mUnk78;
    s32 mUnk7C;
};

#endif