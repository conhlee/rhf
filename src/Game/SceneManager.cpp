#include "SceneManager.hpp"

#include <revolution/OS.h>

#include "GameManager.hpp"

#include "TickFlowManager.hpp"

#include "Menu/SceneMenu.hpp"
#include "Prologue/ScenePrologue.hpp"
#include "Strap/SceneStrap.hpp"
#include "Logo/SceneLogo.hpp"
#include "Credit/SceneCredit.hpp"
#include "AgbGhost/SceneAgbGhost.hpp"
#include "Error/SceneError.hpp"
#include "AgbTap/SceneAgbTap.hpp"
#include "Rap/SceneRap.hpp"

CSceneManager::CSceneManager(void) {}

CSceneManager::~CSceneManager(void) {
    _08();
}

void CSceneManager::_10(void) {}

void CSceneManager::_08(void) {}

void CSceneManager::_14(void) {
    mUnk404 = false;
    for (s32 i = 0; i < (s32)ARRAY_LENGTH(mSceneHist); i++) {
        mSceneHist[i] = eScene_Null;
    }
}

extern "C" void fn_8008A8D8(void);

void CSceneManager::_18(void) {
    if (mUnk404) {
        fn_8008A8D8();
        mUnk404 = false;
    }
}

// TODO: migrate these functions into their respective scene files
extern "C" void fn_800136F4(void);
extern "C" void fn_800204F0(void);
extern "C" void fn_80022D8C(void);
extern "C" void fn_80027FB8(void);
extern "C" void fn_8002A5B4(void);
extern "C" void fn_8002CF84(void);
extern "C" void fn_80034354(void);
extern "C" void fn_800302A8(void);
extern "C" void fn_80036734(void);
extern "C" void fn_8005CF38(void);
extern "C" void fn_800535D4(void);
extern "C" void fn_80059D3C(void);
extern "C" void fn_8003BE58(void);
extern "C" void fn_800454B0(void);
extern "C" void fn_80056CF0(void);
extern "C" void fn_80057D58(void);
extern "C" void fn_8004BE50(void);
extern "C" void fn_80047AEC(void);
extern "C" void fn_8004DDC4(void);
extern "C" void fn_80069814(void);
extern "C" void fn_8006D5C0(void);
extern "C" void fn_800657B4(void);
extern "C" void fn_80060534(void);
extern "C" void fn_8006F390(void);
extern "C" void fn_8006C6DC(void);
extern "C" void fn_800759C4(void);
extern "C" void fn_800B5460(void);
extern "C" void fn_800BC6DC(void);
extern "C" void fn_800B956C(void);
extern "C" void fn_800D7F70(void);
extern "C" void fn_800C6114(void);
extern "C" void fn_8003F2F8(void);
extern "C" void fn_80015AFC(void);
extern "C" void fn_800D5294(void);
extern "C" void fn_800AD4F8(void);
extern "C" void fn_8000F838(void);
extern "C" void fn_80090A54(void);
extern "C" void fn_8001DCD8(void);
extern "C" void fn_800107C0(void);
extern "C" void fn_8009E9C8(void);
extern "C" void fn_800C86EC(void);
extern "C" void fn_800CB2B8(void);
extern "C" void fn_8009D6B4(void);
extern "C" void fn_800A5B24(void);
extern "C" void fn_800AB9C8(void);
extern "C" void fn_80092098(void);
extern "C" void fn_8000EC28(void);
extern "C" void fn_800A4674(void);
extern "C" void fn_8000B4B4(void);
extern "C" void fn_8000C2E4(void);
extern "C" void fn_800CF810(void);
extern "C" void fn_80082FB0(void);
extern "C" void fn_8009B39C(void);
extern "C" void fn_8009A1C8(void);
extern "C" void fn_800B2918(void);
extern "C" void fn_800B3FBC(void);

