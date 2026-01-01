#include "Perfect/MyChecker.hpp"

u32 Perfect::CMyChecker::_24(u32 a) {
    mScene->fn_8008328C(a, 0);

    return 1;
}

u32 Perfect::CMyChecker::_28(u32 a) {
    mScene->fn_8008328C(a, 1);

    return 1;
}

u32 Perfect::CMyChecker::_2C(void) {
    return 1;
}

u32 Perfect::CMyChecker::_30(u32 a) {
    mScene->fn_8008328C(a, 2);

    return 1;
}

Perfect::CMyChecker::~CMyChecker(void) {}

void Perfect::CMyChecker::_14(void) {
    this->~CMyChecker();
}
