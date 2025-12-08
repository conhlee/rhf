#ifndef GAME_PAUSELAYOUT_HPP
#define GAME_PAUSELAYOUT_HPP

#include <revolution/types.h>

#include "Layout.hpp"

class CPauseLayout : public CLayout {
public:
    virtual ~CPauseLayout(void);
    virtual void _10(void);

    CPauseLayout(void) {
        setAnimationCount(3);
    }

    bool getUnk1C(void) const { return mUnk1C; }

    void setUnk1D(bool unk) { mUnk1D = unk; }

    s32 getUnk24(void) const { return mUnk24; }

private:
    bool mUnk1C;
    u8 mUnk1D;
    s32 mUnk20;
    s32 mUnk24;
    s32 mUnk28;
};

#endif