// ??
const char lbl_803251B8[] = "Menu";
const char lbl_80255710[] = "Badminton";
const char lbl_8025571C[] = "Baseball";
const char lbl_803251C0[] = "Batting";
const char lbl_803251C8[] = "Birds";
const char lbl_803251D0[] = "Bun";
const char lbl_803251D8[] = "Concons";
const char lbl_803251E0[] = "Date";
const char lbl_803251E8[] = "Fishing";
const char lbl_803251F0[] = "Flea";
const char lbl_803251F8[] = "Fork";
const char lbl_80325200[] = "Goma";
const char lbl_80255728[] = "Interview";
const char lbl_80255734[] = "Karateman";
const char lbl_80325208[] = "Muscle";
const char lbl_80325210[] = "Panel";
const char lbl_80325218[] = "Rap";
const char lbl_80325220[] = "Receive";
const char lbl_80325228[] = "Robot";
const char lbl_80325230[] = "Rocket";
const char lbl_80255740[] = "Rotation";
const char lbl_80325238[] = "Samurai";
const char lbl_80325240[] = "Seal";
const char lbl_80325248[] = "Seesaw";
const char lbl_80325250[] = "Shrimp";
const char lbl_80325258[] = "Sort";
const char lbl_8025574C[] = "Tambourine";
const char lbl_80325260[] = "Tap";
const char lbl_80325264[] = "Watch";
const char lbl_8032526C[] = "Alarm";
const char lbl_80325274[] = "Eatmen";
const char lbl_80255758[] = "Metroman";
const char lbl_80255764[] = "Shooting";
const char lbl_8032527C[] = "Boat";
const char lbl_80325284[] = "Kungfu";
const char lbl_8032528C[] = "Omochi";
const char lbl_80325294[] = "Sword";
const char lbl_8032529C[] = "Basket";
const char lbl_80255770[] = "Tutorial";
const char lbl_803252A4[] = "Frog";
const char lbl_803252AC[] = "Choro";
const char lbl_803252B4[] = "Hihat";
const char lbl_803252BC[] = "Mail";
const char lbl_80255780[] = "RhythmFighterVS";
const char lbl_803252C8[] = "AgbClap";
const char lbl_80255790[] = "AgbGhost";
const char lbl_8025579C[] = "AgbKanji";
const char lbl_803252D0[] = "AgbTap";
const char lbl_802557A8[] = "NightWalk";
const char lbl_803252D8[] = "Error";
const char lbl_803252E0[] = "Strap";
const char lbl_803252E8[] = "Logo";
const char lbl_803252F0[] = "Title";
const char lbl_802557B4[] = "GameSelect";
const char lbl_802557C0[] = "BonusSelect";
const char lbl_802557CC[] = "Prologue";
const char lbl_802557D8[] = "Epilogue";
const char lbl_803252F8[] = "Grading";
const char lbl_802557E4[] = "Grading2P";
const char lbl_80325300[] = "Perfect";
const char lbl_80325308[] = "Navi";
const char lbl_80325310[] = "Cafe";
const char lbl_802557F0[] = "CafeMenu";
const char lbl_80325318[] = "Reading";
const char lbl_80325320[] = "Credit";
const char lbl_80325328[] = "XXX";

const char *lbl_802ECE20[eScene_Count + 1] = {
    lbl_803251B8, lbl_80255710, lbl_8025571C, lbl_803251C0, lbl_803251C8, lbl_803251D0, lbl_803251D8, lbl_803251E0, 
    lbl_803251E8, lbl_803251F0, lbl_803251F8, lbl_80325200, lbl_80255728, lbl_80255734, lbl_80325208, lbl_80325210, 
    lbl_80325218, lbl_80325220, lbl_80325228, lbl_80325230, lbl_80255740, lbl_80325238, lbl_80325240, lbl_80325248, 
    lbl_80325250, lbl_80325258, lbl_8025574C, lbl_80325260, lbl_80325264, lbl_8032526C, lbl_80325274, lbl_80255758, 
    lbl_80255764, lbl_8032527C, lbl_80325284, lbl_8032528C, lbl_80325294, lbl_8032529C, lbl_80255770, lbl_803252A4, 
    lbl_803252AC, lbl_803252B4, lbl_803252BC, lbl_80255780, lbl_803252C8, lbl_80255790, lbl_8025579C, lbl_803252D0, 
    lbl_802557A8, lbl_803252D8, lbl_803252E0, lbl_803252E8, lbl_803252F0, lbl_802557B4, lbl_802557C0, lbl_802557CC, 
    lbl_802557D8, lbl_803252F8, lbl_802557E4, lbl_80325300, lbl_80325308, lbl_80325310, lbl_802557F0, lbl_80325318, 
    lbl_80325320, lbl_80325328, 
};

