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
    virtual bool _24(void);
    virtual void _28(void);

    CSceneError(void) {}

    SCENE_DECL_CREATE_FN()

private:
    u8 mUnk34;
    u8 mUnk35;
    u8 mUnk36;
    u8 mUnk37;
    u8 mUnk38;
};

#endif