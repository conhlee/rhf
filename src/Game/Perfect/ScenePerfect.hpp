#ifndef GAME_PERFECT_SCENEPERFECT_HPP
#define GAME_PERFECT_SCENEPERFECT_HPP

#include <revolution/types.h>

#include "ExScene.hpp"

namespace Perfect {

class CMyFlow; // Forward-declaration
class CMyChecker; // Forward-declaration
class CMyLayout; // Forward-declaration

extern s32 sceneVer;

} // namespace Perfect

class CScenePerfect : public CExScene {
    friend class Perfect::CMyFlow;
    friend class Perfect::CMyChecker;
    friend class Perfect::CMyLayout;
    
public:
    virtual ~CScenePerfect(void) {}
    virtual void _10(void);
    virtual void _14(void);
    virtual void _18(void);
    virtual void _1C(void);
    virtual void _20(void);
    virtual bool _24(void);
    virtual void _28(void);

    CScenePerfect(void) {}

    SCENE_DECL_CREATE_FN()

    void fn_8008328C(u32, u32);

    static void fn_80082FAC(u32);

    static void fn_80083014(void);
};

#endif