void CSceneManager::fn_80089FE0(ESceneID sceneID, const TickFlowCode *tickFlowCode) {
    CScene::CreateFn fn = NULL;

    switch (sceneID) {
    case eScene_Menu:
        fn = CSceneMenu::create;
        break;
    case eScene_Bun:
        fn = (CScene::CreateFn)fn_800136F4;
        break;
    case eScene_Birds:
        fn = (CScene::CreateFn)fn_800204F0;
        break;
    case eScene_Panel:
        fn = (CScene::CreateFn)fn_80022D8C;
        break;
    case eScene_Date:
        fn = (CScene::CreateFn)fn_80027FB8;
        break;
    case eScene_Rocket:
        fn = (CScene::CreateFn)fn_8002A5B4;
        break;
    case eScene_Seesaw:
        fn = (CScene::CreateFn)fn_8002CF84;
        break;
    case eScene_Muscle:
        fn = (CScene::CreateFn)fn_80034354;
        break;
    case eScene_Concons:
        fn = (CScene::CreateFn)fn_800302A8;
        break;
    case eScene_Shrimp:
        fn = (CScene::CreateFn)fn_80036734;
        break;
    case eScene_Tap:
        fn = (CScene::CreateFn)fn_8005CF38;
        break;
    case eScene_Receive:
        fn = (CScene::CreateFn)fn_800535D4;
        break;
    case eScene_Sort:
        fn = (CScene::CreateFn)fn_80059D3C;
        break;
    case eScene_Batting:
        fn = (CScene::CreateFn)fn_8003BE58;
        break;
    case eScene_Fishing:
        fn = (CScene::CreateFn)fn_800454B0;
        break;
    case eScene_Rotation:
        fn = (CScene::CreateFn)fn_80056CF0;
        break;
    case eScene_Rap:
        fn = CSceneRap::create;
        break;
    case eScene_Seal:
        fn = (CScene::CreateFn)fn_80057D58;
        break;
    case eScene_Fork:
        fn = (CScene::CreateFn)fn_8004BE50;
        break;
    case eScene_Flea:
        fn = (CScene::CreateFn)fn_80047AEC;
        break;
    case eScene_Goma:
        fn = (CScene::CreateFn)fn_8004DDC4;
        break;
    case eScene_Karateman:
        fn = (CScene::CreateFn)fn_80069814;
        break;
    case eScene_Tambourine:
        fn = (CScene::CreateFn)fn_8006D5C0;
        break;
    case eScene_Baseball:
        fn = (CScene::CreateFn)fn_800657B4;
        break;
    case eScene_Badminton:
        fn = (CScene::CreateFn)fn_80060534;
        break;
    case eScene_Watch:
        fn = (CScene::CreateFn)fn_8006F390;
        break;
    case eScene_Robot:
        fn = (CScene::CreateFn)fn_8006C6DC;
        break;
    case eScene_Samurai:
        fn = (CScene::CreateFn)fn_800759C4;
        break;
    case eScene_Interview:
        fn = (CScene::CreateFn)fn_800B5460;
        break;
    case eScene_Alarm:
        fn = (CScene::CreateFn)fn_800BC6DC;
        break;
    case eScene_Eatmen:
        fn = (CScene::CreateFn)fn_800B956C;
        break;
    case eScene_Metroman:
        fn = (CScene::CreateFn)fn_800D7F70;
        break;
    case eScene_Shooting:
        fn = (CScene::CreateFn)fn_800C6114;
        break;
    case eScene_Boat:
        fn = (CScene::CreateFn)fn_8003F2F8;
        break;
    case eScene_Kungfu:
        fn = (CScene::CreateFn)fn_80015AFC;
        break;
    case eScene_Omochi:
        fn = (CScene::CreateFn)fn_800D5294;
        break;
    case eScene_Sword:
        fn = (CScene::CreateFn)fn_800AD4F8;
        break;
    case eScene_Basket:
        fn = (CScene::CreateFn)fn_8000F838;
        break;
    case eScene_Tutorial:
        fn = (CScene::CreateFn)fn_80090A54;
        break;
    case eScene_Frog:
        fn = (CScene::CreateFn)fn_8001DCD8;
        break;
    case eScene_Choro:
        fn = (CScene::CreateFn)fn_800107C0;
        break;
    case eScene_Hihat:
        fn = (CScene::CreateFn)fn_8009E9C8;
        break;
    case eScene_Mail:
        fn = (CScene::CreateFn)fn_800C86EC;
        break;
    case eScene_RhythmFighterVS:
        fn = (CScene::CreateFn)fn_800CB2B8;
        break;
    case eScene_AgbClap:
        fn = (CScene::CreateFn)fn_8009D6B4;
        break;
    case eScene_AgbGhost:
        fn = CSceneAgbGhost::create;
        break;
    case eScene_AgbKanji:
        fn = (CScene::CreateFn)fn_800A5B24;
        break;
    case eScene_AgbTap:
        fn = CSceneAgbTap::create;
        break;
    case eScene_NightWalk:
        fn = (CScene::CreateFn)fn_800AB9C8;
        break;
    case eScene_Error:
        fn = CSceneError::create;
        break;
    case eScene_Strap:
        fn = CSceneStrap::create;
        break;
    case eScene_Logo:
        fn = CSceneLogo::create;
        break;
    case eScene_Title:
        fn = (CScene::CreateFn)fn_80092098;
        break;
    case eScene_GameSelect:
        fn = (CScene::CreateFn)fn_8000EC28;
        break;
    case eScene_BonusSelect:
        fn = (CScene::CreateFn)fn_800A4674;
        break;
    case eScene_Prologue:
        fn = CScenePrologue::create;
        break;
    case eScene_Epilogue:
        fn = (CScene::CreateFn)fn_8000B4B4;
        break;
    case eScene_Grading:
        fn = (CScene::CreateFn)fn_8000C2E4;
        break;
    case eScene_Grading2P:
        fn = (CScene::CreateFn)fn_800CF810;
        break;
    case eScene_Perfect:
        fn = (CScene::CreateFn)fn_80082FB0;
        break;
    case eScene_Navi:
        fn = (CScene::CreateFn)fn_8009B39C;
        break;
    case eScene_Cafe:
        fn = (CScene::CreateFn)fn_8009A1C8;
        break;
    case eScene_CafeMenu:
        fn = (CScene::CreateFn)fn_800B2918;
        break;
    case eScene_Reading:
        fn = (CScene::CreateFn)fn_800B3FBC;
        break;
    case eScene_Credit:
        fn = CSceneCredit::create;
        break;
    }

    gGameManager->_20(fn, eHeapGroup_Scene);
    gGameManager->_34(tickFlowCode);

    for (int i = ARRAY_LENGTH(mSceneHist) - 1; i > 0; i--) {
        mSceneHist[i] = mSceneHist[i - 1];
    }

    mSceneHist[0] = sceneID;
    if (mSceneHist[1] == eScene_Null) {
        OSReport("Scene Change [ NULL ] → [ %s ]\n", lbl_802ECE20[mSceneHist[0]]);
    }
    else {
        OSReport("Scene Change [ %s ] → [ %s ]\n", lbl_802ECE20[mSceneHist[1]], lbl_802ECE20[mSceneHist[0]]);
    }
}


