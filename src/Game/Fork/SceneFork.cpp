#include "Fork/SceneFork.hpp"

#include "Fork/MyFlow.hpp"

#include "GameManager.hpp"
#include "FileManager.hpp"
#include "InputCheckManager.hpp"
#include "TickFlowManager.hpp"
#include "CellAnimManager.hpp"
#include "LayoutManager.hpp"

#include "MyCanvas.hpp"

#include "Controller.hpp"

#include "PauseLayout.hpp"
#include "CursorLayout.hpp"
#include "BalloonLayout.hpp"

#include "SNDHandle.hpp"

#include "Mem.hpp"

#include "code_801ED7D4.hpp"

#include "cellanim/fork/ver0/rcad_fork_labels.h"
#include "cellanim/fork/2play/rcad_fork2_labels.h"

#include "cellanim/permanent/rcad_permanent_labels.h"

#include "rev_tengoku.rsid"

namespace Fork {

s32 sceneVer;

} // namespace Fork

void CSceneFork::fn_8004BE3C(u32 arg1) {
    CSceneFork *scene = gGameManager->getCurrentScene<CSceneFork>(); 
    scene->fn_8004C91C(arg1, 2);
}

SCENE_IMPL_CREATE_FN(CSceneFork)

void CSceneFork::fn_8004BEB4(void) {
    fn_801D369C(eHeapGroup_SceneAsset);

    if (gFileManager->fn_801D42E0(19)) {
        if (Fork::sceneVer == 0) {
            gFileManager->fn_801D3F94(19, "content2/cellanim/fork/ver0/cellanim.szs");
        }
        else if (Fork::sceneVer == 1 || Fork::sceneVer == 2 || Fork::sceneVer == 3) {
            gFileManager->fn_801D3F94(19, "content2/cellanim/fork/2play/cellanim.szs");
        }
        else if (Fork::sceneVer == 4) {
            gFileManager->fn_801D3F94(19, "content2/cellanim/fork/remix02/cellanim.szs");
        }
        else if (Fork::sceneVer == 5) {
            gFileManager->fn_801D3F94(19, "content2/cellanim/fork/remix09/cellanim.szs");
        }
        else if (Fork::sceneVer == 6) {
            gFileManager->fn_801D3F94(19, "content2/cellanim/fork/remix10/cellanim.szs");
        }
    }

    fn_801D3644();
}

void CSceneFork::_10(void) {
    fn_8004BEB4();
}

void CSceneFork::fn_8004BFD0(void) {
    if (Fork::sceneVer >= 0 && Fork::sceneVer <= 6) {
        gFileManager->fn_801D41CC(19);
    }
}

bool CSceneFork::_24(void) {
    return gFileManager->fn_801D42FC(19);
}

extern "C" void fn_800D2920(u16, u16, u16, BOOL, BOOL, SNDHandle *);

extern CCellAnim *lbl_80320204; // gPermanentAnimP1
extern CCellAnim *lbl_80320208; // gPermanentAnimP2

