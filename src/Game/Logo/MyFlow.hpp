#ifndef GAME_LOGO_MYFLOW_HPP
#define GAME_LOGO_MYFLOW_HPP

#include <revolution/types.h>

#include "ExFlow.hpp"

#include "Logo/SceneLogo.hpp"

#include "GameManager.hpp"

namespace Logo {

class CMyFlow : public CExFlow {
public:
    virtual ~CMyFlow(void);
    virtual void _14(void);
    virtual bool _1C(u32 opcode, u32 arg0, const s32 *args);

    CMyFlow(const TickFlowCode *code, f32 initRest) :
        CExFlow(code, initRest)
    {
        mScene = gGameManager->getCurrentScene<CSceneLogo>();
    }

    TICKFLOW_DECL_CREATE_FN()

private:
    CSceneLogo *mScene;
};

} // namespace Logo

#endif
