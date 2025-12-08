#ifndef GAME_RAP_MYFLOW_HPP
#define GAME_RAP_MYFLOW_HPP

#include <revolution/types.h>

#include "ExFlow.hpp"

#include "Rap/SceneRap.hpp"

#include "GameManager.hpp"

namespace Rap {

class CMyFlow : public CExFlow {
public:
    virtual ~CMyFlow(void);
    virtual void _14(void);
    virtual bool _1C(u32 opcode, u32 arg0, const s32 *args);

    CMyFlow(const TickFlowCode *code, f32 initRest) :
        CExFlow(code, initRest)
    {
        mScene = gGameManager->getCurrentScene<CSceneRap>();
    }

    TICKFLOW_DECL_CREATE_FN()

private:
    CSceneRap *mScene;
};

} // namespace Rap

#endif
