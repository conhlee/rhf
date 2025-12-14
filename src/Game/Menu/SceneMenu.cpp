#include "Menu/SceneMenu.hpp"
#include <string.h>
#include <nw4r/lyt/textBox.h>
#include "Mem.hpp"
#include "FileManager.hpp"
#include "MessageManager.hpp"
#include "SceneManager.hpp"
#include "MyCanvas.hpp"
#include "LayoutManager.hpp"
#include "SoundManager.hpp"
#include "Controller.hpp"
#include "CheckPointManager.hpp"
#include "TickFlowManager.hpp"
#include "SaveData.hpp"
#include "GameManager.hpp"

#include "Menu/MenuData.inc"

#include "rev_tengoku.rsid"

wchar_t CSceneMenu::sTextBuffer[1024];
wchar_t CSceneMenu::sEntryNumTextBuffer[16];

static u8 lbl_80320140;
static u8 lbl_80320141;
static u8 lbl_80320142;
static u8 lbl_80320143;

namespace Menu {

s32 sceneVer;

} // namespace Menu

SCENE_IMPL_CREATE_FN(CSceneMenu)

void CSceneMenu::fn_80006FA4(void) {
    fn_80008EFC();

    fn_801D369C(eHeapGroup_SceneAsset);
    if (gFileManager->fn_801D42E0(55)) {
        gFileManager->fn_801D3F94(55, "content2/layout/layout_msg.szs");
    }
    fn_801D3644();

    if (!gFileManager->fn_801D42FC(0)) {
        return;
    }

    fn_801D369C(eHeapGroup_CommonAsset);
    gMessageManager->fn_800883F4();
    gMessageManager->fn_8008807C();
    gMessageManager->fn_80088034();
    gMessageManager->fn_80088088();
    fn_801D3644();
}

void CSceneMenu::_10(void) {
    fn_80006FA4();
}

namespace {

class CMenuLayout : public CLayout {
public:
    virtual ~CMenuLayout(void);
    virtual void _10(void);
    virtual void _14(void);

    CMenuLayout(void) {
        setAnimationCount(0);
    }

    nw4r::lyt::TextBox *getTitlePane(void) {
        return mPaneTitle;
    }
    nw4r::lyt::TextBox *getCommentPane(void) {
        return mPaneComment;
    }

private:
    nw4r::lyt::TextBox *mPaneTitle;
    nw4r::lyt::TextBox *mPaneComment;
};

} // namespace

void CSceneMenu::_14(void) {
    gSceneManager->fn_8008B068();

    lbl_80320274 = false;

    memset(mUnk34, 0x00, sizeof(mUnk34));

    this->CExScene::_14();

    gMyCanvasManager->fn_8007BE0C();

    gLayoutManager->_20(1);
    gLayoutManager->_24(55, "");
    gLayoutManager->fn_801D6AEC(1);
    gLayoutManager->registerLayout<CMenuLayout>();

    gSoundManager->fn_801E6ECC(1.0f);

    fn_800077A8(lbl_80320143);
    mUnkB4 = false;

    fn_801D3638(300);
}

