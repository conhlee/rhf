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
    eLevelState_Hidden,
    eLevelState_Locked,
    eLevelState_AttemptNG,
    eLevelState_CompleteOK,
    eLevelState_CompleteHI,
    eLevelState_Perfect,
};

#define SAVE_COUNT 4

struct SaveDataFile {
    s8 mProfileIconIndex;
    RFLCreateID mProfileMiiID;
    s8 unk9;
    s8 mSelectedGameIndex;
    u8 mLevelStates[50];
    u8 unk3D;
    s16 mLevelScores[50];
    bool mHasPendingResult;
    s8 mPendingLevelState;
    s16 mPendingLevelScore;
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
    s8 unk12C[16];
    s8 unk13C[16];
    SaveDataFile_Sub00 unk14C[16];
    SaveDataFile_Sub00 unk1CC[16];
    u32 unk24C;
    u8 unk250;
    u8 unk251;
    bool unk252;
    u8 unk253;
    u8 _pad254[0x2c];

    void reset(void);

    s8 getProfileIconIndex(void);
    void setProfileIconIndex(s8 index);
    bool isOccupied(void);
    bool isUsingProfileMii(void);

    void useProfileMii(void);
    RFLCreateID getProfileMiiID(void);
    void setProfileMiiID(RFLCreateID createID);

    s8 getUnk9(void);
    void setUnk9(s8 arg);

    s8 getSelectedGameIndex(void);
    void setSelectedGameIndex(s8 index);

    ELevelState getLevelState(s32 index);
    void setLevelState(u8 index, ELevelState levelState);

    s16 getLevelScore(u8 index);
    void updateLevelScore(u8 index, s16 score);

    u32 calculateFlow(void);
    u32 countMedals(void);
    u32 fn_8007819C(void);
    u8 countPerfects(void);
    u8 countRemainingPerfects(void);

    bool hasPendingResult(void);
    void setPendingResult(ELevelState levelState, s16 score);
    void clearPendingResult(void);
    s8 getPendingLevelState(void);
    s16 getPendingLevelScore(void);

    void fn_80077BC0(s8 arg);
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
    bool fn_80077D74(void);
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

    void initialize(void);
    void resetAllSaveFiles(void);

    u8 getSelectedSaveIndex(void);
    void setSelectedSaveIndex(u8 saveIndex);

    SaveDataFile *getSaveFile(s32 saveIndex);
    SaveDataFile *getSelectedSaveFile(void);

    u8 *getUnkA04(void);
    void updateSnapshot(void);

    void recordPendingResult(ELevelState levelState) {
        getSelectedSaveFile()->setPendingResult(levelState, gInputCheckManager->fn_801E94D4());
    }

private:
    SaveDataFile mSaveFiles[4];
    u8 unkA04[0x20];
    SaveDataFile mSaveFilesSnapshots[4];
    u8 unk1424[0x20];
    u8 mSelectedSaveIndex;
};

extern CSaveData *gSaveData;

#endif
