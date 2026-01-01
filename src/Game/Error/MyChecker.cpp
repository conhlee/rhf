#include "Error/MyChecker.hpp"

u32 Error::CMyChecker::_24(u32 a) {
    mScene->fn_80079B3C(a, 0);

    return 1;
}

u32 Error::CMyChecker::_28(u32 a) {
    mScene->fn_80079B3C(a, 1);

    return 1;
}

u32 Error::CMyChecker::_2C(void) {
    return 1;
}

u32 Error::CMyChecker::_30(u32 a) {
    mScene->fn_80079B3C(a, 2);

    return 1;
}

Error::CMyChecker::~CMyChecker(void) {}

void Error::CMyChecker::_14(void) {
    this->~CMyChecker();
}
