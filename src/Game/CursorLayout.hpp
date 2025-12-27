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

    void setEnabled(bool enable) {
        mCursorDrawEnable = enable;
    }

private:
    enum { CURSOR_COUNT = 2 };

private:
    nw4r::lyt::Pane *mPaneTrans[CURSOR_COUNT];
    nw4r::lyt::Pane *mPaneSRot[CURSOR_COUNT];
    nw4r::lyt::Pane *mPaneRot[CURSOR_COUNT];

    // NOTE: technically useless; the base class already has the
    // property mDrawEnable
    bool mCursorDrawEnable;

    bool mEnableTrans[CURSOR_COUNT];
    bool mEnableRot[CURSOR_COUNT];
    bool mEnableScale[CURSOR_COUNT];
};

#endif
