#include "Title/SceneTitle.hpp"

#include "Title/MyFlow.hpp"

#include "Title/MyLayoutF.hpp"
#include "Title/MyLayoutT.hpp"

#include "ModeChangeLayout.hpp"

#include "FileManager.hpp"

#include "InputCheckManager.hpp"

#include "CellAnimManager.hpp"

#include "TickFlowManager.hpp"

#include "LayoutManager.hpp"

#include "Controller.hpp"

#include "TPL.hpp"

#include "Random.hpp"

#include "rev_tengoku.rsid"

extern bool lbl_80320498;

void CSceneTitle::fn_80092094(u32 input) {}

SCENE_IMPL_CREATE_FN(CSceneTitle)

void CSceneTitle::loadArchives() {
    memPushGroup(eMemGroup_SceneAsset);
    if (gFileManager->getArcFree(2) && gFileManager->getArcFree(55) && gFileManager->getArcFree(56)) {
        gFileManager->startArchive(2, "content2/cellanim/title/ver0/cellanim.szs");
        gFileManager->startArchive(55, "content2/layout/layout_title.szs");
        gFileManager->startArchive(56, "content2/layout/layout_fileSelect.szs");
    }
    memPopGroup();
}

void CSceneTitle::_10(void) {
    loadArchives();
}

void CSceneTitle::_14(void) {
    this->CExScene::_14();

    gInputCheckManager->setUnk418(fn_80092094);

    gTickFlowManager->registerFlow<Title::CMyFlow>();

    void *titleGirlAddr = gFileManager->arcGetFileAddr(2, "./title_girl.brcad");

    u32 tplLen = gFileManager->arcGetFileLen(2, "./cellanim.tpl");
    void *tplAddr = gFileManager->arcGetFileAddr(2, "./cellanim.tpl");

    UserTPLBind(tplAddr);
    DCStoreRange(tplAddr, tplLen);

    gCellAnimManager->registBank(titleGirlAddr, tplAddr, 0);

    mTitleGirlAnim = gCellAnimManager->createCellAnim(0, 0);
    mTitleGirlAnim->goToAnimEnd();
    mTitleGirlAnim->setEnabled(false);
    mTitleGirlAnim->setPos(180.0f, 450.0f);
    mTitleGirlAnim->setLayer(-1);

    setupLayouts(0);

    mTitleGirlPos.setEasing(0, 1);
    unk34 = 0;
    unk94 = false;
    unk95 = false;
}

void CSceneTitle::_28(void) {
    CController *controller = gControllerManager->fn_801D5FF0(0);
    if (unk34)
        return;

    if (unk94 && !unk95 && controller->unk136CCheck()) {
        unk95 = true;
        lbl_80320498 = false;

        static_cast<Title::CMyLayoutT *>(gLayoutManager->fn_801D6C50(0))->playAnimFromBeginForward(9);
        if (!unk34) {
            mTitleGirlPos.fn_801EA8E0(0.0f, mTitleGirlAnim->getPosY(), 0.0f, 0.0f, 450.0f, 0.0f, -1.0f);
            mTitleGirlPos.fn_801EA550(0.0f, 12.0f, true);
        }

        gSoundManager->play(SE_KETTEI_BIG);
        gTickFlowManager->fn_801E3E2C(0.5f, 30);
    }

    if (unk95) {
        if (!static_cast<Title::CMyLayoutT *>(gLayoutManager->fn_801D6C50(0))->getAnimIsPlaying(9)) {
            unk34 = 1;
            setupLayouts(1);
            mTitleGirlAnim->setEnabled(false);
        }
    }

    if (mTitleGirlPos.getIsTicking()) {
        mTitleGirlPos.fn_801E9DE8(FALSE);
        mTitleGirlAnim->setPosY(mTitleGirlPos.getCurrentY());
    }
}

void CSceneTitle::_1C(void) {
    this->CExScene::_1C();
}

void CSceneTitle::_20(void) {
    gSoundManager->all_player_volume(1.0f);

    gCellAnimManager->endBank(0);
    gCellAnimManager->endCellAnimByBank(0);

    gFileManager->endArchive(2);
    gFileManager->endArchive(55);
    gFileManager->endArchive(56);

    this->CExScene::_20();
}

void CSceneTitle::fn_800925C0(u32 arg1, u32 arg2) {}

void CSceneTitle::showTitleGirl() {
    if (unk34)
        return;

    mTitleGirlAnim->setEnabled(true);
    mTitleGirlPos.fn_801EA8E0(0.0f, 450.0f, 0.0f, 0.0f, 250.0f, 0.0f, -240.0f);
    mTitleGirlPos.fn_801EA550(0.0f, 48.0f, true);
}

void CSceneTitle::playTitleGirlAnim(u16 animID) {
    if (!mTitleGirlPos.getIsTicking() && mTitleGirlAnim->getEnabled()) {
        mTitleGirlAnim->setAnim(animID);
        if (sRandom.nextU32(5) == 0) {
            mTitleGirlAnim->setAnim(3);
        }
    }
}

void CSceneTitle::setupLayouts(s32 arg) {
    if (arg == 0) {
        gLayoutManager->fn_801D6BB0();
        gLayoutManager->_28();
        gLayoutManager->_20(1);
        gLayoutManager->_24(55, "");
        gLayoutManager->fn_801D6AEC(1);
        gLayoutManager->registerLayout<Title::CMyLayoutT>();
    } else if (arg == 1) {
        f32 frame = static_cast<Title::CMyLayoutT *>(gLayoutManager->fn_801D6C50(0))->getBackgroundLoopFrame();

        gLayoutManager->fn_801D6BB0();
        gLayoutManager->_28();
        gLayoutManager->_20(2);
        gLayoutManager->_24(94, "");
        gLayoutManager->_24(56, "");
        gLayoutManager->fn_801D6AEC(3);

        gLayoutManager->fn_801D6B2C(new Title::CMyLayoutF);
        gLayoutManager->registerLayout<CCursorLayout>();
        gLayoutManager->registerLayout<CModeChangeLayout>();

        static_cast<Title::CMyLayoutF *>(gLayoutManager->fn_801D6C50(0))->setBackgroundLoopFrame(frame);
        static_cast<CCursorLayout *>(gLayoutManager->fn_801D6C50(1))->setEnabled(lbl_80320498);

        Title::CMyLayoutF::unkCallback(2, 1);
    }
}

void CSceneTitle::_18(void) {
    this->CExScene::_18();
}