// TODO: migrate these vars into their respective scene files
extern s32 lbl_80320378;
extern s32 lbl_80320380;
extern s32 lbl_80320308;
extern s32 lbl_803202AC;
extern s32 lbl_80320288;
extern s32 lbl_803202D0;
extern s32 lbl_803202B8;
extern s32 lbl_80320328;
extern s32 lbl_80320330;
extern s32 lbl_80320338;
extern s32 lbl_80320340;
extern s32 lbl_80320538;
extern s32 lbl_80320388;
extern s32 lbl_803202D8;
extern s32 lbl_803202B0;
extern s32 lbl_80320350;
extern s32 lbl_80320390;
extern s32 lbl_803202C0;
extern s32 lbl_80320358;
extern s32 lbl_803203B0;
extern s32 lbl_80320360;
extern s32 lbl_803202C8;
extern s32 lbl_803202E0;
extern s32 lbl_80320368;
extern s32 lbl_80320398;
extern s32 lbl_80320370;
extern s32 lbl_803203A0;
extern s32 lbl_80320568;
extern s32 lbl_80320540;
extern s32 lbl_803205E8;
extern s32 lbl_80320580;
extern s32 lbl_80320324;
extern s32 lbl_80320290;
extern s32 lbl_803205E0;
extern s32 lbl_80320520;
extern s32 lbl_80320278;
extern s32 lbl_803204A0;
extern s32 lbl_80320298;
extern s32 lbl_80320280;
extern s32 lbl_803204E0;
extern s32 lbl_80320590;
extern s32 lbl_80320598;
extern s32 lbl_803204D0;
extern s32 lbl_80320508;
extern s32 lbl_80320500;
extern s32 lbl_80320510;
extern s32 lbl_80320518;
extern s32 lbl_803204A8;
extern s32 lbl_80320270;
extern s32 lbl_803204F8;
extern s32 lbl_80320250;
extern s32 lbl_80320260;
extern s32 lbl_803205A0;
extern s32 lbl_80320438;
extern s32 lbl_803204C8;
extern s32 lbl_803204B8;
extern s32 lbl_80320528;
extern s32 lbl_80320530;

