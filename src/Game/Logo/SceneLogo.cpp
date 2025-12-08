#include "Logo/SceneLogo.hpp"

#include "Logo/MyFlow.hpp"

#include "Mem.hpp"

#include "InputCheckManager.hpp"
#include "TickFlowManager.hpp"
#include "HBMManager.hpp"

#include "TickFlowManager.hpp"
#include "ExFlowDecl.hpp"

#include "Controller.hpp"

#include "DebugPrint.hpp"

namespace Logo {

s32 sceneVer;

} // namespace Logo

void CSceneLogo::fn_800A0B5C(u32) {}

SCENE_IMPL_CREATE_FN(CSceneLogo)

void CSceneLogo::fn_800A0BC4(void) {
    fn_801D369C(eHeapGroup_SceneAsset);

    // Nothing to load ..

    fn_801D3644();
}

void CSceneLogo::_10(void) {
    fn_800A0BC4();
}

bool CSceneLogo::_24(void) {
    return this->CScene::_24();
}

void CSceneLogo::_14(void) {
    this->CExScene::_14();

    gInputCheckManager->setUnk418(fn_800A0B5C);

    gTickFlowManager->registerFlow<Logo::CMyFlow>();

    gHBMManager->setUnk416(false);

    mTimer = 0;
    mUnk78 = 0;
    mUnk7C = 0;
}

TFD_EXTERN(lbl_80256044)

void CSceneLogo::_28(void) {
    CController *controller = gControllerManager->fn_801D5FF0(0);

    mTimer++;
    if ((mTimer >= 120) && (controller->getUnk133C() & 0x800)) {
        gTickFlowManager->fn_801E1E4C();
        gTickFlowManager->fn_801E1CC0(lbl_80256044);
    }
}

void CSceneLogo::_1C(void) {
    this->CExScene::_1C();

    if (mTimer >= 120) {
        gDebugPrint->fn_801EC674(270, 300, 1, "Push A to Start");
    }
}

void CSceneLogo::_20(void) {
    gHBMManager->setUnk416(true);
    this->CExScene::_20();
}

void CSceneLogo::fn_800A0D64(u32, u32) {}

void CSceneLogo::_18(void) {
    this->CExScene::_18();
}
