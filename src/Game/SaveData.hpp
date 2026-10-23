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
    s8 unk0;
    RFLCreateID unk1;
    s8 unk9;
    s8 unkA;
    u8 mLevelStates[50];
    u8 unk3D;
    s16 unk3E[50];
    u8 unkA2;
    s8 unkA3;
    s16 unkA4;
    u8 unkA6;
    s8 unkA7;
    s8 unkA8;
    u8 unkA9;
    u8 unkAA;
    u8 unkAB[50];
    u8 _padDD[3];
    u8 unkE0[8]; // TODO: These might be `ELevelState`s?
    s8 unkE8;
    u8 unkE9;
    s8 unkEA;
    s8 _padEB;
    s32 unkEC[16];
    s8 unk12C[1]; // TODO: size
    u8 _pad12D[0xf];
    s8 unk13C[1]; // TODO: size
    u8 _pad13D[0xf];
    SaveDataFile_Sub00 unk14C[1]; // TODO: size
    u8 _pad14D[0x78];
    SaveDataFile_Sub00 unk1CC[1]; // TODO: size
    u8 _pad1CD[0x78];
    u32 unk24C;
    u8 unk250;
    u8 unk251;
    bool unk252;
    u8 unk253;
    u8 _pad254[0x2c];

    void fn_80077908(void);
    void fn_80077A74(s8 arg);
    void fn_80077A7C(void);
    void fn_80077A88(RFLCreateID createID);
    void fn_80077A9C(s8 arg);
    void fn_80077AA4(s8 arg);
    void fn_80077AAC(u8 arg1, ELevelState levelState);
    void fn_80077BC0(s8 arg);
    void fn_80077BC8(s32 arg1, s16 arg2);
    void fn_80077BDC(void);
    void fn_80077BE8(s8 arg);
    void fn_80077C00(void);
    void fn_80077C0C(s8 arg);
    void fn_80077C14(void);
    void fn_80077C20(void);
    void fn_80077C2C(s32 offset, u8 value);
    void fn_80077C38(s8 arg);
    void fn_80077C40(s8 arg);
    void fn_80077C50(void);
    void fn_80077C5C(s32 offset, s32 value);
    void fn_80777C6C(s32 offset, s8 value);
    void fn_80077C78(s32 offset, s8 value);
    void fn_80077C84(s32 offset);
    void fn_80077C94(s32 offset);
    void fn_80077CA4(s32 offset, SaveDataFile_Sub00 *sub);
    void fn_80077CC0(s32 offset, SaveDataFile_Sub00 *sub);
    void fn_80077CDC(s8 arg);
    void fn_80077CE4(s8 arg);
    void fn_80077CEC(bool arg);
    void fn_80077CF4(s8 arg);
    s8 fn_80077CFC(void);
    bool fn_80077D04(void);
    bool fn_80077D20(void);
    RFLCreateID fn_80077D38(void);
    s8 fn_80077D48(void);
    s8 fn_80077D50(void);
    ELevelState fn_80077D58(s32 arg);
    s16 fn_80077D64(u8 arg);
    bool fn_80077D74(void);
    bool fn_80077D88(void);
    s8 fn_80077D9C(void);
    s16 fn_80077DA4(void);
    bool fn_80077DAC(void);
    bool fn_80077DC0(void);
    bool fn_80077DDC(void);
    s8 fn_80077DF8(void);
    s8 fn_80077E00(void);
    u8 fn_80077E08(void);
    u8 fn_80077E10(s32 arg);
    s8 fn_80077E1C(void);
    bool fn_80077E24(void);
    s8 fn_80077E38(void);
    s32 fn_80077E40(s32 arg);
    s8 fn_80077E50(s32 arg);
    s8 fn_80077E5C(s32 arg);
    bool fn_80077E68(s32 arg);
    bool fn_80077E84(s32 arg);
    SaveDataFile_Sub00 fn_80077EA0(s32 arg);
    SaveDataFile_Sub00 fn_80077EB4(s32 arg);
    bool fn_80077EC8(void);
    bool fn_80077EDC(void);
    bool fn_80077EF0(void);
    bool fn_80077F04(void);
    void fn_80077F18(u8 arg, s16 value);
    u32 fn_80077F78(void);
    u32 fn_800780E0(void);
    u32 fn_8007819C(void);
    u8 fn_80078238(void);
    u8 fn_800782F4(void);
    void fn_800783B4(void);
    s8 fn_80078548(void);
    void fn_800786E4(void);
    void fn_800786FC(void);
    void fn_80078808(void);
    bool fn_80078848(void);
    bool fn_80078898(void);
    bool fn_800788DC(s32 giftIndex);
    bool fn_80078A40(s32 giftIndex);
    u8 fn_80078BA4(s32 arg);
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
