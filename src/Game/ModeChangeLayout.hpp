#ifndef GAME_MODECHANGELAYOUT_HPP
#define GAME_MODECHANGELAYOUT_HPP

#include <revolution/types.h>

#include <nw4r/lyt.h>

#include "Layout.hpp"

class CModeChangeLayout : public CLayout {
public:
    virtual ~CModeChangeLayout(void);
    virtual void _10(void);
    virtual void _14(void);

    CModeChangeLayout(void) {
        setAnimationCount(2);
    }

    void fn_800833E4(void);
    void fn_8008347C(void);
};

#endif
