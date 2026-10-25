#include "Grading/SceneGrading.hpp"

#include "Grading/MyLayout.hpp"

#include "Grading/MyFlow.hpp"

#include "FileManager.hpp"

#include "LayoutManager.hpp"

#include "SceneManager.hpp"

#include "SaveData.hpp"

#include "BackupManager.hpp"

s32 Grading::sceneVer;

static bool lbl_80320264;

SCENE_IMPL_CREATE_FN(CSceneGrading)

void CSceneGrading::fn_8000C348(void) {
    fn_80008EFC();

    memPushGroup(eMemGroup_SceneAsset);
    if (gFileManager->getArcFree(93)) {
        gFileManager->startArchive(93, "content2/layout/layout_grading.szs");
    }
    memPopGroup();
}

void CSceneGrading::_10(void) {
    fn_8000C348();
}

void CSceneGrading::_14(void) {
    this->CExScene::_14();

    mGrade = gCheckPointManager->getUnk2FC();
    mBut = gCheckPointManager->fn_801EB800();

    if (gSceneManager->fn_8008B27C()) {
        ELevelState levelState =
            (mGrade == eGrade_HI) ? eLevelState_CompleteHI :
            (mGrade == eGrade_OK) ? eLevelState_CompleteOK :
            eLevelState_AttemptNG;

        // TODO: fakematch
        CSaveData *saveData = gSaveData;
        saveData->do80077BC8(levelState);

        if (lbl_80320264) {
            gSaveData->fn_80078F4C()->fn_80078808();
        }

        gSaveData->fn_80078F68();

        if (gBackupManager != NULL) {
            gBackupManager->fn_80084FC8(1);
        }
    }

    gLayoutManager->_20(1);
    gLayoutManager->_24(93, ""); // layout_grading

    gLayoutManager->fn_801D6AEC(1);
    gLayoutManager->registerLayout<Grading::CMyLayout>(); // Index 0

    mLayout = gLayoutManager->getLayout<Grading::CMyLayout>(0);

    gTickFlowManager->registerFlow<Grading::CMyFlow>();

    memStub_801D3638(300);
}

void CSceneGrading::_28(void) {}

void CSceneGrading::_1C(void) {
    this->CExScene::_1C();
}

void CSceneGrading::_20(void) {
    gFileManager->endArchive(93);
    this->CExScene::_20();
}

void Grading::fn_8000C5F4(bool param_1) {
    lbl_80320264 = param_1;
}

void CSceneGrading::_18(void) {
    this->CExScene::_18();
}
