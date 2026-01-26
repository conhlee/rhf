#include "AgbGhost/SceneAgbGhost.hpp"

#include "AgbGhost/MyFlow.hpp"

#include <revolution/SC.h>

#include "PauseLayout.hpp"
#include "CursorLayout.hpp"
#include "BalloonLayout.hpp"

#include "Mem.hpp"

#include "GameManager.hpp"
#include "CellAnimManager.hpp"
#include "FileManager.hpp"
#include "InputCheckManager.hpp"
#include "LayoutManager.hpp"
#include "TickFlowManager.hpp"

#include "Controller.hpp"

#include "Random.hpp"

#include "TPL.hpp"

#include "cellanim/agbGhost/ver0/rcad_ghost_labels.h"

#include "rev_tengoku.rsid"

namespace AgbGhost {

s32 sceneVer;

} // namespace AgbGhost

void CSceneAgbGhost::fn_800A75DC(u32 button) {
    CSceneAgbGhost *scene = gGameManager->getCurrentScene<CSceneAgbGhost>();
    if (button == eInputType_TriggerA) {
        bool isArrowReady = (scene->mBowArrowAnim->getAnimID() == 3) &&
                            (scene->mBowArrowAnim->fn_801DD43C() != 0);
        if (isArrowReady) {
            scene->mBowArrowAnim->fn_801DD0AC(ghost_arrow_shoot);

            CCellAnim *arrowMissAnim = gCellAnimManager->fn_801DBE7C(0, ghost_arrow_miss);
            arrowMissAnim->fn_801DCF94(2500);
            arrowMissAnim->setPos(-147.0f, -70.0f);
            arrowMissAnim->fn_801DD184(5);

            gSoundManager->play(SE_AGB_AIM_DOOR);
        }
    }
}

SCENE_IMPL_CREATE_FN(CSceneAgbGhost)

void CSceneAgbGhost::fn_800A772C(void) {
    fn_801D369C(eHeapGroup_SceneAsset);
    if (gFileManager->fn_801D42E0(2)) {
        gFileManager->fn_801D3F94(2, "content2/cellanim/agbGhost/ver0/cellanim.szs");
    }
    fn_801D3644();
}

void CSceneAgbGhost::_10(void) {
    fn_800A772C();
}

bool CSceneAgbGhost::_24(void) {
    return gFileManager->fn_801D42FC(2);
}

