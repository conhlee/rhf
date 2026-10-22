#ifndef GAME_SAVEDATA_HPP
#define GAME_SAVEDATA_HPP

#include <revolution/types.h>

#include "Singleton.hpp"

#include "InputCheckManager.hpp"

#include "RVLFaceLibrary.h"

struct SaveDataFile_Sub00 {
    u32 unk00;
    u32 unk04;
};

enum ELevelState {
    eLevelState_Locked, // TODO: confirm
    eLevelState_Unlocked, // TODO: confirm
    eLevelState_AttemptNG,
    eLevelState_CompleteOK,
    eLevelState_CompleteHI,
    eLevelState_Perfect,
};

#define SAVE_COUNT 4

struct SaveDataFile {
    u8 pad00[0x280];

    void fn_80077908(void);
    void fn_80077A74(s8);
    void fn_80077A7C(void);
    void fn_80077A88(RFLCreateID);

    void fn_80077BC8(s32, s16);
    
    void fn_80077C0C(s8);

    void fn_80077C5C(s32, s32);

    void fn_80777C6C(s32, s8);
    void fn_80077C78(s32, s8);

    void fn_80077C84(s32);
    void fn_80077C94(s32);

    void fn_80077CA4(s32, SaveDataFile_Sub00 *);
    void fn_80077CC0(s32, SaveDataFile_Sub00 *);

    void fn_80077CEC(bool);

    s8 fn_80077CFC(void);

    bool fn_80077D04(void);
    bool fn_80077D20(void);
    RFLCreateID fn_80077D38(void);

    s8 fn_80077D50(void);

    ELevelState fn_80077D58(s32);

    s8 fn_80077DF8(void);
    u8 fn_80077E08(void);

    bool fn_80077EC8(void);
    bool fn_80077EDC(void);
    bool fn_80077EF0(void);
    bool fn_80077F04(void);

    u32 fn_80077F78(void);
    u32 fn_800780E0(void);

    u32 fn_8007819C(void);

    void fn_800786E4(void);

    void fn_80078808(void);

    s32 fn_80077E40(s32);

    u8 fn_80078238(void);

    s8 fn_800782F4(void);
};

class CSaveData : public TSingleton<CSaveData> {
public:
    virtual void _08(void);
    virtual ~CSaveData(void);

    CSaveData(void);

    void fn_80078E04(void);
    void fn_80078E8C(void);

    void fn_80078F2C(u8 arg);
    u8 fn_80078F34(void);
    SaveDataFile *fn_80078F3C(s32 saveIndex);
    SaveDataFile *fn_80078F4C(void);
    u8 *fn_80078F60(void);
    void fn_80078F68(void);

    void do80077BC8(s32 param_1) {
        fn_80078F4C()->fn_80077BC8(param_1, gInputCheckManager->fn_801E94D4());
    }

private:
    SaveDataFile unk4[4]; // current save files, duh
    u8 unkA04[0x20];
    SaveDataFile unkA24[4]; // might be a backup/unstaged changes?
    u8 unk1424[0x20];
    u8 unk1444; // might be selected save index?
};

extern CSaveData *gSaveData;

#endif
