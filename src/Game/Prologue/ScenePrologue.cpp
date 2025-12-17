#include "Prologue/ScenePrologue.hpp"

#include "Prologue/MyLayout.hpp"
#include "Prologue/MyFlow.hpp"

#include <cstdio>

#include "FileManager.hpp"
#include "SceneManager.hpp"
#include "BackupManager.hpp"
#include "CellAnimManager.hpp"
#include "LayoutManager.hpp"
#include "TickFlowManager.hpp"

#include "SaveData.hpp"

#include "Mem.hpp"

#include "code_801ED7D4.hpp"

#include "cellanim/prologue/rcad_prologue_labels.h"

static char lbl_8032A278[0x100];
static u32 lbl_80320238;

namespace Prologue {

s32 sceneVer;

} // namespace Prologue

SCENE_IMPL_CREATE_FN(CScenePrologue)

void CScenePrologue::fn_8000AD98(void) {
    fn_801D369C(eHeapGroup_SceneAsset);
    if (gFileManager->fn_801D42E0(51)) {
        gFileManager->fn_801D3F94(51, "content2/cellanim/prologue/cellanim.szs");
        gFileManager->fn_801D3F94(91, lbl_8032A278);
    }
    fn_801D3644();
}

void CScenePrologue::_10(void) {
    fn_8000AD98();
}

bool CScenePrologue::_24(void) {
    return gFileManager->fn_801D42FC(51) && gFileManager->fn_801D42FC(91);
}

extern "C" void fn_8000C5F4(u8);

void CScenePrologue::_14(void) {
    this->CExScene::_14();

    if (gSceneManager->fn_8008B27C() && !fn_80009FB4()) {
        const char *nightWalkName = "nightWalk";
        if (strstr(lbl_8032A278, nightWalkName) == NULL) {
            if (gSaveData->fn_80078F4C()->fn_80077E08() != 0) {
                gSaveData->fn_80078F4C()->fn_800786E4();
                gSaveData->fn_80078F68();
                if (fn_80009E1C() && (gBackupManager != NULL)) {
                    gBackupManager->fn_80084FC8(1);
                } 
                fn_8000C5F4(0);
            }
            else {
                fn_8000C5F4(1);
            }
        }
    }

    void *brcadAddr = gFileManager->fn_801D4270(51, "./prologue.brcad");

    u32 tplLen = gFileManager->fn_801D422C(51, "./cellanim.tpl");
    void *tplAddr = gFileManager->fn_801D4270(51, "./cellanim.tpl");

    fn_801ED7D4(tplAddr);
    DCStoreRange(tplAddr, tplLen);

    gCellAnimManager->fn_801DB568(brcadAddr, tplAddr, 0);

    CCellAnim *maskAnim = gCellAnimManager->fn_801DBE7C(0, prologue_mask);

    maskAnim->fn_801DCF94(-1);

    gLayoutManager->_20(1);
    gLayoutManager->_24(91, "");

    gLayoutManager->fn_801D6AEC(1);
    gLayoutManager->registerLayout<Prologue::CMyLayout>();

    gTickFlowManager->registerFlow<Prologue::CMyFlow>();

    fn_801D3638(300);
}

void CScenePrologue::_28(void) {}

void CScenePrologue::_1C(void) {
    this->CExScene::_1C();
}

void CScenePrologue::_20(void) {
    gCellAnimManager->fn_801DBA98(0);
    gCellAnimManager->fn_801DC068(0);

    gFileManager->fn_801D41CC(51);
    gFileManager->fn_801D41CC(91);

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
