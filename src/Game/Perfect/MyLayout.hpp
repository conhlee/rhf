#ifndef GAME_PERFECT_MYLAYOUT_HPP
#define GAME_PERFECT_MYLAYOUT_HPP

#include <revolution/types.h>

#include "Layout.hpp"

#include "Perfect/ScenePerfect.hpp"

namespace Perfect {

class CMyLayout : public CLayout {
public:
    virtual ~CMyLayout(void);
    virtual void _10(void);

    CMyLayout(void) {
        setAnimationCount(1);
    }
};

} // namespace Perfect

#endif
