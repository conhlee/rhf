#ifndef GAME_STRAP_MYFLOW_HPP
#define GAME_STRAP_MYFLOW_HPP

#include <revolution/types.h>

#include "ExFlow.hpp"

#include "Strap/SceneStrap.hpp"

#include "GameManager.hpp"

namespace Strap {

class CMyFlow : public CExFlow {
public:
    virtual ~CMyFlow(void);
    virtual void _14(void);
    virtual bool _1C(u32 opcode, u32 arg0, const s32 *args);

    CMyFlow(const TickFlowCode *code, f32 initRest) :
        CExFlow(code, initRest)
    {
        mScene = gGameManager->getCurrentScene<CSceneStrap>();
    }

    TICKFLOW_DECL_CREATE_FN()

private:
    CSceneStrap *mScene;
};

} // namespace Strap

#endif
