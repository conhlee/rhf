#ifndef GAME_PROLOGUE_MYLAYOUT_HPP
#define GAME_PROLOGUE_MYLAYOUT_HPP

#include <revolution/types.h>

#include "Layout.hpp"

namespace Prologue {

class CMyLayout : public CLayout {
public:
    virtual ~CMyLayout(void);
    virtual void _10(void);

    CMyLayout(void) {
        setAnimationCount(1);
    }

    void fn_8007785C(u32 animIndex);
};

} // namespace Prologue

#endif