void CSceneManager::fn_8008A4DC(ESceneID sceneID, s32 ver) {
    switch (sceneID) {
    case eScene_Menu:
        Menu::sceneVer = ver;
        break;
    case eScene_Badminton:
        lbl_80320378 = ver;
        break;
    case eScene_Baseball:
        lbl_80320380 = ver;
        break;
    case eScene_Batting:
        lbl_80320308 = ver;
        break;
    case eScene_Birds:
        lbl_803202AC = ver;
        break;
    case eScene_Bun:
        lbl_80320288 = ver;
        break;
    case eScene_Concons:
        lbl_803202D0 = ver;
        break;
    case eScene_Date:
        lbl_803202B8 = ver;
        break;
    case eScene_Fishing:
        lbl_80320328 = ver;
        break;
    case eScene_Flea:
        lbl_80320330 = ver;
        break;
    case eScene_Fork:
        lbl_80320338 = ver;
        break;
    case eScene_Goma:
        lbl_80320340 = ver;
        break;
    case eScene_Interview:
        lbl_80320538 = ver;
        break;
    case eScene_Karateman:
        lbl_80320388 = ver;
        break;
    case eScene_Muscle:
        lbl_803202D8 = ver;
        break;
    case eScene_Panel:
        lbl_803202B0 = ver;
        break;
    case eScene_Rap:
        Rap::sceneVer = ver;
        break;
    case eScene_Receive:
        lbl_80320350 = ver;
        break;
    case eScene_Robot:
        lbl_80320390 = ver;
        break;
    case eScene_Rocket:
        lbl_803202C0 = ver;
        break;
    case eScene_Rotation:
        lbl_80320358 = ver;
        break;
    case eScene_Samurai:
        lbl_803203B0 = ver;
        break;
    case eScene_Seal:
        lbl_80320360 = ver;
        break;
    case eScene_Seesaw:
        lbl_803202C8 = ver;
        break;
    case eScene_Shrimp:
        lbl_803202E0 = ver;
        break;
    case eScene_Sort:
        lbl_80320368 = ver;
        break;
    case eScene_Tambourine:
        lbl_80320398 = ver;
        break;
    case eScene_Tap:
        lbl_80320370 = ver;
        break;
    case eScene_Watch:
        lbl_803203A0 = ver;
        break;
    case eScene_Alarm:
        lbl_80320568 = ver;
        break;
    case eScene_Eatmen:
        lbl_80320540 = ver;
        break;
    case eScene_Metroman:
        lbl_803205E8 = ver;
        break;
    case eScene_Shooting:
        lbl_80320580 = ver;
        break;
    case eScene_Boat:
        lbl_80320324 = ver;
        break;
    case eScene_Kungfu:
        lbl_80320290 = ver;
        break;
    case eScene_Omochi:
        lbl_803205E0 = ver;
        break;
    case eScene_Sword:
        lbl_80320520 = ver;
        break;
    case eScene_Basket:
        lbl_80320278 = ver;
        break;
    case eScene_Tutorial:
        lbl_803204A0 = ver;
        break;
    case eScene_Frog:
        lbl_80320298 = ver;
        break;
    case eScene_Choro:
        lbl_80320280 = ver;
        break;
    case eScene_Hihat:
        lbl_803204E0 = ver;
        break;
    case eScene_Mail:
        lbl_80320590 = ver;
        break;
    case eScene_RhythmFighterVS:
        lbl_80320598 = ver;
        break;
    case eScene_AgbClap:
        lbl_803204D0 = ver;
        break;
    case eScene_AgbGhost:
        lbl_80320508 = ver;
        break;
    case eScene_AgbKanji:
        lbl_80320500 = ver;
        break;
    case eScene_AgbTap:
        lbl_80320510 = ver;
        break;
    case eScene_NightWalk:
        lbl_80320518 = ver;
        break;
    case eScene_Error:
        Error::sceneVer = ver;
        break;
    case eScene_Strap:
        Strap::sceneVer = ver;
        break;
    case eScene_Logo:
        Logo::sceneVer = ver;
        break;
    case eScene_Title:
        lbl_803204A8 = ver;
        break;
    case eScene_GameSelect:
        lbl_80320270 = ver;
        break;
    case eScene_BonusSelect:
        lbl_803204F8 = ver;
        break;
    case eScene_Prologue:
        Prologue::sceneVer = ver;
        break;
    case eScene_Epilogue:
        lbl_80320250 = ver;
        break;
    case eScene_Grading:
        lbl_80320260 = ver;
        break;
    case eScene_Grading2P:
        lbl_803205A0 = ver;
        break;
    case eScene_Perfect:
        lbl_80320438 = ver;
        break;
    case eScene_Navi:
        lbl_803204C8 = ver;
        break;
    case eScene_Cafe:
        lbl_803204B8 = ver;
        break;
    case eScene_CafeMenu:
        lbl_80320528 = ver;
        break;
    case eScene_Reading:
        lbl_80320530 = ver;
        break;
    case eScene_Credit:
        Credit::sceneVer = ver;
        break;
    }
}


