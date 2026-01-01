#include "Error/MyLayout.hpp"

#include "LayoutManager.hpp"

#include "ModeChangeLayout.hpp"

#include "CursorLayout.hpp"

#include "MessageManager.hpp"

#include "BackupManager.hpp"

#include "GameManager.hpp"

#include "Controller.hpp"

#include "rev_tengoku.rsid"

extern bool lbl_80320498;

static s32 lbl_8031F408;
static s32 lbl_8031F40C;

static const char layoutFile_error[] = "error.brlyt";

DECL_SECTION(".sdata") static const char * const layoutFileTable[] = { layoutFile_error, NULL };

void Error::CMyLayout::fn_80079FA4(bool useCursor) {
    if ((lbl_80320498 != useCursor) && (lbl_8031F408 >= 0)) {
        if (useCursor) {
            gLayoutManager->getLayout<CModeChangeLayout>(lbl_8031F408)->fn_8008347C();
        }
        else {
            gLayoutManager->getLayout<CModeChangeLayout>(lbl_8031F408)->fn_800833E4();
        }
    }
    lbl_80320498 = useCursor;

    if (lbl_8031F40C >= 0) {
        gLayoutManager->getLayout<CCursorLayout>(lbl_8031F40C)->setEnabled(useCursor);
    }

    CMyLayout *myLayout = gLayoutManager->getLayout<CMyLayout>(0);
    myLayout->setButtonSelectSfxID(lbl_80320498 ? SE_CURSOR_WAKU : SE_CURSOR_KEY);
}

void Error::CMyLayout::fn_8007A060(s32 modeChangeLayoutIndex, s32 cursorLayoutIndex) {
    lbl_8031F408 = modeChangeLayoutIndex;
    lbl_8031F40C = cursorLayoutIndex;
}