void CSceneAgbGhost::_14(void) {
    this->CExScene::_14();

    gInputCheckManager->setUnk418(fn_800A75DC);

    gTickFlowManager->registerFlow<AgbGhost::CMyFlow>();

    fn_80008FC0(true, WSD_AGB_SHIROI_OBAKE);

    void *ghost_brcadAddr = gFileManager->fn_801D4270(2, "./ghost.brcad");

    u32 ghost_tplLen = gFileManager->fn_801D422C(2, "./cellanim.tpl");
    void *ghost_tplAddr = gFileManager->fn_801D4270(2, "./cellanim.tpl");

    UserTPLBind(ghost_tplAddr);
    DCStoreRange(ghost_tplAddr, ghost_tplLen);

    gCellAnimManager->fn_801DB568(ghost_brcadAddr, ghost_tplAddr, 0);

    CCellAnim *bgBackAnim = gCellAnimManager->fn_801DBE7C(0, ghost_bg_back);
    CCellAnim *bgMaskAnim = gCellAnimManager->fn_801DBE7C(0, ghost_bg_front);
    CCellAnim *bgFrontAnim = gCellAnimManager->fn_801DBE7C(0, ghost_bg_front);

    bgBackAnim->fn_801DCF94(3000);
    bgMaskAnim->fn_801DCF94(1000);
    bgFrontAnim->fn_801DCF94(400);

    bgFrontAnim->fn_801DD2B4(1);

    mBowArrowAnim = gCellAnimManager->fn_801DBE7C(0, ghost_arrow_ready);
    mBowArrowAnim->fn_801DCF94(300);
    mBowArrowAnim->setPos(1000.0f, 107.0f);

    mGhostAnim = gCellAnimManager->fn_801DBE7C(0, ghost_ghost_step);
    mGhostAnim->fn_801DCF94(2000);
    mGhostAnim->setEnabled(false);

    mBgTreeAnim = gCellAnimManager->fn_801DBE7C(0, ghost_bg_tree);
    mBgTreeAnim->fn_801DCF94(2900);
    mBgTreeAnim->fn_801DD1F0(2);
    mBgTreeAnim->setPos(260.0f, -60.0f);

    mDoorAnim = gCellAnimManager->fn_801DBE7C(0, ghost_door_close);
    mDoorAnim->fn_801DCF38();
    mDoorAnim->setPos(-146.0f, -69.0f);
    mDoorAnim->fn_801DCF94(2800);

    mDoorMaskAnim = gCellAnimManager->fn_801DBE7C(0, ghost_door_mask);
    mDoorMaskAnim->setPosX(mDoorAnim->getPosX());
    mDoorMaskAnim->setPosY(mDoorAnim->getPosY());
    mDoorMaskAnim->fn_801DCF94(1500);
    mDoorMaskAnim->setEnabled(false);

    for (s32 i = 0; i < (s32)ARRAY_LENGTH(mRainAnim); i++) {
        mRainAnim[i] = gCellAnimManager->fn_801DBE7C(0, ghost_rain_fast);
        mRainAnim[i]->fn_801DCF94(200);
        mRainAnim[i]->setEnabled(false);
    }

    for (s32 i = 0; i < (s32)ARRAY_LENGTH(mRainRippleAnim); i++) {
        mRainRippleAnim[i] = gCellAnimManager->fn_801DBE7C(0, ghost_ripple);
        mRainRippleAnim[i]->fn_801DCF94(500);
        mRainRippleAnim[i]->setEnabled(false);
    }

    gLayoutManager->_20(1);
    gLayoutManager->_24(94, ""); // layout_common
    
    gLayoutManager->fn_801D6AEC(3);
    gLayoutManager->registerLayout<CPauseLayout>();   // Index 0
    gLayoutManager->registerLayout<CCursorLayout>();  // Index 1
    gLayoutManager->registerLayout<CBalloonLayout>(); // Index 2

    gLayoutManager->getLayout<CBalloonLayout>(2)->fn_8003A1D0(
        0, 0.0f, -170.0f, 480.0f, 80.0f, false, 0.0f, 0.0f, false
    );

    mPauseLayout = gLayoutManager->getLayout<CPauseLayout>(0);

    gLayoutManager->getLayout<CPauseLayout>(0)->setDrawEnable(0);
    gLayoutManager->getLayout<CCursorLayout>(1)->setDrawEnable(0);

    mUnk110.setEasing(0, 1);
    mUnk168.setEasing(3, 2);

    gSoundManager->play(SE_AGB_AIM_KANKYO, 0.0f, &mRainSound);

    mGhostHeight = 0x100;

    mRainNext = 0;
    mRainRippleNext = 0;

    mIsRainSlow = false;
    mRainMotion = false;
    mUnk1B2 = true;
}

void CSceneAgbGhost::_28(void) {
    CController *controller = gControllerManager->fn_801D5FF0(0);

    if (mUnk110.getIsTicking()) {
        mUnk110.fn_801E9DE8(FALSE);
        if (mGhostAnim->getAnimID() == 7) {
            mGhostAnim->setPos(mUnk110.getCurrentX(), mUnk110.getCurrentZ());
        }
    }

    if (mUnk168.getIsTicking()) {
        mUnk168.fn_801E9DE8(FALSE);
        mBowArrowAnim->setPosX(mUnk168.getCurrent());
    }

    if (!mRainSound.IsPause() && mUnk1B2) {
        f32 volume = 0.0f;
        if (
            (0.0f < ((256.0f - mGhostHeight) / 2.0f)) &&
            (128.0f > ((256.0f - mGhostHeight) / 2.0f))
        ) {
            volume = (256.0f - mGhostHeight) / 2.0f;
        }
        gSoundManager->fn_801E65F4((volume + 64.0f) / 256.0f, 15, &mRainSound);
        mUnk1B2 = false;
    }

    if (!mIsRainSlow) {
        for (s32 i = 0; i < 4; i++) {
            CCellAnim *rainAnim = mRainAnim[mRainNext];
            CCellAnim *rainRippleAnim = mRainRippleAnim[mRainRippleNext];

            rainAnim->fn_801DD0AC(ghost_rain_fast);

            f32 rainPosY = sRandom.nextF32(180.0f) - (180.0f / 2.0f);
            f32 rainPosX = sRandom.nextF32(832.0f) - (832.0f / 2.0f);
            rainAnim->setPos(rainPosX, rainPosY);

            rainRippleAnim->fn_801DD0AC(ghost_ripple);

            f32 rainRipplePosY = sRandom.nextF32(180.0f) + (180.0f / 6.0f);
            f32 rainRipplePosX = sRandom.nextF32(832.0f) - (832.0f / 2.0f);
            rainRippleAnim->setPos(rainRipplePosX, rainRipplePosY);

            mRainNext++;
            if (mRainNext >= (s32)ARRAY_LENGTH(mRainAnim)) {
                mRainNext = 0;
            }

            mRainRippleNext++;
            if (mRainRippleNext >= (s32)ARRAY_LENGTH(mRainRippleAnim)) {
                mRainRippleNext = 0;
            }
        }
    }
}