// TODO: migrate these functions into their respective scene files
extern "C" void fn_800606E4(void);
extern "C" void fn_800658F8(void);
extern "C" void fn_8003BEE4(void);
extern "C" void fn_8002055C(void);
extern "C" void fn_80013768(void);
extern "C" void fn_80030434(void);
extern "C" void fn_800281D0(void);
extern "C" void fn_8004553C(void);
extern "C" void fn_80047D08(void);
extern "C" void fn_8004BEB4(void);
extern "C" void fn_8004DE50(void);
extern "C" void fn_800B5508(void);
extern "C" void fn_80069878(void);
extern "C" void fn_800344C0(void);
extern "C" void fn_80022F74(void);
extern "C" void fn_8005380C(void);
extern "C" void fn_8006C740(void);
extern "C" void fn_8002A770(void);
extern "C" void fn_80056D80(void);
extern "C" void fn_80075A68(void);
extern "C" void fn_80057E80(void);
extern "C" void fn_8002D0B8(void);
extern "C" void fn_80036798(void);
extern "C" void fn_80059EA8(void);
extern "C" void fn_8006D644(void);
extern "C" void fn_8005D04C(void);
extern "C" void fn_8006F4B4(void);
extern "C" void fn_800BC830(void);
extern "C" void fn_800B95F4(void);
extern "C" void fn_800D7FDC(void);
extern "C" void fn_800C6180(void);
extern "C" void fn_8003F550(void);
extern "C" void fn_80015F10(void);
extern "C" void fn_800D5308(void);
extern "C" void fn_800AD578(void);
extern "C" void fn_8000F8A4(void);
extern "C" void fn_80090AD8(void);
extern "C" void fn_8001DD54(void);
extern "C" void fn_80010840(void);
extern "C" void fn_8009EA84(void);
extern "C" void fn_800C8810(void);
extern "C" void fn_800CB354(void);
extern "C" void fn_8009D718(void);
extern "C" void fn_800A5BA4(void);
extern "C" void fn_800ABBC8(void);
extern "C" void fn_8007963C(void);
extern "C" void fn_80092104(void);
extern "C" void fn_8000EC8C(void);
extern "C" void fn_800A46D8(void);
extern "C" void fn_8000B518(void);
extern "C" void fn_8000C348(void);
extern "C" void fn_800CF884(void);
extern "C" void fn_80083014(void);
extern "C" void fn_8009B458(void);
extern "C" void fn_8009A22C(void);
extern "C" void fn_800B297C(void);
extern "C" void fn_800B4020(void);

void CSceneManager::fn_8008A704(ESceneID sceneID) {
    switch (sceneID) {
    case eScene_Menu:
        CSceneMenu::fn_80006FA4();
        break;
    case eScene_Badminton:
        fn_800606E4();
        break;
    case eScene_Baseball:
        fn_800658F8();
        break;
    case eScene_Batting:
        fn_8003BEE4();
        break;
    case eScene_Birds:
        fn_8002055C();
        break;
    case eScene_Bun:
        fn_80013768();
        break;
    case eScene_Concons:
        fn_80030434();
        break;
    case eScene_Date:
        fn_800281D0();
        break;
    case eScene_Fishing:
        fn_8004553C();
        break;
    case eScene_Flea:
        fn_80047D08();
        break;
    case eScene_Fork:
        fn_8004BEB4();
        break;
    case eScene_Goma:
        fn_8004DE50();
        break;
    case eScene_Interview:
        fn_800B5508();
        break;
    case eScene_Karateman:
        fn_80069878();
        break;
    case eScene_Muscle:
        fn_800344C0();
        break;
    case eScene_Panel:
        fn_80022F74();
        break;
    case eScene_Rap:
        CSceneRap::fn_80050340();
        break;
    case eScene_Receive:
        fn_8005380C();
        break;
    case eScene_Robot:
        fn_8006C740();
        break;
    case eScene_Rocket:
        fn_8002A770();
        break;
    case eScene_Rotation:
        fn_80056D80();
        break;
    case eScene_Samurai:
        fn_80075A68();
        break;
    case eScene_Seal:
        fn_80057E80();
        break;
    case eScene_Seesaw:
        fn_8002D0B8();
        break;
    case eScene_Shrimp:
        fn_80036798();
        break;
    case eScene_Sort:
        fn_80059EA8();
        break;
    case eScene_Tambourine:
        fn_8006D644();
        break;
    case eScene_Tap:
        fn_8005D04C();
        break;
    case eScene_Watch:
        fn_8006F4B4();
        break;
    case eScene_Alarm:
        fn_800BC830();
        break;
    case eScene_Eatmen:
        fn_800B95F4();
        break;
    case eScene_Metroman:
        fn_800D7FDC();
        break;
    case eScene_Shooting:
        fn_800C6180();
        break;
    case eScene_Boat:
        fn_8003F550();
        break;
    case eScene_Kungfu:
        fn_80015F10();
        break;
    case eScene_Omochi:
        fn_800D5308();
        break;
    case eScene_Sword:
        fn_800AD578();
        break;
    case eScene_Basket:
        fn_8000F8A4();
        break;
    case eScene_Tutorial:
        fn_80090AD8();
        break;
    case eScene_Frog:
        fn_8001DD54();
        break;
    case eScene_Choro:
        fn_80010840();
        break;
    case eScene_Hihat:
        fn_8009EA84();
        break;
    case eScene_Mail:
        fn_800C8810();
        break;
    case eScene_RhythmFighterVS:
        fn_800CB354();
        break;
    case eScene_AgbClap:
        fn_8009D718();
        break;
    case eScene_AgbGhost:
        CSceneAgbGhost::fn_800A772C();
        break;
    case eScene_AgbKanji:
        fn_800A5BA4();
        break;
    case eScene_AgbTap:
        CSceneAgbTap::fn_800A9034();
        break;
    case eScene_NightWalk:
        fn_800ABBC8();
        break;
    case eScene_Error:
        fn_8007963C();
        break;
    case eScene_Strap:
        CSceneStrap::fn_8007B2D8();
        break;
    case eScene_Logo:
        CSceneLogo::fn_800A0BC4();
        break;
    case eScene_Title:
        fn_80092104();
        break;
    case eScene_GameSelect:
        fn_8000EC8C();
        break;
    case eScene_BonusSelect:
        fn_800A46D8();
        break;
    case eScene_Prologue:
        CScenePrologue::fn_8000AD98();
        break;
    case eScene_Epilogue:
        fn_8000B518();
        break;
    case eScene_Grading:
        fn_8000C348();
        break;
    case eScene_Grading2P:
        fn_800CF884();
        break;
    case eScene_Perfect:
        fn_80083014();
        break;
    case eScene_Navi:
        fn_8009B458();
        break;
    case eScene_Cafe:
        fn_8009A22C();
        break;
    case eScene_CafeMenu:
        fn_800B297C();
        break;
    case eScene_Reading:
        fn_800B4020();
        break;
    case eScene_Credit:
        CSceneCredit::fn_800C42AC();
        break;
    }
}