void Error::CMyLayout::_10(void) {
    lbl_80320498 = true;

    nw4r::lyt::MultiArcResourceAccessor *resAccessor = gLayoutManager->getResAccessor();

    u32 layoutBinSize;
    void *layoutBin = gLayoutManager->getResAccessor()->GetResource(0, layoutFileTable[0], &layoutBinSize);

    buildLayout(layoutBin, resAccessor);

    gMessageManager->fn_80088474(getLayout()->GetRootPane());

    mBtn1_00._08(getLayout()->GetRootPane()->FindPaneByName("W_btn_btn1_00"));
    mBtn2_00._08(getLayout()->GetRootPane()->FindPaneByName("W_btn_btn2_00"));
    mBtn2_01._08(getLayout()->GetRootPane()->FindPaneByName("W_btn_btn2_01"));

    mBtn1_00.setSelectSfxID(SE_CURSOR_WAKU);
    mBtn2_00.setSelectSfxID(SE_CURSOR_WAKU);
    mBtn2_01.setSelectSfxID(SE_CURSOR_WAKU);

    mTMsg_00 = static_cast<nw4r::lyt::TextBox *>(getLayout()->GetRootPane()->FindPaneByName("T_msg_00"));
    mTMsg_Btn1_00 = static_cast<nw4r::lyt::TextBox *>(getLayout()->GetRootPane()->FindPaneByName("T_msg_btn1_00"));
    mTMsg_Btn2_00 = static_cast<nw4r::lyt::TextBox *>(getLayout()->GetRootPane()->FindPaneByName("T_msg_btn2_00"));
    mTBtn_Btn1_00 = static_cast<nw4r::lyt::TextBox *>(getLayout()->GetRootPane()->FindPaneByName("T_btn_btn1_00"));

    nw4r::lyt::Pane *pane;
    
    pane = getLayout()->GetRootPane()->FindPaneByName("N_error_00");
    getAnimation(0)->fn_801D9E70(getLayout(), pane, "error_open", resAccessor);

    pane = getLayout()->GetRootPane()->FindPaneByName("N_error_btn1_00");
    getAnimation(1)->fn_801D9E70(getLayout(), pane, "error_open", resAccessor);

    pane = getLayout()->GetRootPane()->FindPaneByName("N_error_btn2_00");
    getAnimation(2)->fn_801D9E70(getLayout(), pane, "error_open", resAccessor);

    pane = getLayout()->GetRootPane()->FindPaneByName("N_error_00");
    getAnimation(3)->fn_801D9E70(getLayout(), pane,"error_close", resAccessor);

    pane = getLayout()->GetRootPane()->FindPaneByName("N_error_btn1_00");
    getAnimation(4)->fn_801D9E70(getLayout(), pane, "error_close", resAccessor);

    pane = getLayout()->GetRootPane()->FindPaneByName("N_error_btn2_00");
    getAnimation(5)->fn_801D9E70(getLayout(), pane, "error_close", resAccessor);

    getAnimation(6)->fn_801D9E70(getLayout(), mBtn1_00.getPane(), "error_focus", resAccessor);
    getAnimation(7)->fn_801D9E70(getLayout(), mBtn2_00.getPane(), "error_focus", resAccessor);
    getAnimation(8)->fn_801D9E70(getLayout(), mBtn2_01.getPane(), "error_focus", resAccessor);

    getAnimation(9)->fn_801D9E70(getLayout(), mBtn1_00.getPane(), "error_action", resAccessor);
    getAnimation(10)->fn_801D9E70(getLayout(), mBtn2_00.getPane(), "error_action", resAccessor);
    getAnimation(11)->fn_801D9E70(getLayout(), mBtn2_01.getPane(), "error_action", resAccessor);

    mBtn1_00.setFocusAnime(getAnimation(6));
    mBtn2_00.setFocusAnime(getAnimation(7));
    mBtn2_01.setFocusAnime(getAnimation(8));

    mBtn1_00.setActionAnime(getAnimation(9));
    mBtn2_00.setActionAnime(getAnimation(10));
    mBtn2_01.setActionAnime(getAnimation(11));

    nw4r::lyt::Pane *cursorFrm = getLayout()->GetRootPane()->FindPaneByName("N_cursor_frm_00");
    getAnimation(12)->fn_801D9E70(getLayout(), cursorFrm, "error_cursor_frm_on", resAccessor);
    getAnimation(13)->fn_801D9E70(getLayout(), cursorFrm, "error_cursor_frm_off", resAccessor);

    nw4r::lyt::Pane *cursorFrmNull = getLayout()->GetRootPane()->FindPaneByName("N_cursor_frm_00");
    nw4r::lyt::Pane *cursorFrmWin = getLayout()->GetRootPane()->FindPaneByName("W_cursor_frm_00");

    mButtonManager0 = new CExBtnPaneManager(1);
    mButtonManager0->fn_8008F688(&lbl_802ED5F0);
    mButtonManager0->fn_8008F96C(cursorFrmNull, cursorFrmWin, getAnimation(12), getAnimation(13), 16.0f);
    mButtonManager0->fn_8008F65C(0, &mBtn1_00);

    mButtonManager1 = new CExBtnPaneManager(2);
    mButtonManager1->fn_8008F688(&lbl_802ED604);
    mButtonManager1->fn_8008F96C(cursorFrmNull, cursorFrmWin, getAnimation(12), getAnimation(13), 16.0f);
    mButtonManager1->fn_8008F65C(0, &mBtn2_00);
    mButtonManager1->fn_8008F65C(1, &mBtn2_01);

    mButtonManager0->fn_8008F81C(lbl_80320498, -1);
    mButtonManager1->fn_8008F81C(lbl_80320498, -1);

    setButtonSelectSfxID(lbl_80320498 ? SE_CURSOR_WAKU : SE_CURSOR_KEY);

    this->CLayout::_10();

    mUnkE8 = gBackupManager->getUnk4C();
    mUnkEC = gBackupManager->getUnk50();

    if (gBackupManager->getUnk4C() == 7) {
        mUnk1C = 1;
        mUnkF8 = false;

        switch (mUnkEC) {
        case NAND_RESULT_ALLOC_FAILED:
        case NAND_RESULT_BUSY:
            setTMsg_00("0201001");
            break;
        case NAND_RESULT_CORRUPT:
            setTMsg_00("0201002");
            break;
        case NAND_RESULT_MAXBLOCKS:
            setTMsg_00("0201003");
            break;
        case NAND_RESULT_MAXFILES:
            setTMsg_00("0201004");
            break;
        default:
            setTMsg_00("0201000");
            break;
        }

        getAnimation(0)->playFromBeginForward();
    }
    else if ((mUnkE8 == 2) || (mUnkE8 == 3)) {
        mUnk1C = 0;
        mUnkF8 = false;

        if (mUnkE8 == 2) {
            setTMsg_Btn1_00("0201010");
        }
        else {
            setTMsg_Btn1_00("0201011");
        }

        setTBtn_Btn1_00("0201030");

        getAnimation(1)->playFromBeginForward();

        mButtonManager0->fn_8008F984(1, gLayoutManager->getDrawInfo());

        mBtn1_00.setActionSfxID(SE_KETTEI_MIDDLE);
    }
    else if ((mUnkE8 == 4) || (mUnkE8 == 5) || (mUnkE8 == 6)) {
        mUnk1C = 2;
        mUnkF8 = false;

        setTMsg_Btn2_00("0201020");

        getAnimation(2)->playFromBeginForward();

        mButtonManager1->fn_8008F984(1, gLayoutManager->getDrawInfo());

        mBtn1_00.setActionSfxID(SE_KETTEI_MIDDLE);
        mBtn2_00.setActionSfxID(SE_DATA_FILE_CLEAR);
        mBtn2_01.setActionSfxID(SE_CANCEL_SMALL);
    }

    mScene = gGameManager->getCurrentScene<CSceneError>();

    _18(gLayoutManager->getDrawInfo());

    mButtonManager1->fn_8008F80C(fn_80079FA4);
    mButtonManager0->fn_8008F80C(fn_80079FA4);

    mExitTimer = 0;
    mUnkF8 = false;
}

