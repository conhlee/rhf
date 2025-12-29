#include "InputChecker.hpp"
#include "CheckPointManager.hpp"
#include "InputCheckManager.hpp"
#include "TickFlowManager.hpp"

CInputChecker::CInputChecker() {
    unk2C = 0;
    unk50 = 0;
    unk51 = 0;
    mTickPass = 0.0f;
    unk71 = false;
    unk72 = false;
    unk74 = 2;
    unk70 = true;
    unk78 = gCheckPointManager->getUnkC5();
}

CInputChecker::~CInputChecker() {}

void CInputChecker::_14(void) {
    this->~CInputChecker();
}

u32 CInputChecker::_18(void) {
    mTickPass += gTickFlowManager->fn_801E2698();
    if (unk71 && !unk72 && unk50 && (unk74 <= 1) && unk51 && !(--unk51)) {
        unk72 = true;
        _34();
        if (unk70) {
            return 1;
        }
    }

    if (!unk71 && (mTickPass > (mTickJust + mRangeMissB))) {
        unk71 = true;
        gInputCheckManager->fn_801E9488(0);
        gCheckPointManager->fn_801EAF2C(unk78, 2);
        if (_2C()) {
            return 1;
        }
    }
    return _1C();
}

void CInputChecker::fn_801E7D5C(u32 arg0) {
    unk0C[0] = arg0;
    unk2C = 1;
}

void CInputChecker::fn_801E7D6C(u32 arg0) {
    unk0C[unk2C] = arg0;
    unk2C++;
}

void CInputChecker::fn_801E7D8C(void) {
    unk2C = 0;
}

void CInputChecker::fn_801E7D98(u32 arg0) {
    unk30[0] = arg0;
    unk50 = 1;
}

void CInputChecker::fn_801E7DA8(void) {
    unk50 = 0;
}

void CInputChecker::fn_801E7DB4(u32 arg0) {
    unk51 = arg0;
}

void CInputChecker::fn_801E7DBC(f32 tickJust, f32 rangeMissF, f32 rangeJustF, f32 rangeJustB, f32 rangeMissB) {
    mTickJust = tickJust;
    mRangeMissF = rangeMissF;
    mRangeJustF = rangeJustF;
    mRangeJustB = rangeJustB;
    mRangeMissB = rangeMissB;
}

void CInputChecker::fn_801E7DD4(f32 *tickJust, f32 *rangeMissF, f32 *rangeJustF, f32 *rangeJustB, f32 *rangeMissB) {
    if (tickJust != NULL) {
        *tickJust = mTickJust;
    }
    if (rangeMissF != NULL) {
        *rangeMissF = mRangeMissF;
    }
    if (rangeJustF != NULL) {
        *rangeJustF = mRangeJustF;
    }
    if (rangeJustB != NULL) {
        *rangeJustB = mRangeJustB;
    }
    if (rangeMissB != NULL) {
        *rangeMissB = mRangeMissB;
    }
}

bool CInputChecker::fn_801E7E28(u32 arg0) {
    for (s32 i = 0; i < unk2C; i++) {
        if (unk0C[i] == arg0) {
            return true;
        }
    }
    return false;
}

bool CInputChecker::fn_801E7E5C(u32 arg0) {
    return unk0C[0] == arg0;
}

bool CInputChecker::fn_801E7E70(u32 arg0) {
    for (s32 i = 0; i < unk50; i++) {
        if (unk30[i] == arg0) {
            unk72 = true;
            return true;
        }
    }
    return false;
}

u32 CInputChecker::_20(u8 &arg0, u32 arg1, f32 arg2) {
    if (unk71) {
        return 2;
    }

    f32 rangeMissF = mRangeMissF;
    f32 rangeJustF = mRangeJustF;
    f32 rangeJustB = mRangeJustB;
    f32 rangeMissB = mRangeMissB;
    mLag = (mTickPass - mTickJust) - arg2;
    if (arg1) {
        fn_801E80BC(rangeMissF, rangeJustF, rangeJustB, rangeMissB);
    }
    if ((rangeJustF <= mLag) && (mLag <= rangeJustB)) {
        f32 just = rangeJustB - rangeJustF;
        unk71 = true;
        f32 justF = (just <= 4.0f) ? rangeJustF : (rangeJustF + just / 4.0f);
        f32 justB = (just <= 4.0f) ? rangeJustB : (rangeJustB - just / 4.0f);
        arg0 = ((justF <= mLag) && (mLag <= justB)) ? 100 : 95;
        unk74 = 0;
        return 0;
    }
    if ((rangeMissF <= mLag) && (mLag <= rangeMissB)) {
        unk71 = true;
        arg0 = 80;
        unk74 = 1;
        return 1;
    }
    return 2;
}

bool CInputChecker::fn_801E8018(f32 arg0, f32 arg1) {
    f32 currTick = mTickJust - mTickPass;
    f32 missF = currTick + mRangeMissF;
    f32 missB = currTick + mRangeMissB;
    return ((missF < arg1) && (arg0 < missB));
}

u32 CInputChecker::_24(u32 arg0) {
    return 1;
}

u32 CInputChecker::_28(u32 arg0) {
    return 1;
}

u32 CInputChecker::_2C(void) {
    return 1;
}

u32 CInputChecker::_30(u32 arg0) {
    return 1;
}

void CInputChecker::finalInsert(void) {}

void CInputChecker::finalDestroy(void) {
    _14();
    gInputCheckManager->fn_801E9150(this);
}

void CInputChecker::fn_801E80BC(f32 &rangeMissF, f32 &rangeJustF, f32 &rangeJustB, f32 &rangeMissB) {
    if ((rangeJustB - rangeJustF) > 4.0) {
        f32 mid = (rangeJustB + rangeJustF) / 2;
        rangeMissF = mid - 4.0;
        rangeJustF = mid - 1.0;
        rangeJustB = mid + 1.0;
        rangeMissB = mid + 4.0;
    }
}
