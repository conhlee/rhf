#include "Rap/SceneRap.hpp"

#include "Rap/MyFlow.hpp"

#include "Rap/MyFlowDecl.hpp"

#include "Rap/MyLayout.hpp"

#include <nw4r/math.h>

#include "GameManager.hpp"

#include "FileManager.hpp"

#include "InputCheckManager.hpp"

#include "TickFlowManager.hpp"

#include "LayoutManager.hpp"

#include "CellAnimManager.hpp"

#include "SoundManager.hpp"

#include "Controller.hpp"

#include "PauseLayout.hpp"
#include "CursorLayout.hpp"
#include "BalloonLayout.hpp"

#include "code_801ED7D4.hpp"

#include "rev_tengoku.rsid"

#include "cellanim/rap/ver0/rcad_rap_labels.h"
#include "cellanim/rap/ver0/rcad_rap_bg_labels.h"
#include "cellanim/rap/ver0/rcad_rap_bg_01_labels.h"
#include "cellanim/rap/ver0/rcad_rap_airplane_labels.h"
#include "cellanim/rap/ver1/rcad_rap_chandelier_labels.h"

void CSceneRap::fn_80050284(u32 button) {
    gGameManager->getCurrentScene<CSceneRap>()->fn_8005128C(button, 2, 0.0f);
}

SCENE_IMPL_CREATE_FN(CSceneRap)

void CSceneRap::fn_80050340(void) {
    fn_801D369C(eHeapGroup_SceneAsset);
    if (gFileManager->fn_801D42E0(25)) {
        if (Rap::sceneVer == 0) {
            gFileManager->fn_801D3F94(25, "content2/cellanim/rap/ver0/cellanim.szs");
            gFileManager->fn_801D3F94(78, "content2/layout/layout_rap.szs");
        }
        else if (Rap::sceneVer == 1) {
            gFileManager->fn_801D3F94(25, "content2/cellanim/rap/ver1/cellanim.szs");
            gFileManager->fn_801D3F94(78, "content2/layout/layout_rap.szs");
        }
        else if (Rap::sceneVer == 2) {
            gFileManager->fn_801D3F94(25, "content2/cellanim/rap/remix06/cellanim.szs");
            gFileManager->fn_801D3F94(78, "content2/layout/layout_rap.szs");
        }
        else if (Rap::sceneVer == 3) {
            gFileManager->fn_801D3F94(25, "content2/cellanim/rap/remix09/cellanim.szs");
            gFileManager->fn_801D3F94(78, "content2/layout/layout_rap.szs");
        }
        else if (Rap::sceneVer == 4) {
            gFileManager->fn_801D3F94(25, "content2/cellanim/rap/remix10/cellanim.szs");
            gFileManager->fn_801D3F94(78, "content2/layout/layout_rap.szs");
        }
    }
    fn_801D3644();
}

void CSceneRap::_10(void) {
    fn_80050340();
}

void CSceneRap::fn_800504DC(void) {
    if (
        (Rap::sceneVer == 0) || (Rap::sceneVer == 1) || (Rap::sceneVer == 2) ||
        (Rap::sceneVer == 3) || (Rap::sceneVer == 4)
    ) {
        gFileManager->fn_801D41CC(25);
        gFileManager->fn_801D41CC(78);
    }
}

bool CSceneRap::_24(void) {
    return gFileManager->fn_801D42FC(25) && gFileManager->fn_801D42FC(78);
}

