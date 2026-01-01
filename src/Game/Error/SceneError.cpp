#include "Error/SceneError.hpp"

#include "Error/MyChecker.hpp"

#include "Error/MyFlow.hpp"

#include "Error/MyLayout.hpp"

#include <revolution/OS.h>
#include <revolution/VI.h>

#include "FileManager.hpp"

#include "InputCheckManager.hpp"

#include "TickFlowManager.hpp"

#include "LayoutManager.hpp"

#include "CursorLayout.hpp"
#include "ModeChangeLayout.hpp"

#include "HBMManager.hpp"

#include "BackupManager.hpp"

#include "Controller.hpp"

namespace {

// A dummy scene is created to force the destruction of the
// current scene (CSceneError).
class CDummyScene : public CScene {
public:
    virtual ~CDummyScene(void);
    virtual void _10(void);
    virtual void _14(void);
    virtual void _18(void);
    virtual void _1C(void);
    virtual void _20(void);

    SCENE_DECL_CREATE_FN()
};

} // namespace

void CSceneError::fn_80079568(u32) {}

static bool sIsPowerOff;
static void error_powercallback(void) {
    OSReport("error_powercallback()\n");
    sIsPowerOff = true;
}

static bool sIsReset;
static void error_resetcallback(void) {
    OSReport("error_resetcallback\n");
    sIsReset = true;
}

namespace Error {

s32 sceneVer;

} // namespace Error

SCENE_IMPL_CREATE_FN(CSceneError)

void CSceneError::fn_8007963C(void) {
    fn_801D369C(eHeapGroup_SceneAsset);
    if (gFileManager->fn_801D42E0(55)) {
        gFileManager->fn_801D3F94(55, "content2/layout/layout_error.szs");
    }
    fn_801D3644();
}

void CSceneError::_10(void) {
    fn_8007963C();
}

extern bool lbl_80320498;

void CSceneError::_14(void) {
    this->CExScene::_14();

    gInputCheckManager->setUnk418(fn_80079568);

    gTickFlowManager->registerFlow<Error::CMyFlow>();

    gLayoutManager->_20(2);
    gLayoutManager->_24(94, ""); // layout_common
    gLayoutManager->_24(55, ""); // layout_error

    gLayoutManager->fn_801D6AEC(3);
    gLayoutManager->registerLayout<Error::CMyLayout>(); // Index 0
    gLayoutManager->registerLayout<CCursorLayout>(); // Index 1
    gLayoutManager->registerLayout<CModeChangeLayout>(); // Index 2

    gLayoutManager->getLayout<CCursorLayout>(1)->setEnabled(lbl_80320498);

    Error::CMyLayout::fn_8007A060(2, 1);

    gHBMManager->setUnk416(false);

    if (gBackupManager->getUnk4C() == 7) {
        gLayoutManager->getLayout<CCursorLayout>(1)->setDrawEnable(false);

        OSSetPowerCallback(NULL);
        OSSetResetCallback(NULL);
    }
    else {
        OSSetPowerCallback(error_powercallback);
        OSSetResetCallback(error_resetcallback);
    }

    fn_8000A708(1);

    VISetBlack(FALSE);
    VIFlush();

    mDoExitOnEnd = false;
    mRestart = false;
    mReturnToMenu = false;
    mReturnToDataManager = false;
    mShutdownSystem = false;

    sIsPowerOff = false;
    sIsReset = false;
}

void CSceneError::_28(void) {
    CController *controller = gControllerManager->fn_801D5FF0(0);

    if (sIsPowerOff) {
        sIsPowerOff = false;

        gLayoutManager->getLayout<Error::CMyLayout>(0)->setUpdateEnable(false);
        mShutdownSystem = true;
        mDoExitOnEnd = true;

        OSSetPowerCallback(NULL);
        OSSetResetCallback(NULL);

        gGameManager->_20(CDummyScene::create, eHeapGroup_SceneEx);
    }
    if (sIsReset) {
        sIsReset = false;

        gLayoutManager->getLayout<Error::CMyLayout>(0)->setUpdateEnable(false);
        mRestart = true;
        mDoExitOnEnd = true;

        OSSetPowerCallback(NULL);
        OSSetResetCallback(NULL);

        gGameManager->_20(CDummyScene::create, eHeapGroup_SceneEx);
    }
}

void CSceneError::_1C(void) {
    this->CExScene::_1C();
}

void CSceneError::_20(void) {
    gFileManager->fn_801D41CC(55);

    this->CExScene::_20();

    if (mDoExitOnEnd) {
        VISetBlack(TRUE);
        VIFlush();
        VIWaitForRetrace();
        VIWaitForRetrace();

        if (mRestart) {
            OSReport("OSRestart()\n");
            OSRestart(0);
        }
        else if (mReturnToMenu) {
            OSReport("OSReturnToMenu()\n");
            OSReturnToMenu();
        }
        else if (mReturnToDataManager) {
            OSReport("OSReturnToDataManager()\n");
            OSReturnToDataManager();
        }
        else if (mShutdownSystem) {
            OSReport("OSShutdownSystem()\n");
            OSShutdownSystem();
        }
    }

    Error::sceneVer = 0;
}

void CSceneError::fn_80079B3C(u32, u32) {}

void CSceneError::fn_80079B40(void) {
    mRestart = true;
    mDoExitOnEnd = true;

    OSSetPowerCallback(NULL);
    OSSetResetCallback(NULL);

    gGameManager->_20(CDummyScene::create, eHeapGroup_SceneEx);
}

void CSceneError::fn_80079B98(void) {
    mReturnToMenu = true;
    mDoExitOnEnd = true;

    OSSetPowerCallback(NULL);
    OSSetResetCallback(NULL);

    gGameManager->_20(CDummyScene::create, eHeapGroup_SceneEx);
}

void CSceneError::fn_80079BF0(void) {
    mReturnToDataManager = true;
    mDoExitOnEnd = true;

    OSSetPowerCallback(NULL);
    OSSetResetCallback(NULL);

    gGameManager->_20(CDummyScene::create, eHeapGroup_SceneEx);
}

SCENE_IMPL_CREATE_FN(CDummyScene)

// TODO: Why is this implemented here ???
void CScene::_08(void) {}

void CDummyScene::_20(void) {}

void CDummyScene::_1C(void) {}

void CDummyScene::_18(void) {}

void CDummyScene::_14(void) {}

void CDummyScene::_10(void) {}

CDummyScene::~CDummyScene(void) {}

void CSceneError::_18(void) {
    this->CExScene::_18();
}
