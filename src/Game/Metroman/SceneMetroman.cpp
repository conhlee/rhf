#include "Metroman/SceneMetroman.hpp"

#include "Metroman/MyFlow.hpp"

#include <revolution/OS.h>

#include "GameManager.hpp"

#include "Sound.hpp"

#include "FileManager.hpp"

#include "InputCheckManager.hpp"

#include "CellAnimManager.hpp"

#include "LayoutManager.hpp"

#include "Controller.hpp"

#include "SaveData.hpp"

#include "PauseLayout.hpp"
#include "CursorLayout.hpp"
#include "BalloonLayout.hpp"

#include "TPL.hpp"

#include "rev_tengoku.rsid"

#include "cellanim/metroman/ver0/rcad_metroman_00_labels.h"
#include "cellanim/metroman/ver0/rcad_metroman_bg_labels.h"

#include "cellanim/endless/rcad_score_labels.h"

void CSceneMetroman::fn_800D7EBC(u32 button) {
    CSceneMetroman *scene = gGameManager->getCurrentScene<CSceneMetroman>();
    scene->fn_800D8890(button);
}

SCENE_IMPL_CREATE_FN(CSceneMetroman)

void CSceneMetroman::fn_800D7FDC(void) {
    memPushGroup(eMemGroup_SceneAsset);
    if (Metroman::sceneVer >= 0 && Metroman::sceneVer <= 2 && gFileManager->getArcFree(40)) {
        gFileManager->startArchive(40, "content2/cellanim/metroman/ver0/cellanim.szs");
    }
    memPopGroup();
}

void CSceneMetroman::_10(void) {
    fn_800D7FDC();
}

void CSceneMetroman::fn_800D80A4(void) {
    if (Metroman::sceneVer >= 0 && Metroman::sceneVer <= 2) {
        gFileManager->endArchive(40);
    }
}

bool CSceneMetroman::_24(void) {
    if (Metroman::sceneVer >= 0 && Metroman::sceneVer <= 2) {
        return gFileManager->getArcReady(40);
    }
    return this->CScene::_24();
}

// TODO make public in ExScene
extern s32 lbl_803201C4;
extern s32 lbl_803201C8;

