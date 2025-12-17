#include "Controller.hpp"

#include <cstring>

#include <revolution/PAD.h>
#include <revolution/KPAD.h>
#include <revolution/WPAD.h>

#include <nw4r/math.h>

#include "Mem.hpp"

#include "TickFlowManager.hpp"

/* 
        CController
*/

CController::CController(s32 channel) {
    mMyChannel = channel;
    unk136D = false;
    unk136C = false;
    unk1370 = 0;
    mMotorSeqPlaying = 0;
    unk1338 = 0;
    unk133C = 0;
    unk1340 = 0;
    for (s32 i = 0; i < (s32)ARRAY_LENGTH(unk1344); i++) {
        unk1344[i] = 0;
    }
    unk1364 = 10;
    unk1365 = 4;
    for (s32 i = 0; i < (s32)ARRAY_LENGTH(unk1354); i++) {
        if ((u16)(1 << i)) {
            unk1354[i] = 0;
        }
    }

    for (s32 i = 0; i < (s32)ARRAY_LENGTH(unk1354); i++) {
        if (((1 << i) & (0x800 | 0x400))) {
            unk1354[i] = 6;
        }
    }
}

void CController::_0C(void) {
    mInfoUpdated = false;
}

void CController::_10(void) {
    u32 temp_r30 = unk1338;
    fn_801D4DDC();
    fn_801D4E38(temp_r30);
    unk1368 = 0;
    fn_801D4F74(temp_r30);
    fn_801D4FD8();

    KPADSetPosParam(mMyChannel, .05f, .8f);
    if (mCoreStatus[0].speed < .01f) {
        KPADSetPosParam(mMyChannel, .05f, .13f);
    }
}

void CController::_14(void) {
    _4C();
}

void CController::fn_801D4DDC(void) {
    mProbeErrcode = WPADProbe(mMyChannel, &mProbeType);
    mKPADReadLength = KPADRead(mMyChannel, mCoreStatus, ARRAY_LENGTH(mCoreStatus));
    KPADGetUnifiedWpadStatus(mMyChannel, mUnifiedStatus, mKPADReadLength);
}

void CController::fn_801D4E38(u32 arg1) {
    do801D4EA4(arg1);
}

void CController::fn_801D4EA4(u32 arg1, u32 arg2) {
    u32 xorinput = arg2 ^ arg1;
    unk1338 = 0;
    unk133C = 0;
    unk1340 = 0;
    u32 xorarg1 = xorinput & arg1;
    u32 xorarg2 = xorinput & arg2;

    for (s32 i = 0; i < (s32)ARRAY_LENGTH(unk1344); i++) {
        u32 temp_r12 = 1 << i;
        if (unk1344[i]) {
            if (gTickFlowManager->getUnk6D()) {
                continue;
            }
            unk1344[i]--;
            if ((unk1344[i] == 0) && (arg1 & temp_r12)) {
                unk1344[i] = 1;
            }
        }
        else {
            if (arg1 & temp_r12) {
                unk1338 |= temp_r12;
            }
            if (xorarg1 & temp_r12) {
                unk133C |= temp_r12;
            }
            if (xorarg2 & temp_r12) {
                unk1340 |= temp_r12;
                if (!gTickFlowManager->getUnk6D()) {
                    unk1344[i] = unk1354[i];
                }
            }
        }
    }
}

void CController::fn_801D4F74(u32 arg1) {
    unk1368 = 0;
    if (unk133C) {
        unk1366 = 0;
    }
    if (arg1 == unk1338) {
        unk1366++;
        if ((unk1366 == unk1364) || (unk1366 == (unk1364 + unk1365))) {
            unk1368 = unk1338;
            unk1366 = unk1364;
        }
    }
}

void CController::fn_801D4FD8(void) {
    u32 temp;
    unk136D = unk136C;
    if (!unk136D) {
        if ((unk1370 - 1) > 1) { // TODO: maybe fake?
            if (!unk1370) {
                temp = unk133C;
                if (((temp >> 11) & 1) && (temp & 0x400)) {
                    unk1370 = 3;
                    unk136C = true;
                    return;
                }
                if ((temp >> 11) & 1) {
                    unk1370 = 1;
                    unk1374 = 0;
                    return;
                }
                if (temp & 0x400) {
                    unk1370 = 2;
                    unk1374 = 0;
                }
            }
            return;
        }
        unk1374++;
        if (unk1374 >= 4) {
            unk1370 = 0;
            return;
        }
        if (((unk133C & 0x800)) || ((unk133C & 0x400))) {
            unk1370 = 3;
            unk136C = true;
        }
        return;
    }
    if ((unk1370 - 1) > 1) { // TODO: maybe fake?
        if ((s32)unk1370 != 3) {
            return;
        }
        temp = unk1340;
        if (((temp >> 11) & 1) && (temp & 0x400)) {
            unk1370 = 0;
            unk136C = false;
            return;
        }
        if ((temp >> 11) & 1) {
            unk1370 = 2;
            unk1374 = 0;
            return;
        }
        if (temp & 0x400) {
            unk1370 = 1;
            unk1374 = 0;
        }
        return;
    }
    unk1374++;
    if (unk1374 >= 4) {
        unk1370 = 0;
        unk136D = false;
        unk136C = false;
        return;
    }
    if (((unk1340 & 0x800)) || ((unk1340 & 0x400))) {
        unk1370 = 0;
        unk136C = false;
    }
    return;
}