void CSceneRap::_14(void) {
    this->CExScene::_14();

    gInputCheckManager->setUnk418(fn_80050284);

    gTickFlowManager->registerFlow<Rap::CMyFlow>();

    gLayoutManager->_20(2);
    gLayoutManager->_24(94, ""); // layout_common
    gLayoutManager->_24(78, ""); // layout_rap

    gLayoutManager->fn_801D6AEC(4);
    gLayoutManager->registerLayout<CPauseLayout>();   // Index 0
    gLayoutManager->registerLayout<CCursorLayout>();  // Index 1
    gLayoutManager->registerLayout<CBalloonLayout>(); // Index 2
    gLayoutManager->registerLayout<Rap::CMyLayout>(); // Index 3

    gLayoutManager->getLayout<CBalloonLayout>(2)->fn_8003A1D0(
        0, 0.0f, 20.0f, 480.0f, 120.0f, true, 0.0f, -55.0f, true
    );

    mPauseLayout = gLayoutManager->getLayout<CPauseLayout>(0);

    gLayoutManager->getLayout<CPauseLayout>(0)->setDrawEnable(false);
    gLayoutManager->getLayout<CCursorLayout>(1)->setDrawEnable(false);

    mMyLayout = gLayoutManager->getLayout<Rap::CMyLayout>(3);

    void *rap_brcadAddr = gFileManager->fn_801D4270(25, "./rap.brcad");
    void *rap_bg_brcadAddr = gFileManager->fn_801D4270(25, "./rap_bg.brcad");
    void *rap_bg_01_brcadAddr = gFileManager->fn_801D4270(25, "./rap_bg_01.brcad");
    void *rap_airplane_brcadAddr = gFileManager->fn_801D4270(25, "./rap_airplane.brcad");

    u32 tplLen = gFileManager->fn_801D422C(25, "./cellanim.tpl");
    void *tplAddr = gFileManager->fn_801D4270(25, "./cellanim.tpl");

    fn_801ED7D4(tplAddr);
    DCStoreRange(tplAddr, tplLen);

    gCellAnimManager->fn_801DB568(rap_brcadAddr, tplAddr, 0);
    gCellAnimManager->fn_801DB568(rap_bg_brcadAddr, tplAddr, 1);
    gCellAnimManager->fn_801DB568(rap_bg_01_brcadAddr, tplAddr, 2);
    gCellAnimManager->fn_801DB568(rap_airplane_brcadAddr, tplAddr, 3);

    if (Rap::sceneVer == 1) {
        void *rap_chandelier_brcadAddr = gFileManager->fn_801D4270(25, "./rap_chandelier.brcad");
        gCellAnimManager->fn_801DB568(rap_chandelier_brcadAddr, tplAddr, 4);
    }

    mBGAnim = gCellAnimManager->fn_801DBE7C(1, rap_bg_bg_evening);
    mFenceAnim = gCellAnimManager->fn_801DBE7C(1, rap_bg_fence);
    mGroundDryAnim = gCellAnimManager->fn_801DBE7C(1, rap_bg_ground_normal);
    mGroundWetAnim = gCellAnimManager->fn_801DBE7C(1, rap_bg_ground_wet);

    mBGAnim->fn_801DCF94(4000);
    mFenceAnim->fn_801DCF94(2500);
    mGroundDryAnim->fn_801DCF94(3800);
    mGroundWetAnim->fn_801DCF94(3799);

    mCarAnim = gCellAnimManager->fn_801DBE7C(0, rap_car_beat);
    mCarAnim->fn_801DCF94(2200);
    mCarAnim->fn_801DCF38();

    mCarShadowAnim = gCellAnimManager->fn_801DBE7C(0, rap_car_shadow);
    mCarShadowAnim->fn_801DCF94(3500);

    mAirplaneAnim = gCellAnimManager->fn_801DBE7C(3, rap_airplane_airplane);
    mAirplaneAnim->fn_801DCF94(3000);
    mAirplaneAnim->fn_801DCF38();
    mAirplaneAnim->setEnabled(false);

    mAirplaneShadowAnim = gCellAnimManager->fn_801DBE7C(3, rap_airplane_shadow);
    mAirplaneShadowAnim->fn_801DCF94(3500);
    mAirplaneShadowAnim->setEnabled(false);
    mAirplaneShadowAnim->setOpacity(0xC0);
    mAirplaneShadowAnim->setScale(64.0f, 16.0f);

    mAirplaneTireAnim = gCellAnimManager->fn_801DBE7C(3, rap_airplane_tire);
    mAirplaneTireAnim->fn_801DCF94(2999);
    mAirplaneTireAnim->setEnabled(false);

    mRoomAnim = gCellAnimManager->fn_801DBE7C(2, rap_bg_01_room);
    mRoomAnim->fn_801DCF94(200);
    mRoomAnim->setScale(5.2f, 5.2f);
    mRoomAnim->setEnabled(false);

    mTVAnim = gCellAnimManager->fn_801DBE7C(2, rap_bg_01_tv);
    mTVAnim->fn_801DCF94(300);
    mTVAnim->setBase(mRoomAnim, 0, false);
    mTVAnim->setEnabled(false);

    mSofaAnim = gCellAnimManager->fn_801DBE7C(2, rap_bg_01_sofa);
    mSofaAnim->fn_801DCF94(100);
    mSofaAnim->setBase(mRoomAnim, 0, false);
    mSofaAnim->fn_801DD1F0(rap_bg_01_sofa);
    mSofaAnim->setEnabled(false);

    if (Rap::sceneVer == 1) {
        mChandelierAnim = gCellAnimManager->fn_801DBE7C(4, rap_chandelier_chandelier);
        mChandelierAnim->fn_801DCF94(3900);
        mChandelierAnim->setPosY(-440.0f);
    }

    mPlayerChara.fn_80051770(true, this);
    mOtherChara.fn_80051770(false, this);
    mGirlChara.fn_80051DAC(mCarAnim);

    mPlane3D = new CPseudo3D;
    mPlane3D->fn_80072A14(0.0f, 240.0f, 8.0f, 0.0f, -30.0f, 0.1f);

    mPlaneY.setEasingType(0);
    mPlaneZ.setEasingType(1);

    mPlaneY.setEasingPower(1);
    mPlaneZ.setEasingPower(2);

    mPlaneY.fn_801EA698(-1800.0f, 0.0f);
    mPlaneZ.fn_801EA698(0.0f, 1.0f);

    mPlaneY.fn_801EA550(0.0f, 48.0f, true);
    mPlaneZ.fn_801EA550(0.0f, 48.0f, true);

    mPlaneY.setIsTicking(false);
    mPlaneZ.setIsTicking(false);

    mCharaDarken.setEasingType(0);
    mFenceDarken.setEasingType(0);

    mCharaDarken.setEasingPower(1);
    mFenceDarken.setEasingPower(1);

    mCharaDarken.fn_801EA698(255.0f, 0.0f);
    mFenceDarken.fn_801EA698(255.0f, 0.0f);

    mCharaDarken.fn_801EA550(0.0f, 48.0f, true);
    mFenceDarken.fn_801EA550(0.0f, 48.0f, true);

    mCharaDarken.setIsTicking(false);
    mFenceDarken.setIsTicking(false);

    mAirplaneShadowPos.setEasing(0, 1);
    mAirplaneShadowPos.fn_801EA698(1000.0f, -2000.0f);
    mAirplaneShadowPos.fn_801EA550(0.0f, 48.0f, true);
    mAirplaneShadowPos.setIsTicking(false);

    mUnk34 = 5;

    mUnk1BC = false;
    mUnk1BD = false;
    mUnk1BE = false;
    mUnk1BF = false;

    mGroundWetness.setEasing(3, 2);
    mGroundWetness.fn_801EA698(255.0f, 0.0f);
    mGroundWetness.fn_801EA550(0.0f, 48.0f, true);
    mGroundWetness.setIsTicking(false);

    mUnk1F4.setEasing(3, 2);
    mUnk1F4.fn_801EA698(4.0f, 1.0f);
    mUnk1F4.fn_801EA550(0.0f, 48.0f, true);
    mUnk1F4.setIsTicking(false);

    mUnk228.setEasing(0, 1);
}

