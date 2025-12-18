#ifndef GAMEUTIL_INPUTCHECKMANAGER_HPP
#define GAMEUTIL_INPUTCHECKMANAGER_HPP

#include <revolution/types.h>
#include <revolution/MEM.h>

#include "Singleton.hpp"

#include "InputChecker.hpp"

#include "TickFlowManager.hpp"
#include "CheckPointManager.hpp"

#include "Mem.hpp"

enum EInputType {
    eInputType_TriggerUp = 0,
    eInputType_TriggerDown,
    eInputType_TriggerLeft,
    eInputType_TriggerRight,
    eInputType_TriggerA,
    eInputType_TriggerB,
    eInputType_Trigger_6,
    eInputType_TriggerHome,
    eInputType_TriggerPlus,
    eInputType_TriggerMinus,
    eInputType_Trigger1,
    eInputType_Trigger2,
    eInputType_TriggerZ,
    eInputType_TriggerC,
    
    eInputType_ReleaseUp,
    eInputType_ReleaseDown,
    eInputType_ReleaseLeft,
    eInputType_ReleaseRight,
    eInputType_ReleaseA,
    eInputType_ReleaseB,
    eInputType_Release_20,
    eInputType_ReleaseHome,
    eInputType_ReleasePlus,
    eInputType_ReleaseMinus,
    eInputType_Release1,
    eInputType_Release2,
    eInputType_ReleaseZ,
    eInputType_ReleaseC,

    eInputType_Num,
};

class CInputCheckManager : public TSingleton<CInputCheckManager> {
public:
    typedef void (*FuncUnk414)(void);
    typedef void (*FuncUnk418)(u32);

    virtual void _08(void);
    virtual ~CInputCheckManager(void);
    virtual void _10(u32);
    virtual void _14(void);

    CInputCheckManager(void);
    void fn_801E8A08(void);
    void fn_801E8BD0(void);
    void *fn_801E9144(u32);
    void fn_801E9150(void *);
    void fn_801E9158(CInputChecker *, bool);
    void fn_801E9204(void);
    void fn_801E923C(u32);
    bool fn_801E93E0(u32, f32, f32);
    void fn_801E9474(void);
    void fn_801E9488(u8);
    u32 fn_801E94D4(void);
    void fn_801E94E8(u32);
    void fn_801E9604(bool);
    void fn_801E960C(void);
    void fn_801E9C30(bool);
    void fn_801E9C38(f32);
    void fn_801E9C40(void);
    void fn_801E9D58(u32, u32);
    s32 fn_801E9D68(u32);
    void fn_801E9D7C(CInputChecker *);
    void fn_801E9D80(char *);
    void fn_801E9D84(void);
    void fn_801E9D88(void);
    void fn_801E9D8C(void);

    static void fn_801E8560(void);

    CInputChecker *getUnk0C(void) {
        return unk0C;
    }

    u8 getUnk42E(void) {
        return unk42E;
    }

    void setUnk414(FuncUnk414 func) {
        unk414 = func;
    }
    void setUnk418(FuncUnk418 func) {
        unk418 = func;
    }

    void setUnk41C(u8 value) {
        unk41C = value;
    }

    void setUnk429(bool value) {
        unk429 = value;
    }
    void setUnk484(u8 value) {
        unk484 = value;
    }
    void setUnk42A(u8 value) {
        unk42A[0] = value;
    }
    void setUnk42B(bool value) {
        unk42A[1] = value;
    }
    void setUnk42C(u8 value) {
        unk42A[2] = value;
    }
    void setUnk42D(u8 value) {
        unk42A[3] = value;
    }
    u8 getUnk498(void) {
        return unk498;
    }
    u8 getUnk499(void) {
        return unk499;
    }
    u8 getUnk49A(void) {
        return unk49A;
    }
    u8 getUnk49B(void) {
        return unk49B;
    }

    u32 getUnk4A4(u32 i) {
        return unk4A4[i];
    }

    template <typename T>
    T *makeNew(void) {
        void *alloc = fn_801E9144(sizeof(T));
        return new (alloc) T;
    }

private:
    u8 *unk04;
    MEMiHeapHead *unk08;
    CInputChecker *unk0C;
    struct UnkSub {
        u8 unk0;
        u32 unk4;
        f32 unk8;
        f32 unkC;
    } unk10[0x40];
    u32 unk410;
    FuncUnk414 unk414;
    FuncUnk418 unk418;
    u8 unk41C;
    u8 pad41D[0x420 - 0x41d];
    u32 unk420;
    u32 unk424;
    u8 unk428;
    u8 unk429;
    u8 unk42A[4];
    u8 unk42E;
    u8 unk42F;
    s32 unk430[16];
    f32 unk470;
    f32 unk474[4];
    u8 unk484;
    u8 pad485[7];
    u8 unk48C;
    u8 unk48D;
    u8 pad48E[0xa];
    u8 unk498;
    u8 unk499;
    u8 unk49A;
    u8 unk49B;
    u8 unk49C;
    f32 unk4A0;
    u32 unk4A4[256];
    s32 (*unk8A4)(u32);
    u32 unk8A8;
    u32 unk8AC;

    static void fn_801E8118(void);
    static s32 fn_801E85AC(u32);

    UnkSub *getUnk10(u32 i) {
        return &unk10[i];
    }

    void updateUnk0C(void) {
        // not matching: regswap cur/next
        for (CInputChecker *cur = unk0C, *next; cur != NULL; cur = next) {
            next = cur->getNext();
            if (!cur->_18()) {
                continue;
            }
            if (cur == unk0C) {
                unk0C = cur->getNext();
            }
            cur->removeCurrent();
        }
        for (s32 i = 0; i < (s32)ARRAY_LENGTH(unk474); i++) {
            if (unk474[i] > 0.0) {
                unk474[i] -= gTickFlowManager->fn_801E2698();
                if (unk474[i] <= 0.0) {
                    unk474[i] = 0.0f;
                }
            }
        }
    }

    void updateUnk10(void) {
        for (int i = 0; i < (s32)ARRAY_LENGTH(unk10); i++) {
            if (!unk10[i].unk0) {
                continue;
            }

            unk10[i].unk8 += gTickFlowManager->fn_801E2698();
            if (!(unk10[i].unk8 > unk10[i].unkC)) {
                continue;
            }
            unk10[i].unk0 = 0;
            if (unk418) {
                unk418(unk4A4[unk10[i].unk4]);
            }
            if (!gCheckPointManager->getUnkDD()) {
                continue;
            }
            if (unk428 >= 6) {
                continue;
            }
            unk428++;
        }
    }

    u32 checkUnk430(u32 arg0) {
        for (int i = 0; i < (s32)ARRAY_LENGTH(unk430); i++) {
            if (unk430[i] == arg0) {
                return 1;
            }
        }
        return 0;
    }

    u32 unkCheck(u32 arg0) {
        if (!unk42F) {
            return 0;
        }
        return checkUnk430(arg0);
    }

    void doUnk430(u32 arg0) {
        for (int i = 0; i < (s32)ARRAY_LENGTH(unk430); i++) {
            if (unk430[i] == arg0) {
                return;
            }
        }

        for (int i = 0; i < (s32)ARRAY_LENGTH(unk430); i++) {
            if (unk430[i] == -1) {
                unk430[i] = arg0;
                return;
            }
        }

        
    }
};

extern CInputCheckManager *gInputCheckManager;

#endif