void CSceneFork::_14(void) {
    this->CExScene::_14();

    mActiveFork = 0;

    mIsTwoPlay = Fork::sceneVer == 3;

    mObjDirection = 0;
    mForkCount = (Fork::sceneVer == 3) ? 2 : 1;

    gInputCheckManager->setUnk418(fn_8004BE3C);

    gInputCheckManager->setUnk42B(mIsTwoPlay);

    gTickFlowManager->registerFlow<Fork::CMyFlow>();

    gControllerManager->fn_801D5FF0(0)->fn_801D5500((1 << 11), 2);
    gControllerManager->fn_801D5FF0(1)->fn_801D5500((1 << 11), 2);

    void *fork_brcadAddr = gFileManager->fn_801D4270(19, "./fork.brcad");

    u32 tplLen = gFileManager->fn_801D422C(19, "./cellanim.tpl");
    void *tplAddr = gFileManager->fn_801D4270(19, "./cellanim.tpl");

    fn_801ED7D4(tplAddr);
    DCStoreRange(tplAddr, tplLen);

    gCellAnimManager->fn_801DB568(fork_brcadAddr, tplAddr, 0);

    if (Fork::sceneVer == 1 || Fork::sceneVer == 2 || Fork::sceneVer == 3) {
        void *fork2_brcadAddr = gFileManager->fn_801D4270(19, "./fork2.brcad");
        gCellAnimManager->fn_801DB568(fork2_brcadAddr, tplAddr, 0);
    }

    CCellAnim *bgAnim = gCellAnimManager->fn_801DBE7C(0, fork_bg_00);
    bgAnim->fn_801DCF94(5000);

    mBalloonAnim = gCellAnimManager->fn_801DBE7C(0, fork_balloon);
    mBalloonAnim->setPos(-140.0f, -20.0f);
    mBalloonAnim->fn_801DCF94(3000);

    mHandAnim = gCellAnimManager->fn_801DBE7C(0, fork_hand_set_00);
    mHandAnim->setPos(-140.0f, -20.0f);
    mHandAnim->fn_801DCF94(4000);
    mHandAnim->fn_801DCF38();

    mHandFarAnim = gCellAnimManager->fn_801DBE7C(0, fork_hand_flick_far);
    mHandFarAnim->setPos(-140.0f, -20.0f);
    mHandFarAnim->fn_801DCF94(4000);
    mHandFarAnim->fn_801DCF38();

    if (mIsTwoPlay) {
        for (s32 i = 0; i < 2; i++) {
            mObjAnim[i] = gCellAnimManager->fn_801DBE7C(0, fork_obj_00_far);
            mObjAnim[i]->setEnabled(false);
        }
    }

    for (s32 i = 0; i < mForkCount; i++) {
        mFork[i].mAnimBase = gCellAnimManager->fn_801DBE7C(255, permanent_canvas);
        gCellAnimManager->fn_801DC164(mFork[i].mAnimBase);

        mFork[i].mForkAnim = gCellAnimManager->fn_801DBE7C(i, fork_fork_sting);
        mFork[i].mForkAnim->setPos(100.0f, 180.0f);
        mFork[i].mForkAnim->fn_801DCF94(2000);
        mFork[i].mForkAnim->fn_801DCF38();

        for (u8 j = 0; j < (u8)ARRAY_LENGTH(mFork[i].mObjStingAnim); j++) {
            mFork[i].mObjStingAnim[j] = gCellAnimManager->fn_801DBE7C(i, fork_obj_00_sting_just);
            mFork[i].mObjStingAnim[j]->fn_801DCF94(1000);
            mFork[i].mObjStingAnim[j]->setEnabled(false);
        }

        gMyCanvasManager->fn_8007BEF8(2);
    }

    if (mIsTwoPlay) {
        mBalloonAnim->setPosX(mBalloonAnim->getPosX() + 140.0f);
        mHandAnim->setPosX(mHandAnim->getPosX() + 140.0f);
        mHandFarAnim->setPosX(mHandFarAnim->getPosX() + 140.0f);

        mFork[0].mAnimBase->setPosX(mFork[0].mAnimBase->getPosX() - 140.0f);
        mFork[1].mAnimBase->setPosX(mFork[0].mAnimBase->getPosX() + 140.0f);

        mFork[0].mAnimBase->setScale(-1.0f, 1.0f);
        mFork[1].mAnimBase->setScale(-1.0f, 1.0f);

        lbl_80320204->setEnabled(true);
        lbl_80320208->setEnabled(true);

        lbl_80320204->setPos(-180.0f, 190.0f);
        lbl_80320208->setPos(180.0f, 190.0f);
    }

    if (Fork::sceneVer == 1 || Fork::sceneVer == 2) {
        mBalloonAnim->setID(1);
        mHandAnim->setID(1);

        if (Fork::sceneVer == 1) {
            lbl_80320204->setEnabled(true);
            lbl_80320204->setPos(40.0f, 190.0f);
        }
        else if (Fork::sceneVer == 2) {
            mFork[0].mForkAnim->setID(1);

            lbl_80320208->setEnabled(true);
            lbl_80320208->setPos(40.0f, 190.0f);
        }
    }

    gLayoutManager->_20(1);
    gLayoutManager->_24(94, ""); // layout_common

    gLayoutManager->fn_801D6AEC(3);
    gLayoutManager->registerLayout<CPauseLayout>();      // Index 0
    gLayoutManager->registerLayout<CCursorLayout>();     // Index 1
    gLayoutManager->registerLayout<CBalloonLayout>();    // Index 2

    gLayoutManager->getLayout<CBalloonLayout>(2)->fn_8003A1D0(
        0, 0.0f, 80.0f, 480.0f, 120.0f, false, 0.0f, 0.0f, false
    );

    mPauseLayout = gLayoutManager->getLayout<CPauseLayout>(0);

    gLayoutManager->getLayout<CPauseLayout>(0)->setDrawEnable(false);
    gLayoutManager->getLayout<CCursorLayout>(1)->setDrawEnable(false);

    mObj3D = new CPseudo3D;
    mObj3D->fn_80072A14(-140.0f, -20.0f, 0.1f, -20.0f, 80.0f, 1.0f);

    for (s32 i = 0; i < mForkCount; i++) {
        mFork[i].mObjStingCount = 0;
        mFork[i].mForkIsOut = false;
        mFork[i].mUnk1D = 0;
    }

    mNextHandSet = 0;
}

