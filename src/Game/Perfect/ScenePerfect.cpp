#include "Perfect/ScenePerfect.hpp"

#include "Perfect/MyFlow.hpp"

#include "Perfect/MyLayout.hpp"

#include "FileManager.hpp"

#include "SceneManager.hpp"

#include "InputCheckManager.hpp"

#include "SaveData.hpp"

#include "BackupManager.hpp"

#include "TickFlowManager.hpp"

#include "LayoutManager.hpp"

#include "Controller.hpp"

void CScenePerfect::fn_80082FAC(u32) {}

SCENE_IMPL_CREATE_FN(CScenePerfect)

void CScenePerfect::fn_80083014(void) {
    fn_801D369C(eHeapGroup_SceneAsset);
    if (gFileManager->fn_801D42E0(55)) {
        gFileManager->fn_801D3F94(55, "content2/layout/layout_perfect.szs");
    }
    fn_801D3644();
}

void CScenePerfect::_10(void) {
    fn_80083014();
}

bool CScenePerfect::_24(void) {
    return this->CScene::_24();
}

void CScenePerfect::_14(void) {
    this->CExScene::_14();

    if (gSceneManager->fn_8008B27C()) {
        // TODO: fakematch
        CSaveData *saveData = gSaveData;
        saveData->do80077BC8(5);

        for (s32 i = 0; i < 3; i++) {
            gSaveData->fn_80078F4C()->fn_800786E4();
        }

        if (gSaveData->fn_80078F4C()->fn_80078238() == (50 - 1)) {
            gSaveData->fn_80078F4C()->fn_80077CEC(true);
        }

        gSaveData->fn_80078F68();

        if (gBackupManager != NULL) {
            gBackupManager->fn_80084FC8(1);
        }
    }

    gInputCheckManager->setUnk418(fn_80082FAC);

    gTickFlowManager->registerFlow<Perfect::CMyFlow>();

    gLayoutManager->_20(1);
    gLayoutManager->_24(55, "");

    gLayoutManager->fn_801D6AEC(1);
    gLayoutManager->registerLayout<Perfect::CMyLayout>();
}

void CScenePerfect::_28(void) {
    CController *controller = gControllerManager->fn_801D5FF0(0);
}

void CScenePerfect::_1C(void) {
    this->CExScene::_1C();
}

void CScenePerfect::_20(void) {
    gFileManager->fn_801D41CC(55);

    this->CExScene::_20();
}

void CScenePerfect::fn_8008328C(u32, u32) {}

void CScenePerfect::_18(void) {
    this->CExScene::_18();
}

