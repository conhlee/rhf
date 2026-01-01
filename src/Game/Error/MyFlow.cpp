#include "Error/MyFlow.hpp"

#include "Error/MyFlowDecl.hpp"

#include "Error/MyChecker.hpp"

#include "InputCheckManager.hpp"

TICKFLOW_IMPL_CREATE_FN(Error::CMyFlow)

Error::CMyFlow::~CMyFlow(void) {}

bool Error::CMyFlow::_1C(u32 opcode, u32 arg0, const s32 *args) {
    switch (opcode) {
    case TF_ERROR_INPUT: {
        CMyChecker *checker = gInputCheckManager->makeNew<CMyChecker>();

        gInputCheckManager->fn_801E9158(checker, true);
    } break;
    
    default:
        return this->CExFlow::_1C(opcode, arg0, args);
    }

    return false;
}

void Error::CMyFlow::_14(void) {
    this->~CMyFlow();
}