// TODO: migrate these functions into their respective scene files
extern "C" void fn_800607D0(void);
extern "C" void fn_80065A94(void);
extern "C" void fn_8003BFD0(void);
extern "C" void fn_800208B4(void);
extern "C" void fn_80013880(void);
extern "C" void fn_800305AC(void);
extern "C" void fn_800282E8(void);
extern "C" void fn_800456B4(void);
extern "C" void fn_80047E24(void);
extern "C" void fn_8004BFD0(void);
extern "C" void fn_8004DF40(void);
extern "C" void fn_800B5624(void);
extern "C" void fn_800699C0(void);
extern "C" void fn_80034618(void);
extern "C" void fn_800230B8(void);
extern "C" void fn_80053994(void);
extern "C" void fn_8006C8DC(void);
extern "C" void fn_8002A88C(void);
extern "C" void fn_80056F50(void);
extern "C" void fn_80075B80(void);
extern "C" void fn_80057F98(void);
extern "C" void fn_8002D230(void);
extern "C" void fn_800369B8(void);
extern "C" void fn_8005A078(void);
extern "C" void fn_8006D760(void);
extern "C" void fn_8005D13C(void);
extern "C" void fn_8006F62C(void);
extern "C" void fn_800B96BC(void);
extern "C" void fn_800D80A4(void);
extern "C" void fn_8003F690(void);
extern "C" void fn_8000F954(void);
extern "C" void fn_80090B88(void);
extern "C" void fn_8001DE04(void);

void CSceneManager::fn_8008A82C(ESceneID sceneID) {
    switch (sceneID) {
    case eScene_Menu:
        break;
    case eScene_Badminton:
        fn_800607D0();
        break;
    case eScene_Baseball:
        fn_80065A94();
        break;
    case eScene_Batting:
        fn_8003BFD0();
        break;
    case eScene_Birds:
        fn_800208B4();
        break;
    case eScene_Bun:
        fn_80013880();
        break;
    case eScene_Concons:
        fn_800305AC();
        break;
    case eScene_Date:
        fn_800282E8();
        break;
    case eScene_Fishing:
        fn_800456B4();
        break;
    case eScene_Flea:
        fn_80047E24();
        break;
    case eScene_Fork:
        fn_8004BFD0();
        break;
    case eScene_Goma:
        fn_8004DF40();
        break;
    case eScene_Interview:
        fn_800B5624();
        break;
    case eScene_Karateman:
        fn_800699C0();
        break;
    case eScene_Muscle:
        fn_80034618();
        break;
    case eScene_Panel:
        fn_800230B8();
        break;
    case eScene_Rap:
        CSceneRap::fn_800504DC();
        break;
    case eScene_Receive:
        fn_80053994();
        break;
    case eScene_Robot:
        fn_8006C8DC();
        break;
    case eScene_Rocket:
        fn_8002A88C();
        break;
    case eScene_Rotation:
        fn_80056F50();
        break;
    case eScene_Samurai:
        fn_80075B80();
        break;
    case eScene_Seal:
        fn_80057F98();
        break;
    case eScene_Seesaw:
        fn_8002D230();
        break;
    case eScene_Shrimp:
        fn_800369B8();
        break;
    case eScene_Sort:
        fn_8005A078();
        break;
    case eScene_Tambourine:
        fn_8006D760();
        break;
    case eScene_Tap:
        fn_8005D13C();
        break;
    case eScene_Watch:
        fn_8006F62C();
        break;
    case eScene_Alarm:
        break;
    case eScene_Eatmen:
        fn_800B96BC();
        break;
    case eScene_Metroman:
        fn_800D80A4();
        break;
    case eScene_Shooting:
        break;
    case eScene_Boat:
        fn_8003F690();
        break;
    case eScene_Kungfu:
        break;
    case eScene_Omochi:
        break;
    case eScene_Sword:
        break;
    case eScene_Basket:
        fn_8000F954();
        break;
    case eScene_Tutorial:
        fn_80090B88();
        break;
    case eScene_Frog:
        fn_8001DE04();
        break;
    }
}