void CSceneMetroman::_14(void) {
    this->CExScene::_14();

    mUnk9C = Metroman::sceneVer == 1 || Metroman::sceneVer == 2;

    gInputCheckManager->setUnk418(fn_800D7EBC);

    gTickFlowManager->registerFlow<Metroman::CMyFlow>();

    CController *controller = gControllerManager->fn_801D5FF0(0);
    controller->fn_801D5500(WPAD_BUTTON_A, 3);

    void *metroman_00_brcadAddr = gFileManager->arcGetFileAddr(40, "./metroman_00.brcad");
    void *metroman_bg_brcadAddr = gFileManager->arcGetFileAddr(40, "./metroman_bg.brcad");

    u32 metroman_tplLen = gFileManager->arcGetFileLen(40, "./cellanim.tpl");
    void *metroman_tplAddr = gFileManager->arcGetFileAddr(40, "./cellanim.tpl");

    UserTPLBind(metroman_tplAddr);
    DCStoreRange(metroman_tplAddr, metroman_tplLen);

    gCellAnimManager->registBank(metroman_00_brcadAddr, metroman_tplAddr, 0);
    gCellAnimManager->registBank(metroman_bg_brcadAddr, metroman_tplAddr, 1);

    fn_80009A64(true);
    fn_80009A70(0xFF);
    fn_80009A7C(0xE0, 0xE0, 0xE0);
    fn_80009A90(9999);

    mManAnim = gCellAnimManager->createCellAnim(0, metroman_00_man_up_R);
    mManAnim->goToAnimEnd();
    mManAnim->setLayer(2000);
    mManAnim->setPos(0.0f, -40.0f);

    mManShadowAnim = gCellAnimManager->createCellAnim(0, metroman_00_man_shadow);
    mManShadowAnim->setFrameAtKey(1);
    mManShadowAnim->setLayer(3000);
    mManShadowAnim->setPos(mManAnim->getPos());

    mManNextDirection = 2;
    mManIsLeft = true;

    mHandAnim = gCellAnimManager->createCellAnim(0, metroman_00_hand);
    mHandAnim->setLayer(2500);
    mHandAnim->setPos(0.0f, 120.0f);

    mHandRotation.setEasing(2, 2);
    mHandRotation.fn_801EA698(-60.0f, 60.0f);
    mHandRotation.fn_801EA550(0.0f, 48.0f, true);
    mHandRotation.fn_801EA50C(mHandRotation.getMaxTime());

    mHandDirection = 0;
    mUnk84 = false;

    mLampAnim = gCellAnimManager->createCellAnim(0, metroman_00_lamp_0);
    mLampAnim->setBase(mManAnim, 3, false);
    mLampAnim->setLayer(mManAnim->getLayer() - 1);
    mLampAnim->goToAnimEnd();

    mSecretCodeAnim = gCellAnimManager->createCellAnim(0, metroman_00_secret_code);
    mSecretCodeAnim->setBase(mManAnim, 3, false);
    mSecretCodeAnim->setLayer(1500);
    mSecretCodeAnim->setEnabled(false);

    mUnk9E = false;
    mLampSize = 0;

    mLampAnim->switchAnim(metroman_00_lamp_0); // Unnessecary?
    mSecretCodeAnim->setEnabled(false);

    mUnkA0 = 0;

    gLayoutManager->_20(1);
    gLayoutManager->_24(94, ""); // layout_common

    gLayoutManager->fn_801D6AEC(3);
    gLayoutManager->registerLayout<CPauseLayout>();   // Index 0
    gLayoutManager->registerLayout<CCursorLayout>();  // Index 1
    gLayoutManager->registerLayout<CBalloonLayout>(); // Index 2

    gLayoutManager->getLayout<CBalloonLayout>(2)->fn_8003A1D0(
        0, 0.0f, 60.0f, 480.0f, 120.0f, false, 0.0f, 0.0f, false
    );

    mPauseLayout = gLayoutManager->getLayout<CPauseLayout>(0);

    gLayoutManager->getLayout<CPauseLayout>(0)->setDrawEnable(false);
    gLayoutManager->getLayout<CCursorLayout>(1)->setDrawEnable(false);

    fn_8000A7AC(-2, score_game_over, score_hiscore, -1, 1001);

    mScoreCounter = new CEndlessCounter(6, 0, -2, score_number_00_L, false, 2, 20.0f);
    mScoreCounter->fn_80083CB8(1000);
    
    mScoreCounter->fn_80083C5C(260.0f, -145.0f);
    mScoreCounter->fn_8008397C(0);
    mScoreCounter->fn_80083CAC(mUnk9C);

    mHiScoreCounter = new CEndlessCounter(6, 0, -2, score_number_00_S, false, 2, 14.0f);
    mHiScoreCounter->fn_80083CB8(1000);
    mHiScoreCounter->fn_80083C5C(260.0f, -180.0f);
    mHiScoreCounter->fn_8008397C(gSaveData->getSelectedSaveFile()->fn_80077E40(3));
    mHiScoreCounter->fn_80083CAC(mUnk9C);

    mHiScoreRecordAnim = gCellAnimManager->createCellAnim(-2, score_hiscore_record);
    mHiScoreRecordAnim->setLayer(1000);
    mHiScoreRecordAnim->setPos(110.0f, -180.0f);
    mHiScoreRecordAnim->setEnabled(mUnk9C);

    if (Metroman::sceneVer == 2) {
        mScoreCounter->fn_8008397C(lbl_803201C4);
        mHiScoreCounter->fn_8008397C(lbl_803201C8);

        s32 highScore = gSaveData->getSelectedSaveFile()->fn_80077E40(4);
        if (mScoreCounter->fn_80083C54() > highScore) {
            fn_8000A94C(true, mScoreCounter->fn_80083C54());
        }
    }

    mUnkA8 = 0;
    mUnk9D = false;
}

void CSceneMetroman::_28(void) {
    CController *controller = gControllerManager->fn_801D5FF0(0);

    if (mHandRotation.getIsTicking()) {
        f32 timeBefore = mHandRotation.getLinearTime();
        mHandRotation.fn_801E9DE8(FALSE);
        f32 timeAfter = mHandRotation.getLinearTime();

        f32 halfway = mHandRotation.getMaxTime() / 2.0f;

        if ((timeBefore <= halfway) && (timeAfter >= halfway)) {
            fn_800D8A78(0, 0);
            gSoundManager->play(SE_URA_MAN_GUIDE_SOUND);
        }
    }
    mHandAnim->setAngle(mHandRotation.getCurrent());

    if (mUnkA0 > 0) {
        mUnkA0--;
        if (mUnkA0 == 0) {
            mLampAnim->switchAnim(metroman_00_lamp_0);
            mSecretCodeAnim->setEnabled(false);
            mUnkA0 = 0;
        }
    }
}

void CSceneMetroman::_1C(void) {
    this->CExScene::_1C();
}

void CSceneMetroman::_20(void) {
    fn_8000A864();

    if (Metroman::sceneVer == 2) {
        lbl_803201C4 = mScoreCounter->fn_80083C54();
        lbl_803201C8 = mHiScoreCounter->fn_80083C54();
    }

    delete mScoreCounter;
    delete mHiScoreCounter;

    gCellAnimManager->endCellAnim(mHiScoreRecordAnim);

    gCellAnimManager->endBank(0);
    gCellAnimManager->endBank(1);

    gCellAnimManager->endCellAnimByBank(0);
    gCellAnimManager->endCellAnimByBank(1);
    
    if (Metroman::sceneVer != 2 && (Metroman::sceneVer >= 0 && Metroman::sceneVer <= 2)) {
        gFileManager->endArchive(40);
    }

    this->CExScene::_20();
}

