#ifndef GAME_PROLOGUE_SCENEPROLOGUE_HPP
#define GAME_PROLOGUE_SCENEPROLOGUE_HPP

#include <revolution/types.h>
#include "ExScene.hpp"

namespace Prologue {

class CMyFlow; // Forward-declaration

extern s32 sceneVer;

void fn_8000B13C(char *, u32);
void fn_8000B160(char *);
void fn_8000B180(u32);
u32 fn_8000B188(void);

} // namespace Prologue

class CScenePrologue : public CExScene {
    friend class Prologue::CMyFlow;
    
public:
    virtual void _10(void);
    virtual void _14(void);
    virtual void _18(void);
    virtual void _1C(void);
    virtual void _20(void);
    virtual bool _24(void);
    virtual void _28(void);

    CScenePrologue(void) {}

    SCENE_DECL_CREATE_FN()

    static void fn_8000AD98(void);
};

#endif
