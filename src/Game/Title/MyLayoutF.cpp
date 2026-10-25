#include "Title/MyLayoutF.hpp"

#include "LayoutManager.hpp"

#include "GameManager.hpp"

#include "MessageManager.hpp"

#include "ModeChangeLayout.hpp"

#include "RFLManager.hpp"

#include "SaveData.hpp"

#include "Controller.hpp"

#include "PlayHistory.hpp"

#include "CafeTopic.hpp"

#include "BackupManager.hpp"

#include "rev_tengoku.rsid"

u8 lbl_8031F5C8[4] = {0xFF, 0x00, 0xFF, 0x02};
u8 lbl_8031F5CC[4] = {0x01, 0xFF, 0x03, 0xFF};
u8 lbl_8031F5D0[4] = {0xFF, 0xFF, 0x00, 0x01};
u8 lbl_8031F5D4[4] = {0x02, 0x03, 0xFF, 0xFF};
u8 lbl_8031F5D8[4] = {0xFF, 0xFF, 0x01, 0x02};
u8 lbl_8031F5DC[4] = {0xFF, 0x02, 0x03, 0xFF};
u8 lbl_8031F5E0[4] = {0xFF, 0x00, 0x00, 0x00};
u8 lbl_8031F5E4[4] = {0x01, 0xFF, 0xFF, 0xFF};

s32 lbl_8031F5E8 = -1;
s32 lbl_8031F5EC = -1;

CExBtnPaneManager_Sub lbl_802ED858 = {0, lbl_8031F5C8, lbl_8031F5CC, lbl_8031F5D0, lbl_8031F5D4};
CExBtnPaneManager_Sub lbl_802ED86C = {0, lbl_8031F5D8, lbl_8031F5DC, lbl_8031F5E0, lbl_8031F5E4};

u8 lbl_802ED880[20] = {0xFF, 0x00, 0x01, 0x02, 0x03, 0xFF, 0x05, 0x06, 0x07, 0x08, 0xFF, 0x0A, 0x0B, 0x0C, 0x0D, 0xFF, 0x0F, 0x00, 0x00, 0x00};
u8 lbl_802ED894[20] = {0x01, 0x02, 0x03, 0x04, 0xFF, 0x06, 0x07, 0x08, 0x09, 0xFF, 0x0B, 0x0C, 0x0D, 0x0E, 0xFF, 0x10, 0xFF, 0x00, 0x00, 0x00};
u8 lbl_802ED8A8[20] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0A, 0x00, 0x00, 0x00};
u8 lbl_802ED8BC[20] = {0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x0F, 0x10, 0x10, 0x10, 0xFF, 0xFF, 0x00, 0x00, 0x00};

CExBtnPaneManager_Sub lbl_802ED8D0 = {0, lbl_802ED880, lbl_802ED894, lbl_802ED8A8, lbl_802ED8BC};

u8 lbl_802ED8E4[13] = {0x0A, 0x00, 0x01, 0x02, 0x03, 0x0A, 0x05, 0x06, 0x07, 0x08, 0xFF, 0x04, 0x0A};
u8 lbl_802ED8F4[13] = {0x01, 0x02, 0x03, 0x04, 0x0B, 0x06, 0x07, 0x08, 0x09, 0x0B, 0x00, 0xFF, 0x0B};
u8 lbl_802ED904[13] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0xFF, 0xFF, 0x05};
u8 lbl_802ED914[13] = {0x05, 0x06, 0x07, 0x08, 0x09, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x05, 0x09, 0xFF};

CExBtnPaneManager_Sub lbl_802ED924 = {0, lbl_802ED8E4, lbl_802ED8F4, lbl_802ED904, lbl_802ED914};

const char *sIconNamePaneNames[16] = {"P_icon_name_00", "P_icon_name_01", "P_icon_name_02", "P_icon_name_03", "P_icon_name_04", "P_icon_name_05", "P_icon_name_06", "P_icon_name_07", "P_icon_name_08", "P_icon_name_09", "P_icon_name_10", "P_icon_name_11", "P_icon_name_12", "P_icon_name_13", "P_icon_name_14", NULL};

const char layoutFile_fileSelect[] = "fileSelect.brlyt";
DECL_SECTION(".sdata") static const char *layoutFileTable[] = { layoutFile_fileSelect };

extern const char *sFileButtonLabelNames[4];
extern const char *sFileActionButtonLabelNames[4];
extern const char *sNameButtonLabelNames[17];
extern const char *sMiiButtonLabelNames[13];
extern const char *sNameFramePaneNames[15];
extern const char *sNamePaneNames[15];
extern const char *sIconBackgroundPaneNames[10];
extern const char *sIconMiiPaneNames[10];

const char sAnimName_fileSelect_focus[] = "fileSelect_focus";
const char sAnimName_fileSelect_action[] = "fileSelect_action";
const char sAnimName_fileSelect_name_activate[] = "fileSelect_name_activate";
const char sAnimName_fileSelect_name_deactivate[] = "fileSelect_name_deactivate";
const char sAnimName_fileSelect_bg_loop[] = "fileSelect_bg_loop";
const char sAnimName_fileSelect_cursor_frm_on[] = "fileSelect_cursor_frm_on";
const char sAnimName_fileSelect_cursor_frm_off[] = "fileSelect_cursor_frm_off";
const char sAnimName_fileSelect_window_in[] = "fileSelect_window_in";
const char sAnimName_fileSelect_window_out[] = "fileSelect_window_out";
const char sAnimName_fileSelect_file_btn_in[] = "fileSelect_file_btn_in";
const char sAnimName_fileSelect_file_btn_out[] = "fileSelect_file_btn_out";
const char sAnimName_fileSelect_top_frm_in[] = "fileSelect_top_frm_in";
const char sAnimName_fileSelect_top_frm_out[] = "fileSelect_top_frm_out";

const char *sAnimNamePtr_fileSelect_focus = sAnimName_fileSelect_focus;
const char *sAnimNamePtr_fileSelect_action = sAnimName_fileSelect_action;
const char *sAnimNamePtr_fileSelect_name_activate = sAnimName_fileSelect_name_activate;
const char *sAnimNamePtr_fileSelect_name_deactivate = sAnimName_fileSelect_name_deactivate;
const char *sAnimNamePtr_fileSelect_bg_loop = sAnimName_fileSelect_bg_loop;
const char *sAnimNamePtr_fileSelect_cursor_frm_on = sAnimName_fileSelect_cursor_frm_on;
const char *sAnimNamePtr_fileSelect_cursor_frm_off = sAnimName_fileSelect_cursor_frm_off;
const char *sAnimNamePtr_fileSelect_window_in = sAnimName_fileSelect_window_in;
const char *sAnimNamePtr_fileSelect_window_out = sAnimName_fileSelect_window_out;
const char *sAnimNamePtr_fileSelect_file_btn_in = sAnimName_fileSelect_file_btn_in;
const char *sAnimNamePtr_fileSelect_file_btn_out = sAnimName_fileSelect_file_btn_out;
const char *sAnimNamePtr_fileSelect_top_frm_in = sAnimName_fileSelect_top_frm_in;
const char *sAnimNamePtr_fileSelect_top_frm_out = sAnimName_fileSelect_top_frm_out;

bool lbl_803204B0[4] = {};
u8 lbl_803204B4[4] = {};

extern bool lbl_80320498;
TFD_EXTERN(lbl_80253E18)
TFD_EXTERN(lbl_80259540)

RFLCreateID lbl_8034C820[4] = {};

enum {
    anim_fileSelect_focus_fileButton = 0, // x{SAVE_COUNT}
    anim_fileSelect_action_fileButton = 4, // x{SAVE_COUNT}
    anim_fileSelect_file_btn_in = 8, // x{SAVE_COUNT}
    anim_fileSelect_file_btn_out = 12, // x{SAVE_COUNT}
    anim_fileSelect_window_in = 16, // x{SAVE_COUNT}
    anim_fileSelect_window_out = 20, // x{SAVE_COUNT}
    anim_fileSelect_focus_fileActionButton = 24, // x{SAVE_COUNT}
    anim_fileSelect_action_fileActionButton = 28, // x{SAVE_COUNT}

    anim_fileSelect_focus_nameButton = 32, // x17
    anim_fileSelect_action_nameButton = 49, // x17

    anim_fileSelect_focus_miiButton = 66, // x13
    anim_fileSelect_action_miiButton = 79, // x13

    anim_fileSelect_focus_deleteBackBtn = 92,
    anim_fileSelect_action_deleteBackBtn = 93,
    anim_fileSelect_focus_confirmBtn = 94,
    anim_fileSelect_focus_cancelBtn = 95,
    anim_fileSelect_action_confirmBtn = 96,
    anim_fileSelect_action_cancelBtn = 97,
    anim_fileSelect_focus_messageOkBtn = 98,
    anim_fileSelect_action_messageOkBtn = 99,

    anim_fileSelect_name_activate_nameFramePane = 100, // x15
    anim_fileSelect_name_activate_namePane = 115, // x15
    anim_fileSelect_name_deactivate_nameFramePane = 130, // x15
    anim_fileSelect_name_deactivate_namePane = 145, // x15

    anim_fileSelect_name_activate_iconBackgroundPane = 160, // x10
    anim_fileSelect_name_activate_iconMiiPane = 170, // x10
    anim_fileSelect_name_deactivate_iconBackgroundPane = 180, // x10
    anim_fileSelect_name_deactivate_iconMiiPane = 190, // x10

    anim_fileSelect_bg_loop_N_bg_all_00 = 200,
    anim_fileSelect_cursor_frm_on_N_cursor_frm_00 = 201,
    anim_fileSelect_cursor_frm_off_N_cursor_frm_00 = 202,
    anim_fileSelect_window_in_W_menu_00 = 203,
    anim_fileSelect_window_out_W_menu_00 = 204,
    anim_fileSelect_window_in_W_menu_01 = 205,
    anim_fileSelect_window_out_W_menu_01 = 206,
    anim_fileSelect_window_in_W_menu_mii_00 = 207,
    anim_fileSelect_window_out_W_menu_mii_00 = 208,
    anim_fileSelect_window_in_W_msg_00 = 209,
    anim_fileSelect_window_out_W_msg_00 = 210,
    anim_fileSelect_window_in_W_msg_02 = 211,
    anim_fileSelect_window_out_W_msg_02 = 212,
    anim_fileSelect_window_in_W_msg_01 = 213,
    anim_fileSelect_window_out_W_msg_01 = 214,
    anim_fileSelect_top_frm_in_W_msg_01 = 215,
    anim_fileSelect_top_frm_out_W_msg_01 = 216,

