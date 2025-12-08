#ifndef GAME_RAP_MYCHECKER_HPP
#define GAME_RAP_MYCHECKER_HPP

#include <revolution/types.h>

#include "ExChecker.hpp"

#include "Rap/SceneRap.hpp"

#include "GameManager.hpp"

namespace Rap {

class CMyChecker : public CExChecker {
public:
    virtual ~CMyChecker(void);
    virtual void _14(void);
    virtual u32 _24(u32);
    virtual u32 _28(u32);
    virtual u32 _2C(void);
    virtual u32 _30(u32);

    CMyChecker(void) {
        mScene = gGameManager->getCurrentScene<CSceneRap>();
    }

private:
    CSceneRap *mScene;
};

} // namespace Rap

#endif