void CController::fn_801D5170(bool arg1) {
    if (arg1) {
        KPADSetPosParam(mMyChannel, .05f, .8f);
        if (mCoreStatus[0].speed < .01f) {
            KPADSetPosParam(mMyChannel, .05f, .13f);
        }
    }
    else {
        KPADSetPosParam(mMyChannel, .0f, 1.0f);
    }
}

Vec2 CController::fn_801D51E4(f32 arg1, f32 arg2) {
    return (Vec2) {
        getCorePos().x * arg1 / 2.0f,
        getCorePos().y * arg2 / 2.0f
    };
}

Vec2 CController::fn_801D523C(CLayout *arg1) {
    nw4r::ut::Rect rect = arg1->getLayout()->GetLayoutRect();
    return fn_801D51E4(rect.right - rect.left, rect.bottom - rect.top);
}

bool CController::fn_801D52D4(void) {
    bool out = false;
    if (_24() && (mKPADReadLength > 0) && (mCoreStatus[0].dpd_valid_fg)) {
        out = true;
    }
    return out;
}

bool CController::fn_801D5340(void) {
    // TODO: maybe inlines here?
    bool temp_r30 = false;
    bool temp_r29 = false;
    bool temp_r28 = false;
    bool temp_r27 = false;
    if (fn_801D52D4()) {
        Vec2 a = mCoreStatus[0].pos;
        if (-1.0f <= a.x) {
            temp_r27 = true;
        }
    }
    if (temp_r27) {
        Vec2 a = mCoreStatus[0].pos;
        if (a.x <= 1.0f) {
            temp_r28 = true;
        }
    }
    if (temp_r28) {
        Vec2 a = mCoreStatus[0].pos;
        if (-1.0f <= a.y) {
            temp_r29 = true;
        }
    }
    if (temp_r29) {
        Vec2 a = mCoreStatus[0].pos;
        if (a.y <= 1.0f) {
            temp_r30 = true;
        }
    }
    return temp_r30;
}


f32 CController::fn_801D547C(void) {
    static Vec2 lbl_80320F90;

    if (_24() && (mKPADReadLength > 0)) {
        lbl_80320F90 = mCoreStatus[0].horizon;
    }
    Vec2 temp = lbl_80320F90;
    return 1.40625f * nw4r::math::Atan2FIdx(temp.y, temp.x);
}

void CController::fn_801D5500(u32 arg1, u8 arg2) {
    for (s32 i = 0; i < (s32)ARRAY_LENGTH(unk1354); i++) {
        if (arg1 & (1 << i)) {
            unk1354[i] = arg2;
        }
    }
}

void CController::fn_801D55D8(u32 arg1, u8 arg2) {
    for (s32 i = 0; i < (s32)ARRAY_LENGTH(unk1354); i++) {
        if (arg1 & (1 << i)) {
            unk1344[i] = arg2;
        }
    }
}

void CController::_40(const char *seqText, bool forcePlay) {
    if (!mMotorSeqPlaying || forcePlay) {
        u32 seqLen = strlen(seqText);

        mMotorSeqPlaying = true;
        mMotorSeqPos = 0;

        for (s32 j = 0; j < seqLen; j++) {
            if (seqText[j] == '*') {
                mMotorSeq[j] = MOTOR_SEQ_ON;
            }
            else if (seqText[j] == '-') {
                mMotorSeq[j] = MOTOR_SEQ_OFF;
            }
        }
        mMotorSeq[seqLen] = MOTOR_SEQ_END;

        _4C();
    }
}

void CController::_44(void) {
    mMotorSeqPlaying = false;
    WPADControlMotor(mMyChannel, FALSE);
}

void CController::_4C(void) {
    if (mMotorSeqPlaying) {
        if (mMotorSeq[mMotorSeqPos] == MOTOR_SEQ_ON) {
            WPADControlMotor(mMyChannel, TRUE);
            mMotorSeqPos++;
        }
        else if (mMotorSeq[mMotorSeqPos] == MOTOR_SEQ_OFF) {
            WPADControlMotor(mMyChannel, FALSE);
            mMotorSeqPos++;
        }
        else if (mMotorSeq[mMotorSeqPos] == MOTOR_SEQ_END) {
            WPADControlMotor(mMyChannel, FALSE);
            mMotorSeqPlaying = false;
        }
    }
}

void CController::fn_801D5830(s32 channel, s32 result) {
    CController *controller = gControllerManager->fn_801D5FF0(channel);
    controller->mInfoErrcode = result;
    controller->mInfoUpdated = false;
}