    anim_fileSelect_COUNT,
};

void Title::CMyLayoutF::fileSelectManagerCallback(s32 arg1, s8 arg2) {
    if (arg2 == arg1)
        return;
    if (arg2 != 0)
        return;

    lbl_802ED86C.buttonIndices3[0] = static_cast<u8>(arg1);
}

void Title::CMyLayoutF::iconSelectManagerCallback(s32 arg1, s8 arg2) {
    if (arg2 == arg1)
        return;
    if (static_cast<u32>(arg1) >= 15)
        return;
    if (arg2 < 15)
        return;

    lbl_802ED8D0.buttonIndices2[arg2] = static_cast<u8>(arg1);
}

void Title::CMyLayoutF::miiSelectManagerCallback(s32 arg1, s8 arg2) {
    if (arg2 == arg1)
        return;
    if (static_cast<u32>(arg1) >= 10)
        return;
    if (arg2 < 10)
        return;

    if (arg2 == 10)
        lbl_802ED924.buttonIndices1[arg2] = arg1;
    if (arg2 == 11)
        lbl_802ED924.buttonIndices0[arg2] = arg1;
    if (arg2 == 12)
        lbl_802ED924.buttonIndices2[arg2] = arg1;
}

void Title::CMyLayoutF::miscManagerCursorCallback(bool useCursor) {
    if (lbl_80320498 != useCursor && lbl_8031F5E8 >= 0) {
        if (useCursor) {
            gLayoutManager->getLayout<CModeChangeLayout>(lbl_8031F5E8)->fn_8008347C();
        } else {
            gLayoutManager->getLayout<CModeChangeLayout>(lbl_8031F5E8)->fn_800833E4();
        }
    }

    lbl_80320498 = useCursor;

    if (lbl_8031F5EC >= 0)
        gLayoutManager->getLayout<CCursorLayout>(lbl_8031F5EC)->setEnabled(useCursor);

    gLayoutManager->getLayout<CMyLayoutF>(0)->updateSelectionSfx();
}

void Title::CMyLayoutF::unkCallback(s32 arg1, s32 arg2) {
    lbl_8031F5E8 = arg1;
    lbl_8031F5EC = arg2;
}

Title::CMyLayoutF::CMyLayoutF(void) {
    setAnimationCount(anim_fileSelect_COUNT);
    mScene = gGameManager->getCurrentScene<CSceneTitle>();
}