void CSceneRap::_28(void) {
    CController *controller = gControllerManager->fn_801D5FF0(0);

    if (mPlayerChara.throughStareTimer != 0) {
        mPlayerChara.throughStareTimer--;
        if (mPlayerChara.throughStareTimer == 0) {
            mPlayerChara.headAnim->fn_801DCE9C(rap_C_head);
        }
    }

    if (mOtherChara.throughStareTimer != 0) {
        mOtherChara.throughStareTimer--;
        if (mOtherChara.throughStareTimer == 0) {
            mOtherChara.headAnim->fn_801DCE9C(rap_C_head);
        }
    }

    if (mPlaneY.getIsTicking() || mPlaneZ.getIsTicking()) {
        mPlaneY.fn_801E9DE8(FALSE);
        mPlaneZ.fn_801E9DE8(FALSE);

        fn_800516C0();

        if (mPlaneY.hasEnded() && mPlaneZ.hasEnded()) {
            mAirplaneAnim->setEnabled(false);
            mAirplaneTireAnim->setEnabled(false);
        }
    }

    if (mCharaDarken.getIsTicking()) {
        mCharaDarken.fn_801E9DE8(FALSE);

        f32 color = mCharaDarken.getCurrent();
        color = (color < 0.0f) ? 0.0f : (color > 255.0f) ? 255.0f : color;

        u8 colorInt = static_cast<u8>(color);

        mPlayerChara.footAnim->setBackColor(colorInt, colorInt, colorInt);
        mPlayerChara.bodyAnim->setBackColor(colorInt, colorInt, colorInt);
        mPlayerChara.headAnim->setBackColor(colorInt, colorInt, colorInt);
        mPlayerChara.eyeAnim->setBackColor(colorInt, colorInt, colorInt);
        mPlayerChara.mouthAnim->setBackColor(colorInt, colorInt, colorInt);

        mOtherChara.footAnim->setBackColor(colorInt, colorInt, colorInt);
        mOtherChara.bodyAnim->setBackColor(colorInt, colorInt, colorInt);
        mOtherChara.headAnim->setBackColor(colorInt, colorInt, colorInt);
        mOtherChara.eyeAnim->setBackColor(colorInt, colorInt, colorInt);
        mOtherChara.mouthAnim->setBackColor(colorInt, colorInt, colorInt);

        mGirlChara.footAnim->setBackColor(colorInt, colorInt, colorInt);
        mGirlChara.bodyAnim->setBackColor(colorInt, colorInt, colorInt);
        mGirlChara.eyeAnim->setBackColor(colorInt, colorInt, colorInt);
        mGirlChara.mouthAnim->setBackColor(colorInt, colorInt, colorInt);

        mCarAnim->setBackColor(colorInt, colorInt, colorInt);
    }

    if (mFenceDarken.getIsTicking()) {
        mFenceDarken.fn_801E9DE8(FALSE);

        f32 color = mFenceDarken.getCurrent();
        color = (color < 0.0f) ? 0.0f : (color > 255.0f) ? 255.0f : color;

        u8 colorInt = static_cast<u8>(color);

        mFenceAnim->setBackColor(colorInt, colorInt, colorInt);
    }

    if (mAirplaneShadowPos.getIsTicking()) {
        mAirplaneShadowPos.fn_801E9DE8(FALSE);

        mAirplaneShadowAnim->setPosY(mAirplaneShadowPos.getCurrent());
        if (!mAirplaneShadowPos.getIsTicking()) {
            mAirplaneShadowAnim->setEnabled(false);
        }
    }

    if (TRUE) {
        mGroundWetness.fn_801E9DE8(FALSE);

        f32 color = mGroundWetness.getCurrent();
        color = (color < 0.0f) ? 0.0f : (color > 255.0f) ? 255.0f : color;

        u8 colorInt = static_cast<u8>(color);

        mGroundWetAnim->setOpacity(255 - colorInt);
    }

    if (mUnk1F4.getIsTicking()) {
        mUnk1F4.fn_801E9DE8(FALSE);

        mSofaAnim->setEnabled(true);

        f32 scale = mUnk1F4.getCurrent();
        mSofaAnim->setScale(scale, scale);
    }

    if (mUnk228.getIsTicking()) {
        mUnk228.fn_801E9DE8(FALSE);

        f32 x = -(nw4r::math::SinDeg(mUnk228.getCurrent())) * 120.0f;
        f32 y = (nw4r::math::CosDeg(mUnk228.getCurrent()) * 20.0f) - 20.0f;

        mCarAnim->setPos(x, y);
        mCarShadowAnim->setPos(x, y);

        mMyLayout->fn_80073198(x, y);
    }
}

