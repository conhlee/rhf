#include "InputCheckManager.hpp"
#include "Controller.hpp"

static u8 lbl_803D5D78[16];

// regswaps
void CInputCheckManager::fn_801E8118(void) {
    static const u32 lbl_802E4FE0[] = {
        0x8, 0x4, 0x1, 0x2,
        0x8000, 0x10, 0x1000, 0x200,
        0x100, 0x2000, 0x4000,
    };
    static const u32 lbl_802E500C[] = {
        0, 1, 2, 3, 7, 8, 9, 0xa,
        0xb, 0xc, 0xd,
    };
    static const u32 lbl_802E5038[] = {
        0xe, 0xf, 0x10, 0x11, 0x15,
        0x16, 0x17, 0x18, 0x19, 0x1a,
        0x1b,
    };
    
    for (int i = 0; i < 4; i++) {
        u32 tempUnk4A4;
        int i1C = i * 0x1C;
        CController *controller = gControllerManager->fn_801D5FF0(i);
        
        for (int j = 0; j < 11; j++) {
            if (controller->getUnk133C() & lbl_802E4FE0[j]) {
                gInputCheckManager->fn_801E923C(lbl_802E500C[j] + i1C);
            } else if (controller->getUnk1340() & lbl_802E4FE0[j]) {
                gInputCheckManager->fn_801E923C(lbl_802E5038[j] + i1C);
            }
        }

        if (controller->unk136CCheck() && gInputCheckManager->getUnk42E()) {
            gInputCheckManager->fn_801E923C(i1C + 6);
            lbl_803D5D78[i + 0] = 0;
            lbl_803D5D78[i + 8] = 0;
        } else {
            tempUnk4A4 = gInputCheckManager->getUnk4A4(6 + i * 0x1c);
            if (lbl_803D5D78[i + 0] != 0) {
                lbl_803D5D78[i + 0]--;
                if (lbl_803D5D78[i + 0] == 0) {
                    gInputCheckManager->fn_801E923C(i1C + 4);
                }
            } else {
                if ((controller->getUnk133C() & 0x800) != 0) {
                    if (gInputCheckManager->getUnk498() || 
                        gInputCheckManager->fn_801E93E0(tempUnk4A4, -12.0f, 12.0f)) {
                        lbl_803D5D78[i + 0] = 3;
                    } else {
                        gInputCheckManager->fn_801E923C(i1C + 4);
                    }
                }
            }

            if (lbl_803D5D78[i + 8] != 0) {
                lbl_803D5D78[i + 8]--;
                if (lbl_803D5D78[i + 8] == 0) {
                    gInputCheckManager->fn_801E923C(i1C + 5);
                }
            } else {
                if ((controller->getUnk133C() & 0x400) != 0) {
                    if (gInputCheckManager->getUnk499() || 
                        gInputCheckManager->fn_801E93E0(tempUnk4A4, -12.0f, 12.0f)) {
                        lbl_803D5D78[i + 8] = 3;
                    } else {
                        gInputCheckManager->fn_801E923C(i1C + 5);
                    }
                }
            }
        }

        if (controller->unk136CCheck1() && gInputCheckManager->getUnk42E()) {
            gInputCheckManager->fn_801E923C(i1C + 0x14);
            lbl_803D5D78[i + 4] = 0;
            lbl_803D5D78[i + 12] = 0;
        } else {
            tempUnk4A4 = gInputCheckManager->getUnk4A4(6 + i * 0x1c + 0xe);
            if (lbl_803D5D78[i + 4] != 0) {
                lbl_803D5D78[i + 4]--;
                if (lbl_803D5D78[i + 4] == 0) {
                    gInputCheckManager->fn_801E923C(i1C + 0x12);
                }
            } else {
                if ((controller->getUnk1340() & 0x800) != 0) {
                    if (gInputCheckManager->getUnk49A() || 
                        gInputCheckManager->fn_801E93E0(tempUnk4A4, -12.0f, 12.0f)) {
                        lbl_803D5D78[i + 4] = 3;
                    } else {
                        gInputCheckManager->fn_801E923C(i1C + 0x12);
                    }
                }
            }

            if (lbl_803D5D78[i + 12] != 0) {
                lbl_803D5D78[i + 12]--;
                if (lbl_803D5D78[i + 12] == 0) {
                    gInputCheckManager->fn_801E923C(i1C + 0x13);
                }
            } else {
                if ((controller->getUnk1340() & 0x400) != 0) {
                    if (gInputCheckManager->getUnk49B() || 
                        gInputCheckManager->fn_801E93E0(tempUnk4A4, -12.0f, 12.0f)) {
                        lbl_803D5D78[i + 12] = 3;
                    } else {
                        gInputCheckManager->fn_801E923C(i1C + 0x13);
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
    if (0x1b >= arg0) {
        return 0;
    }
    if ((0x1c <= arg0) && (0x37 >= arg0)) {
        return 1;
    }
    if ((0x38 <= arg0) && (0x53 >= arg0)) {
        return 2;
    }
    if ((0x54 <= arg0) && (0x6f >= arg0)) {
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
    unk04 = new (eHeap_MEM2, 32) u8[size];
    unk08 = MEMCreateExpHeap(unk04, size);
}

void CInputCheckManager::_08(void) {
    unk0C->removeAll();
    unk0C = 0;
    MEMDestroyExpHeap(unk08);
    delete[] unk04;
}

void CInputCheckManager::_14(void) {
    unk0C = 0;
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
    resetUnk10();
    // not matching in resetUnk0C
    resetUnk0C();
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
        for (cur = unk0C; cur != NULL; cur = next) {
            next = cur->getNext();
            if (!cur->getUnk71()) {
                if (!cur->fn_801E7E28(temp_r20)) {
                    continue;
                }
                s32 temp_r27 = unkCheck(temp_r20) && (unk474[unk8A4(temp_r20)] > 0.0);
                u8 sp8 = 0;
                s32 temp_r27_1 = cur->_20(&sp8, temp_r27, sub->unk8);
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
                if (cur == unk0C) {
                    unk0C = cur->getNext();
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
                if (cur == unk0C) {
                    unk0C = cur->getNext();
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
    return MEMAllocFromExpHeap(unk08, size);
}

void CInputCheckManager::fn_801E9150(void *ptr) {
    MEMFreeToExpHeap(unk08, ptr);
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
    checker->insertBefore(unk0C);
    unk0C = checker;
}

void CInputCheckManager::fn_801E9204(void) {
    unk0C->removeAll();
    unk0C = 0;
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
    for (CInputChecker *check = unk0C; check != 0; check = check->getNext()) {
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

