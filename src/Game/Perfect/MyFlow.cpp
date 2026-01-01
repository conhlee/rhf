#include "Perfect/MyFlow.hpp"

#include "Perfect/MyFlowDecl.hpp"

#include "Perfect/MyChecker.hpp"

#include "InputCheckManager.hpp"

TICKFLOW_IMPL_CREATE_FN(Perfect::CMyFlow)

Perfect::CMyFlow::~CMyFlow(void) {}

bool Perfect::CMyFlow::_1C(u32 opcode, u32 arg0, const s32 *args) {
    switch (opcode) {
    case TF_PERFECT_INPUT: {
        CMyChecker *checker = gInputCheckManager->makeNew<CMyChecker>();

        gInputCheckManager->fn_801E9158(checker, true);
    } break;
    
    default:
        return this->CExFlow::_1C(opcode, arg0, args);
    }

    return false;
}

void Perfect::CMyFlow::_14(void) {
    this->~CMyFlow();
}
