#include "InputCheckManager.hpp"
#include "Controller.hpp"

static u8 lbl_803D5D78[16];

// regswaps
void CInputCheckManager::fn_801E8118(void) {
    static const u32 lbl_802E4FE0[] = {
        WPAD_BUTTON_UP, WPAD_BUTTON_DOWN, WPAD_BUTTON_LEFT, WPAD_BUTTON_RIGHT,
        WPAD_BUTTON_HOME, 
        WPAD_BUTTON_PLUS, WPAD_BUTTON_MINUS, 
        WPAD_BUTTON_1, WPAD_BUTTON_2, 
        WPAD_BUTTON_FS_Z, WPAD_BUTTON_FS_C,
    };
    static const u32 lbl_802E500C[] = {
        eInputType_TriggerUp, eInputType_TriggerDown, eInputType_TriggerLeft, eInputType_TriggerRight,
        eInputType_TriggerHome,
        eInputType_TriggerPlus, eInputType_TriggerMinus,
        eInputType_Trigger1, eInputType_Trigger2,
        eInputType_TriggerZ, eInputType_TriggerC,
    };
    static const u32 lbl_802E5038[] = {
        eInputType_ReleaseUp, eInputType_ReleaseDown, eInputType_ReleaseLeft, eInputType_ReleaseRight,
        eInputType_ReleaseHome,
        eInputType_ReleasePlus, eInputType_ReleaseMinus,
        eInputType_Release1, eInputType_Release2,
        eInputType_ReleaseZ, eInputType_ReleaseC,
    };
    
    for (int i = 0; i < 4; i++) {
        u32 tempUnk4A4;
        int i1C = i * eInputType_Num;
        CController *controller = gControllerManager->fn_801D5FF0(i);
        
        for (int j = 0; j < 11; j++) {
            if (controller->getUnk133C() & lbl_802E4FE0[j]) {
                gInputCheckManager->fn_801E923C(lbl_802E500C[j] + i1C);
            } else if (controller->getUnk1340() & lbl_802E4FE0[j]) {
                gInputCheckManager->fn_801E923C(lbl_802E5038[j] + i1C);
            }
        }

        if (controller->unk136CCheck() && gInputCheckManager->getUnk42E()) {
            gInputCheckManager->fn_801E923C(i1C + eInputType_Trigger_6);
            lbl_803D5D78[i + 0] = 0;
            lbl_803D5D78[i + 8] = 0;
        } else {
            tempUnk4A4 = gInputCheckManager->getUnk4A4(eInputType_Trigger_6 + i * eInputType_Num);
            if (lbl_803D5D78[i + 0] != 0) {
                lbl_803D5D78[i + 0]--;
                if (lbl_803D5D78[i + 0] == 0) {
                    gInputCheckManager->fn_801E923C(i1C + eInputType_TriggerA);
                }
            } else {
                if ((controller->getUnk133C() & WPAD_BUTTON_A) != 0) {
                    if (gInputCheckManager->getUnk498() || 
                        gInputCheckManager->fn_801E93E0(tempUnk4A4, -12.0f, 12.0f)) {
                        lbl_803D5D78[i + 0] = 3;
                    } else {
                        gInputCheckManager->fn_801E923C(i1C + eInputType_TriggerA);
                    }
                }
            }

            if (lbl_803D5D78[i + 8] != 0) {
                lbl_803D5D78[i + 8]--;
                if (lbl_803D5D78[i + 8] == 0) {
                    gInputCheckManager->fn_801E923C(i1C + eInputType_TriggerB);
                }
            } else {
                if ((controller->getUnk133C() & WPAD_BUTTON_B) != 0) {
                    if (gInputCheckManager->getUnk499() || 
                        gInputCheckManager->fn_801E93E0(tempUnk4A4, -12.0f, 12.0f)) {
                        lbl_803D5D78[i + 8] = 3;
                    } else {
                        gInputCheckManager->fn_801E923C(i1C + eInputType_TriggerB);
                    }
                }
            }
        }

        if (controller->unk136CCheck1() && gInputCheckManager->getUnk42E()) {
            gInputCheckManager->fn_801E923C(i1C + eInputType_Release_20);
            lbl_803D5D78[i + 4] = 0;
            lbl_803D5D78[i + 12] = 0;
        } else {
            tempUnk4A4 = gInputCheckManager->getUnk4A4(eInputType_Release_20 + i * eInputType_Num);
            if (lbl_803D5D78[i + 4] != 0) {
                lbl_803D5D78[i + 4]--;
                if (lbl_803D5D78[i + 4] == 0) {
                    gInputCheckManager->fn_801E923C(i1C + eInputType_ReleaseA);
                }
            } else {
                if ((controller->getUnk1340() & WPAD_BUTTON_A) != 0) {
                    if (gInputCheckManager->getUnk49A() || 
                        gInputCheckManager->fn_801E93E0(tempUnk4A4, -12.0f, 12.0f)) {
                        lbl_803D5D78[i + 4] = 3;
                    } else {
                        gInputCheckManager->fn_801E923C(i1C + eInputType_ReleaseA);
                    }
                }
            }

            if (lbl_803D5D78[i + 12] != 0) {
                lbl_803D5D78[i + 12]--;
                if (lbl_803D5D78[i + 12] == 0) {
                    gInputCheckManager->fn_801E923C(i1C + eInputType_ReleaseB);
                }
            } else {
                if ((controller->getUnk1340() & WPAD_BUTTON_B) != 0) {
                    if (gInputCheckManager->getUnk49B() || 
                        gInputCheckManager->fn_801E93E0(tempUnk4A4, -12.0f, 12.0f)) {
                        lbl_803D5D78[i + 12] = 3;
                    } else {
                        gInputCheckManager->fn_801E923C(i1C + eInputType_ReleaseB);
                    }
                }
            }
        }
    }
}

