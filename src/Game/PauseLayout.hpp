#ifndef GAME_PAUSELAYOUT_HPP
#define GAME_PAUSELAYOUT_HPP

#include <revolution/types.h>

#include "Layout.hpp"

class CPauseLayout : public CLayout {
public:
    virtual ~CPauseLayout(void);
    virtual void _10(void);
    virtual void _14(void);
    virtual void _18(nw4r::lyt::DrawInfo *drawInfo);
    virtual void _1C(nw4r::lyt::DrawInfo *drawInfo);

    CPauseLayout(void) {
        setAnimationCount(3);
    }

    bool getPaused(void) const { return mIsPaused; }

    void setUnk1D(bool unk) { mIsEnabled = unk; }

    s32 getUnk24(void) const { return mUnk24; }

private:
    bool mIsPaused;
    bool mIsEnabled;
    s32 mUnk20;
    s32 mUnk24;
    s32 mUnk28;
};

#endif
