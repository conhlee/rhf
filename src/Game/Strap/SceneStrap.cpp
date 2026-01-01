#include "Strap/SceneStrap.hpp"

#include "Strap/MyFlow.hpp"

#include "Mem.hpp"

#include <revolution/SC.h>
#include <revolution/OS.h>
#include <revolution/WPAD.h>

#include "FileManager.hpp"
#include "InputCheckManager.hpp"
#include "CellAnimManager.hpp"
#include "HBMManager.hpp"
#include "RFLManager.hpp"

#include "TickFlowManager.hpp"
#include "ExFlowDecl.hpp"

#include "Controller.hpp"

#include "TPL.hpp"

#include "cellanim/strap/verE/rcad_strap_labels.h"

namespace Strap {

s32 sceneVer;

} // namespace Strap

void CSceneStrap::fn_8007B270(u32) {}

SCENE_IMPL_CREATE_FN(CSceneStrap)

void CSceneStrap::fn_8007B2D8(void) {
    fn_801D369C(eHeapGroup_SceneAsset);

    if (gFileManager->fn_801D42E0(2)) {
        switch (SCGetLanguage()) {
        case SC_LANG_FR:
            gFileManager->fn_801D3F94(2, "content2/cellanim/strap/verF/cellanim.szs");
            break;
        case SC_LANG_SP:
            gFileManager->fn_801D3F94(2, "content2/cellanim/strap/verS/cellanim.szs");
            break;
        default:
            gFileManager->fn_801D3F94(2, "content2/cellanim/strap/verE/cellanim.szs");
            break;
        }
    }

    fn_801D3644();
}

void CSceneStrap::_10(void) {
    fn_8007B2D8();
}

bool CSceneStrap::_24(void) {
    return gFileManager->fn_801D42FC(2);
}

void CSceneStrap::_14(void) {
    this->CExScene::_14();

    gInputCheckManager->setUnk418(fn_8007B270);

    gTickFlowManager->registerFlow<Strap::CMyFlow>();

    void *brcadAddr = gFileManager->fn_801D4270(2, "./strap.brcad");

    u32 tplLen = gFileManager->fn_801D422C(2, "./cellanim.tpl");
    void *tplAddr = gFileManager->fn_801D4270(2, "./cellanim.tpl");
    UserTPLBind(tplAddr);
    DCStoreRange(tplAddr, tplLen);

    gCellAnimManager->fn_801DB568(brcadAddr, tplAddr, 0);

    mStrapAnim = gCellAnimManager->fn_801DBE7C(0, strap_fade_in);

    mStrapAnim->fn_801DCEE8(strap_show_a, NULL);
    mStrapAnim->fn_801DCF18();

    gHBMManager->setUnk416(false);

    mFrameCounter = 0;
    mFadeOpacity = -1;

    gRFLManager->fn_800C2C40();
}

TFD_EXTERN(lbl_80256044)

void CSceneStrap::_28(void) {
    CController *controller = gControllerManager->fn_801D5FF0(0);

    mFrameCounter++;
    if (mFadeOpacity > 0) {
        mFadeOpacity -= 8;
        mStrapAnim->setOpacity(mFadeOpacity);
        if (mFadeOpacity == 0) {
            gTickFlowManager->fn_801E1CC0(lbl_80256044);
        }
    }
    else if (mFadeOpacity != 0) {
        if (!mStrapAnim->getPlaying()) {
            mFadeOpacity = 0x100;
        }
        else if ((mFrameCounter >= 90) && (controller->getUnk133C() & 0x1F1F)) {
            mFadeOpacity = 0x100;
        }
    }
}

void CSceneStrap::_1C(void) {
    this->CExScene::_1C();
}

void CSceneStrap::_20(void) {
    gFileManager->fn_801D4364(1);

    gRFLManager->fn_800C2CA4();
    gRFLManager->fn_800C2E04();
    gRFLManager->fn_800C2C98();

    gHBMManager->setUnk416(true);

    gCellAnimManager->fn_801DBA98(0);
    gCellAnimManager->fn_801DC068(0);

    gFileManager->fn_801D41CC(2);

    this->CExScene::_20();
}

void CSceneStrap::fn_8007B6C4(u32, u32) {}

void CSceneStrap::_18(void) {
    this->CExScene::_18();
}