void CSceneRap::_1C(void) {
    mPlayerChara.fn_8005193C();
    mOtherChara.fn_8005193C();
    this->CExScene::_1C();
}

void CSceneRap::_20(void) {
    delete mPlane3D;

    gCellAnimManager->fn_801DBA98(0);
    gCellAnimManager->fn_801DBA98(1);
    gCellAnimManager->fn_801DBA98(2);
    gCellAnimManager->fn_801DBA98(3);

    gCellAnimManager->fn_801DC068(0);
    gCellAnimManager->fn_801DC068(1);
    gCellAnimManager->fn_801DC068(2);
    gCellAnimManager->fn_801DC068(3);

    if (Rap::sceneVer == 1) {
        gCellAnimManager->fn_801DBA98(4);
        gCellAnimManager->fn_801DC068(4);
    }

    this->CExScene::_20();
}

TFD_BEGIN(lbl_8023EDC0)
    TFC_RAP_201_0(0, 0, 0)
    TFC_PLAY_SFX(698)
    TFC_PLAY_SFX(256)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_RAP_20C(1)
    TFC_IF_EQU(0)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
    TFC_ENDIF()
    TFC_RAP_201_1(0)
TFD_STOP()

TFD_BEGIN(lbl_8023EE1C)
    TFC_RAP_201_0(0, 0, -1)
    TFC_PLAY_SFX(698)
    TFD_CMD(TF_025, 1, 0), 192,
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_RAP_20C(1)
    TFC_IF_EQU(0)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
    TFC_ENDIF()
    TFC_RAP_201_1(0)
TFD_STOP()

TFD_BEGIN(lbl_8023EE78)
    TFC_RAP_201_0(0, 0, 1)
    TFC_PLAY_SFX(698)
    TFD_CMD(0x025, 1, 0), 320,
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_RAP_20C(1)
    TFC_IF_EQU(0)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
    TFC_ENDIF()
    TFC_RAP_201_1(0)
TFD_STOP()

TFD_BEGIN(lbl_8023EED4)
    TFC_RAP_201_0(0, 0, 0)
    TFC_PLAY_SFX(699)
    TFD_CMD(0x025, 1, 0), 256,
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_RAP_20C(1)
    TFC_IF_EQU(0)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
    TFC_ENDIF()
    TFC_RAP_201_1(0)
TFD_STOP()

TFD_BEGIN(lbl_8023EF30)
    TFC_RAP_201_0(0, 0, -1)
    TFC_PLAY_SFX(699)
    TFD_CMD(0x025, 1, 0), 192,
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_RAP_20C(1)
    TFC_IF_EQU(0)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
    TFC_ENDIF()
    TFC_RAP_201_1(0)
TFD_STOP()

TFD_BEGIN(lbl_8023EF8C)
    TFC_RAP_201_0(0, 0, 1)
    TFC_PLAY_SFX(699)
    TFD_CMD(0x025, 1, 0), 320,
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_RAP_20C(1)
    TFC_IF_EQU(0)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
    TFC_ENDIF()
    TFC_RAP_201_1(0)
TFD_STOP()

TFD_BEGIN(lbl_8023EFE8)
    TFC_RAP_201_0(0, 1, 0)
    TFC_PLAY_SFX(700)
    TFD_CMD(0x025, 1, 0), 256,
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_RAP_20C(1)
    TFC_IF_EQU(0)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
    TFC_ENDIF()
    TFC_RAP_201_1(0)
TFD_STOP()

TFD_BEGIN(lbl_8023F054)
    TFC_RAP_201_0(0, 1, -1)
    TFC_PLAY_SFX(700)
    TFD_CMD(0x025, 1, 0), 192,
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_RAP_20C(1)
    TFC_IF_EQU(0)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
    TFC_ENDIF()
    TFC_RAP_201_1(0)
TFD_STOP()

TFD_BEGIN(lbl_8023F0C0)
    TFC_RAP_201_0(0, 1, 1)
    TFC_PLAY_SFX(700)
    TFD_CMD(0x025, 1, 0), 320,
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_RAP_20C(1)
    TFC_IF_EQU(0)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
    TFC_ENDIF()
    TFC_RAP_201_1(0)
TFD_STOP()

TFD_BEGIN(lbl_8023F12C)
    TFC_RAP_201_0(0, 1, 0)
    TFC_PLAY_SFX(701)
    TFD_CMD(0x025, 1, 0), 256,
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_RAP_20C(1)
    TFC_IF_EQU(0)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
    TFC_ENDIF()
    TFC_RAP_201_1(0)
TFD_STOP()

TFD_BEGIN(lbl_8023F198)
    TFC_RAP_201_0(0, 1, -1)
    TFC_PLAY_SFX(701)
    TFD_CMD(0x025, 1, 0), 192,
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_RAP_20C(1)
    TFC_IF_EQU(0)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
    TFC_ENDIF()
    TFC_RAP_201_1(0)
TFD_STOP()

TFD_BEGIN(lbl_8023F204)
    TFC_RAP_201_0(0, 1, 1)
    TFC_PLAY_SFX(701)
    TFD_CMD(0x025, 1, 0), 320,
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_RAP_20C(1)
    TFC_IF_EQU(0)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
    TFC_ENDIF()
    TFC_RAP_201_1(0)
TFD_STOP()

TFD_BEGIN(lbl_8023F270)
    TFC_RAP_201_0(0, 2, 0)
    TFC_PLAY_SFX(702)
    TFD_CMD(0x025, 1, 0), 256,
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_RAP_20C(1)
    TFC_IF_EQU(0)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
    TFC_ENDIF()
    TFC_RAP_201_1(0)