void CInputCheckManager::fn_801E8560(void) {
    for (int i = 0; i < (s32)ARRAY_LENGTH(lbl_803D5D78); i++) {
        lbl_803D5D78[i] = 0;
    }
}

s32 CInputCheckManager::fn_801E85AC(u32 arg0) {
    if (((eInputType_Num * 0) <= arg0) && (((eInputType_Num * 1) - 1) >= arg0)) {
        return 0;
    }
    if (((eInputType_Num * 1) <= arg0) && (((eInputType_Num * 2) - 1) >= arg0)) {
        return 1;
    }
    if (((eInputType_Num * 2) <= arg0) && (((eInputType_Num * 3) - 1) >= arg0)) {
        return 2;
    }
    if (((eInputType_Num * 3) <= arg0) && (((eInputType_Num * 4) - 1) >= arg0)) {
        return 3;
    }
    return -1;
}

CInputCheckManager::CInputCheckManager(void) {

}

CInputCheckManager::~CInputCheckManager(void) {
    _08();
}

void CInputCheckManager::_10(u32 size) {
    size = ROUND_UP(size, 32);

    mHeapStart = new (eHeap_MEM2, 32) u8[size];
    mHeap = MEMCreateExpHeap(mHeapStart, size);
}

void CInputCheckManager::_08(void) {
    mCheckerHead->removeAll();
    mCheckerHead = NULL;

    MEMDestroyExpHeap(mHeap);
    delete[] mHeapStart;
}

void CInputCheckManager::_14(void) {
    mCheckerHead = NULL;
    unk418 = 0;
    unk414 = fn_801E8118;
    unk410 = 0;
    for (int i = 0; i < (s32)ARRAY_LENGTH(unk10); i++) {
        unk10[i].unk0 = 0;
    }
    unk41C = 0;
    unk429 = 1;
    unk42E = 0;
    unk484 = 0;
    for (int i = 0; i < (s32)ARRAY_LENGTH(unk42A); i++) {
        unk42A[i] = 1;
    }
    unk42F = 1;
    fn_801E960C();
    unk49C = 0;
    unk4A0 = 1.0f;
    fn_801E9C40();
    unk8A4 = fn_801E85AC;
    unk48C = 0;
    unk48D = 0;
}

