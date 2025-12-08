#ifndef GAME_RAP_MYLAYOUT_HPP
#define GAME_RAP_MYLAYOUT_HPP

#include <revolution/types.h>

#include "Layout.hpp"

#include "MyTagProcessor.hpp"

namespace Rap {

class CMyLayout : public CLayout {
public:
    virtual ~CMyLayout(void);
    virtual void _10(void);
    virtual void _14(void);

    CMyLayout(void) {
        setAnimationCount(8);
    }

    void fn_80072E94(const char *mesgID, bool miss);
    void fn_80072F74(const char *mesgID);
    void fn_80072FF8(const char *mesgID);
    void fn_8007307C(void);
    void fn_80073100(void);
    void fn_8007314C(void);
    void fn_80073198(f32 x, f32 y);

private:
    CMyTagProcessor *mTagProcessor;

    nw4r::lyt::Pane *mNullMessage00;
    nw4r::lyt::Pane *mNullMissMsg00;
    nw4r::lyt::Pane *mNullMessage01;
    nw4r::lyt::Pane *mNullMessage02;

    nw4r::lyt::TextBox *mTextMessage00;
    nw4r::lyt::TextBox *mTextMissMsg00;
    nw4r::lyt::TextBox *mTextMessage01;
    nw4r::lyt::TextBox *mTextMessage02;

    f32 mBaseX;
    f32 mBaseY;

    bool mUnk48;
};

} // namespace Rap

#endif
