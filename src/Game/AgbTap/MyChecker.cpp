#include "AgbTap/MyChecker.hpp"

#include "TickFlowManager.hpp"

#include "SoundManager.hpp"

#include "ExScene.hpp"

#include "rev_tengoku.rsid"

#include "cellanim/agbTap/ver0/rcad_agb_tap_labels.h"

u32 AgbTap::CMyChecker::_24(u32 button) {
    mScene->fn_800A9698(button, 0);
    return 1;
}

u32 AgbTap::CMyChecker::_28(u32 button) {
    mScene->fn_800A9698(button, 1);

    CExScene::fn_80009ABC();

    return 1;
}

u32 AgbTap::CMyChecker::_2C(void) {
    mScene->mThroughTapCount++;

    CExScene::fn_80009ABC();

    switch (mScene->mLastAnimeType) {
    case eAgbTapMove_Round: {
        mScene->mGirlAnim->fn_801DD0AC(agb_tap_girl_land_miss);
        mScene->mLastAnimeType = eAgbTapMove_Land;
        mScene->mGirlAnimeTimer = gTickFlowManager->fn_801E26B4(72.0f);
        
        gSoundManager->play(SE_AGB_TAP_MISS_OH);
    } break;

    case eAgbTapMove_Jump_Last: {
        mScene->mGirlAnim->fn_801DD0AC(agb_tap_girl_pause_miss);
        mScene->mLastAnimeType = eAgbTapMove_Jump_Last;
        mScene->mGirlAnimeTimer = gTickFlowManager->fn_801E26B4(72.0f);
        
        gSoundManager->play(SE_AGB_TAP_MISS_IYAN);
    } break;
    
    default:
        break;
    }

    if (mScene->mGiraffeAnim->getEnabled()) {
        mScene->fn_800A9D20();
        mScene->mGiraffeAnimeTimer = gTickFlowManager->fn_801E26B4(96.0f);
    }

    return 1;
}

u32 AgbTap::CMyChecker::_30(u32 button) {
    mScene->fn_800A9698(button, 2);
    return 1;
}

AgbTap::CMyChecker::~CMyChecker(void) {}

void AgbTap::CMyChecker::_14(void) {
    this->~CMyChecker();
}