void CSceneMenu::_28(void) {
    if (mUnkB4) {
        return;
    }

    CController *controller = gControllerManager->fn_801D5FF0(0);
    CGCController *gcController = gControllerManager->fn_801D6000(0);

    // TODO: regswap in unkInputCheck
    if ((controller->getUnk1340() & 0x800) || (gcController->unkInputCheck(0x100))) {
        const TickFlowCode *tickFlowCode = lbl_801F8460[lbl_80320143].tickFlowCode;
        if (tickFlowCode != NULL) {
            if (tickFlowCode == lbl_80320FE0) {
                // 0 : HI, 1 : OK, 2 : NG
                if (lbl_80320142 == 0) {
                    gCheckPointManager->setUnk2FC(0);
                }
                if (lbl_80320142 == 1) {
                    gCheckPointManager->setUnk2FC(1);
                }
                if (lbl_80320142 == 2) {
                    gCheckPointManager->setUnk2FC(2);
                }

                u32 temp = lbl_80320141 * 9 + lbl_80320140;
                if (!lbl_802E5740[temp]) {
                    return;
                }
                gTickFlowManager->fn_801E1E4C();
                gTickFlowManager->fn_801E1CC0(lbl_802E5740[temp]);
                mUnkB4 = true;
                return;
            }
            if (tickFlowCode == lbl_801F7890) {
                if (gSaveData->fn_80078F4C()->fn_80077DF8() < 5) {
                    gSaveData->fn_80078F4C()->fn_80077C0C(5);
                }
                gTickFlowManager->fn_801E1E4C();
                gTickFlowManager->fn_801E1CC0(lbl_801F8460[lbl_80320143].tickFlowCode);
                mUnkB4 = true;
                return;
            }
            if (
                (tickFlowCode == lbl_8026F248) || (tickFlowCode == lbl_80276C90) || 
                (tickFlowCode == lbl_80278AF8) || (tickFlowCode == lbl_8027A4D8) || 
                (tickFlowCode == lbl_8026D3F0) || (tickFlowCode == lbl_80273A50) || 
                (tickFlowCode == lbl_80284530) || (tickFlowCode == lbl_802BA4BC)
            ) {
                lbl_80320274 = true;
            }
            gTickFlowManager->fn_801E1E4C();
            gTickFlowManager->fn_801E1CC0(lbl_801F8460[lbl_80320143].tickFlowCode);
            mUnkB4 = true;
            return;
        }
        gGameManager->_20(lbl_801F8460[lbl_80320143].sceneCreateFn, eHeapGroup_Scene);
        return;
    }
    if (((controller->getUnk133C() & 8) || (controller->getUnk1368() & 8)) 
    || ((gcController->unkInputCheck(8) || (gcController->getUnk14() & 8)))) {
        fn_800077A8((lbl_80320143 + ARRAY_LENGTH(lbl_801F8460) - 1) % ARRAY_LENGTH(lbl_801F8460));
        return;
    }
    if (((controller->getUnk133C() & 4) || (controller->getUnk1368() & 4)) 
    || ((gcController->unkInputCheck(4) || (gcController->getUnk14() & 4)))) {
        fn_800077A8((lbl_80320143 + 1) % ARRAY_LENGTH(lbl_801F8460));
        return;
    }
    if (((controller->getUnk133C() & 1) || (controller->getUnk1368() & 1)) 
    || ((gcController->unkInputCheck(1) || (gcController->getUnk14() & 1)))) {
        if ((lbl_80320143 - 20) >= 0) {
            fn_800077A8(lbl_80320143 - 20);
            return;
        }
        if (((lbl_80320143 % 20) + 100) >= ARRAY_LENGTH(lbl_801F8460)) {
            fn_800077A8(ARRAY_LENGTH(lbl_801F8460) - 1);
            return;
        }
        fn_800077A8((lbl_80320143 % 20) + 100);
        return;
    }
    if (((controller->getUnk133C() & 2) || (controller->getUnk1368() & 2)) 
    || ((gcController->unkInputCheck(2) || (gcController->getUnk14() & 2)))) {
        if ((lbl_80320143 + 20) < ARRAY_LENGTH(lbl_801F8460)) {
            fn_800077A8(lbl_80320143 + 20);
            return;
        }
        s32 temp = ((lbl_80320143) / 20);
        s32 temp1 = ((lbl_80320143) % 20);
        if (temp < 5) {
            fn_800077A8(ARRAY_LENGTH(lbl_801F8460) - 1);
            return;
        }
        fn_800077A8(temp1);
        return;
    }
}

void CSceneMenu::_20(void) {
    gFileManager->fn_801D41CC(55);

    this->CExScene::_20();
}

