#include "Grading/MyLayout.hpp"

#include "LayoutManager.hpp"

#include "MessageManager.hpp"

#include "CheckPointManager.hpp"

#include "SceneManager.hpp"

#include "SaveData.hpp"

#include "GameTable.hpp"

const char layoutFile_grading[] = "grading.brlyt";

DECL_SECTION(".sdata") static const char * const layoutFileTable[] = { layoutFile_grading, NULL };
// TODO: ^^ hack!

static const wchar_t *sCaptionString;

void Grading::CMyLayout::_10(void) {
    nw4r::lyt::MultiArcResourceAccessor *resAccessor = gLayoutManager->getResAccessor();

    u32 layoutBinSize;
    void *layoutBin = gLayoutManager->getResAccessor()->GetResource(0, layoutFileTable[0], &layoutBinSize);

    buildLayout(layoutBin, resAccessor);

    gMessageManager->fn_80088474(getLayout()->GetRootPane());

    mCaptionWnd = getLayout()->GetRootPane()->FindPaneByName("W_Caption_00");
    mCaptionText = static_cast<nw4r::lyt::TextBox *>(getLayout()->GetRootPane()->FindPaneByName("T_Caption_00"));

    mCommentText[0] = static_cast<nw4r::lyt::TextBox *>(getLayout()->GetRootPane()->FindPaneByName("T_Comment_00"));
    mCommentText[1] = static_cast<nw4r::lyt::TextBox *>(getLayout()->GetRootPane()->FindPaneByName("T_Comment_01"));
    mCommentText[2] = static_cast<nw4r::lyt::TextBox *>(getLayout()->GetRootPane()->FindPaneByName("T_Comment_02"));

    mFollowText = static_cast<nw4r::lyt::TextBox *>(getLayout()->GetRootPane()->FindPaneByName("T_Follow_00"));

    mCaptionWnd->SetVisible(false);
    mCaptionText->SetVisible(false);

    mCommentText[0]->SetVisible(false);
    mCommentText[1]->SetVisible(false);
    mCommentText[2]->SetVisible(false);

    mFollowText->SetVisible(false);

    mCaptionText->SetString(sCaptionString);
    fn_801D8A5C(mCaptionText, 32.0f, 38.0f);

    f32 minScale = 100.0f;
    for (s32 i = 0; i < 3; i++) {
        if (i < gCheckPointManager->fn_801EB7B0()) {
            const wchar_t *string = gCheckPointManager->fn_801EB7D0(i);
            mCommentText[i]->SetString(string);

            f32 scale = fn_801D8A5C(mCommentText[i], 25.6f, 30.4f);
            if (scale < minScale) {
                minScale = scale;
            }
        }
        else {
            mCommentText[i]->SetString(L"");
        }
    }

    if (minScale < 100.0f) {
        nw4r::lyt::Size size = nw4r::lyt::Size(minScale * 25.6f, 30.4f);
        mCommentText[0]->SetFontSize(size);
        mCommentText[1]->SetFontSize(size);
        mCommentText[2]->SetFontSize(size);
    }

    f32 temp = mCommentText[1]->GetTranslate().y - mCommentText[0]->GetTranslate().y;
    if (gCheckPointManager->fn_801EB7B0() == 1) {
        nw4r::math::VEC3 translate = mCommentText[0]->GetTranslate();
        translate.y += temp;

        mCommentText[0]->SetTranslate(translate);
    }
    else if (gCheckPointManager->fn_801EB7B0() == 2) {
        nw4r::math::VEC3 translate0 = mCommentText[0]->GetTranslate();
        nw4r::math::VEC3 translate1 = mCommentText[1]->GetTranslate();

        translate0.y += (temp / 2.0f);
        translate1.y += (temp / 2.0f);

        mCommentText[0]->SetTranslate(translate0);
        mCommentText[1]->SetTranslate(translate1);
    }

    if (gCheckPointManager->fn_801EB7A8()) {
        wchar_t buffer[128];
        swprintf(buffer, ARRAY_LENGTH(buffer), L"");

        wcscat(buffer, gCheckPointManager->fn_801EB7F8());
        wcscat(buffer, gCheckPointManager->fn_801EB7E0());
        
        s32 i = wcslen(gCheckPointManager->fn_801EB7F8());
        buffer[i] = CCheckPointManager::fn_801EC270(buffer[i]);

        mFollowText->SetString(buffer);
    }
    else {
        mFollowText->SetString(L"");
    }

    mHIPane = getLayout()->GetRootPane()->FindPaneByName("N_HI_00");
    mOKPane = getLayout()->GetRootPane()->FindPaneByName("N_OK_00");
    mNGPane = getLayout()->GetRootPane()->FindPaneByName("N_NG_00");

    mMedalPane = getLayout()->GetRootPane()->FindPaneByName("N_Medal_00");
    mButPane = getLayout()->GetRootPane()->FindPaneByName("N_But_00");

    mFollowPane = getLayout()->GetRootPane()->FindPaneByName("N_Follow_00");

    mHIPane->SetVisible(false);
    mOKPane->SetVisible(false);
    mNGPane->SetVisible(false);

    mMedalPane->SetVisible(false);
    mButPane->SetVisible(false);

    mFollowPane->SetVisible(false);

    getAnimation(0)->fn_801D9E70(getLayout(), NULL, "grading_loop_00", resAccessor);
    getAnimation(0)->setLoop(true);
    getAnimation(0)->playFromBeginForward();

    this->CLayout::_10();
}

void Grading::CMyLayout::fn_8000C088(void) {
    mCaptionWnd->SetVisible(true);
    mCaptionText->SetVisible(true);
}

void Grading::CMyLayout::fn_8000C0B4(void) {
    mCommentText[0]->SetVisible(true);
    mCommentText[1]->SetVisible(true);
    mCommentText[2]->SetVisible(true);
}

void Grading::CMyLayout::fn_8000C0F4(void) {
    mFollowText->SetVisible(true);
    mFollowPane->SetVisible(true);
}

void Grading::fn_8000C120(const char *mesgIDStr) {
    sCaptionString = gMessageManager->getString(mesgIDStr, eMesgSource_Comment);
}

void Grading::CMyLayout::fn_8000C15C(void) {
    if (mScene->mBut) {
        mButPane->SetVisible(true);
    }

    switch (mScene->mGrade) {
    case eGrade_HI:
        mHIPane->SetVisible(true);
        break;
    case eGrade_OK:
        mOKPane->SetVisible(true);
        break;
    case eGrade_NG:
        mNGPane->SetVisible(true);
        break;
    }

    if (gSceneManager->fn_8008B27C() && (mScene->mGrade == eGrade_HI)) {
        u8 index = fn_8000EBB0(gSaveData->getSelectedSaveFile()->fn_80077D50());
        ELevelState prevState = gSaveData->getSelectedSaveFile()->fn_80077D58(index);

        if (prevState < eLevelState_CompleteHI) {
            mMedalPane->SetVisible(true);
        }
    }
}

Grading::CMyLayout::~CMyLayout() {
    _14();
}