void Title::CMyLayoutF::_10(void) {
    nw4r::lyt::MultiArcResourceAccessor *resAccessor = gLayoutManager->getResAccessor();

    u32 layoutBinSize;
    void *layoutBin = resAccessor->GetResource(0, layoutFileTable[0], &layoutBinSize);

    buildLayout(layoutBin, resAccessor);

    gMessageManager->fn_80088474(getLayout()->GetRootPane());

    nw4r::lyt::TextBox *textBox = static_cast<nw4r::lyt::TextBox *>(getLayout()->GetRootPane()->FindPaneByName("T_msg_02"));

    /* Continue? */
    gMessageManager->setupTextBox(textBox, "0104300", 0);

    for (u8 i = 0; i < ARRAY_LENGTH(mFileButtons); i++) {
        mFileButtons[i]._08(getLayout()->GetRootPane()->FindPaneByName(sFileButtonLabelNames[i]));
        mFileButtons[i].setSelectSfxID(SE_CURSOR_WAKU);
        mFileButtons[i].setActionSfxID(SE_KETTEI_SMALL);
    }

    for (u8 i = 0; i < ARRAY_LENGTH(mFileActionButtons); i++) {
        mFileActionButtons[i]._08(getLayout()->GetRootPane()->FindPaneByName(sFileActionButtonLabelNames[i]));
        mFileActionButtons[i].setSelectSfxID(SE_CURSOR_WAKU);
        mFileActionButtons[i].setActionSfxID(i == 0 ? SE_KETTEI_MIDDLE : (i == 1 ? SE_CANCEL_SMALL : SE_KETTEI_SMALL));
    }

    for (u8 i = 0; i < ARRAY_LENGTH(mNameButtons); i++) {
        mNameButtons[i]._08(getLayout()->GetRootPane()->FindPaneByName(sNameButtonLabelNames[i]));
        mNameButtons[i].setSelectSfxID(SE_CURSOR_WAKU);
        mNameButtons[i].setActionSfxID(i == 15 ? SE_CANCEL_SMALL : SE_KETTEI_SMALL);
    }

    for (u8 i = 0; i < ARRAY_LENGTH(mMiiButtons); i++) {
        mMiiButtons[i]._08(getLayout()->GetRootPane()->FindPaneByName(sMiiButtonLabelNames[i]));
        mMiiButtons[i].setSelectSfxID(SE_CURSOR_WAKU);
        mMiiButtons[i].setActionSfxID(i == 12 ? SE_CANCEL_SMALL : SE_KETTEI_SMALL);
    }

    mDeleteBackButton._08(getLayout()->GetRootPane()->FindPaneByName("N_back_btn_01"));
    mDeleteBackButton.setSelectSfxID(SE_CURSOR_WAKU);
    mDeleteBackButton.setActionSfxID(SE_CANCEL_SMALL);

    mConfirmButton._08(getLayout()->GetRootPane()->FindPaneByName("N_OK_btn_00"));
    mCancelButton._08(getLayout()->GetRootPane()->FindPaneByName("N_NG_btn_00"));

    mConfirmButton.setSelectSfxID(SE_CURSOR_WAKU);
    mCancelButton.setSelectSfxID(SE_CURSOR_WAKU);

    mConfirmButton.setActionSfxID(SE_KETTEI_MIDDLE);
    mCancelButton.setActionSfxID(SE_CANCEL_SMALL);

    mMessageOkButton._08(getLayout()->GetRootPane()->FindPaneByName("N_ok_btn_00"));
    mMessageOkButton.setSelectSfxID(SE_CURSOR_WAKU);
    mMessageOkButton.setActionSfxID(SE_KETTEI_SMALL);

    mMenuMessageTextBox = static_cast<nw4r::lyt::TextBox *>(getLayout()->GetRootPane()->FindPaneByName("T_menu_msg_01"));
    mDialogMessageTextBox = static_cast<nw4r::lyt::TextBox *>(getLayout()->GetRootPane()->FindPaneByName("T_msg_01"));
    mCurrentPageTextBox = static_cast<nw4r::lyt::TextBox *>(getLayout()->GetRootPane()->FindPaneByName("T_page_00"));
    mTotalPagesTextBox = static_cast<nw4r::lyt::TextBox *>(getLayout()->GetRootPane()->FindPaneByName("T_page_02"));

    static_cast<nw4r::lyt::TextBox *>(getLayout()->GetRootPane()->FindPaneByName("T_page_01"))->SetString(L"/");

    for (u8 i = 0; i < ARRAY_LENGTH(mFileButtons); i++) {
        getAnimation(anim_fileSelect_focus_fileButton + i)->fn_801D9E70(getLayout(), mFileButtons[i].getPane(), sAnimNamePtr_fileSelect_focus, resAccessor);
        getAnimation(anim_fileSelect_action_fileButton + i)->fn_801D9E70(getLayout(), mFileButtons[i].getPane(), sAnimNamePtr_fileSelect_action, resAccessor);

        getAnimation(anim_fileSelect_file_btn_in + i)->fn_801D9E70(getLayout(), mFileButtons[i].getPane(), sAnimNamePtr_fileSelect_file_btn_in, resAccessor);
        getAnimation(anim_fileSelect_file_btn_out + i)->fn_801D9E70(getLayout(), mFileButtons[i].getPane(), sAnimNamePtr_fileSelect_file_btn_out, resAccessor);
        getAnimation(anim_fileSelect_window_in + i)->fn_801D9E70(getLayout(), mFileButtons[i].getPane(), sAnimNamePtr_fileSelect_window_in, resAccessor);
        getAnimation(anim_fileSelect_window_out + i)->fn_801D9E70(getLayout(), mFileButtons[i].getPane(), sAnimNamePtr_fileSelect_window_out, resAccessor);

        mFileButtons[i].setFocusAnime(getAnimation(anim_fileSelect_focus_fileButton + i));
        mFileButtons[i].setActionAnime(getAnimation(anim_fileSelect_action_fileButton + i));
    }

    for (u8 i = 0; i < ARRAY_LENGTH(mFileActionButtons); i++) {
        getAnimation(anim_fileSelect_focus_fileActionButton + i)->fn_801D9E70(getLayout(), mFileActionButtons[i].getPane(), sAnimNamePtr_fileSelect_focus, resAccessor);
        getAnimation(anim_fileSelect_action_fileActionButton + i)->fn_801D9E70(getLayout(), mFileActionButtons[i].getPane(), sAnimNamePtr_fileSelect_action, resAccessor);

        mFileActionButtons[i].setFocusAnime(getAnimation(anim_fileSelect_focus_fileActionButton + i));
        mFileActionButtons[i].setActionAnime(getAnimation(anim_fileSelect_action_fileActionButton + i));
    }

    for (u8 i = 0; i < ARRAY_LENGTH(mNameButtons); i++) {
        s32 focusAnimationIndex = anim_fileSelect_focus_nameButton + i;
        s32 actionAnimationIndex = anim_fileSelect_action_nameButton + i;
        bool recurseBind = i == 15 || i == 16;

        getAnimation(focusAnimationIndex)->fn_801D9E70(getLayout(), mNameButtons[i].getPane(), sAnimNamePtr_fileSelect_focus, resAccessor, recurseBind);
        getAnimation(actionAnimationIndex)->fn_801D9E70(getLayout(), mNameButtons[i].getPane(), sAnimNamePtr_fileSelect_action, resAccessor, recurseBind);

        mNameButtons[i].setFocusAnime(getAnimation(focusAnimationIndex));
        mNameButtons[i].setActionAnime(getAnimation(actionAnimationIndex));

        if (i < ARRAY_LENGTH(mNameButtons) - 2) {
            nw4r::lyt::Pane *nameFramePane = getLayout()->GetRootPane()->FindPaneByName(sNameFramePaneNames[i]);
            nw4r::lyt::Pane *namePane = getLayout()->GetRootPane()->FindPaneByName(sNamePaneNames[i]);

            getAnimation(anim_fileSelect_name_activate_nameFramePane + i)->fn_801D9E70(getLayout(), nameFramePane, sAnimNamePtr_fileSelect_name_activate, resAccessor);
            getAnimation(anim_fileSelect_name_activate_namePane + i)->fn_801D9E70(getLayout(), namePane, sAnimNamePtr_fileSelect_name_activate, resAccessor);
            getAnimation(anim_fileSelect_name_deactivate_nameFramePane + i)->fn_801D9E70(getLayout(), nameFramePane, sAnimNamePtr_fileSelect_name_deactivate, resAccessor);
            getAnimation(anim_fileSelect_name_deactivate_namePane + i)->fn_801D9E70(getLayout(), namePane, sAnimNamePtr_fileSelect_name_deactivate, resAccessor);

            getAnimation(anim_fileSelect_name_activate_nameFramePane + i)->setLoop(true);
            getAnimation(anim_fileSelect_name_activate_namePane + i)->setLoop(true);
            getAnimation(anim_fileSelect_name_deactivate_nameFramePane + i)->setLoop(true);
            getAnimation(anim_fileSelect_name_deactivate_namePane + i)->setLoop(true);
        }
    }

    for (u8 i = 0; i < ARRAY_LENGTH(mMiiButtons); i++) {
        getAnimation(anim_fileSelect_focus_miiButton + i)->fn_801D9E70(getLayout(), mMiiButtons[i].getPane(), sAnimNamePtr_fileSelect_focus, resAccessor);
        getAnimation(anim_fileSelect_action_miiButton + i)->fn_801D9E70(getLayout(), mMiiButtons[i].getPane(), sAnimNamePtr_fileSelect_action, resAccessor);

        mMiiButtons[i].setFocusAnime(getAnimation(anim_fileSelect_focus_miiButton + i));
        mMiiButtons[i].setActionAnime(getAnimation(anim_fileSelect_action_miiButton + i));

        if (i < ARRAY_LENGTH(mMiiButtons) - 3) {
            nw4r::lyt::Pane *iconBackgroundPane = getLayout()->GetRootPane()->FindPaneByName(sIconBackgroundPaneNames[i]);
            nw4r::lyt::Pane *iconMiiPane = getLayout()->GetRootPane()->FindPaneByName(sIconMiiPaneNames[i]);

            getAnimation(anim_fileSelect_name_activate_iconBackgroundPane + i)->fn_801D9E70(getLayout(), iconBackgroundPane, sAnimNamePtr_fileSelect_name_activate, resAccessor);
            getAnimation(anim_fileSelect_name_activate_iconMiiPane + i)->fn_801D9E70(getLayout(), iconMiiPane, sAnimNamePtr_fileSelect_name_activate, resAccessor);
            getAnimation(anim_fileSelect_name_deactivate_iconBackgroundPane + i)->fn_801D9E70(getLayout(), iconBackgroundPane, sAnimNamePtr_fileSelect_name_deactivate, resAccessor);
            getAnimation(anim_fileSelect_name_deactivate_iconMiiPane + i)->fn_801D9E70(getLayout(), iconMiiPane, sAnimNamePtr_fileSelect_name_deactivate, resAccessor);

            getAnimation(anim_fileSelect_name_activate_iconBackgroundPane + i)->setLoop(true);
            getAnimation(anim_fileSelect_name_activate_iconMiiPane + i)->setLoop(true);
            getAnimation(anim_fileSelect_name_deactivate_iconBackgroundPane + i)->setLoop(true);
            getAnimation(anim_fileSelect_name_deactivate_iconMiiPane + i)->setLoop(true);
        }
    }

    getAnimation(anim_fileSelect_focus_deleteBackBtn)->fn_801D9E70(getLayout(), mDeleteBackButton.getPane(), sAnimNamePtr_fileSelect_focus, resAccessor);
    getAnimation(anim_fileSelect_action_deleteBackBtn)->fn_801D9E70(getLayout(), mDeleteBackButton.getPane(), sAnimNamePtr_fileSelect_action, resAccessor);
    mDeleteBackButton.setFocusAnime(getAnimation(anim_fileSelect_focus_deleteBackBtn));
    mDeleteBackButton.setActionAnime(getAnimation(anim_fileSelect_action_deleteBackBtn));

    getAnimation(anim_fileSelect_focus_confirmBtn)->fn_801D9E70(getLayout(), mConfirmButton.getPane(), sAnimNamePtr_fileSelect_focus, resAccessor);
    getAnimation(anim_fileSelect_focus_cancelBtn)->fn_801D9E70(getLayout(), mCancelButton.getPane(), sAnimNamePtr_fileSelect_focus, resAccessor);
    getAnimation(anim_fileSelect_action_confirmBtn)->fn_801D9E70(getLayout(), mConfirmButton.getPane(), sAnimNamePtr_fileSelect_action, resAccessor);
    getAnimation(anim_fileSelect_action_cancelBtn)->fn_801D9E70(getLayout(), mCancelButton.getPane(), sAnimNamePtr_fileSelect_action, resAccessor);
    mConfirmButton.setFocusAnime(getAnimation(anim_fileSelect_focus_confirmBtn));
    mCancelButton.setFocusAnime(getAnimation(anim_fileSelect_focus_cancelBtn));
    mConfirmButton.setActionAnime(getAnimation(anim_fileSelect_action_confirmBtn));
    mCancelButton.setActionAnime(getAnimation(anim_fileSelect_action_cancelBtn));

    getAnimation(anim_fileSelect_focus_messageOkBtn)->fn_801D9E70(getLayout(), mMessageOkButton.getPane(), sAnimNamePtr_fileSelect_focus, resAccessor);
    getAnimation(anim_fileSelect_action_messageOkBtn)->fn_801D9E70(getLayout(), mMessageOkButton.getPane(), sAnimNamePtr_fileSelect_action, resAccessor);
    mMessageOkButton.setFocusAnime(getAnimation(anim_fileSelect_focus_messageOkBtn));
    mMessageOkButton.setActionAnime(getAnimation(anim_fileSelect_action_messageOkBtn));

    nw4r::lyt::Pane *pane = getLayout()->GetRootPane()->FindPaneByName("N_cursor_frm_00");
    getAnimation(anim_fileSelect_cursor_frm_on_N_cursor_frm_00)->fn_801D9E70(getLayout(), pane, sAnimNamePtr_fileSelect_cursor_frm_on, resAccessor);
    getAnimation(anim_fileSelect_cursor_frm_off_N_cursor_frm_00)->fn_801D9E70(getLayout(), pane, sAnimNamePtr_fileSelect_cursor_frm_off, resAccessor);

    pane = getLayout()->GetRootPane()->FindPaneByName("N_bg_all_00");
    getAnimation(anim_fileSelect_bg_loop_N_bg_all_00)->fn_801D9E70(getLayout(), pane, sAnimNamePtr_fileSelect_bg_loop, resAccessor);

    pane = getLayout()->GetRootPane()->FindPaneByName("W_menu_00");
    getAnimation(anim_fileSelect_window_in_W_menu_00)->fn_801D9E70(getLayout(), pane, sAnimNamePtr_fileSelect_window_in, resAccessor, false);
    getAnimation(anim_fileSelect_window_out_W_menu_00)->fn_801D9E70(getLayout(), pane, sAnimNamePtr_fileSelect_window_out, resAccessor, false);

    pane = getLayout()->GetRootPane()->FindPaneByName("W_menu_01");
    getAnimation(anim_fileSelect_window_in_W_menu_01)->fn_801D9E70(getLayout(), pane, sAnimNamePtr_fileSelect_window_in, resAccessor, false);
    getAnimation(anim_fileSelect_window_out_W_menu_01)->fn_801D9E70(getLayout(), pane, sAnimNamePtr_fileSelect_window_out, resAccessor, false);

    pane = getLayout()->GetRootPane()->FindPaneByName("W_menu_mii_00");
    getAnimation(anim_fileSelect_window_in_W_menu_mii_00)->fn_801D9E70(getLayout(), pane, sAnimNamePtr_fileSelect_window_in, resAccessor, false);
    getAnimation(anim_fileSelect_window_out_W_menu_mii_00)->fn_801D9E70(getLayout(), pane, sAnimNamePtr_fileSelect_window_out, resAccessor, false);

    pane = getLayout()->GetRootPane()->FindPaneByName("W_msg_00");
    getAnimation(anim_fileSelect_window_in_W_msg_00)->fn_801D9E70(getLayout(), pane, sAnimNamePtr_fileSelect_window_in, resAccessor, false);
    getAnimation(anim_fileSelect_window_out_W_msg_00)->fn_801D9E70(getLayout(), pane, sAnimNamePtr_fileSelect_window_out, resAccessor, false);

    pane = getLayout()->GetRootPane()->FindPaneByName("W_msg_02");
    getAnimation(anim_fileSelect_window_in_W_msg_02)->fn_801D9E70(getLayout(), pane, sAnimNamePtr_fileSelect_window_in, resAccessor, false);
    getAnimation(anim_fileSelect_window_out_W_msg_02)->fn_801D9E70(getLayout(), pane, sAnimNamePtr_fileSelect_window_out, resAccessor, false);

    pane = getLayout()->GetRootPane()->FindPaneByName("W_msg_01");
    getAnimation(anim_fileSelect_window_in_W_msg_01)->fn_801D9E70(getLayout(), pane, sAnimNamePtr_fileSelect_window_in, resAccessor, false);
    getAnimation(anim_fileSelect_window_out_W_msg_01)->fn_801D9E70(getLayout(), pane, sAnimNamePtr_fileSelect_window_out, resAccessor, false);

    pane = getLayout()->GetRootPane()->FindPaneByName("N_top_frm_00");
    getAnimation(anim_fileSelect_top_frm_in_W_msg_01)->fn_801D9E70(getLayout(), pane, sAnimNamePtr_fileSelect_top_frm_in, resAccessor, false);
    getAnimation(anim_fileSelect_top_frm_out_W_msg_01)->fn_801D9E70(getLayout(), pane, sAnimNamePtr_fileSelect_top_frm_out, resAccessor, false);

    mCursorFramePane = getLayout()->GetRootPane()->FindPaneByName("N_cursor_frm_00");
    mCursorWindowPane = getLayout()->GetRootPane()->FindPaneByName("W_cursor_frm_00");

    mFileSelectManager = new CExBtnPaneManager(ARRAY_LENGTH(mFileButtons));
    mFileSelectManager->fn_8008F688(&lbl_802ED858);
    mFileSelectManager->fn_8008F96C(mCursorFramePane, mCursorWindowPane, getAnimation(anim_fileSelect_cursor_frm_on_N_cursor_frm_00), getAnimation(anim_fileSelect_cursor_frm_off_N_cursor_frm_00), 16.0f);
    for (u8 i = 0; i < ARRAY_LENGTH(mFileButtons); i++)
        mFileSelectManager->fn_8008F65C(i, &mFileButtons[i]);

    mActionMenuManager = new CExBtnPaneManager(ARRAY_LENGTH(mFileActionButtons));
    mActionMenuManager->fn_8008F688(&lbl_802ED86C);
    mActionMenuManager->fn_8008F96C(mCursorFramePane, mCursorWindowPane, getAnimation(anim_fileSelect_cursor_frm_on_N_cursor_frm_00), getAnimation(anim_fileSelect_cursor_frm_off_N_cursor_frm_00), 16.0f);
    for (u8 i = 0; i < ARRAY_LENGTH(mFileActionButtons); i++)
        mActionMenuManager->fn_8008F65C(i, &mFileActionButtons[i]);
    mActionMenuManager->fn_8008F814(fileSelectManagerCallback);

    mIconSelectManager = new CExBtnPaneManager(ARRAY_LENGTH(mNameButtons));
    mIconSelectManager->fn_8008F688(&lbl_802ED8D0);
    mIconSelectManager->fn_8008F96C(mCursorFramePane, mCursorWindowPane, getAnimation(anim_fileSelect_cursor_frm_on_N_cursor_frm_00), getAnimation(anim_fileSelect_cursor_frm_off_N_cursor_frm_00), 16.0f);
    mIconSelectManager->unk44 = 256;
    for (u8 i = 0; i < ARRAY_LENGTH(mNameButtons); i++)
        mIconSelectManager->fn_8008F65C(i, &mNameButtons[i]);
    mIconSelectManager->fn_8008F814(iconSelectManagerCallback);

    mMiiSelectManager = new CExBtnPaneManager(ARRAY_LENGTH(mMiiButtons));
    mMiiSelectManager->fn_8008F688(&lbl_802ED924);
    mMiiSelectManager->fn_8008F96C(mCursorFramePane, mCursorWindowPane, getAnimation(anim_fileSelect_cursor_frm_on_N_cursor_frm_00), getAnimation(anim_fileSelect_cursor_frm_off_N_cursor_frm_00), 16.0f);
    mMiiSelectManager->unk44 = 256;
    for (u8 i = 0; i < ARRAY_LENGTH(mMiiButtons); i++)
        mMiiSelectManager->fn_8008F65C(i, &mMiiButtons[i]);
    mMiiSelectManager->fn_8008F814(miiSelectManagerCallback);

    mDeleteDialogManager = new CExBtnPaneManager(1);
    mDeleteDialogManager->fn_8008F688(&lbl_802ED5F0);
    mDeleteDialogManager->fn_8008F96C(mCursorFramePane, mCursorWindowPane, getAnimation(anim_fileSelect_cursor_frm_on_N_cursor_frm_00), getAnimation(anim_fileSelect_cursor_frm_off_N_cursor_frm_00), 16.0f);
    mDeleteDialogManager->fn_8008F65C(0, &mDeleteBackButton);

    mConfirmDialogManager = new CExBtnPaneManager(2);
    mConfirmDialogManager->fn_8008F688(&lbl_802ED604);
    mConfirmDialogManager->fn_8008F96C(mCursorFramePane, mCursorWindowPane, getAnimation(anim_fileSelect_cursor_frm_on_N_cursor_frm_00), getAnimation(anim_fileSelect_cursor_frm_off_N_cursor_frm_00), 16.0f);
    mConfirmDialogManager->fn_8008F65C(0, &mConfirmButton);
    mConfirmDialogManager->fn_8008F65C(1, &mCancelButton);

    mMessageDialogManager = new CExBtnPaneManager(1);
    mMessageDialogManager->fn_8008F688(&lbl_802ED5F0);
    mMessageDialogManager->fn_8008F96C(mCursorFramePane, mCursorWindowPane, getAnimation(anim_fileSelect_cursor_frm_on_N_cursor_frm_00), getAnimation(anim_fileSelect_cursor_frm_off_N_cursor_frm_00), 16.0f);
    mMessageDialogManager->fn_8008F65C(0, &mMessageOkButton);

    updateSelectionSfx();

    mDefaultMiiTexture = getLayout()->GetRootPane()->FindPaneByName("P_icon_mii_00")->GetMaterial()->GetTexMap(0);

    this->CLayout::_10();

    for (s32 saveIndex = 0; saveIndex < SAVE_COUNT; saveIndex++)
        updateSaveDisplay(saveIndex);

    getAnimation(anim_fileSelect_bg_loop_N_bg_all_00)->setLoop(true);
    getAnimation(anim_fileSelect_bg_loop_N_bg_all_00)->playFromBeginForward();

    mSelectedSaveIndex = -1;
    mMiiPage = 0;
    mUnk24 = 5;

    getAnimation(anim_fileSelect_top_frm_in_W_msg_01)->playFromBeginForward();

    for (u8 saveIndex = 0; saveIndex < SAVE_COUNT; saveIndex++) {
        if (saveIndex != mSelectedSaveIndex) {
            getAnimation(anim_fileSelect_file_btn_in + saveIndex)->playFromBeginForward();
        }
    }

    mState = TITLE_STATE_FILE_SELECT_IN;
    updateState(TITLE_STATE_FILE_SELECT_IN);

    mFileSelectManager->fn_8008F80C(miscManagerCursorCallback);
    mActionMenuManager->fn_8008F80C(miscManagerCursorCallback);
    mIconSelectManager->fn_8008F80C(miscManagerCursorCallback);
    mMiiSelectManager->fn_8008F80C(miscManagerCursorCallback);
    mDeleteDialogManager->fn_8008F80C(miscManagerCursorCallback);
    mConfirmDialogManager->fn_8008F80C(miscManagerCursorCallback);
    mMessageDialogManager->fn_8008F80C(miscManagerCursorCallback);
}

