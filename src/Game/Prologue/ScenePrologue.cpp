#include "Prologue/ScenePrologue.hpp"

#include "Prologue/MyLayout.hpp"
#include "Prologue/MyFlow.hpp"

#include "Grading/SceneGrading.hpp"

#include <cstdio>

#include "FileManager.hpp"
#include "SceneManager.hpp"
#include "BackupManager.hpp"
#include "CellAnimManager.hpp"
#include "LayoutManager.hpp"
#include "TickFlowManager.hpp"

#include "SaveData.hpp"

#include "Mem.hpp"

#include "TPL.hpp"

#include "cellanim/prologue/rcad_prologue_labels.h"

static char lbl_8032A278[0x100];
static u32 lbl_80320238;

s32 Prologue::sceneVer;

SCENE_IMPL_CREATE_FN(CScenePrologue)

void CScenePrologue::fn_8000AD98(void) {
    memPushGroup(eMemGroup_SceneAsset);
    if (gFileManager->getArcFree(51)) {
        gFileManager->startArchive(51, "content2/cellanim/prologue/cellanim.szs");
        gFileManager->startArchive(91, lbl_8032A278);
    }
    memPopGroup();
}

void CScenePrologue::_10(void) {
    fn_8000AD98();
}

bool CScenePrologue::_24(void) {
    return gFileManager->getArcReady(51) && gFileManager->getArcReady(91);
}

void CScenePrologue::_14(void) {
    this->CExScene::_14();

    if (gSceneManager->fn_8008B27C() && !fn_80009FB4()) {
        const char *nightWalkName = "nightWalk";
        if (strstr(lbl_8032A278, nightWalkName) == NULL) {
            if (gSaveData->getSelectedSaveFile()->fn_80077E08() != 0) {
                gSaveData->getSelectedSaveFile()->fn_800786E4();
                gSaveData->updateSnapshot();
                if (getPerfectEligible() && (gBackupManager != NULL)) {
                    gBackupManager->fn_80084FC8(1);
                } 
                Grading::fn_8000C5F4(false);
            }
            else {
                Grading::fn_8000C5F4(true);
            }
        }
    }

    void *brcadAddr = gFileManager->arcGetFileAddr(51, "./prologue.brcad");

    u32 tplLen = gFileManager->arcGetFileLen(51, "./cellanim.tpl");
    void *tplAddr = gFileManager->arcGetFileAddr(51, "./cellanim.tpl");

    UserTPLBind(tplAddr);
    DCStoreRange(tplAddr, tplLen);

    gCellAnimManager->registBank(brcadAddr, tplAddr, 0);

    CCellAnim *maskAnim = gCellAnimManager->createCellAnim(0, prologue_mask);

    maskAnim->setLayer(-1);

    gLayoutManager->_20(1);
    gLayoutManager->_24(91, "");

    gLayoutManager->fn_801D6AEC(1);
    gLayoutManager->registerLayout<Prologue::CMyLayout>();

    gTickFlowManager->registerFlow<Prologue::CMyFlow>();

    memStub_801D3638(300);
}

void CScenePrologue::_28(void) {}

void CScenePrologue::_1C(void) {
    this->CExScene::_1C();
}

void CScenePrologue::_20(void) {
    gCellAnimManager->endBank(0);
    gCellAnimManager->endCellAnimByBank(0);

    gFileManager->endArchive(51);
    gFileManager->endArchive(91);

    this->CExScene::_20();
}

void Prologue::fn_8000B13C(char *name, u32 ver) {
    sprintf(lbl_8032A278, "content2/layout/layout_prologue_%s_ver%d.szs", name, ver);
}

void Prologue::fn_8000B160(char *name) {
    sprintf(lbl_8032A278, "content2/layout/layout_prologue_%s_2play.szs", name);
}

void Prologue::fn_8000B180(u32 arg0) {
    lbl_80320238 = arg0;
}

u32 Prologue::fn_8000B188(void) {
    return lbl_80320238;
}

void CScenePrologue::_18(void) {
    this->CExScene::_18();
}