void CSceneFork::_28(void) {
    CController *controller = gControllerManager->fn_801D5FF0(0);

    CFork *fork = mFork;
    for (s32 i = 0; i < mForkCount; i++, fork++) {
        if (fork->mForkIsOut && !fork->mForkAnim->getPlaying()) {
            if ((i == 0 && mActiveFork != 2) || (i == 1 && mActiveFork != 1)) {
                fork->mForkAnim->fn_801DD0AC(fork_fork_in);
            }

            s32 eatObjCount = 0;
            for (s32 j = 0; j < (s32)ARRAY_LENGTH(fork->mObjStingAnim); j++) {
                CCellAnim *objStingAnim = fork->mObjStingAnim[j];

                u16 animID = objStingAnim->getAnimID();
                if (
                    (
                        animID == fork_obj_01_sting_just || animID == fork_obj_02_sting_just ||
                        animID == fork_obj_03_sting_just
                    ) && objStingAnim->getEnabled()
                ) {
                    eatObjCount++;
                }

                objStingAnim->setBase(fork->mAnimBase, 0, false);
                objStingAnim->setEnabled(false);
            }

            if (eatObjCount == 3) {
                fn_800D2920(
                    SE_KUSI_EAT_HAMB, SE_KUSI_EAT_HAMB_1P, SE_KUSI_EAT_HAMB_2P,
                    mIsTwoPlay, i == 0, NULL
                );
            }
            else if (fork->mUnk1D == 0) {
                if (fork->mObjStingCount > 0) {
                    fn_800D2920(
                        SE_KUSI_EAT, SE_KUSI_EAT_1P, SE_KUSI_EAT_2P,
                        mIsTwoPlay, i == 0, NULL
                    );
                }
            }
            else {
                fn_800D2920(
                    SE_KUSI_EAT_MISS, SE_KUSI_EAT_MISS_1P, SE_KUSI_EAT_MISS_2P,
                    mIsTwoPlay, i == 0, NULL
                );
            }

            fork->mObjStingCount = 0;
            fork->mForkIsOut = false;
            fork->mUnk1D = false;
        }
    }

    fn_8004CDE4();
}

void CSceneFork::_1C(void) {
    this->CExScene::_1C();
}

void CSceneFork::_20(void) {
    delete mObj3D;

    for (s32 i = 0; i < mForkCount; i++) {
        gCellAnimManager->fn_801DBFA0(mFork[i].mAnimBase);
    }

    gCellAnimManager->fn_801DBA98(0);
    gCellAnimManager->fn_801DC068(0);

    if (Fork::sceneVer == 1 || Fork::sceneVer == 2 || Fork::sceneVer == 3) {
        gCellAnimManager->fn_801DBA98(1);
        gCellAnimManager->fn_801DC068(1);
    }

    this->CExScene::_20();
}

void CSceneFork::fn_8004C91C(u32 arg1, s32 arg2) {
    s32 forkIndex;

    if (arg1 == 4) {
        forkIndex = 0;
    }
    else if (arg1 == 32) {
        forkIndex = 1;
    }
    else {
        return;
    }

    // WTH? If you duplicate the code, it almost works ??
    /*
    if (arg1 == 4) {
        forkIndex = 0;
    }
    else if (arg1 == 32) {
        forkIndex = 1;
    }
    else {
        return;
    }
    */

    if (!mFork[forkIndex].mForkIsOut) {
        mFork[forkIndex].mForkAnim->fn_801DD0AC(fork_fork_sting);

        if (arg2 == 0) {
            fn_800D2920(
                SE_KUSI_GOOD, SE_KUSI_GOOD_1P, SE_KUSI_GOOD_2P,
                mIsTwoPlay, arg1 == 4, NULL
            );
        }
        else if (arg2 == 1) {
            fn_800D2920(
                SE_KUSI_OSII, SE_KUSI_OSII_1P, SE_KUSI_OSII_2P,
                mIsTwoPlay, arg1 == 4, NULL
            );

            fn_800D2920(
                SE_MISS, SE_KUSI_MISS_1P, SE_KUSI_MISS_2P,
                mIsTwoPlay, arg1 == 4, NULL
            );
        }
        else if (arg2 == 2) {
            if (mFork[forkIndex].mObjStingCount > 0) {
                fn_800D2920(
                    SE_KUSI_SWING_MAME, SE_KUSI_SWING_MAME_1P, SE_KUSI_SWING_MAME_2P,
                    mIsTwoPlay, arg1 == 4, NULL
                );
            }
            else {
                fn_800D2920(
                    SE_KUSI_SWING, SE_KUSI_SWING_1P, SE_KUSI_SWING_2P,
                    mIsTwoPlay, arg1 == 4, NULL
                );
            }
        }
    }
}