const char *sFileButtonLabelNames[4] = {"N_file_btn_00", "N_file_btn_01", "N_file_btn_02", "N_file_btn_03"};
const char *sFileActionButtonLabelNames[4] = {"N_start_btn_00", "N_back_btn_00", "N_delete_btn_00", "N_change_btn_00"};
const char *sNameButtonLabelNames[17] = {"N_name_00", "N_name_01", "N_name_02", "N_name_03", "N_name_04", "N_name_05", "N_name_06", "N_name_07", "N_name_08", "N_name_09", "N_name_10", "N_name_11", "N_name_12", "N_name_13", "N_name_14", "N_back_btn_02", "N_mii_btn_00"};
const char *sMiiButtonLabelNames[13] = {"N_mii_00", "N_mii_01", "N_mii_02", "N_mii_03", "N_mii_04", "N_mii_05", "N_mii_06", "N_mii_07", "N_mii_08", "N_mii_09", "N_prev_btn_00", "N_next_btn_00", "N_back_btn_03"};
const char *sNameFramePaneNames[15] = {"P_icon_frm_00", "P_icon_frm_01", "P_icon_frm_02", "P_icon_frm_03", "P_icon_frm_04", "P_icon_frm_05", "P_icon_frm_06", "P_icon_frm_07", "P_icon_frm_08", "P_icon_frm_09", "P_icon_frm_10", "P_icon_frm_11", "P_icon_frm_12", "P_icon_frm_13", "P_icon_frm_14"};
const char *sNamePaneNames[15] = {"P_icon_name_00", "P_icon_name_01", "P_icon_name_02", "P_icon_name_03", "P_icon_name_04", "P_icon_name_05", "P_icon_name_06", "P_icon_name_07", "P_icon_name_08", "P_icon_name_09", "P_icon_name_10", "P_icon_name_11", "P_icon_name_12", "P_icon_name_13", "P_icon_name_14"};
const char *sIconBackgroundPaneNames[10] = {"P_iconBG_00", "P_iconBG_01", "P_iconBG_02", "P_iconBG_03", "P_iconBG_04", "P_iconBG_05", "P_iconBG_06", "P_iconBG_07", "P_iconBG_08", "P_iconBG_09"};
const char *sIconMiiPaneNames[10] = {"P_icon_mii_00", "P_icon_mii_01", "P_icon_mii_02", "P_icon_mii_03", "P_icon_mii_04", "P_icon_mii_05", "P_icon_mii_06", "P_icon_mii_07", "P_icon_mii_08", "P_icon_mii_09"};

