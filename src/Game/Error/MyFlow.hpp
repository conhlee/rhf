#ifndef GAME_ERROR_MYFLOW_HPP
#define GAME_ERROR_MYFLOW_HPP

#include <revolution/types.h>

#include "ExFlow.hpp"

#include "Error/SceneError.hpp"

#include "GameManager.hpp"

namespace Error {

class CMyFlow : public CExFlow {
public:
    virtual ~CMyFlow(void);
    virtual void _14(void);
    virtual bool _1C(u32 opcode, u32 arg0, const s32 *args);

    CMyFlow(const TickFlowCode *code, f32 initRest) :
        CExFlow(code, initRest)
    {
        mScene = gGameManager->getCurrentScene<CSceneError>();
    }

    TICKFLOW_DECL_CREATE_FN()

private:
    CSceneError *mScene;
};

} // namespace Error

#endif