TFD_STOP()

TFD_BEGIN(lbl_8023F2C4)
    TFC_RAP_201_0(0, 2, -1)
    TFC_PLAY_SFX(702)
    TFD_CMD(0x025, 1, 0), 192,
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_RAP_20C(1)
    TFC_IF_EQU(0)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
    TFC_ENDIF()
    TFC_RAP_201_1(0)
TFD_STOP()

TFD_BEGIN(lbl_8023F318)
    TFC_RAP_201_0(0, 2, 1)
    TFC_PLAY_SFX(702)
    TFD_CMD(0x025, 1, 0), 320,
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_RAP_20C(1)
    TFC_IF_EQU(0)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
    TFC_ENDIF()
    TFC_RAP_201_1(0)
TFD_STOP()

TFD_BEGIN(lbl_8023F36C)
    TFC_RAP_201_0(0, 2, 0)
    TFC_PLAY_SFX(703)
    TFD_CMD(0x025, 1, 0), 256,
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_RAP_20C(1)
    TFC_IF_EQU(0)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
    TFC_ENDIF()
    TFC_RAP_201_1(0)
TFD_STOP()

TFD_BEGIN(lbl_8023F3C0)
    TFC_RAP_201_0(0, 2, -1)
    TFC_PLAY_SFX(703)
    TFD_CMD(0x025, 1, 0), 192,
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_RAP_20C(1)
    TFC_IF_EQU(0)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
    TFC_ENDIF()
    TFC_RAP_201_1(0)
TFD_STOP()

TFD_BEGIN(lbl_8023F414)
    TFC_RAP_201_0(0, 2, 1)
    TFC_PLAY_SFX(703)
    TFD_CMD(0x025, 1, 0), 320,
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_RAP_20C(1)
    TFC_IF_EQU(0)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
    TFC_ENDIF()
    TFC_RAP_201_1(0)
TFD_STOP()

TFD_BEGIN(lbl_8023F468)
    TFC_RAP_201_0(0, 3, 0)
    TFC_PLAY_SFX(704)
    TFD_CMD(0x025, 1, 0), 256,
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_RAP_20C(1)
    TFC_IF_EQU(0)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
    TFC_ENDIF()
    TFC_RAP_201_1(0)
TFD_STOP()

TFD_BEGIN(lbl_8023F4CC)
    TFC_RAP_201_0(0, 3, -1)
    TFC_PLAY_SFX(704)
    TFD_CMD(0x025, 1, 0), 192,
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_RAP_20C(1)
    TFC_IF_EQU(0)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
    TFC_ENDIF()
    TFC_RAP_201_1(0)
TFD_STOP()

TFD_BEGIN(lbl_8023F530)
    TFC_RAP_201_0(0, 3, 1)
    TFC_PLAY_SFX(704)
    TFD_CMD(0x025, 1, 0), 320,
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_RAP_20C(1)
    TFC_IF_EQU(0)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
    TFC_ENDIF()
    TFC_RAP_201_1(0)
TFD_STOP()

TFD_BEGIN(lbl_8023F594)
    TFC_RAP_201_0(0, 3, 0)
    TFC_PLAY_SFX(705)
    TFD_CMD(0x025, 1, 0), 256,
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_RAP_20C(1)
    TFC_IF_EQU(0)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
    TFC_ENDIF()
    TFC_RAP_201_1(0)
TFD_STOP()

TFD_BEGIN(lbl_8023F5F8)
    TFC_RAP_201_0(0, 3, -1)
    TFC_PLAY_SFX(705)
    TFD_CMD(0x025, 1, 0), 192,
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_RAP_20C(1)
    TFC_IF_EQU(0)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
    TFC_ENDIF()
    TFC_RAP_201_1(0)
TFD_STOP()

TFD_BEGIN(lbl_8023F65C)
    TFC_RAP_201_0(0, 3, -1)
    TFC_PLAY_SFX(705)
    TFD_CMD(0x025, 1, 0), 320,
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_REST(12)
    TFC_RAP_20C(1)
    TFC_IF_EQU(0)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
        TFC_REST(12)
    TFC_ENDIF()
    TFC_RAP_201_1(0)
TFD_STOP()