void Title::CMyLayoutF::_18(nw4r::lyt::DrawInfo *drawInfo) {
    switch (mState) {
        case TITLE_STATE_FILE_SELECT_IN: {
            bool finished = true;
            for (u8 saveIndex = 0; saveIndex < SAVE_COUNT; saveIndex++)
                if (getAnimation(anim_fileSelect_file_btn_in + saveIndex)->getIsPlaying())
                    finished = false;

            if (finished)
                updateState(TITLE_STATE_FILE_SELECT);

            break;
        }
        case TITLE_STATE_FILE_SELECT: {
            CController *controller = gControllerManager->fn_801D5FF0(0);

            nw4r::math::VEC2 vec(
                controller->fn_801D523C(this).x,
                controller->fn_801D523C(this).y
            );

            if (mUnk9B8 || !controller->fn_801D52D4()) {
                vec.x = lbl_80320FA0.x;
                vec.y = lbl_80320FA0.y;
            }

            nw4r::math::VEC2 buttonVec = vec;
            nw4r::math::VEC2 buttonVecCopy = buttonVec;
            mFileSelectManager->_0C(&buttonVecCopy, true, drawInfo);

            for (u8 saveIndex = 0; saveIndex < SAVE_COUNT; saveIndex++) {
                bool selected = mFileButtons[saveIndex].getEnabled() && mFileButtons[saveIndex].getUnk26();
                if (selected)
                    mUnk9B8 = true;

                bool action = mFileButtons[saveIndex].getEnabled() && mFileButtons[saveIndex].getUnk27();
                if (action) {
                    mSelectedSaveIndex = saveIndex;
                    if (gSaveData->getSaveFile(saveIndex)->isOccupied()) {
                        getAnimation(anim_fileSelect_top_frm_out_W_msg_01)->playFromBeginForward();

                        for (u8 otherSaveIndex = 0; otherSaveIndex < SAVE_COUNT; otherSaveIndex++)
                            if (otherSaveIndex != mSelectedSaveIndex)
                                getAnimation(anim_fileSelect_file_btn_out + otherSaveIndex)->playFromBeginForward();

                        updateState(TITLE_STATE_FILE_SELECT_OUT);
                    } else {
                        getAnimation(anim_fileSelect_window_in_W_menu_01)->playFromBeginForward();

                        setupIconSelection();
                        updateState(TITLE_STATE_ICON_SELECT_IN);
                    }

                    break;
                }
            }

            break;
        }
        case TITLE_STATE_FILE_SELECT_OUT:
        case TITLE_STATE_FILE_SELECT_RETURN: {
            bool finished = true;
            for (u8 saveIndex = 0; saveIndex < SAVE_COUNT; saveIndex++)
                if (getAnimation(anim_fileSelect_file_btn_in + saveIndex)->getIsPlaying() || getAnimation(anim_fileSelect_file_btn_out + saveIndex)->getIsPlaying())
                    finished = false;

            if (finished) {
                if (mState == TITLE_STATE_FILE_SELECT_OUT) {
                    getAnimation(anim_fileSelect_window_in_W_menu_00)->playFromBeginForward();
                    getAnimation(anim_fileSelect_window_in + mSelectedSaveIndex)->playFromBeginForward();

                    updateState(TITLE_STATE_ACTION_MENU_IN);
                } else if (mState == TITLE_STATE_FILE_SELECT_RETURN) {
                    updateState(TITLE_STATE_FILE_SELECT);
                }
            }

            break;
        }
        case TITLE_STATE_ACTION_MENU_IN:
        case TITLE_STATE_ACTION_MENU_OUT: {
            bool actionFinished = false;
            bool fileFinished = false;
            bool menuFinished = !getAnimation(anim_fileSelect_window_in_W_menu_00)->getIsPlaying() && !getAnimation(anim_fileSelect_window_out_W_menu_00)->getIsPlaying();
            if (menuFinished && !getAnimation(anim_fileSelect_window_in + mSelectedSaveIndex)->getIsPlaying())
                fileFinished = true;
            if (fileFinished && !getAnimation(anim_fileSelect_window_out + mSelectedSaveIndex)->getIsPlaying())
                actionFinished = true;

            if (actionFinished) {
                if (mState == TITLE_STATE_ACTION_MENU_IN) {
                    s32 saveIndex = mSelectedSaveIndex;
                    bool isProfileMiiMissing = false;
                    bool usesProfileMii = gSaveData->getSaveFile(saveIndex)->isOccupied() && gSaveData->getSaveFile(saveIndex)->isUsingProfileMii();

                    if (usesProfileMii) {
                        if (gRFLManager->fn_800C2FE0(gSaveData->getSaveFile(saveIndex)->getProfileMiiID()) == NULL)
                            isProfileMiiMissing = true;
                    }

                    if (isProfileMiiMissing) {
                        mUnk24 = 3;

                        /*
                        This Mii character has
                        been deleted from the
                        Mii Plaza. Please
                        choose a different Mii.
                        */
                        gMessageManager->setupTextBox(mDialogMessageTextBox, "0104103", 0);

                        getAnimation(anim_fileSelect_window_in_W_msg_01)->playFromBeginForward();

                        updateState(TITLE_STATE_MESSAGE_DIALOG_IN);
                    } else {
                        mActionMenuManager->fn_8008F71C(0, 1, 0);
                        lbl_802ED86C.buttonIndices3[0] = 1;

                        updateState(TITLE_STATE_ACTION_MENU);
                    }
                } else if (mState == TITLE_STATE_ACTION_MENU_OUT) {
                    getAnimation(anim_fileSelect_top_frm_in_W_msg_01)->playFromBeginForward();

                    for (u8 saveIndex = 0; saveIndex < SAVE_COUNT; saveIndex++) {
                        if (saveIndex != mSelectedSaveIndex) {
                            getAnimation(anim_fileSelect_file_btn_in + saveIndex)->playFromBeginForward();
                        }
                    }

                    updateState(TITLE_STATE_FILE_SELECT_RETURN);
                }
            }

            break;
        }
        case TITLE_STATE_ACTION_MENU: {
            CController *controller = gControllerManager->fn_801D5FF0(0);

            nw4r::math::VEC2 vec(
                controller->fn_801D523C(this).x,
                controller->fn_801D523C(this).y
            );

            if (mUnk9B8 || !controller->fn_801D52D4()) {
                vec.x = lbl_80320FA0.x;
                vec.y = lbl_80320FA0.y;
            }

            nw4r::math::VEC2 buttonVec = vec;
            nw4r::math::VEC2 buttonVecCopy = buttonVec;
            mActionMenuManager->_0C(&buttonVecCopy, true, drawInfo);

            for (u8 actionIndex = 0; actionIndex < ARRAY_LENGTH(mFileActionButtons); actionIndex++) {
                bool selected = mFileActionButtons[actionIndex].getEnabled() && mFileActionButtons[actionIndex].getUnk26();
                if (selected)
                    mUnk9B8 = true;

                bool doAction = mFileActionButtons[actionIndex].getEnabled() && mFileActionButtons[actionIndex].getUnk27();
                if (doAction) {
                    if (actionIndex == TITLE_FILE_ACTION_BUTTON_START) {
                        gPlayHistory->fn_8009A710();
                        gCafeTopic->fn_8009CE34();
                        gSaveData->setSelectedSaveIndex(mSelectedSaveIndex);

                        CExScene::fn_8000A74C(30);
                        CExScene::fn_8000A790(30);

                        updateState(TITLE_STATE_START_GAME);
                    } else if (actionIndex == TITLE_FILE_ACTION_BUTTON_BACK) {
                        getAnimation(anim_fileSelect_window_out_W_menu_00)->playFromBeginForward();
                        getAnimation(anim_fileSelect_window_out + mSelectedSaveIndex)->playFromBeginForward();
                        updateState(TITLE_STATE_ACTION_MENU_OUT);
                    } else if (actionIndex == TITLE_FILE_ACTION_BUTTON_DELETE) {
                        getAnimation(anim_fileSelect_window_in_W_msg_00)->playFromBeginForward();
                        updateState(TITLE_STATE_DELETE_DIALOG_IN);
                    } else if (actionIndex == TITLE_FILE_ACTION_BUTTON_CHANGE) {
                        getAnimation(anim_fileSelect_window_in_W_menu_01)->playFromBeginForward();
                        setupIconSelection();
                        updateState(TITLE_STATE_ICON_SELECT_IN);
                    }

                    break;
                }

            }

            break;
        }
        case TITLE_STATE_ICON_SELECT_IN:
        case TITLE_STATE_ICON_SELECT_OUT: {
            bool finished = false;
            if (!getAnimation(anim_fileSelect_window_in_W_menu_01)->getIsPlaying() && !getAnimation(anim_fileSelect_window_out_W_menu_01)->getIsPlaying())
                finished = true;

            if (finished) {
                if (mState == TITLE_STATE_ICON_SELECT_IN) {
                    updateState(TITLE_STATE_ICON_SELECT);
                } else if (mState == TITLE_STATE_ICON_SELECT_OUT) {
                    s32 saveIndex = mSelectedSaveIndex;

                    bool isProfileMiiMissing = false;
                    bool usesProfileMii = gSaveData->getSaveFile(saveIndex)->isOccupied() && gSaveData->getSaveFile(saveIndex)->isUsingProfileMii();

                    if (usesProfileMii)
                        if (gRFLManager->fn_800C2FE0(gSaveData->getSaveFile(saveIndex)->getProfileMiiID()) == NULL)
                            isProfileMiiMissing = true;

                    if (isProfileMiiMissing) {
                        getAnimation(anim_fileSelect_window_out_W_menu_00)->playFromBeginForward();
                        getAnimation(anim_fileSelect_window_out + mSelectedSaveIndex)->playFromBeginForward();
                        updateState(TITLE_STATE_ACTION_MENU_OUT);
                    } else {
                        if (gSaveData->getSaveFile(mSelectedSaveIndex)->isOccupied())
                            updateState(TITLE_STATE_ACTION_MENU);
                        else
                            updateState(TITLE_STATE_FILE_SELECT);
                    }
                }
            }

            break;
        }
        case TITLE_STATE_ICON_SELECT: {
            CController *controller = gControllerManager->fn_801D5FF0(0);

            nw4r::math::VEC2 vec(
                controller->fn_801D523C(this).x,
                controller->fn_801D523C(this).y
            );

            if (mUnk9B8 || !controller->fn_801D52D4()) {
                vec.x = lbl_80320FA0.x;
                vec.y = lbl_80320FA0.y;
            }

            nw4r::math::VEC2 buttonVec = vec;
            nw4r::math::VEC2 buttonVecCopy = buttonVec;
            mIconSelectManager->_0C(&buttonVecCopy, true, drawInfo);

            for (u8 buttonIndex = 0; buttonIndex < ARRAY_LENGTH(mNameButtons); buttonIndex++) {
                bool selected = mNameButtons[buttonIndex].getEnabled() && mNameButtons[buttonIndex].getUnk26();
                if (selected) {
                    if (buttonIndex == TITLE_NAME_BUTTON_MII) {
                        if (gRFLManager->fn_800C2FBC() != RFLErrcode_Success)  {
                            mUnk24 = 2;

                            /*
                            Unable to read Mii Channel save
                            data. Please go to Mii Channel
                            and check your data.
                            */
                            gMessageManager->setupTextBox(mDialogMessageTextBox, "0104102", 0);

                            mUnk9B8 = true;
                        } else {
                            if (gRFLManager->fn_800C2FC4() <= 0) {
                                mUnk24 = 1;

                                /*
                                Unable to find any Mii
                                characters. Please go to
                                the Mii Channel to create
                                a new Mii.
                                */
                                gMessageManager->setupTextBox(mDialogMessageTextBox, "0104101", 0);
                            }
                            mUnk9B8 = true;
                        }
                    } else {
                        mUnk9B8 = true;
                    }
                }

                bool doAction = mNameButtons[buttonIndex].getEnabled() && mNameButtons[buttonIndex].getUnk27();
                if (doAction) {
                    if (buttonIndex < TITLE_NAME_BUTTON_BACK) {
                        if (static_cast<u32>(mNameButtons[buttonIndex].getActionSfxID()) == SE_ERROR) {
                            mUnk9B8 = false;
                            continue;
                        }

                        if (gSaveData->getSaveFile(mSelectedSaveIndex)->isOccupied()) {
                            s32 saveIndex = mSelectedSaveIndex;
                            gSaveData->getSaveFile(saveIndex)->setProfileIconIndex(buttonIndex);
                            gSaveData->updateSnapshot();

                            if (gBackupManager != NULL)
                                gBackupManager->fn_80084FC8(1);

                            updateSaveDisplay(saveIndex);
                            getAnimation(anim_fileSelect_window_out_W_menu_01)->playFromBeginForward();
                            updateState(TITLE_STATE_ICON_SELECT_OUT);

                            continue;
                        }

                        RFLCreateID createID = {};
                        s32 saveIndex = mSelectedSaveIndex;
                        lbl_803204B0[saveIndex] = false;
                        lbl_8034C820[saveIndex] = createID;
                        lbl_803204B4[saveIndex] = buttonIndex;

                        nw4r::lyt::TexMap texture;
                        nw4r::lyt::Material *material = getLayout()->GetRootPane()->FindPaneByName(sIconNamePaneNames[buttonIndex])->GetMaterial();
                        texture = material->GetTexMap(0);

                        nw4r::lyt::Pane *pane = getLayout()->GetRootPane()->FindPaneByName("P_player_icon_00");
                        pane->GetMaterial()->SetTexMap(0, texture);

                        fn_800C5F70(pane, saveIndex, false);
                        getAnimation(anim_fileSelect_window_in_W_msg_02)->playFromBeginForward();
                        mPrevState = mState;

                        updateState(TITLE_STATE_CONFIRM_DIALOG_IN);
                    } else if (buttonIndex == TITLE_NAME_BUTTON_BACK) {
                        getAnimation(anim_fileSelect_window_out_W_menu_01)->playFromBeginForward();
                        updateState(TITLE_STATE_ICON_SELECT_OUT);
                    } else if (buttonIndex == TITLE_NAME_BUTTON_MII) {
                        if (gRFLManager->fn_800C2FBC() == RFLErrcode_Success && gRFLManager->fn_800C2FC4() > 0) {
                            mMiiPage = 0;
                            updateMiiPage(0);

                            getAnimation(anim_fileSelect_window_in_W_menu_mii_00)->playFromBeginForward();
                            updateState(TITLE_STATE_MII_SELECT_IN);
                        } else {
                            getAnimation(anim_fileSelect_window_in_W_msg_01)->playFromBeginForward();
                            updateState(TITLE_STATE_MESSAGE_DIALOG_IN);
                        }
                    }
                }
            }

            break;
        }
        case TITLE_STATE_MII_SELECT_IN:
        case TITLE_STATE_MII_SELECT_OUT: {
            bool finished = false;
            if (!getAnimation(anim_fileSelect_window_in_W_menu_mii_00)->getIsPlaying() && !getAnimation(anim_fileSelect_window_out_W_menu_mii_00)->getIsPlaying())
                finished = true;

            if (finished) {
                if (mState == TITLE_STATE_MII_SELECT_IN)
                    updateState(TITLE_STATE_MII_SELECT);
                else if (mState == TITLE_STATE_MII_SELECT_OUT)
                    updateState(TITLE_STATE_ICON_SELECT);
            }

            break;
        }
        case TITLE_STATE_MII_SELECT: {
            CController *controller = gControllerManager->fn_801D5FF0(0);

            nw4r::math::VEC2 vec(
                controller->fn_801D523C(this).x,
                controller->fn_801D523C(this).y
            );

            if (mUnk9B8 || !controller->fn_801D52D4()) {
                vec.x = lbl_80320FA0.x;
                vec.y = lbl_80320FA0.y;
            }

            nw4r::math::VEC2 buttonVec = vec;
            nw4r::math::VEC2 buttonVecCopy = buttonVec;
            mMiiSelectManager->_0C(&buttonVecCopy, true, drawInfo);

            for (u8 buttonIndex = 0; buttonIndex < ARRAY_LENGTH(mMiiButtons); buttonIndex++) {
                bool selected = mMiiButtons[buttonIndex].getEnabled() && mMiiButtons[buttonIndex].getUnk26();
                if (selected)
                    mUnk9B8 = true;

                bool action = mMiiButtons[buttonIndex].getEnabled() && mMiiButtons[buttonIndex].getUnk27();
                if (action) {
                    mUnk9B8 = false;

                    if (buttonIndex == TITLE_MII_BUTTON_PAGE_LEFT) {
                        updateMiiPage(--mMiiPage);
                    } else if (buttonIndex == TITLE_MII_BUTTON_PAGE_RIGHT) {
                        updateMiiPage(++mMiiPage);
                    } else if (buttonIndex == TITLE_MII_BUTTON_BACK) {
                        getAnimation(anim_fileSelect_window_out_W_menu_mii_00)->playFromBeginForward();
                        updateState(TITLE_STATE_MII_SELECT_OUT);
                    } else if (static_cast<u32>(mMiiButtons[buttonIndex].getActionSfxID()) == SE_ERROR) {
                        mUnk9B8 = false;
                    } else {
                        s32 num = buttonIndex + mMiiPage * 10;
                        if (gSaveData->getSaveFile(mSelectedSaveIndex)->isOccupied()) {
                            RFLCreateID createID = gRFLManager->fn_800C3060(num);
                            s32 saveIndex = mSelectedSaveIndex;
                            gSaveData->getSaveFile(saveIndex)->useProfileMii();
                            gSaveData->getSaveFile(saveIndex)->setProfileMiiID(createID);

                            gSaveData->updateSnapshot();

                            if (gBackupManager != NULL)
                                gBackupManager->fn_80084FC8(1);

                            updateSaveDisplay(saveIndex);

                            getAnimation(anim_fileSelect_window_out_W_menu_mii_00)->playFromBeginForward();
                            getAnimation(anim_fileSelect_window_out_W_menu_01)->playFromBeginForward();

                            updateState(TITLE_STATE_ICON_SELECT_OUT);
                        } else {
                            RFLCreateID createID = gRFLManager->fn_800C3060(num);
                            s32 saveIndex = mSelectedSaveIndex;
                            lbl_8034C820[saveIndex] = createID;
                            lbl_803204B0[saveIndex] = true;
                            lbl_803204B4[saveIndex] = 0;

                            nw4r::lyt::TexMap texMap;
                            GXTexObj *texObj = gRFLManager->fn_800C2FE0(createID);
                            texMap = nw4r::lyt::TexMap(*texObj);
                            texMap.SetImage(reinterpret_cast<void *>(reinterpret_cast<u32>(GXGetTexObjData(texObj)) + 0x80000000));

                            nw4r::lyt::Pane *pane = getLayout()->GetRootPane()->FindPaneByName("P_player_icon_00");
                            pane->GetMaterial()->SetTexMap(0, texMap);
                            fn_800C5F70(pane, saveIndex, true);

                            getAnimation(anim_fileSelect_window_in_W_msg_02)->playFromBeginForward();

                            mPrevState = mState;

                            updateState(TITLE_STATE_CONFIRM_DIALOG_IN);
                        }
                    }
                }
            }

            break;
        }
        case TITLE_STATE_DELETE_DIALOG_IN:
        case TITLE_STATE_DELETE_DIALOG_OUT: {
            bool finished = false;
            if (!getAnimation(anim_fileSelect_window_in_W_msg_00)->getIsPlaying() && !getAnimation(anim_fileSelect_window_out_W_msg_00)->getIsPlaying())
                finished = true;

            if (finished) {
                if (mState == TITLE_STATE_DELETE_DIALOG_IN)
                    updateState(TITLE_STATE_DELETE_DIALOG);
                else if (mState == TITLE_STATE_DELETE_DIALOG_OUT)
                    updateState(TITLE_STATE_ACTION_MENU);
            }

            break;
        }
        case TITLE_STATE_DELETE_DIALOG: {
            CController *controller = gControllerManager->fn_801D5FF0(0);

            nw4r::math::VEC2 vec(
                controller->fn_801D523C(this).x,
                controller->fn_801D523C(this).y
            );

            if (mUnk9B8 || !controller->fn_801D52D4()) {
                vec.x = lbl_80320FA0.x;
                vec.y = lbl_80320FA0.y;
            }

            nw4r::math::VEC2 buttonVec = vec;
            nw4r::math::VEC2 buttonVecCopy = buttonVec;
            mDeleteDialogManager->_0C(&buttonVecCopy, true, drawInfo);

            bool selected = mDeleteBackButton.getEnabled() && mDeleteBackButton.getUnk26();
            if (selected)
                mUnk9B8 = true;

            bool action = mDeleteBackButton.getEnabled() && mDeleteBackButton.getUnk27();
            if (action) {
                getAnimation(anim_fileSelect_window_out_W_msg_00)->playFromBeginForward();
                updateState(TITLE_STATE_DELETE_DIALOG_OUT);
            }

            if (!mUnk9B8) {
                if (gControllerManager->fn_801D5FF0(0)->checkHold(WPAD_BUTTON_1) && gControllerManager->fn_801D5FF0(0)->checkHold(WPAD_BUTTON_2)) {
                    if (mDeleteHoldFrames < 0)
                        break;

                    if (++mDeleteHoldFrames >= 180) {
                        getAnimation(anim_fileSelect_window_out_W_msg_00)->playFromBeginForward();
                        getAnimation(anim_fileSelect_window_in_W_msg_01)->playFromBeginForward();

                        s32 selectedSaveIndex = mSelectedSaveIndex;
                        gSaveData->getSaveFile(selectedSaveIndex)->reset();
                        gSaveData->updateSnapshot();

                        if (gBackupManager != NULL)
                            gBackupManager->fn_80084FC8(1);

                        updateSaveDisplay(selectedSaveIndex);

                        mUnk24 = 0;

                        /*
                        Data has been erased.
                        */
                        gMessageManager->setupTextBox(mDialogMessageTextBox, "0104100", 0);

                        updateState(TITLE_STATE_MESSAGE_DIALOG_IN);

                        gSoundManager->play(SE_DATA_FILE_CLEAR);
                    }

                    break;
                }
            }

            mDeleteHoldFrames = 0;

            break;
        }
        case TITLE_STATE_CONFIRM_DIALOG_IN:
        case TITLE_STATE_CONFIRM_DIALOG_OUT: {
            bool finished = false;
            if (!getAnimation(anim_fileSelect_window_in_W_msg_02)->getIsPlaying() && !getAnimation(anim_fileSelect_window_out_W_msg_02)->getIsPlaying())
                finished = true;

            if (finished) {
                if (mState == TITLE_STATE_CONFIRM_DIALOG_IN) {
                    updateState(TITLE_STATE_CONFIRM_DIALOG);
                } else if (mState == TITLE_STATE_CONFIRM_DIALOG_OUT) {
                    updateState(mPrevState);
                }

            }

            break;
        }
        case TITLE_STATE_CONFIRM_DIALOG: {
            CController *controller = gControllerManager->fn_801D5FF0(0);

            nw4r::math::VEC2 vec(
                controller->fn_801D523C(this).x,
                controller->fn_801D523C(this).y
            );

            if (mUnk9B8 || !controller->fn_801D52D4()) {
                vec.x = lbl_80320FA0.x;
                vec.y = lbl_80320FA0.y;
            }

            nw4r::math::VEC2 buttonVec = vec;
            nw4r::math::VEC2 buttonVecCopy = buttonVec;
            mConfirmDialogManager->_0C(&buttonVecCopy, true, drawInfo);

            bool selected = mConfirmButton.getEnabled() && mConfirmButton.getUnk26();
            if (!selected)
                selected = mCancelButton.getEnabled() && mCancelButton.getUnk26();
            if (selected)
                mUnk9B8 = true;

            bool doAction = mConfirmButton.getEnabled() && mConfirmButton.getUnk27();
            if (doAction) {
                s32 selectedSaveIndex = mSelectedSaveIndex;
                bool useProfileMii = lbl_803204B0[selectedSaveIndex];
                s8 profileIconIndex = lbl_803204B4[selectedSaveIndex];
                RFLCreateID createID = lbl_8034C820[selectedSaveIndex];

                if (useProfileMii) {
                    gSaveData->getSaveFile(selectedSaveIndex)->useProfileMii();
                    gSaveData->getSaveFile(selectedSaveIndex)->setProfileMiiID(createID);
                } else {
                    gSaveData->getSaveFile(selectedSaveIndex)->setProfileIconIndex(profileIconIndex);
                }

                gSaveData->updateSnapshot();

                if (gBackupManager != NULL)
                    gBackupManager->fn_80084FC8(1);

                updateSaveDisplay(selectedSaveIndex);

                gPlayHistory->fn_8009A710();
                gCafeTopic->fn_8009CE34();
                gSaveData->setSelectedSaveIndex(mSelectedSaveIndex);

                CExScene::fn_8000A74C(30);
                CExScene::fn_8000A790(30);

                updateState(TITLE_STATE_START_GAME);
            }

            bool doCancel = mCancelButton.getEnabled() && mCancelButton.getUnk27();
            if (doCancel) {
                getAnimation(anim_fileSelect_window_out_W_msg_02)->playFromBeginForward();
                updateState(TITLE_STATE_CONFIRM_DIALOG_OUT);
            }

            break;
        }
        case TITLE_STATE_MESSAGE_DIALOG_IN:
        case TITLE_STATE_MESSAGE_DIALOG_OUT: {
            bool finished = false;
            bool windowFinished = !getAnimation(anim_fileSelect_window_in_W_msg_01)->getIsPlaying() && !getAnimation(anim_fileSelect_window_out_W_msg_01)->getIsPlaying();
            if (windowFinished && !getAnimation(anim_fileSelect_window_out_W_msg_00)->getIsPlaying())
                finished = true;

            if (finished) {
                if (mState == TITLE_STATE_MESSAGE_DIALOG_IN) {
                    updateState(TITLE_STATE_MESSAGE_DIALOG);
                } else if (mState == TITLE_STATE_MESSAGE_DIALOG_OUT) {
                    switch (mUnk24) {
                    case 1:
                        updateState(TITLE_STATE_ICON_SELECT);
                        break;
                    case 2:
                        updateState(TITLE_STATE_ICON_SELECT);
                        break;
                    case 3:
                        getAnimation(anim_fileSelect_window_in_W_menu_01)->playFromBeginForward();
                        setupIconSelection();
                        updateState(TITLE_STATE_ICON_SELECT_IN);
                        break;
                    }
                }
            }

            break;
        }
        case TITLE_STATE_MESSAGE_DIALOG: {
            CController *controller = gControllerManager->fn_801D5FF0(0);

            nw4r::math::VEC2 vec(
                controller->fn_801D523C(this).x,
                controller->fn_801D523C(this).y
            );

            if (mUnk9B8 || !controller->fn_801D52D4()) {
                vec.x = lbl_80320FA0.x;
                vec.y = lbl_80320FA0.y;
            }

            nw4r::math::VEC2 buttonVec = vec;
            nw4r::math::VEC2 buttonVecCopy = buttonVec;
            mMessageDialogManager->_0C(&buttonVecCopy, true, drawInfo);

            bool selected = mMessageOkButton.getEnabled() && mMessageOkButton.getUnk26();
            if (selected)
                mUnk9B8 = true;

            bool action = mMessageOkButton.getEnabled() && mMessageOkButton.getUnk27();
            if (action) {
                if (static_cast<u32>(mUnk24) - 1 > 2) {
                    if (mUnk24 == 0) {
                        getAnimation(anim_fileSelect_window_out + mSelectedSaveIndex)->playFromBeginForward();
                        getAnimation(anim_fileSelect_window_out_W_menu_00)->playFromBeginForward();
                        getAnimation(anim_fileSelect_window_out_W_msg_01)->playFromBeginForward();

                        updateState(TITLE_STATE_ACTION_MENU_OUT);
                    }
                } else {
                    getAnimation(anim_fileSelect_window_out_W_msg_01)->playFromBeginForward();

                    updateState(TITLE_STATE_MESSAGE_DIALOG_OUT);
                }
            }

            break;
        }
        case TITLE_STATE_START_GAME: {
            if (++mStartGameFrames == 30) {
                if (gSaveData->getSaveFile(mSelectedSaveIndex)->fn_80077EC8()) {
                    gTickFlowManager->fn_801E1E4C();
                    gTickFlowManager->fn_801E1CC0(lbl_80253E18);
                } else {
                    gTickFlowManager->fn_801E1E4C();
                    gTickFlowManager->fn_801E1CC0(lbl_80259540);
                }
            }

            break;
        }
    }

    this->CLayout::_18(drawInfo);
}