void CInputCheckManager::fn_801E8A08(void) {
    if (unk41C) {
        return;
    }

    if (!unk48D && !unk484) {
        unk414();
    }
    fn_801E8BD0();
    updateUnk10();
    // not matching in updateUnk0C
    updateUnk0C();
}


// regswaps, missing code (may be nonequivalent)
void CInputCheckManager::fn_801E8BD0(void) {
    u32 temp_r23;
    CInputChecker *cur;
    CInputChecker *next;
    int i;
    u32 temp_r20;

    for (i = 0; i < (s32)ARRAY_LENGTH(unk10); i++) {
        CInputCheckManager::UnkSub *sub = getUnk10(i);
        if (!sub->unk0) {
            continue;
        }
        temp_r23 = 0;
        temp_r20 = unk4A4[sub->unk4];
        for (cur = mCheckerHead; cur != NULL; cur = next) {
            next = cur->getNext();
            if (!cur->getUnk71()) {
                if (!cur->fn_801E7E28(temp_r20)) {
                    continue;
                }
                s32 temp_r27 = unkCheck(temp_r20) && (unk474[unk8A4(temp_r20)] > 0.0);
                u8 sp8 = 0;
                s32 temp_r27_1 = cur->_20(sp8, temp_r27, sub->unk8);
                switch (temp_r27_1) {
                    case 0:
                        temp_r23 = 1;
                        if (!cur->_24(temp_r20)) {
                            break;
                        }
                        fn_801E9488(sp8);
                        gCheckPointManager->fn_801EAF2C(cur->getUnk78(), 0);
                        break;
                    case 1:
                        temp_r23 = 1;
                        if (!cur->_28(temp_r20)) {
                            break;
                        }
                        fn_801E9488(sp8);
                        gCheckPointManager->fn_801EAF2C(cur->getUnk78(), 1);
                        break;
                    default:
                        break;
                }
                if ((temp_r27_1 == 2) || !cur->getUnk70() || cur->getUnk50()) {
                    continue;
                }
                if (cur == mCheckerHead) {
                    mCheckerHead = cur->getNext();
                }
                cur->removeCurrent();
            } else {
                if (cur->getUnk72() || !cur->getUnk50() || (cur->getUnk74() > 1)) {
                    continue;
                }
                if (!cur->fn_801E7E70(temp_r20)) {
                    continue;
                }
                temp_r23 = 1;
                if (cur->_30(temp_r20)) {
                    switch ((s32)cur->getUnk74()) {
                        case 0:
                            gCheckPointManager->fn_801EB00C(cur->getUnk78(), 0);
                            break;
                        case 1:
                            gCheckPointManager->fn_801EB00C(cur->getUnk78(), 1);
                            break;
                        default:
                            break;
                    }
                }
                if (!cur->getUnk70()) {
                    continue;
                }
                if (cur == mCheckerHead) {
                    mCheckerHead = cur->getNext();
                }
                cur->removeCurrent();
            }
        }

        if (temp_r23) {
            sub->unk0 = 0;
            continue;
        }
        if (0 != getUnk10(i)->unkC) {
            continue;
        }
        sub->unk0 = 0;
        if (unk418) {
            unk418(temp_r20);
        }
        fn_801E94E8(temp_r20);
        if (!gCheckPointManager->getUnkDD()) {
            continue;
        }
        if (unk428 >= 6) {
            continue;
        }
        unk428++;
    }
}

void *CInputCheckManager::fn_801E9144(u32 size) {
    return MEMAllocFromExpHeap(mHeap, size);
}

void CInputCheckManager::fn_801E9150(void *ptr) {
    MEMFreeToExpHeap(mHeap, ptr);
}

void CInputCheckManager::fn_801E9158(CInputChecker *checker, bool arg1) {
    if (unk49C) {
        f32 tickJust, rangeMissF, rangeJustF, rangeJustB, rangeMissB;
        checker->fn_801E7DD4(&tickJust, &rangeMissF, &rangeJustF, &rangeJustB, &rangeMissB);
        
        f32 leniency = unk4A0;
        rangeJustF *= leniency;
        rangeMissF *= leniency;
        rangeMissB *= leniency;
        rangeJustB *= leniency;
        checker->fn_801E7DBC(tickJust, rangeMissF, rangeJustF, rangeJustB, rangeMissB);
    }
    checker->insertBefore(mCheckerHead);
    mCheckerHead = checker;
}