void CSceneRap::fn_8005128C(u32 button, s32 type, f32 ticks) {
    if (button == eInputType_TriggerA) {
        if (type == 0) {
            switch (mUnk34) {
            case 0: {
                if (mUnk1BC) {
                    gTickFlowManager->fn_801E1CC0(lbl_8023EED4, -ticks);
                }
                else {
                    gTickFlowManager->fn_801E1CC0(lbl_8023EDC0, -ticks);
                }
            } break;

            case 1: {
                if (mUnk1BC) {
                    gTickFlowManager->fn_801E1CC0(lbl_8023F12C, -ticks);
                }
                else {
                    gTickFlowManager->fn_801E1CC0(lbl_8023EFE8, -ticks);
                }
            } break;

            case 2: {
                if (mUnk1BC) {
                    gTickFlowManager->fn_801E1CC0(lbl_8023F36C, -ticks);
                }
                else {
                    gTickFlowManager->fn_801E1CC0(lbl_8023F270, -ticks);
                }
            } break;

            case 3: {
                if (mUnk1BC) {
                    gTickFlowManager->fn_801E1CC0(lbl_8023F594, -ticks);
                }
                else {
                    gTickFlowManager->fn_801E1CC0(lbl_8023F468, -ticks);
                }
            } break;
            
            default:
                break;
            }

            if (mUnk1BD) {
                for (s32 i = 0; i < 4; i++) {
                    CCellAnim *lightAnim = gCellAnimManager->fn_801DBE7C(0, rap_car_light);
                    lightAnim->setBase(mCarAnim, 1 + i, false);
                    lightAnim->fn_801DCF94(2199);
                    lightAnim->fn_801DD184(rap_car_light);
                }

                mCharaDarken.fn_801EA698(32.0f, 255.0f);
                mCharaDarken.fn_801EA550(0.0f, 12.0f, true);
            }

            if (mUnk1BF) {
                mPlayerChara.eyeAnim->fn_801DD0AC(rap_eye_D_arrange);
                mOtherChara.eyeAnim->fn_801DD0AC(rap_eye_D_arrange);
            }
        }
        else if (type == 1) {
            fn_80009ABC();

            if (ticks >= 0.0f) {
                switch (mUnk34) {
                case 0: {
                    if (mUnk1BC) {
                        gTickFlowManager->fn_801E1CC0(lbl_8023EF8C);
                    }
                    else {
                        gTickFlowManager->fn_801E1CC0(lbl_8023EE78);
                    }
                } break;

                case 1: {
                    if (mUnk1BC) {
                        gTickFlowManager->fn_801E1CC0(lbl_8023F204);
                    }
                    else {
                        gTickFlowManager->fn_801E1CC0(lbl_8023F0C0);
                    }
                } break;

                case 2: {
                    if (mUnk1BC) {
                        gTickFlowManager->fn_801E1CC0(lbl_8023F414);
                    }
                    else {
                        gTickFlowManager->fn_801E1CC0(lbl_8023F318);
                    }
                } break;

                case 3: {
                    if (mUnk1BC) {
                        gTickFlowManager->fn_801E1CC0(lbl_8023F65C);
                    }
                    else {
                        gTickFlowManager->fn_801E1CC0(lbl_8023F530);
                    }
                } break;
                
                default:
                    break;
                }

                gSoundManager->play(SE_MISS);
            }
            else {
                switch (mUnk34) {
                case 0: {
                    if (mUnk1BC) {
                        gTickFlowManager->fn_801E1CC0(lbl_8023EF30);
                    }
                    else {
                        gTickFlowManager->fn_801E1CC0(lbl_8023EE1C);
                    }
                } break;

                case 1: {
                    if (mUnk1BC) {
                        gTickFlowManager->fn_801E1CC0(lbl_8023F198);
                    }
                    else {
                        gTickFlowManager->fn_801E1CC0(lbl_8023F054);
                    }
                } break;

                case 2: {
                    if (mUnk1BC) {
                        gTickFlowManager->fn_801E1CC0(lbl_8023F3C0);
                    }
                    else {
                        gTickFlowManager->fn_801E1CC0(lbl_8023F2C4);
                    }
                } break;

                case 3: {
                    if (mUnk1BC) {
                        gTickFlowManager->fn_801E1CC0(lbl_8023F5F8);
                    }
                    else {
                        gTickFlowManager->fn_801E1CC0(lbl_8023F4CC);
                    }
                } break;
                
                default:
                    break;
                }

                gSoundManager->play(SE_MISS);
            }
        }
        else if (type == 2) {
            fn_80009ABC();

            mPlayerChara.footAnim->fn_801DCE9C(rap_P_cough);
            mPlayerChara.footAnim->fn_801DCF18();

            mOtherChara.fn_80051D00();

            gSoundManager->play(SE_MISS);
        }
    }
}

void CSceneRap::fn_800516C0(void) {
    f32 x, y, scale;
    mPlane3D->fn_80072A54(0.0f, mPlaneY.getCurrent(), mPlaneZ.getCurrent(), &x, &y, &scale);

    mAirplaneAnim->setPos(x, y);
    mAirplaneAnim->setScale(scale, scale);

    mAirplaneTireAnim->setPos(x, y);
    mAirplaneTireAnim->setScale(scale, scale);
}

void CSceneRap::CharaRapper::fn_80051770(bool isPlayer, CSceneRap *_scene) {
    scene = _scene;
    amIPlayer = isPlayer;

    footAnim = gCellAnimManager->fn_801DBE7C(0, rap_foot_MD);
    bodyAnim = gCellAnimManager->fn_801DBE7C(0, rap_body_MD);
    eyeAnim = gCellAnimManager->fn_801DBE7C(0, rap_eye_MD);
    mouthAnim = gCellAnimManager->fn_801DBE7C(0, rap_mouth_MD);

    u16 headAnimID = amIPlayer ? rap_P_head : rap_C_head;
    headAnim = gCellAnimManager->fn_801DBE7C(0, headAnimID);

    bodyAnim->setBase(footAnim, 0, false);
    headAnim->setBase(bodyAnim, 4, true);
    eyeAnim->setBase(headAnim, 0, false);
    mouthAnim->setBase(headAnim, 1, false);

    footAnim->fn_801DCF94(1000);
    bodyAnim->fn_801DCF94(999);
    headAnim->fn_801DCF94(998);
    eyeAnim->fn_801DCF94(997);
    mouthAnim->fn_801DCF94(996);

    footAnim->fn_801DCF38();
    bodyAnim->fn_801DCF38();
    headAnim->fn_801DCF38();
    eyeAnim->fn_801DCF38();
    mouthAnim->fn_801DCF38();

    if (amIPlayer) {
        footAnim->setPosX(200.0f);
    }
    else {
        footAnim->setPosX(-200.0f);
    }

    if (amIPlayer) {
        CExScene::fn_80009A40(true);
        CExScene::fn_80009950(footAnim->getPosX(), footAnim->getPosY() - 100.0f);
    }

    throughStareTimer = 0;
}

