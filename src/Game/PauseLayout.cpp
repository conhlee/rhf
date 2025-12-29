#include "PauseLayout.hpp"

#include "LayoutManager.hpp"

#include "MessageManager.hpp"

#include "SoundManager.hpp"

#include "TickFlowManager.hpp"

#include "InputCheckManager.hpp"

#include "SceneManager.hpp"

#include "Controller.hpp"

#include "ExScene.hpp"

#include "rev_tengoku.rsid"

static const char layoutFile_pause_00[] = "pause_00.brlyt";

DECL_SECTION(".sdata") static const char * const layoutFileTable[] = { layoutFile_pause_00, NULL };
// TODO: hack!

static const char * layoutAnimTable[] = {
    "pause_00_open",
    "pause_00_close",
    "pause_00_loop"
};

extern u8 lbl_80320300;
extern bool lbl_80320274;

void CPauseLayout::_10(void) {
    nw4r::lyt::MultiArcResourceAccessor *resAccessor = gLayoutManager->getResAccessor();

    u32 layoutBinSize;
    void *layoutBin = resAccessor->GetResource(0, layoutFileTable[0], &layoutBinSize);

    buildLayout(layoutBin, resAccessor);

    for (u32 i = 0; (s32)i < (s32)ARRAY_LENGTH(layoutAnimTable); i++) {
        getAnimation(i)->fn_801D9E70(getLayout(), NULL, layoutAnimTable[i], resAccessor);
    }
    getAnimation(2)->setLoop(true);

    gMessageManager->fn_80088474(getLayout()->GetRootPane());

    this->CLayout::_10();
    fn_801D9E24(false);

    mIsPaused = false;
    mIsEnabled = true;

    getLayout()->GetRootPane()->SetVisible(false);

    lbl_80320300 = 0;

    mUnk28 = 5;
}

void CPauseLayout::_14(void) {
    this->CLayout::_14();
}

void CPauseLayout::_18(nw4r::lyt::DrawInfo *drawInfo) {
    CController *controllerP1 = gControllerManager->fn_801D5FF0(0);
    CController *controllerP2 = gControllerManager->fn_801D5FF0(1);

    mUnk24 = 0;

    if (mIsPaused) {
        bool doUnpause = false;
    
        if (mUnk20 == 0) {
            if (!getAnimation(0)->getIsPlaying()) {
                mUnk20 = 1;
                getAnimation(2)->playFromBeginning();
            }
        }
        else if (mUnk20 == 1) {
            CGCController *gcController = gControllerManager->fn_801D6000(0);
    
            u32 trig = controllerP1->getCoreTrig();
            if ((trig & WPAD_BUTTON_PLUS) != 0 || (trig & WPAD_BUTTON_A) != 0) {
                mUnk20 = 2;
    
                getAnimation(2)->setIsPlaying(false);
        
                getAnimation(1)->playFromBeginning();
        
                gSoundManager->play(SE_PAUSE_CONTINUE);
            }
            else {
                if (trig & WPAD_BUTTON_MINUS) {
                    mUnk24 = 2;
                    doUnpause = true;
                    getAnimation(2)->setIsPlaying(false);
                    gSoundManager->play(SE_PAUSE_QUIT);
                }
            }
        }
        else if (mUnk20 == 2) {
            if (!getAnimation(1)->getIsPlaying()) {
                mUnk24 = 1;
                doUnpause = true;
            }
        }

        if (doUnpause) {
            mIsPaused = false;
            
            if (mUnk24 != 2) {
                getLayout()->GetRootPane()->SetVisible(false);
            }
    
            gTickFlowManager->fn_801E4048(false);
            gInputCheckManager->setUnk41C(false);
            gSoundManager->fn_801E6F98(false);
        }
    }
    else {
        if (mIsEnabled) {
            bool inTwoPlay = lbl_80320274 || (gSceneManager->fn_8008B058(0) == eScene_RhythmFighterVS);

            if (!CExScene::fn_8000A370()) {
                if (
                    !(
                        inTwoPlay && (
                            (controllerP1->getUnk1338() & 0x9F0F) ||
                            (controllerP2->getUnk1338() & 0x9F1F)
                        )
                    ) && (inTwoPlay || !(controllerP1->getUnk1338() & 0x9F0F))
                ) {
                    if ((mUnk28 <= 0) && ((controllerP1->getUnk133C() & WPAD_BUTTON_PLUS) || lbl_80320300)) {
                        if (lbl_80320300) {
                            mUnk20 = 1;
                        }
                        else {
                            mUnk20 = 0;
                            getAnimation(0)->playFromBeginning();
                        }

                        mIsPaused = true;

                        getLayout()->GetRootPane()->SetVisible(true);

                        CExScene::fn_800098D0();

                        gTickFlowManager->fn_801E4048(true);
                        gInputCheckManager->setUnk41C(true);
                        gSoundManager->fn_801E6F98(true);

                        if (controllerP1->getUnk133C() & WPAD_BUTTON_PLUS) {
                            gSoundManager->play(SE_PAUSE_ON);
                        }
                    }
                }
            }
        }
    }

    if (mUnk28 > 0) {
        mUnk28--;
    }

    this->CLayout::_18(drawInfo);
}

void CPauseLayout::_1C(nw4r::lyt::DrawInfo *drawInfo) {
    if (lbl_80320300) {
        return;
    }
    this->CLayout::_1C(drawInfo);
}

CPauseLayout::~CPauseLayout(void) {
    _14();
}