void Title::CMyLayoutF::_14(void) {
    delete mFileSelectManager;
    delete mActionMenuManager;
    delete mIconSelectManager;
    delete mMiiSelectManager;
    delete mDeleteDialogManager;
    delete mConfirmDialogManager;
    delete mMessageDialogManager;

    this->CLayout::_14();
}

const char *sExistingSavePaneNames[4] = {"N_data_00", "N_data_01", "N_data_02", "N_data_03"};
const char *sNewSavePaneNames[4] = {"N_first_00", "N_first_01", "N_first_02", "N_first_03"};
const char *sFlowTextBoxNames[4] = {"T_nori_num_00", "T_nori_num_01", "T_nori_num_02", "T_nori_num_03"};
const char *sMedalTextBoxNames[4] = {"T_medal_num_00", "T_medal_num_01", "T_medal_num_02", "T_medal_num_03"};
const char *sMiiTexturePaneNames[10] = {"P_icon_mii_00", "P_icon_mii_01", "P_icon_mii_02", "P_icon_mii_03", "P_icon_mii_04", "P_icon_mii_05", "P_icon_mii_06", "P_icon_mii_07", "P_icon_mii_08", "P_icon_mii_09"};

const u8 lbl_80256870[20] = {0xFF, 0x00, 0x01, 0x02, 0x03, 0xFF, 0x05, 0x06, 0x07, 0x08, 0xFF, 0x0A, 0x0B, 0x0C, 0x0D, 0xFF, 0x0F, 0x00, 0x00, 0x00};
const u8 lbl_80256884[20] = {0x01, 0x02, 0x03, 0x04, 0xFF, 0x06, 0x07, 0x08, 0x09, 0xFF, 0x0B, 0x0C, 0x0D, 0x0E, 0xFF, 0x10, 0xFF, 0x00, 0x00, 0x00};
const u8 lbl_80256898[20] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0A, 0x00, 0x00, 0x00};
const u8 lbl_802568AC[20] = {0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x0F, 0x10, 0x10, 0x10, 0xFF, 0xFF, 0x00, 0x00, 0x00};

