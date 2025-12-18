#include "Metroman/MyFlow.hpp"

#include "Metroman/MyFlowDecl.hpp"

#include "Metroman/MyChecker.hpp"

#include "InputCheckManager.hpp"

#include "Mem.hpp"

#include "cellanim/metroman/ver0/rcad_metroman_00_labels.h"

TICKFLOW_IMPL_CREATE_FN(Metroman::CMyFlow)

Metroman::CMyFlow::~CMyFlow(void) {}

bool Metroman::CMyFlow::_1C(u32 opcode, u32 arg0, const s32 *args) {
    switch (opcode) {
    case TF_METROMAN_INPUT: {
        CMyChecker *checker = gInputCheckManager->makeNew<CMyChecker>();

        checker->fn_801E7D5C(eInputType_TriggerA);

        checker->fn_801E7DBC(args[0], -6.0f, -4.0f, 4.0f, 6.0f);

        gInputCheckManager->fn_801E9158(checker, true);

        mScene->mUnk9D = false;
    } break;

    case TF_METROMAN_GUIDE_PULSE: {
        mScene->fn_800D8A78(1, 0);
        mScene->fn_800D8B78();
    } break;

    case TF_METROMAN_202: {
        if (arg0 == 0) {
            mScene->fn_800D8B8C(args[0]);
        }
        else if (arg0 == 1) {
            mCondvar = mScene->mUnk84;
        }
    } break;

    case TF_METROMAN_203: {
        if (arg0 == 0) {
            mScene->mManAnim->fn_801DD0AC(metroman_00_man_up_R);
            mScene->mManAnim->fn_801DCF38();

            mScene->mManShadowAnim->fn_801DD24C(metroman_00_man_shadow, 0.0f);
            mScene->mManShadowAnim->fn_801DD2B4(1);
            mScene->mManShadowAnim->setPos(1.0f, 1.0f);

            mScene->mManNextDirection = 2;
            mScene->mManIsLeft = true;

            mScene->mHandRotation.fn_801EA698(-60.0f, 60.0f);
            mScene->mHandRotation.fn_801EA550(0.0f, 48.0f, true);
            mScene->mHandRotation.fn_801EA50C(mScene->mHandRotation.getMaxTime());

            mScene->mHandDirection = 0;
            mScene->mUnk84 = false;
        }
    } break;

    case TF_METROMAN_204: {
        mScene->mUnk9C = true;
        mScene->mScoreCounter->fn_80083CAC(true);
        mScene->mHiScoreCounter->fn_80083CAC(true);
        mScene->mHiScoreRecordAnim->setEnabled(true);
    } break;

    case TF_METROMAN_205: {
        CCellAnim *anim;
        u16 animID = metroman_00_man_down_R_from_L;
        
        if (mScene->fn_8000A958()) {
            anim = mScene->mManAnim;
            switch (anim->getAnimID()) {
            case metroman_00_man_hit_R_from_L:
                animID = metroman_00_man_guts_R_from_L;
                break;
            case metroman_00_man_hit_R_from_R:
                animID = metroman_00_man_guts_R_from_R;
                break;
            case metroman_00_man_hit_L_from_R:
                animID = metroman_00_man_guts_L_from_R;
                break;
            case metroman_00_man_hit_L_from_L:
                animID = metroman_00_man_guts_L_from_L;
                break;
            default:
                break;
            }
        }
        else {
            anim = mScene->mManAnim;
            switch (anim->getAnimID()) {
            case metroman_00_man_hit_R_from_L:
                animID = metroman_00_man_down_R_from_L;
                break;
            case metroman_00_man_hit_R_from_R:
                animID = metroman_00_man_down_R_from_R;
                break;
            case metroman_00_man_hit_L_from_R:
                animID = metroman_00_man_down_L_from_R;
                break;
            case metroman_00_man_hit_L_from_L:
                animID = metroman_00_man_down_L_from_L;
                break;
            default:
                break;
            }
        }

        anim->fn_801DD0AC(animID);
    } break;

    case TF_METROMAN_206: {
        if (arg0 == 0) {
            mScene->fn_800D8CA4(args[0]);
        }
        else if (arg0 == 1) {
            mScene->fn_800D8D1C();
        }
        else if (arg0 == 2) {
            mScene->mUnk9E = true;
            mScene->mLampSize = 0;
        }
    } break;

    case TF_METROMAN_207: {
        if (arg0 == 0) {
            mScene->mUnkA8 = 0;
        }
        else if (arg0 == 1) {
            mScene->mUnkA8++;
        }
        else if (arg0 == 2) {
            if (mScene->mUnkA8 != 0) {
                mCondvar = mScene->mUnkA8 != 0;
            }
            else {
                mCondvar = 0;
            }
        }
        else if (arg0 == 3) {
            mCondvar = mScene->mUnkA8;
        }
    } break;
    
    default:
        return this->CExFlow::_1C(opcode, arg0, args);
    }

    return false;
}

void Metroman::CMyFlow::_14(void) {
    this->~CMyFlow();
}