// TODO: migrate tickflow into respective files
TFD_EXTERN(lbl_80253E18)
TFD_EXTERN(lbl_8027C3E8)
TFD_EXTERN(lbl_8027C424)
TFD_EXTERN(lbl_8027C460)
TFD_EXTERN(lbl_8027C49C)
TFD_EXTERN(lbl_80256F30)

void CSceneManager::fn_8008A8D8(void) {
    gTickFlowManager->fn_801E1E4C();

    bool temp_r31 = false;
    bool temp_r30 = false;
    bool temp_r29 = false;
    bool temp_r28 = false;

    if (mSceneHist[0] == eScene_Navi) {
        s32 gameSelectPosInHist = findSceneInHist(eScene_GameSelect);
        s32 bonusSelectPosInHist = findSceneInHist(eScene_Cafe);

        if ((gameSelectPosInHist >= 1) && (bonusSelectPosInHist >= 1)) {
            if (gameSelectPosInHist < bonusSelectPosInHist) {
                temp_r31 = true;
            }
            else if (bonusSelectPosInHist < gameSelectPosInHist) {
                temp_r29 = true;
            }
        }
        else if (gameSelectPosInHist >= 1) {
            temp_r31 = true;
        }
        else if (bonusSelectPosInHist >= 1) {
            temp_r29 = true;
        }
        else {
            temp_r28 = true;
        }
    }
    else if ((mSceneHist[0] == eScene_Title) || (mSceneHist[0] == eScene_GameSelect)) {
        temp_r28 = true;
    }
    else {
        if (CExScene::fn_80009FB4()) {
            temp_r29 = true;
        }
        else {
            s32 gameSelectPosInHist = findSceneInHist(eScene_GameSelect);
            s32 bonusSelectPosInHist = findSceneInHist(eScene_BonusSelect);

            if ((gameSelectPosInHist >= 1) && (bonusSelectPosInHist >= 1)) {
                if (gameSelectPosInHist < bonusSelectPosInHist) {
                    temp_r31 = true;
                }
                else if (bonusSelectPosInHist < gameSelectPosInHist) {
                    temp_r30 = true;
                }
            }
            else if (gameSelectPosInHist >= 1) {
                temp_r31 = true;
            }
            else if (bonusSelectPosInHist >= 1) {
                temp_r30 = true;
            }
            else {
                temp_r28 = true;
            }
        }
    }

    if (temp_r31) {
        gTickFlowManager->fn_801E1CC0(lbl_80253E18);
    }
    else if (temp_r30) {
        switch (lbl_803204F8) {
        case 0:
            gTickFlowManager->fn_801E1CC0(lbl_8027C3E8);
            break;
        case 1:
            gTickFlowManager->fn_801E1CC0(lbl_8027C424);
            break;
        case 2:
            gTickFlowManager->fn_801E1CC0(lbl_8027C460);
            break;
        case 5:
            gTickFlowManager->fn_801E1CC0(lbl_8027C49C);
            break;
        }
    }
    else if (temp_r29) {
        gTickFlowManager->fn_801E1CC0(lbl_80256F30);
    }
    else if (temp_r28) {
        OSReport("Invalid Scene Change 0002\n");
    }
}

u32 CSceneManager::fn_8008B058(s32 pos) {
    return mSceneHist[pos];
}

void CSceneManager::fn_8008B068(void) {
    for (s32 i = 0; i < (s32)ARRAY_LENGTH(mSceneHist); i++) {
        mSceneHist[i] = eScene_Null;
    }
}

bool CSceneManager::fn_8008B0FC(s32 pos, u32 sceneID) {
    return mSceneHist[pos] == sceneID;
}

bool CSceneManager::fn_8008B118(ESceneID sceneID) {
    return findSceneInHist(sceneID) >= 0;
}

bool CSceneManager::fn_8008B27C(void) {
    if (lbl_80320274) {
        return false;
    }

    if (gSceneManager->findSceneInHist(eScene_GameSelect) < 0) {
        return false;
    }

    s32 gameSelectPosInHist = findSceneInHist(eScene_GameSelect);
    s32 bonusSelectPosInHist = findSceneInHist(eScene_BonusSelect);

    if (
        (bonusSelectPosInHist != -1) &&
        (bonusSelectPosInHist < gameSelectPosInHist)
    ) {
        return false;
    }

    s32 creditPosInHist = findSceneInHist(eScene_Credit);
    if ((creditPosInHist != -1) && (creditPosInHist < gameSelectPosInHist)) {
        return false;
    }

    return true;
}