void CSceneAgbGhost::_1C(void) {
    this->CExScene::_1C();
}

void CSceneAgbGhost::_20(void) {
    gSoundManager->fn_801E62B8(0, &mRainSound);

    gCellAnimManager->fn_801DBA98(0);
    gCellAnimManager->fn_801DC068(0);

    gFileManager->fn_801D41CC(2);

    this->CExScene::_20();
}

void CSceneAgbGhost::fn_800A7FA8(u32 button, u32 type) {
    if (button == eInputType_TriggerA) {
        bool isArrowReady = (mBowArrowAnim->getAnimID() == 3) &&
                            (mBowArrowAnim->fn_801DD43C() != 0);
        if (isArrowReady) {
            mBowArrowAnim->fn_801DD0AC(ghost_arrow_shoot);

            if (type != 0) {
                CCellAnim *arrowMissAnim = gCellAnimManager->fn_801DBE7C(0, ghost_arrow_miss);
                arrowMissAnim->fn_801DCF94(2500);
                arrowMissAnim->setPos(-147.0f, -70.0f);
                arrowMissAnim->fn_801DD184(5);

                gSoundManager->play(SE_AGB_AIM_DOOR);
            }
        }
    }
}

void CSceneAgbGhost::fn_800A8080(void) {
    // TODO: this is 100% fn_800A815C but it WILL NOT inline!
    bool isArrowReady = (mBowArrowAnim->getAnimID() == ghost_arrow_ready) &&
                        (mBowArrowAnim->fn_801DD43C() != 0);
    if (!isArrowReady) {
        mBowArrowAnim->fn_801DD0AC(ghost_arrow_ready);
    }
}

void CSceneAgbGhost::fn_800A80EC(f32 start, f32 time) {
    if (mBowArrowAnim->getPosX() > 400.0f) {
        mUnk168.fn_801EA698(start, 210.0f);
        mUnk168.fn_801EA550(0.0f, time, true);
    }
}

bool CSceneAgbGhost::fn_800A815C(void) {
    return mBowArrowAnim->getAnimID() == ghost_arrow_ready && mBowArrowAnim->fn_801DD43C() != 0;
}

void CSceneAgbGhost::fn_800A81A8(u32 pos) {
    bool isWidescreen = SCGetAspectRatio() == SC_ASPECT_WIDE;

    s32 baseX = isWidescreen ? -350 : -280;
    s32 baseY = isWidescreen ? 140 : 134;

    f32 yFac = isWidescreen ? 10.0f : 6.0f;

    s32 height = (256 - mGhostHeight) * 80;

    mGhostAnim->setPos(
        (f32)baseX + (pos * -(f32)baseX) / 6.0f,
        (f32)baseY + (pos * ((f32)baseY - yFac - (f32)baseY)) / 6.0f + height
    );
    mGhostAnim->fn_801DD118(6);

    gSoundManager->play(SE_AGB_AIM_WALK);
    gSoundManager->fn_801E65F4((mGhostHeight / 2.13333333f) * (1.0f / 256.0f));
}

void CSceneAgbGhost::fn_800A8348(void) {
    f32 ghostStartX = -100.0f;
    f32 ghostStartY = -40.0f;

    f32 ghostEndX = ghostStartX - 80.0f;
    f32 ghostEndY = ghostStartY - 40.0f;
    
    mUnk110.fn_801EA8E0(ghostStartX, 0.0f, ghostStartY, ghostEndX, 0.0f, ghostEndY, 0.0f);
    mUnk110.fn_801EA550(0.0f, 48.0f, false);

    mGhostAnim->setPos(ghostStartX, ghostStartY);
    mGhostAnim->fn_801DD24C(7, 0.0f);
    mGhostAnim->fn_801DD2B4(sRandom.nextU32(4));

    CCellAnim *damageAnim = gCellAnimManager->fn_801DBE7C(0, ghost_ghost_damage);
    damageAnim->setPos(0.0f, 20.0f);
    damageAnim->fn_801DCF94(mGhostAnim->getLayer() - 1);
    damageAnim->fn_801DD184(12);

    gTickFlowManager->fn_801E2C04(0.25f); // Slowdown

    fn_800A8788(true);

    gSoundManager->play(SE_AGB_AIM_JUST_HIT);
    gSoundManager->play(SE_AGB_AIM_JUST_HIT_VOICE);
}