void CInputCheckManager::fn_801E9204(void) {
    mCheckerHead->removeAll();
    mCheckerHead = NULL;
}

void CInputCheckManager::fn_801E923C(u32 arg0) {
    if ((!unk429 || !unk42A[unk8A4(arg0)]) && !unk484) {
        return;
    }

    f32 unkFloat = -1.0f;
    CInputCheckManager::UnkSub *sub = 0;
    
    for (int i = 0; i < ARRAY_LENGTH(unk10); i++) {
        if (unk10[i].unk0 == 0) {
            sub = &unk10[i];
            break;
        }
        if (unk10[i].unk8 > unkFloat) {
            unkFloat = unk10[i].unk8;
            sub = &unk10[i];
        }
    }
    sub->unk0 = 1;
    sub->unk4 = arg0;
    sub->unk8 = 0.0f;
    sub->unkC = unk410;
}

bool CInputCheckManager::fn_801E93E0(u32 arg0, f32 arg1, f32 arg2) {
    for (CInputChecker *check = mCheckerHead; check != 0; check = check->getNext()) {
        if (check->fn_801E7E5C(arg0) && 
            check->fn_801E8018(arg1, arg2)) {
                return true;
            }
    }
    return false;
}

void CInputCheckManager::fn_801E9474(void) {
    unk420 = 0;
    unk424 = 0;
    unk428 = 0;
}

void CInputCheckManager::fn_801E9488(u8 arg0) {
    if (gCheckPointManager->getUnkDD()) {
        if (!arg0) {
            arg0 = unk428 * 5;
        }
        unk428 = 0;
        unk420 += arg0;
        unk424++;
    }
}

u32 CInputCheckManager::fn_801E94D4(void) {
    return (unk420 * 100) / unk424;
}

void CInputCheckManager::fn_801E94E8(u32 arg0) {
    if (!unk42F) {
        return;
    }
    if (!checkUnk430(arg0)) {
        return;
    }
    s32 idx = unk8A4(arg0);
    unk474[idx] = unk470;
}

void CInputCheckManager::fn_801E9604(bool arg0) {
    unk42F = arg0;
}

void CInputCheckManager::fn_801E960C(void) {
    unk470 = 48.0f;
    for (int i = 0; i < (s32)ARRAY_LENGTH(unk430); i++) {
        unk430[i] = -1;
    }

    doUnk430(0x4);
    doUnk430(0x20);
    doUnk430(0x3c);
    doUnk430(0x58);
    doUnk430(0x6);
    doUnk430(0x22);
    doUnk430(0x3e);
    doUnk430(0x5a);

    for (int i = 0; i < (s32)ARRAY_LENGTH(unk474); i++) {
        unk474[i] = 0.0f;
    }
}

void CInputCheckManager::fn_801E9C30(bool arg0) {
    unk49C = arg0;
}

void CInputCheckManager::fn_801E9C38(f32 arg0) {
    unk4A0 = arg0;
}

void CInputCheckManager::fn_801E9C40(void) {
    for (int i = 0; i < (s32)ARRAY_LENGTH(unk4A4); i++) {
        unk4A4[i] = i;
    }
}

void CInputCheckManager::fn_801E9D58(u32 arg0, u32 arg1) {
    unk4A4[arg0] = arg1;
}

s32 CInputCheckManager::fn_801E9D68(u32 arg0) {
    return unk8A4(arg0);
}

void CInputCheckManager::fn_801E9D7C(CInputChecker *checker) {

}

void CInputCheckManager::fn_801E9D80(char *arg0) {

}

void CInputCheckManager::fn_801E9D84(void) {

}

void CInputCheckManager::fn_801E9D88(void) {

}

void CInputCheckManager::fn_801E9D8C(void) {

}

