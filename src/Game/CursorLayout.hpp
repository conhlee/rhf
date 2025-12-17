#ifndef GAME_CURSORLAYOUT_HPP
#define GAME_CURSORLAYOUT_HPP

#include <revolution/types.h>

#include <nw4r/lyt.h>

#include "Layout.hpp"

class CCursorLayout : public CLayout {
public:
    virtual ~CCursorLayout(void);
    virtual void _10(void);
    virtual void _18(nw4r::lyt::DrawInfo *drawInfo);
    virtual void _1C(nw4r::lyt::DrawInfo *drawInfo);

    CCursorLayout(void) {
        setAnimationCount(0);
    }

    void fn_8003A898(s32 cursorIndex, bool visible);

private:
    enum { CURSOR_COUNT = 2 };

private:
    nw4r::lyt::Pane *mPaneTrans[CURSOR_COUNT];
    nw4r::lyt::Pane *mPaneSRot[CURSOR_COUNT];
    nw4r::lyt::Pane *mPaneRot[CURSOR_COUNT];

    u8 mUnk34[7];
};

#endif
