#include "Credit/MyFlow.hpp"

#include "Credit/MyFlowDecl.hpp"

#include "Credit/MyLayout.hpp"

#include "Credit/MyChecker.hpp"

#include "InputCheckManager.hpp"
#include "InputChecker.hpp"

#include "LayoutManager.hpp"

#include "cellanim/credit/ver0/rcad_chara_00_labels.h"
#include "cellanim/credit/ver0/rcad_wipe_labels.h"
#include "cellanim/credit/ver0/rcad_title_labels.h"

TICKFLOW_IMPL_CREATE_FN(Credit::CMyFlow)

Credit::CMyFlow::~CMyFlow(void) {}

extern s32 lbl_80320518; // Night Walk scene version. TODO: replace when possible

bool Credit::CMyFlow::_1C(u32 opcode, u32 arg0, const s32 *args) {
    switch (opcode) {
    case TF_CREDIT_INPUT: {
        CMyChecker *checker = gInputCheckManager->makeNew<CMyChecker>();

        gInputCheckManager->fn_801E9158(checker, true);
    } break;

    case TF_CREDIT_NEXT_CHARA: {
        mScene->fn_800C4C74(args[0]);
    } break;

    case TF_CREDIT_SET_TEXT_POS: {
        if (arg0 == 0) {
            gLayoutManager->getLayout<CMyLayout>(0)->fn_800C416C(args[0], args[1], args[2]);
        }
    } break;

    case TF_CREDIT_CHARA_BEAT: {
        if (arg0 == 0) {
            u32 charaIndex = mScene->mCharaIndex;
            if (
                charaIndex <= (CSceneCredit::CELL_INDEX_CHARA_END - 1) &&
                !mScene->mScreenScroll.getIsTicking()
            ) {
                mScene->mCharaAnim[charaIndex]->fn_801DD0AC(chara_00_beat);
            }
        }
    } break;

    case TF_CREDIT_MASTER_ANIM: {
        if (arg0 == 0) {
            s32 type = args[0];
            if (type == 0) {
                mScene->mCharaAnim[0]->fn_801DD0AC(chara_00_beat);
            }
            else if (type == 1) {
                mScene->mMasterAnim->fn_801DD0AC(chara_00_master_beat_00);
            }
            else if (type == 2) {
                mScene->mMasterAnim->fn_801DD0AC(chara_00_master_beat_01);
            }
        }
        else if (arg0 == 1) {
            mScene->mMasterAnim->fn_801DD0AC(chara_00_master_look);
        }
    } break;

    case TF_CREDIT_MASCOT_ANIM: {
        if (arg0 == 0) {
            mScene->fn_800C4D2C(args[0], args[1], args[2], args[3]);
        }
        else if (arg0 == 1) {
            mScene->fn_800C4DB4(args[0], args[1]);
        }
        else if (arg0 == 2) {
            for (s32 i = 0; i < CSceneCredit::MASCOT_COUNT; i++) {
                mScene->fn_800C4E38(i);
            }
        }
    } break;

    case TF_CREDIT_WIPE_ANIM: {
        if (arg0 == 0) {
            if (args[0] == 0) {
                mScene->mWipeAnim->fn_801DD0AC(wipe_wipe_show_00);
            }
            else if (args[0] == 1) {
                mScene->mWipeAnim->fn_801DD0AC(wipe_wipe_show_01);
            }
        }
        else if (arg0 == 1) {
            if (args[0] == 0) {
                mScene->mWipeAnim->fn_801DD0AC(wipe_wipe_hide_00);
            }
            else if (args[0] == 1) {
                mScene->mWipeAnim->fn_801DD0AC(wipe_wipe_hide_01);
            }
        }
        else if (arg0 == 2) {
            s32 repeatCount = args[0];
            for (s32 i = 0; i < repeatCount; i++) {
                if (i == 0) {
                    mScene->mWipeAnim->fn_801DD0AC(wipe_talk_00);
                }
                else {
                    mScene->mWipeAnim->fn_801DCEE8(wipe_talk_00, NULL);
                }
            }
        }
    } break;

    case TF_CREDIT_CHECK_NIGHTWALK_VER: {
        mCondvar = (lbl_80320518 == 2);
    } break;

    case TF_CREDIT_TITLE_CLOSE: {
        if (arg0 == 0) {
            mScene->mTitleAnim->fn_801DD0AC(title_title_close);
        }
    } break;
    
    default:
        return this->CExFlow::_1C(opcode, arg0, args);
    }

    return false;
}

void Credit::CMyFlow::_14(void) {
    this->~CMyFlow();
}
