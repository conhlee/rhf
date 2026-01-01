#ifndef GAME_ERROR_MYLAYOUT_HPP
#define GAME_ERROR_MYLAYOUT_HPP

#include <revolution/types.h>

#include <revolution/NAND.h>

#include "Layout.hpp"

#include "Error/SceneError.hpp"

#include "MessageManager.hpp"

namespace Error {

class CMyLayout : public CLayout {
public:
    virtual ~CMyLayout(void);
    virtual void _10(void);
    virtual void _14(void);
    virtual void _18(nw4r::lyt::DrawInfo *drawInfo);

    CMyLayout(void) {
        setAnimationCount(14);
    }

    static void fn_8007A060(s32 modeChangeLayoutIndex, s32 cursorLayoutIndex);

private:
    void setButtonSelectSfxID(u16 soundID) {
        mBtn1_00.setSelectSfxID(soundID);
        mBtn2_00.setSelectSfxID(soundID);
        mBtn2_01.setSelectSfxID(soundID);
    }

    void setTMsg_00(const char *mesgID) {
        gMessageManager->setupTextBox(mTMsg_00, mesgID, 0);
    }
    void setTMsg_Btn1_00(const char *mesgID) {
        gMessageManager->setupTextBox(mTMsg_Btn1_00, mesgID, 0);
    }
    void setTMsg_Btn2_00(const char *mesgID) {
        gMessageManager->setupTextBox(mTMsg_Btn2_00, mesgID, 0);
    }
    void setTBtn_Btn1_00(const char *mesgID) {
        gMessageManager->setupTextBox(mTBtn_Btn1_00, mesgID, 0);
    }

    static void fn_80079FA4(bool useCursor);

private:
    s32 mUnk1C;
    s32 mUnk20;
    CSceneError *mScene;
    nw4r::lyt::TextBox *mTMsg_00;
    nw4r::lyt::TextBox *mTMsg_Btn1_00;
    nw4r::lyt::TextBox *mTMsg_Btn2_00;
    nw4r::lyt::TextBox *mTBtn_Btn1_00;
    CExBtnPaneManager *mButtonManager0;
    CExBtnPaneManager *mButtonManager1;
    CExBtnPane mBtn1_00;
    CExBtnPane mBtn2_00;
    CExBtnPane mBtn2_01;
    s32 mUnkE8;
    NANDResult mUnkEC;
    s32 mUnkF0;
    s32 mExitTimer;
    bool mUnkF8;
};

} // namespace Error

#endif