const char *sSaveIconPaneNames[4] = {"P_name_00", "P_name_01", "P_name_02", "P_name_03"};

void Title::CMyLayoutF::setBackgroundLoopFrame(float frame) {
    getAnimation(anim_fileSelect_bg_loop_N_bg_all_00)->fn_801DA66C(frame);
}

void Title::CMyLayoutF::updateState(ETitleState state) {
    mState = state;
    mUnk9B8 = false;
    mDeleteHoldFrames = -1;
    mStartGameFrames = 0;

    CExBtnPaneManager *manager = NULL;
    switch (state) {
        case TITLE_STATE_FILE_SELECT:
            manager = mFileSelectManager;
            break;
        case TITLE_STATE_ACTION_MENU:
            manager = mActionMenuManager;
            break;
        case TITLE_STATE_ICON_SELECT:
            manager = mIconSelectManager;
            break;
        case TITLE_STATE_MII_SELECT:
            manager = mMiiSelectManager;
            break;
        case TITLE_STATE_DELETE_DIALOG:
            manager = mDeleteDialogManager;
            break;
        case TITLE_STATE_CONFIRM_DIALOG:
            manager = mConfirmDialogManager;
            break;
        case TITLE_STATE_MESSAGE_DIALOG:
            manager = mMessageDialogManager;
            break;
    }

    if (manager != NULL) {
        manager->fn_8008F984(true, gLayoutManager->getDrawInfo());
        manager->fn_8008F81C(lbl_80320498, -1);

        mCursorFramePane->SetVisible(true);
    } else {
        mCursorFramePane->SetVisible(false);
    }
}

static inline nw4r::lyt::Pane *setSaveIconTexture(nw4r::lyt::Layout *layout, s32 saveIndex, nw4r::lyt::TexMap texture) {
    nw4r::lyt::Pane *pane = layout->GetRootPane()->FindPaneByName(sSaveIconPaneNames[saveIndex]);
    pane->GetMaterial()->SetTexMap(0, texture);
    return pane;
}