bool CController::fn_801D5850(void) {
    mInfoUpdated = true;
    mInfoErrcode = WPADGetInfoAsync(mMyChannel, &mInfo, CController::fn_801D5830);
    return mInfoErrcode == WPAD_ERR_OK;
}

bool CController::fn_801D58A0(void) {
    return mInfoUpdated;
}

u32 CController::fn_801D58A8(void) {
    return (mInfoErrcode == WPAD_ERR_OK) ? mInfo.battery : mInfoErrcode; 
}

void CGCController::_0C(void) {
    if (mStatus->err) {
        return;
    }
    u16 unk0Cbutton = mStatusPrev->button;
    unk14 = 0;
    if (mStatus->button & (mStatusPrev->button ^ mStatus->button)) {
        unk12 = 0;
    }
    

    if (unk0Cbutton != mStatus->button) {
        return;
    }

    unk12++;
    if ((unk12 == unk10) || (unk12 == (unk10 + unk11))) {
        unk14 = mStatus->button;
        unk12 = unk10;
    }
}

/* 
        CControllerManager
*/

void *CControllerManager::fn_801D5950(u32 size) {
    return gControllerManager->doAlloc(size);
}

BOOL CControllerManager::fn_801D59B0(void *block) {
    return gControllerManager->doFree(block);
}

CControllerManager::CControllerManager(void) {}

CControllerManager::~CControllerManager(void) {
    _08();
}

void CControllerManager::_10(CController::CreateFn createFn) {
    for (s32 i = 0; i < (s32)ARRAY_LENGTH(mController); i++) {
        mController[i] = createFn(i);
    }
    mNullController = new CNullController(-1);

    u32 heapSize = WPADGetWorkMemorySize() + 0x100;
    mHeapStart = new (eHeap_MEM2, 32) u8[heapSize];
    mHeap = MEMCreateExpHeap(mHeapStart, heapSize);
    MEMInitAllocatorForExpHeap(&mAllocator, mHeap, 32);

    for (s32 i = 0; i < (s32)ARRAY_LENGTH(mGCController); i++) {
        mGCController[i] = new CGCController(i);
        mGCController[i]->setUnk08(&mPadStatus[i]);
        mGCController[i]->setUnk0C(&mPadStatusPrev[i]);
    }
}

void CControllerManager::_08(void) {
    for (s32 i = 0; i < (s32)ARRAY_LENGTH(mController); i++) {
        delete mController[i];
    }
    delete mNullController;

    MEMDestroyExpHeap(mHeap);
    delete[] mHeapStart;
}

void CControllerManager::_14(void) {
    WPADRegisterAllocator(CControllerManager::fn_801D5950, CControllerManager::fn_801D59B0);
    KPADInit();

    for (s32 i = 0; i < (s32)ARRAY_LENGTH(mController); i++) {
        mController[i]->_0C();
    }

    PADInit();

    memset(mPadStatus, 0x00, sizeof(mPadStatus));
    memset(mPadStatusPrev, 0x00, sizeof(mPadStatusPrev));

    u8 pos = WPADGetSensorBarPosition();
    if (pos == 0) {
        KPADSetSensorHeight(0, -.1f);
        return;
    }
    else if (pos == 1) {
        KPADSetSensorHeight(0, .1f);
        return;
    }
}

void CControllerManager::_18(void) {
    for (s32 i = 0; i < (s32)ARRAY_LENGTH(mController); i++) {
        mController[i]->_10();
    }
    for (s32 i = 0; i < (s32)ARRAY_LENGTH(mGCController); i++) {
        mGCController[i]->_0C();
        mPadStatusPrev[i] = mPadStatus[i];
    }

    PADRead(mPadStatus);

    u32 resetMask = 0;
    for (s32 chan = 0; chan < (s32)ARRAY_LENGTH(mPadStatus); chan++) {
        if (mPadStatus[chan].err == PAD_ERR_NO_CONTROLLER) {
            resetMask |= (PAD_CHAN0_BIT >> chan);
        }
    }

    if (resetMask != 0) {
        PADReset(resetMask);
    }
}

void CControllerManager::_1C(void) {
    for (s32 i = 0; i < (s32)ARRAY_LENGTH(mController); i++) {
        mController[i]->_14();
    }
    for (s32 i = 0; i < (s32)ARRAY_LENGTH(mGCController); i++) {
        mGCController[i]->_10();
    }
}

CController *CControllerManager::fn_801D5FF0(s32 channel) {
    return mController[channel];
}

CGCController *CControllerManager::fn_801D6000(s32 channel) {
    return mGCController[channel];
}

CGCController::~CGCController(void) {}

bool CNullController::_3C(void) {
    return false;
}

bool CNullController::_38(void) {
    return false;
}

bool CNullController::_34(void) {
    return false;
}

bool CNullController::_30(void) {
    return false;
}

bool CNullController::_2C(void) {
    return false;
}

bool CNullController::_28(void) {
    return false;
}

bool CNullController::_24(void) {
    return false;
}

u8 CNullController::_18(void) {
    return WPAD_DEV_NOT_FOUND;
}