void CSceneRap::CharaRapper::fn_8005193C(void) {
    if (throughStareTimer != 0) {
        bodyAnim->setEnabled(true);
        headAnim->setEnabled(true);
        eyeAnim->setEnabled(false);
        mouthAnim->setEnabled(false);
    }
    else if (footAnim->getAnimID() == rap_P_cough) {
        bodyAnim->setEnabled(false);
        headAnim->setEnabled(false);
        eyeAnim->setEnabled(false);
        mouthAnim->setEnabled(false);
    }
    else {
        bodyAnim->setEnabled(true);
        headAnim->setEnabled(true);
        eyeAnim->setEnabled(true);
        mouthAnim->setEnabled(true);
    }
}

void CSceneRap::CharaRapper::fn_800519D4(void) {
    if ((footAnim->getAnimID() != rap_P_cough) || !footAnim->getPlaying()) {
        footAnim->fn_801DCE9C(rap_foot_beat);
        footAnim->fn_801DCF18();
    }
}

void CSceneRap::CharaRapper::fn_80051A2C(s32 type, s32 unk) {
    if ((footAnim->getAnimID() != rap_P_cough) || !footAnim->getPlaying() || (unk != 2)) {
        switch (type) {
        case 0: {
            footAnim->fn_801DD0AC(rap_foot_D);
            bodyAnim->fn_801DD0AC(rap_body_D);
            mouthAnim->fn_801DD0AC(rap_mouth_D);
            if (eyeAnim->getAnimID() != rap_eye_D_arrange) {
                eyeAnim->fn_801DD0AC(rap_eye_D);
            }
        } break;

        case 1: {
            footAnim->fn_801DD0AC(rap_foot_MD);
            bodyAnim->fn_801DD0AC(rap_body_MD);
            eyeAnim->fn_801DD0AC(rap_eye_MD);
            mouthAnim->fn_801DD0AC(scene->mUnk1BC ? rap_mouth_MD_ura : rap_mouth_MD);
        } break;

        case 2: {
            footAnim->fn_801DD0AC(rap_foot_H);
            bodyAnim->fn_801DD0AC(rap_body_H);
            eyeAnim->fn_801DD0AC(rap_eye_H);
            mouthAnim->fn_801DD0AC(scene->mUnk1BC ? rap_mouth_H_ura : rap_mouth_H);
        } break;

        case 3: {
            footAnim->fn_801DD0AC(rap_foot_S);
            bodyAnim->fn_801DD0AC(rap_body_S);
            eyeAnim->fn_801DD0AC(rap_eye_S);
            mouthAnim->fn_801DD0AC(rap_mouth_S);
        } break;
        
        default:
            break;
        }

        if (throughStareTimer != 0) {
            fn_80051D34();
        }
    }
}

void CSceneRap::CharaRapper::fn_80051BB8(s32 type) {
    if ((footAnim->getAnimID() != rap_P_cough) || !footAnim->getPlaying()) {
        switch (type) {
        case 0: {
            footAnim->fn_801DD0AC(rap_foot_D_B);
            bodyAnim->fn_801DD0AC(rap_body_D_B);
            eyeAnim->fn_801DD0AC(rap_eye_D_B);
            mouthAnim->fn_801DD0AC(rap_mouth_D_B);
        } break;

        case 1: {
            footAnim->fn_801DD0AC(rap_foot_MD_B);
            bodyAnim->fn_801DD0AC(rap_body_MD_B);
            eyeAnim->fn_801DD0AC(rap_eye_MD_B);
            mouthAnim->fn_801DD0AC(rap_mouth_MD_B);
        } break;

        case 2: {
            footAnim->fn_801DD0AC(rap_foot_H_B);
            bodyAnim->fn_801DD0AC(rap_body_H_B);
            eyeAnim->fn_801DD0AC(rap_eye_H_B);
            mouthAnim->fn_801DD0AC(rap_mouth_H_B);
        } break;

        case 3: {
            footAnim->fn_801DD0AC(rap_foot_S_B);
            bodyAnim->fn_801DD0AC(rap_body_S_B);
            eyeAnim->fn_801DD0AC(rap_eye_S_B);
            mouthAnim->fn_801DD0AC(rap_mouth_S_B);
        } break;
        
        default:
            break;
        }

        if (throughStareTimer != 0) {
            fn_80051D34();
        }
    }
}

void CSceneRap::CharaRapper::fn_80051D00(void) {
    fn_80051D34();
    throughStareTimer = 120;
}

void CSceneRap::CharaRapper::fn_80051D34(void) {
    u16 animID = bodyAnim->getAnimID();
    if ((animID == rap_body_D) || (animID == rap_body_D_B)) {
        headAnim->fn_801DCE9C(rap_C_head_through_D);
    }
    else if ((animID == rap_body_MD) || (animID == rap_body_MD_B)) {
        headAnim->fn_801DCE9C(rap_C_head_through_MD);
    }
    else if ((animID == rap_body_H) || (animID == rap_body_H_B)) {
        headAnim->fn_801DCE9C(rap_C_head_through_H);
    }
    else if ((animID == rap_body_S) || (animID == rap_body_S_B)) {
        headAnim->fn_801DCE9C(rap_C_head_through_S);
    }
}

