#ifndef GAME_CREDIT_MYLAYOUT_HPP
#define GAME_CREDIT_MYLAYOUT_HPP

#include <revolution/types.h>

#include "Layout.hpp"

namespace Credit {

class CMyLayout : public CLayout {
public:
    virtual ~CMyLayout(void);
    virtual void _10(void);
    virtual void _14(void);

    CMyLayout(void) {
        setAnimationCount(0);
    }

    void fn_800C416C(s32, f32, f32);

private:
    nw4r::lyt::Pane *mPanesN[4];
    nw4r::lyt::TextBox *mPanesT[4];
};

} // namespace Credit

#endif
