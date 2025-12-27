#include "Scene.hpp"

#include "GameManager.hpp"
#include "FileManager.hpp"

void CScene::fn_801D83BC(void) {
    delete this;
}

void CScene::fn_801D83DC(void) {
    fn_801D369C(mHeapGroup);

    switch (mState) {
    case eState_Initial:
        _10();
        mState = eState_Loading;
        break;
    case eState_Loading:
        if (_24()) {
            _14();
            if (mUnk04) {
                _18();
                mUnk04 = false;
            }
            mState = eState_Ready;
            fn_801D3634();
        }
        break;
    case eState_Ready:
        if (gGameManager->_30()) {
            mState = eState_Active;
        }
        break;
    case eState_Active:
        _18();
        break;
    case eState_ScheduleDown:
        if (gGameManager->_2C()) {
            mState = eState_Down;
        }
        break;
    case eState_Down:
        _20();
        mState = eState_Dead;
        break;
    }

    fn_801D3644();
}

void CScene::fn_801D8554(void) {
    if (((mState == eState_Ready) || (mState == eState_Active) || (mState == eState_ScheduleDown))) {
        _1C();
    }
}

void CScene::fn_801D8578(void) {
    mState = eState_ScheduleDown;
    gGameManager->_28();
}

bool CScene::_24(void) {
    return gFileManager->fn_801D431C();
}
