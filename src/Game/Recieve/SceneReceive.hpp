#ifndef GAME_RECEIVE_SCENERECEIVE_HPP
#define GAME_RECEIVE_SCENERECEIVE_HPP

#include <revolution/types.h>

#include "ExScene.hpp"

#include "CellAnim.hpp"

#include "Pseudo3D.hpp"

namespace Receive {

class CMyFlow; // Forward-declaration
class CMyChecker; // Forward-declaration

extern s32 sceneVer;

} // namespace Receive

class CSceneReceive : public CExScene {
    friend class Receive::CMyFlow;
    friend class Receive::CMyChecker;
    
public:
    virtual ~CSceneReceive(void) {}
    virtual void _10(void);
    virtual void _14(void);
    virtual void _18(void);
    virtual void _1C(void);
    virtual void _20(void);
    virtual bool _24(void);
    virtual void _28(void);

    CSceneReceive(void) {}

    SCENE_DECL_CREATE_FN()

private:
    
};

#endif