void CSceneMetroman::fn_800D8890(u32 button) {
    if (button == eInputType_TriggerA) {
        switch (mManNextDirection) {
        case 1:
            mManAnim->setAnim(metroman_00_man_up_R);
            mManShadowAnim->setFrameAtKey(1);
            mManIsLeft = true;
            mManNextDirection = 2;
            break;
        case 2:
            mManAnim->setAnim(metroman_00_man_up_L);
            mManShadowAnim->setFrameAtKey(2);
            mManIsLeft = false;
            mManNextDirection = 1;
            break;
        default:
            break;
        }
        gSoundManager->play(SE_URA_MAN_FOOT);
    }
}

void CSceneMetroman::fn_800D8940(s32 handDirection) {
    if (mManIsLeft == FALSE) {
        if (handDirection == 0) {
            mManAnim->setAnim(metroman_00_man_hit_R_from_L);
        }
        else {
            mManAnim->setAnim(metroman_00_man_hit_R_from_R);
        }
    }
    else if (handDirection == 1) {
        mManAnim->setAnim(metroman_00_man_hit_L_from_R);
    }
    else {
        mManAnim->setAnim(metroman_00_man_hit_L_from_L);
    }

    bool shadowFront = mHandDirection == 1;
    mManShadowAnim->setFrameAtKey(3);
    mManShadowAnim->setScale(shadowFront ? 1.0f : -1.0f, 1.0f);

    mUnk84 = true;

    mLampAnim->switchAnim(metroman_00_lamp_0);
    mSecretCodeAnim->setEnabled(false);

    mUnkA0 = 0;
    mUnk9E = false;

    if (mUnk9C) {
        fn_8000A8C4(true);
    }

    fn_80009ABC();

    gInputCheckManager->setUnk429(false);

    gSoundManager->play(SE_URA_MAN_MISS);
}

void CSceneMetroman::fn_800D8A78(s32 a, s32 b) {
    if (a != 0) {
        mLampAnim->setAnim(metroman_00_lamp_0);
    }
    else if (mUnk9E) {
        if (b) {
            switch (mLampSize) {
            case 0:
                mLampAnim->setAnim(metroman_00_lamp_0);
                break;
            case 1:
                mLampAnim->setAnim(metroman_00_lamp_1);
                break;
            case 2:
                mLampAnim->setAnim(metroman_00_lamp_2);
                break;
            case 3:
                mLampAnim->setAnim(metroman_00_lamp_3);
                break;
            case 4:
            default:
                mLampAnim->setAnim(metroman_00_lamp_4);
                break;
            }
        }
    }
    else if (!mUnk9D) {
        mLampAnim->setAnim(metroman_00_lamp_0);
        mLampAnim->switchAnim(metroman_00_lamp_0);
        mSecretCodeAnim->setEnabled(false);
        mUnkA0 = 0;
    }
}

void CSceneMetroman::fn_800D8B78(void) {
    gSoundManager->play(SE_URA_MAN_GUIDE_SOUND);
}

void CSceneMetroman::fn_800D8B8C(f32 ticks) {
    if (!mUnk84) {
        // TODO: what ??
        f32 target = mHandRotation.getTarget();
        f32 end = mHandRotation.getOutputEnd();
        mHandRotation.fn_801EA698(target, end);
        mHandRotation.fn_801EA550(0.0f, ticks, true);
        mHandDirection = 1 - mHandDirection;
        gSoundManager->play(SE_URA_MAN_METRO_KA);
    }
}

void CSceneMetroman::fn_800D8C18(s32 add) {
    if (mUnk9C) {
        mScoreCounter->fn_80083A80(add);
        if (mScoreCounter->fn_80083C54() > mHiScoreCounter->fn_80083C54()) {
            fn_8000A94C(true, mScoreCounter->fn_80083C54());
            mHiScoreCounter->fn_8008397C(mScoreCounter->fn_80083C54());
        }
    }
}

u16 CSceneMetroman::fn_800D8CA4(u16 lampSize) {
    mSecretCodeAnim->setAnimFreezeFrame(metroman_00_secret_code, 0.0f);
    mSecretCodeAnim->setFrameAtKey(lampSize);

    mSecretCodeAnim->setPos(0.0f, 0.0f);

    mUnkA0 = 60;

    return mSecretCodeAnim->getFinalKeyIndex();
}

void CSceneMetroman::fn_800D8D1C(void) {
    mLampAnim->switchAnim(metroman_00_lamp_0);
    mSecretCodeAnim->setEnabled(false);

    mUnkA0 = 0;
}

void CSceneMetroman::_18(void) {
    this->CExScene::_18();
}
