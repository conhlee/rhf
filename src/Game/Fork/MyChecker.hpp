#ifndef GAME_FORK_MYCHECKER_HPP
#define GAME_FORK_MYCHECKER_HPP

#include <revolution/types.h>

#include "ExChecker.hpp"

#include "Fork/SceneFork.hpp"

#include "CellAnim.hpp"

#include "TimeRatio.hpp"

#include "Sound.hpp"

#include "GameManager.hpp"

namespace Fork {

class CMyChecker : public CExChecker {
public:
    virtual ~CMyChecker(void);
    virtual void _14(void);
    virtual u32 _24(u32);
    virtual u32 _28(u32);
    virtual u32 _2C(void);
    virtual u32 _30(u32);

    CMyChecker(void) {
        mScene = gGameManager->getCurrentScene<CSceneFork>();
    }

private:
    CSceneFork *mScene;

    CCellAnim *mObjAnim;
    CCellAnim *mObjTrailAnim[2];
    CTRFloat mTimeRatio;

    s32 mObjType;

    bool mUnkC4;

    SNDHandle mSNDHandle;

    u8 mUnkD0;

    CSceneFork::CFork *mFork;
};

} // namespace Fork

#endif
