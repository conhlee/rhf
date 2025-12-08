#include "AgbTap/MyFlow.hpp"

#include "AgbTap/MyFlowDecl.hpp"

#include "AgbTap/MyChecker.hpp"

#include "InputCheckManager.hpp"

TICKFLOW_IMPL_CREATE_FN(AgbTap::CMyFlow)

AgbTap::CMyFlow::~CMyFlow(void) {}

bool AgbTap::CMyFlow::_1C(u32 opcode, u32 arg0, const s32 *args) {
    switch (opcode) {
    case TF_AGBTAP_INPUT: {
        CMyChecker *checker = gInputCheckManager->makeNew<CMyChecker>();
        checker->fn_801E7D5C(4);
        checker->fn_801E7DBC(48.0f, -10.0f, -6.0f, 6.0f, 10.0f);

        gInputCheckManager->fn_801E9158(checker, true);
    } break;

    case TF_AGBTAP_BEAT_ANIM: {
        mScene->fn_800A9768();
    } break;

    case TF_AGBTAP_SET_ANIME_MONKEY: {
        mScene->fn_800A9988(static_cast<EAgbTapMove>(args[0]));
    } break;

    case TF_AGBTAP_SET_ANIME_GIRL: {
        mScene->fn_800A97DC(static_cast<EAgbTapMove>(args[0]));
    } break;

    case TF_AGBTAP_RESET_PERFORMANCE: {
        mScene->fn_800A9758();
    } break;

    case TF_AGBTAP_GIRAFFE_MOVE: {
        mScene->fn_800A9B20(static_cast<bool>(args[0]));
    } break;

    case TF_AGBTAP_GIRAFFE_ANIM: {
        if (arg0 == 0) {
            mScene->fn_800A9C3C();
        }
        else if (arg0 == 1) {
            mScene->fn_800A9C84(args[0]);
        }
        else if (arg0 == 2) {
            mScene->fn_800A9D20();
        }
    } break;

    case TF_AGBTAP_SET_BALLOON_TARGET: {
        if (arg0 == 0) {
            mScene->fn_800A9D90();
        }
        else if (arg0 == 1) {
            mScene->fn_800A9E24();
        }
    } break;
    
    default:
        return this->CExFlow::_1C(opcode, arg0, args);
    }

    return false;
}

void AgbTap::CMyFlow::_14(void) {
    this->~CMyFlow();
}