void Title::CMyLayoutF::updateSaveDisplay(s32 saveIndex) {
    nw4r::lyt::Pane *existingSavePane = getLayout()->GetRootPane()->FindPaneByName(sExistingSavePaneNames[saveIndex]);
    nw4r::lyt::Pane *newSavePane = getLayout()->GetRootPane()->FindPaneByName(sNewSavePaneNames[saveIndex]);

    if (gSaveData->getSaveFile(saveIndex)->isOccupied()) {
        existingSavePane->SetVisible(true);
        newSavePane->SetVisible(false);

        nw4r::lyt::TextBox *flowTextBox = static_cast<nw4r::lyt::TextBox *>(getLayout()->GetRootPane()->FindPaneByName(sFlowTextBoxNames[saveIndex]));
        nw4r::lyt::TextBox *medalTextBox = static_cast<nw4r::lyt::TextBox *>(getLayout()->GetRootPane()->FindPaneByName(sMedalTextBoxNames[saveIndex]));

        u32 flow = gSaveData->getSaveFile(saveIndex)->calculateFlow();
        u32 medals = gSaveData->getSaveFile(saveIndex)->countMedals();

        wchar_t buffer[8];
        swprintf(buffer, ARRAY_LENGTH(buffer), L"%d", flow);
        flowTextBox->SetString(buffer);
        swprintf(buffer, ARRAY_LENGTH(buffer), L"%d", static_cast<u8>(medals));
        medalTextBox->SetString(buffer);

        nw4r::lyt::TexMap texture;
        if (gSaveData->getSaveFile(saveIndex)->isUsingProfileMii()) {
            bool usesProfileMii = false;
            bool isProfileMiiMissing = false;

            if (gSaveData->getSaveFile(saveIndex)->isOccupied() && gSaveData->getSaveFile(saveIndex)->isUsingProfileMii())
                usesProfileMii = true;

            if (usesProfileMii) {
                GXTexObj *texObj = gRFLManager->fn_800C2FE0(gSaveData->getSaveFile(saveIndex)->getProfileMiiID());
                if (texObj == NULL)
                    isProfileMiiMissing = true;
            }

            if (isProfileMiiMissing) {
                texture = mDefaultMiiTexture;
            } else {
                GXTexObj *texObj = gRFLManager->fn_800C2FE0(gSaveData->getSaveFile(saveIndex)->getProfileMiiID());
                texture = nw4r::lyt::TexMap(*texObj);
                texture.SetImage(reinterpret_cast<void *>(reinterpret_cast<u32>(GXGetTexObjData(texObj)) + 0x80000000));
            }
        } else {
            s8 index = gSaveData->getSaveFile(saveIndex)->getProfileIconIndex();
            texture = getLayout()->GetRootPane()->FindPaneByName(sIconNamePaneNames[index])->GetMaterial()->GetTexMap(0);
        }

        nw4r::lyt::Pane *pane = setSaveIconTexture(getLayout(), saveIndex, texture);

        fn_800C5F70(pane, saveIndex, gSaveData->getSaveFile(saveIndex)->isUsingProfileMii());
    } else {
        existingSavePane->SetVisible(false);
        newSavePane->SetVisible(true);
    }
}

void Title::CMyLayoutF::setupIconSelection(void) {
    for (s32 i = 0; i < 15; i++) {
        lbl_802ED8D0.buttonIndices0[i] = lbl_80256870[i];
        lbl_802ED8D0.buttonIndices1[i] = lbl_80256884[i];
        lbl_802ED8D0.buttonIndices2[i] = lbl_80256898[i];
        lbl_802ED8D0.buttonIndices3[i] = lbl_802568AC[i];

        mNameButtons[i].setActionSfxID(SE_KETTEI_SMALL);
        mNameButtons[i].setEnabled(true);

        getAnimation(anim_fileSelect_name_deactivate_nameFramePane + i)->setIsPlaying(false);
        getAnimation(anim_fileSelect_name_deactivate_namePane + i)->setIsPlaying(false);

        getAnimation(anim_fileSelect_name_activate_nameFramePane + i)->playFromBeginning();
        getAnimation(anim_fileSelect_name_activate_namePane + i)->playFromBeginning();
    }

    for (s32 saveIndex = 0; saveIndex < SAVE_COUNT; saveIndex++) {
        if (gSaveData->getSaveFile(saveIndex)->isOccupied() && !gSaveData->getSaveFile(saveIndex)->isUsingProfileMii()) {
            s8 index = gSaveData->getSaveFile(saveIndex)->getProfileIconIndex();
            mNameButtons[index].setActionSfxID(SE_ERROR);

            getAnimation(anim_fileSelect_name_activate_nameFramePane + index)->setIsPlaying(false);
            getAnimation(anim_fileSelect_name_activate_namePane + index)->setIsPlaying(false);

            getAnimation(anim_fileSelect_name_deactivate_nameFramePane + index)->playFromBeginning();
            getAnimation(anim_fileSelect_name_deactivate_namePane + index)->playFromBeginning();
        }
    }

    mIconSelectManager->fn_8008F71C(0, 1, 0);
    mIconSelectManager->fn_8008F6AC(0);

    for (s32 i = 0; i < static_cast<s32>(ARRAY_LENGTH(sIconNamePaneNames)) - 1; i++) {
        nw4r::lyt::Pane *pane = getLayout()->GetRootPane()->FindPaneByName(sIconNamePaneNames[i]);
        fn_800C5F70(pane, mSelectedSaveIndex, false);
    }

    const char *mesgID = gSaveData->getSaveFile(mSelectedSaveIndex)->isOccupied()
        /*
        Which would you like?
        Choose one!
        */
        ? "0104010"
        /*
        How would you like to label
        your save file? Choose one!
        */
        : "0104005";

    gMessageManager->setupTextBox(mMenuMessageTextBox, mesgID, 0);
}

static inline bool isMiiUsed(RFLCreateID createID) {
    for (s32 saveIndex = 0; saveIndex < SAVE_COUNT; saveIndex++) {
        if (!gSaveData->getSaveFile(saveIndex)->isOccupied())
            continue;
        if (!gSaveData->getSaveFile(saveIndex)->isUsingProfileMii())
            continue;

        RFLCreateID savedID = gSaveData->getSaveFile(saveIndex)->getProfileMiiID();
        if (memcmp(&savedID, &createID, sizeof(RFLCreateID)) == 0)
            return true;
    }

    return false;
}

void Title::CMyLayoutF::updateMiiPage(s32 page) {
    for (s32 buttonIndex = 0; buttonIndex < static_cast<s32>(ARRAY_LENGTH(sMiiTexturePaneNames)); buttonIndex++) {
        s32 miiIndex = page * 10 + buttonIndex;
        nw4r::lyt::Pane *pane = getLayout()->GetRootPane()->FindPaneByName(sMiiTexturePaneNames[buttonIndex]);
        nw4r::lyt::Material *material = pane->GetMaterial();
        mMiiButtons[buttonIndex].setActionSfxID(SE_KETTEI_SMALL);

        bool available = true;
        bool alreadyUsed = false;
        if (miiIndex < gRFLManager->fn_800C2FC4()) {
            GXTexObj *texObj = gRFLManager->fn_800C2FCC(miiIndex);
            nw4r::lyt::TexMap texMap(*texObj);

            material->SetTexMap(0, texMap);

            bool usedBySave = isMiiUsed(gRFLManager->fn_800C3060(miiIndex));
            if (usedBySave) {
                available = false;
                alreadyUsed = true;
            }
        } else {
            material->SetTexMap(0, mDefaultMiiTexture);
            available = false;
        }

        if (available) {
            mMiiButtons[buttonIndex].setEnabled(true);
            getAnimation(anim_fileSelect_name_deactivate_iconBackgroundPane + buttonIndex)->setIsPlaying(false);
            getAnimation(anim_fileSelect_name_deactivate_iconMiiPane + buttonIndex)->setIsPlaying(false);

            getAnimation(anim_fileSelect_name_activate_iconBackgroundPane + buttonIndex)->playFromBeginning();
            getAnimation(anim_fileSelect_name_activate_iconMiiPane + buttonIndex)->playFromBeginning();
        } else {
            if (alreadyUsed) {
                mMiiButtons[buttonIndex].setEnabled(true);
                mMiiButtons[buttonIndex].setActionSfxID(SE_ERROR);
            } else {
                mMiiButtons[buttonIndex].setEnabled(false);
            }

            getAnimation(anim_fileSelect_name_activate_iconBackgroundPane + buttonIndex)->setIsPlaying(false);
            getAnimation(anim_fileSelect_name_activate_iconMiiPane + buttonIndex)->setIsPlaying(false);

            getAnimation(anim_fileSelect_name_deactivate_iconBackgroundPane + buttonIndex)->playFromBeginning();
            getAnimation(anim_fileSelect_name_deactivate_iconMiiPane + buttonIndex)->playFromBeginning();
        }
    }

    if (page > 0) {
        mMiiButtons[TITLE_MII_BUTTON_PAGE_LEFT].setEnabled(true);
        mMiiButtons[TITLE_MII_BUTTON_PAGE_LEFT].getPane()->SetVisible(true);
    } else {
        mMiiButtons[TITLE_MII_BUTTON_PAGE_LEFT].setEnabled(false);
        mMiiButtons[TITLE_MII_BUTTON_PAGE_LEFT].getPane()->SetVisible(false);
    }

    if ((page + 1) * 10 < gRFLManager->fn_800C2FC4()) {
        mMiiButtons[TITLE_MII_BUTTON_PAGE_RIGHT].setEnabled(true);
        mMiiButtons[TITLE_MII_BUTTON_PAGE_RIGHT].getPane()->SetVisible(true);
    } else {
        mMiiButtons[TITLE_MII_BUTTON_PAGE_RIGHT].setEnabled(false);
        mMiiButtons[TITLE_MII_BUTTON_PAGE_RIGHT].getPane()->SetVisible(false);
    }

    if (mMiiSelectManager->fn_8008F948() != &mMiiButtons[10] && mMiiSelectManager->fn_8008F948() != &mMiiButtons[11])
        mMiiSelectManager->fn_8008F71C(0, 1, 0);
    mMiiSelectManager->fn_8008F6AC(0);

    wchar_t currentPage[3] = {};
    wchar_t totalPages[3] = {};
    swprintf(currentPage, ARRAY_LENGTH(currentPage), L"%d", page + 1);
    swprintf(totalPages, ARRAY_LENGTH(totalPages), L"%d", (gRFLManager->fn_800C2FC4() - 1) / 10 + 1);

    mCurrentPageTextBox->SetString(currentPage);
    mTotalPagesTextBox->SetString(totalPages);
}

void Title::CMyLayoutF::updateSelectionSfx(void) {
    u16 sfxId = lbl_80320498 ? SE_CURSOR_WAKU : SE_CURSOR_KEY;

    for (u8 i = 0; i < ARRAY_LENGTH(mFileButtons); i++)
        mFileButtons[i].setSelectSfxID(sfxId);

    for (u8 i = 0; i < ARRAY_LENGTH(mFileActionButtons); i++)
        mFileActionButtons[i].setSelectSfxID(sfxId);

    for (u8 i = 0; i < ARRAY_LENGTH(mNameButtons); i++)
        mNameButtons[i].setSelectSfxID(sfxId);

    for (u8 i = 0; i < ARRAY_LENGTH(mMiiButtons); i++)
        mMiiButtons[i].setSelectSfxID(sfxId);

    mDeleteBackButton.setSelectSfxID(sfxId);
    mMessageOkButton.setSelectSfxID(sfxId);
}

Title::CMyLayoutF::~CMyLayoutF(void) {
    _14();
}