void CSceneAgbGhost::fn_800A84A8(bool through) {
    mGhostAnim->setEnabled(false);

    CCellAnim *ghostMissAnim = gCellAnimManager->fn_801DBE7C(0, ghost_ghost_miss_f);
    ghostMissAnim->fn_801DCF94(mGhostAnim->getLayer());
    ghostMissAnim->setPos(0.0f, 0.0f);

    // TODO: generate srawi instead of srwi
    s16 animIndex = through ? 10 : 9;
    ghostMissAnim->fn_801DD184(animIndex);

    gSoundManager->play(SE_AGB_AIM_OSII);
}

void CSceneAgbGhost::fn_800A854C(bool late) {
    mGhostAnim->setEnabled(false);

    CCellAnim *ghostMissAnim = gCellAnimManager->fn_801DBE7C(0, ghost_ghost_through);
    ghostMissAnim->fn_801DCF94(mGhostAnim->getLayer());
    ghostMissAnim->setPos(0.0f, 0.0f);
    ghostMissAnim->fn_801DD184(10);

    gSoundManager->play(SE_AGB_AIM_DASH);
    if (late) {
        ghostMissAnim->fn_801DCEE8(11, NULL);
        gSoundManager->play(SE_AGB_AIM_WARAI, -30.0f);
    }
}

void CSceneAgbGhost::fn_800A860C(void) {
    mGhostAnim->fn_801DD0AC(ghost_ghost_timing_inst);
    mGhostAnim->setPos(0.0f, 0.0f);
}

void CSceneAgbGhost::fn_800A8658(void) {
    mGhostAnim->setEnabled(false);

    CCellAnim *ghostMissAnim = gCellAnimManager->fn_801DBE7C(0, ghost_ghost_through);
    ghostMissAnim->fn_801DCF94(mGhostAnim->getLayer());
    ghostMissAnim->setPos(0.0f, 0.0f);
    ghostMissAnim->fn_801DD184(10);

    gSoundManager->play(SE_AGB_AIM_DASH);
}

void CSceneAgbGhost::fn_800A86E0(void) {
    if (mDoorAnim->getAnimID() != ghost_door_open) {
        mDoorAnim->fn_801DD0AC(ghost_door_open);
        mDoorMaskAnim->setEnabled(true);
    }
}

void CSceneAgbGhost::fn_800A8730(void) {
    if (mDoorAnim->getAnimID() == ghost_door_open) {
        mDoorAnim->fn_801DD0AC(ghost_door_close);
        mDoorMaskAnim->setEnabled(false);
        mGhostAnim->setEnabled(false);
    }
}

void CSceneAgbGhost::fn_800A8788(bool fallSlow) {
    if (mIsRainSlow != fallSlow) {
        mIsRainSlow = fallSlow;

        if (fallSlow) {
            for (s32 i = 0; i < (s32)ARRAY_LENGTH(mRainAnim); i++) {
                f32 speed;
                if (mRainMotion) {
                    speed = 2.0f / (1.0f + sRandom.nextF32(3.0f));
                }
                else {
                    speed = 0.0f;
                }

                mRainAnim[i]->fn_801DD0AC(ghost_rain_slow);

                f32 posY = sRandom.nextF32(456.0f) - (456.0f / 2.0f);
                f32 posX = sRandom.nextF32(832.0f) - (832.0f / 2.0f);
                mRainAnim[i]->setPos(posX, posY);

                mRainAnim[i]->setSpeed(speed);
            }

            for (s32 i = 0; i < (s32)ARRAY_LENGTH(mRainRippleAnim); i++) {
                mRainRippleAnim[i]->setPlaying(false);
            }

            mBgTreeAnim->setPlaying(false);
        }
        else {
            for (s32 i = 0; i < (s32)ARRAY_LENGTH(mRainAnim); i++) {
                mRainAnim[i]->setEnabled(false);
                mRainAnim[i]->setSpeed(1.0f);
            }

            for (s32 i = 0; i < (s32)ARRAY_LENGTH(mRainRippleAnim); i++) {
                mRainRippleAnim[i]->setPlaying(true);
            }

            mBgTreeAnim->setPlaying(true);
        }
    }
}

void CSceneAgbGhost::_18(void) {
    this->CExScene::_18();
}