void CSceneRap::CharaGirl::fn_80051DAC(CCellAnim *carAnim) {
    footAnim = gCellAnimManager->fn_801DBE7C(0, rap_girl_foot_MD);
    bodyAnim = gCellAnimManager->fn_801DBE7C(0, rap_girl_body_MD);
    eyeAnim = gCellAnimManager->fn_801DBE7C(0, rap_girl_eye_MD);
    mouthAnim = gCellAnimManager->fn_801DBE7C(0, rap_girl_mouth_MD);

    footAnim->setBase(carAnim, 0, false);
    bodyAnim->setBase(footAnim, 4, true);
    eyeAnim->setBase(bodyAnim, 0, false);
    mouthAnim->setBase(bodyAnim, 1, false);

    footAnim->fn_801DCF94(2000);
    bodyAnim->fn_801DCF94(1999);
    eyeAnim->fn_801DCF94(1998);
    mouthAnim->fn_801DCF94(1997);

    footAnim->fn_801DCF38();
    bodyAnim->fn_801DCF38();
    eyeAnim->fn_801DCF38();
    mouthAnim->fn_801DCF38();
}

void CSceneRap::CharaGirl::fn_80051ED0(void) {
    footAnim->fn_801DCE9C(rap_girl_foot_beat);
    footAnim->fn_801DCF18();
}

void CSceneRap::CharaGirl::fn_80051F0C(s32 type, bool unk) {    
    switch (type) {
    case 0: {
        footAnim->fn_801DCE9C(rap_girl_foot_D);
        bodyAnim->fn_801DCE9C(rap_girl_body_D);
        eyeAnim->fn_801DCE9C(rap_girl_eye_D);
        mouthAnim->fn_801DCE9C(rap_girl_mouth_D);
    } break;

    case 1: {
        footAnim->fn_801DCE9C(rap_girl_foot_MD);
        bodyAnim->fn_801DCE9C(rap_girl_body_MD);
        eyeAnim->fn_801DCE9C(rap_girl_eye_MD);
        mouthAnim->fn_801DCE9C(unk ? rap_girl_mouth_MD_ura : rap_girl_mouth_MD);
    } break;

    case 2: {
        footAnim->fn_801DCE9C(rap_girl_foot_H);
        bodyAnim->fn_801DCE9C(rap_girl_body_H);
        eyeAnim->fn_801DCE9C(rap_girl_eye_H);
        mouthAnim->fn_801DCE9C(unk ? rap_girl_mouth_H_ura : rap_girl_mouth_H);
    } break;

    case 3: {
        footAnim->fn_801DCE9C(rap_girl_foot_S);
        bodyAnim->fn_801DCE9C(rap_girl_body_S);
        eyeAnim->fn_801DCE9C(rap_girl_eye_S);
        mouthAnim->fn_801DCE9C(rap_girl_mouth_S);
    } break;
    
    default:
        break;
    }

    footAnim->fn_801DCF18();
    bodyAnim->fn_801DCF18();
    eyeAnim->fn_801DCF18();
    mouthAnim->fn_801DCF18();
}

void CSceneRap::CharaGirl::fn_80052070(s32 type) {
    switch (type) {
    case 0: {
        footAnim->fn_801DCE9C(rap_girl_foot_D_B);
        bodyAnim->fn_801DCE9C(rap_girl_body_D_B);
        eyeAnim->fn_801DCE9C(rap_girl_eye_D_B);
        mouthAnim->fn_801DCE9C(rap_girl_mouth_D_B);
    } break;

    case 1: {
        footAnim->fn_801DCE9C(rap_girl_foot_MD_B);
        bodyAnim->fn_801DCE9C(rap_girl_body_MD_B);
        eyeAnim->fn_801DCE9C(rap_girl_eye_MD_B);
        mouthAnim->fn_801DCE9C(rap_girl_mouth_MD_B);
    } break;

    case 2: {
        footAnim->fn_801DCE9C(rap_girl_foot_H_B);
        bodyAnim->fn_801DCE9C(rap_girl_body_H_B);
        eyeAnim->fn_801DCE9C(rap_girl_eye_H_B);
        mouthAnim->fn_801DCE9C(rap_girl_mouth_H_B);
    } break;

    case 3: {
        footAnim->fn_801DCE9C(rap_girl_foot_S_B);
        bodyAnim->fn_801DCE9C(rap_girl_body_S_B);
        eyeAnim->fn_801DCE9C(rap_girl_eye_S_B);
        mouthAnim->fn_801DCE9C(rap_girl_mouth_S_B);
    } break;
    
    default:
        break;
    }

    footAnim->fn_801DCF18();
    bodyAnim->fn_801DCF18();
    eyeAnim->fn_801DCF18();
    mouthAnim->fn_801DCF18();
}

void CSceneRap::CharaGirl::fn_800521A8(void) {
    bodyAnim->fn_801DD0AC(rap_girl_body_blow_loop);
    bodyAnim->fn_801DCEE8(rap_girl_body_blow_loop, NULL);
    bodyAnim->fn_801DCEE8(rap_girl_body_blow_loop, NULL);
    bodyAnim->fn_801DCEE8(rap_girl_body_blow_end, NULL);
}

void CSceneRap::_18(void) {
    this->CExScene::_18();
}