void CSceneMenu::fn_800077A8(u8 arg1) {
    lbl_80320143 = arg1;

    s32 entriesPerPage = 20;
    s32 pageIndex = (lbl_80320143 / entriesPerPage);

    s32 lastPageIndex = ARRAY_LENGTH(lbl_801F8460) / entriesPerPage;
    s32 lastPageEntries = ARRAY_LENGTH(lbl_801F8460) - (lastPageIndex * entriesPerPage);
    
    s32 entriesInThisPage = (pageIndex == lastPageIndex) ? lastPageEntries : entriesPerPage;

    swprintf(sTextBuffer, ARRAY_LENGTH(sTextBuffer), L"");
    wcscat(sTextBuffer, mUnk34);
    wcscat(sTextBuffer, L"\n");

    for (s32 i = 0; i < entriesInThisPage; i++) {
        s32 entryNum = (pageIndex * entriesPerPage) + i;
        swprintf(sEntryNumTextBuffer, ARRAY_LENGTH(sEntryNumTextBuffer), L"%03d : ", entryNum);
        wcscat(sTextBuffer, (entryNum == lbl_80320143) ? L"→" : L"　");
        wcscat(sTextBuffer, sEntryNumTextBuffer);
        wcscat(sTextBuffer, lbl_801F8460[entryNum].labelText);
        wcscat(sTextBuffer, L"\n");
    }

    CMenuLayout *menuLayout = gLayoutManager->getLayout<CMenuLayout>(0);
    if (sTextBuffer == NULL) {
        menuLayout->getTitlePane()->SetVisible(false);
    }
    else {
        menuLayout->getTitlePane()->SetString(sTextBuffer);
        menuLayout->getTitlePane()->SetVisible(true);
    }

    swprintf(sTextBuffer, ARRAY_LENGTH(sTextBuffer), L"");
    wcscat(sTextBuffer, L"<操作説明、コメント>\n"); // "<Operation Instructions, Comments>"
    wcscat(sTextBuffer, lbl_801F8460[lbl_80320143].commentText);

    menuLayout = gLayoutManager->getLayout<CMenuLayout>(0);
    if (sTextBuffer == NULL) {
        menuLayout->getCommentPane()->SetVisible(false);
    }
    else {
        menuLayout->getCommentPane()->SetString(sTextBuffer);
        menuLayout->getCommentPane()->SetVisible(true);
    }

    gSoundManager->play(SE_CURSOR);
}

void CMenuLayout::_14(void) {
    this->CLayout::_14();
}

void CMenuLayout::_10(void) {
    nw4r::lyt::MultiArcResourceAccessor *resAccessor = gLayoutManager->getResAccessor();

    u32 layoutBinSize;
    void *layoutBin = resAccessor->GetResource(0, layoutFileTable[0], &layoutBinSize);

    buildLayout(layoutBin, resAccessor);

    gMessageManager->fn_80088474(getLayout()->GetRootPane());

    mPaneTitle = static_cast<nw4r::lyt::TextBox *>(getLayout()->GetRootPane()->FindPaneByName("T_Title"));
    mPaneComment = static_cast<nw4r::lyt::TextBox *>(getLayout()->GetRootPane()->FindPaneByName("T_Comment"));

    nw4r::math::VEC3 titleTranslate = mPaneTitle->GetTranslate();
    nw4r::math::VEC3 commentTranslate = mPaneComment->GetTranslate();

    titleTranslate.y -= 25.0f;
    commentTranslate.y -= 25.0f;

    mPaneTitle->SetScale(nw4r::math::VEC2(.9f, .9f));
    mPaneComment->SetScale(nw4r::math::VEC2(.9f, .9f));

    mPaneTitle->SetTranslate(titleTranslate);
    mPaneComment->SetTranslate(commentTranslate);

    this->CLayout::_10();

    mPaneTitle->SetVisible(false);
    mPaneComment->SetVisible(false);
}

CMenuLayout::~CMenuLayout(void) {
    _14();
}

void CSceneMenu::_1C(void) {
    this->CExScene::_1C();
}

void CSceneMenu::_18(void) {
    this->CExScene::_18();
}

CSceneMenu::~CSceneMenu(void) {}
