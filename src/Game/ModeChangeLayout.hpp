#ifndef GAME_MODECHANGELAYOUT_HPP
#define GAME_MODECHANGELAYOUT_HPP

#include <revolution/types.h>

#include <nw4r/lyt.h>

#include "Layout.hpp"

class CModeChangeLayout : public CLayout {
public:
    virtual ~CModeChangeLayout(void);
    virtual void _10(void);
    virtual void _18(nw4r::lyt::DrawInfo *drawInfo);
    virtual void _1C(nw4r::lyt::DrawInfo *drawInfo);

    CModeChangeLayout(void) {
        setAnimationCount(2);
    }

private:
    u8 pad04[0x1C - 0x4];
};

#endif
