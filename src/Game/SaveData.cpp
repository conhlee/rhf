#include "SaveData.hpp"

#include "GameTable.hpp"
#include "Random.hpp"

void SaveDataFile::fn_80077908(void) {
    memset(this, 0, sizeof(SaveDataFile));

    unk0 = -1;

    unk9 = 0;
    unkA = -1;

    for (u8 i = 0; i < ARRAY_LENGTH(mLevelStates); i++) {
        fn_80077AAC(i, eLevelState_Locked);
        unk3E[i] = -1;
    }

    fn_80077AAC(0, eLevelState_AttemptNG);
    fn_80077AAC(1, eLevelState_Unlocked);
    fn_80077AAC(2, eLevelState_Unlocked);
    fn_80077AAC(3, eLevelState_Unlocked);
    fn_80077AAC(4, eLevelState_Unlocked);

    unk3D = 0;
    unkA2 = 0;
    unkA6 = 0;
    fn_800783B4();
    for (u8 i = 0; i < ARRAY_LENGTH(unkE0); i++)
        unkE0[i] = 0;
    unkE8 = 0;
    unkE9 = 0;
    for (u8 i = 0; i < ARRAY_LENGTH(unkEC); i++)
        unkEC[i] = 0;
    unk24C = 0;
    unk250 = 0;
    unk251 = 1;
    unk252 = false;
    unk253 = 1;
}

void SaveDataFile::fn_80077A74(s8 arg) {
    unk0 = arg;
}

void SaveDataFile::fn_80077A7C(void) {
    unk0 = 99;
}

void SaveDataFile::fn_80077A88(RFLCreateID createID) {
    unk1 = createID;
}

void SaveDataFile::fn_80077A9C(s8 arg) {
    unk9 = arg;
}

void SaveDataFile::fn_80077AA4(s8 arg) {
    unkA = arg;
}

void SaveDataFile::fn_80077AAC(u8 arg1, ELevelState levelState) {
    mLevelStates[arg1] = levelState;

    s32 game = fn_80079540(arg1);
    switch (game) {
        case 4:
        case 5:
        case 12:
        case 14:
        case 17:
        case 20:
        case 24:
        case 27: {
            s32 index;
            switch (game) {
                case 4: index = 0; break;
                case 5: index = 1; break;
                case 12: index = 2; break;
                case 14: index = 3; break;
                case 17: index = 4; break;
                case 20: index = 5; break;
                case 24: index = 6; break;
                case 27: index = 7; break;
            }

            s32 oldState = unkE0[index];
            if (levelState >= eLevelState_CompleteOK && oldState <= 0) {
                if (index == 0)
                    unkE0[index] = 2;
                else
                    unkE0[index] = 1;
            }

            if (levelState <= eLevelState_AttemptNG && oldState >= 1)
                unkE0[index] = 0;

            break;
        }
    }
}

void SaveDataFile::fn_80077BC0(s8 arg) {
    unk3D = arg;
}

void SaveDataFile::fn_80077BC8(s32 arg1, s16 arg2) {
    unkA2 = 1;
    unkA3 = arg1;
    unkA4 = arg2;
}

void SaveDataFile::fn_80077BDC(void) {
    unkA2 = 0;
}

void SaveDataFile::fn_80077BE8(s8 arg) {
    unkA6 = 1;
    unkA7 = arg;
    unkA9 = 3;
}

void SaveDataFile::fn_80077C00(void) {
    unkA6 = 0;
}

void SaveDataFile::fn_80077C0C(s8 arg) {
    unkA7 = arg;
}

void SaveDataFile::fn_80077C14(void) {
    unkA7 = -1;
}

void SaveDataFile::fn_80077C20(void) {
    unkA8 = -1;
}

void SaveDataFile::fn_80077C2C(s32 offset, u8 value) {
    unkE0[offset] = value;
}

void SaveDataFile::fn_80077C38(s8 arg) {
    unkE8 = arg;
}

void SaveDataFile::fn_80077C40(s8 arg) {
    unkE9 = 1;
    unkEA = arg;
}

void SaveDataFile::fn_80077C50(void) {
    unkE9 = 0;
}

void SaveDataFile::fn_80077C5C(s32 offset, s32 value) {
    unkEC[offset] = value;
}

void SaveDataFile::fn_80777C6C(s32 offset, s8 value) {
    unk12C[offset] = value;
}

void SaveDataFile::fn_80077C78(s32 offset, s8 value) {
    unk13C[offset] = value;
}

void SaveDataFile::fn_80077C84(s32 offset) {
    unk12C[offset] = 99;
}

void SaveDataFile::fn_80077C94(s32 offset) {
    unk13C[offset] = 99;
}

void SaveDataFile::fn_80077CA4(s32 offset, SaveDataFile_Sub00 *sub) {
    u32 first = sub->unk00;
    u32 second = sub->unk04;

    unk14C[offset].unk00 = first;
    unk14C[offset].unk04 = second;
}

void SaveDataFile::fn_80077CC0(s32 offset, SaveDataFile_Sub00 *sub) {
    u32 first = sub->unk00;
    u32 second = sub->unk04;

    unk1CC[offset].unk00 = first;
    unk1CC[offset].unk04 = second;
}

void SaveDataFile::fn_80077CDC(s8 arg) {
    unk250 = arg;
}

void SaveDataFile::fn_80077CE4(s8 arg) {
    unk251 = arg;
}

void SaveDataFile::fn_80077CEC(bool arg) {
    unk252 = arg;
}

void SaveDataFile::fn_80077CF4(s8 arg) {
    unk253 = arg;
}

s8 SaveDataFile::fn_80077CFC(void) {
    return unk0;
}

bool SaveDataFile::fn_80077D04(void) {
    return unk0 != -1;
}

bool SaveDataFile::fn_80077D20(void) {
    return unk0 == 99;
}

RFLCreateID SaveDataFile::fn_80077D38(void) {
    return unk1;
}

s8 SaveDataFile::fn_80077D48(void) {
    return unk9;
}

s8 SaveDataFile::fn_80077D50(void) {
    return unkA;
}

ELevelState SaveDataFile::fn_80077D58(s32 arg) {
    return static_cast<ELevelState>(mLevelStates[arg]);
}

s16 SaveDataFile::fn_80077D64(u8 arg) {
    return unk3E[arg];
}

bool SaveDataFile::fn_80077D74(void) {
    return unk3D != 0;
}

bool SaveDataFile::fn_80077D88(void) {
    return unkA2 != 0;
}

s8 SaveDataFile::fn_80077D9C(void) {
    return unkA3;
}

s16 SaveDataFile::fn_80077DA4(void) {
    return unkA4;
}

bool SaveDataFile::fn_80077DAC(void) {
    return unkA6 != 0;
}

bool SaveDataFile::fn_80077DC0(void) {
    return unkA7 != -1;
}

bool SaveDataFile::fn_80077DDC(void) {
    return unkA8 != -1;
}

s8 SaveDataFile::fn_80077DF8(void) {
    return unkA7;
}

s8 SaveDataFile::fn_80077E00(void) {
    return unkA8;
}

u8 SaveDataFile::fn_80077E08(void) {
    return unkA9;
}

u8 SaveDataFile::fn_80077E10(s32 arg) {
    return unkE0[arg];
}

s8 SaveDataFile::fn_80077E1C(void) {
    return unkE8;
}

bool SaveDataFile::fn_80077E24(void) {
    return unkE9 != 0;
}

s8 SaveDataFile::fn_80077E38(void) {
    return unkEA;
}

s32 SaveDataFile::fn_80077E40(s32 arg) {
    return unkEC[arg];
}

s8 SaveDataFile::fn_80077E50(s32 arg) {
    return unk12C[arg];
}

s8 SaveDataFile::fn_80077E5C(s32 arg) {
    return unk13C[arg];
}

bool SaveDataFile::fn_80077E68(s32 arg) {
    return unk12C[arg] == 99;
}

bool SaveDataFile::fn_80077E84(s32 arg) {
    return unk13C[arg] == 99;
}

SaveDataFile_Sub00 SaveDataFile::fn_80077EA0(s32 arg) {
    return unk14C[arg];
}

SaveDataFile_Sub00 SaveDataFile::fn_80077EB4(s32 arg) {
    return unk1CC[arg];
}

bool SaveDataFile::fn_80077EC8(void) {
    return unk250 != 0;
}

bool SaveDataFile::fn_80077EDC(void) {
    return unk251 != 0;
}

bool SaveDataFile::fn_80077EF0(void) {
    return unk252 != 0;
}

bool SaveDataFile::fn_80077F04(void) {
    return unk253 != 0;
}

void SaveDataFile::fn_80077F18(u8 arg, s16 value) {
    s16 oldValue = unk3E[arg];

    if (oldValue < 0) {
        unk3E[arg] = value;
        return;
    }

    s32 diff = value - oldValue;
    s32 absDiff = diff < 0 ? -diff : diff;

    if (absDiff < 300) {
        if (value > oldValue)
            oldValue = value;
        unk3E[arg] = oldValue;
    } else {
        unk3E[arg] = (value + oldValue) / 2;
    }
}

u32 SaveDataFile::fn_80077F78(void) {
    s32 index;
    s32 total = 0;
    s32 count = 0;
    s32 result = 0;

    for (s32 column = 4; column >= 0; column--) {
        index = column;
        for (s32 row = 0; row < 10; index += 5, row++) {
            s32 value = unk3E[static_cast<u8>(index)];
            if (value >= 0) {
                total += value + 50;
                count++;
            }
        }
    }

    if (count > 0) {
        s32 bonus = (count - 1) * 70 / 50;
        s32 score = total / count / 100;
        score = score < 0 ? 0 : (score > 100 ? 100 : score);
        result = score * (bonus + 50) / 100 + 20;
    }

    return result;
}

u32 SaveDataFile::fn_800780E0(void) {
    u8 count = 0;

    for (u8 i = 0; i < ARRAY_LENGTH(mLevelStates); i++)
        if (static_cast<ELevelState>(mLevelStates[i]) >= eLevelState_CompleteHI)
            count++;

    return count;
}

u32 SaveDataFile::fn_8007819C(void) {
    u8 count = 0;

    for (u8 i = 0; i < ARRAY_LENGTH(unkE0); i++)
        if (static_cast<s32>(unkE0[i]) >= 4)
            count++;

    return count;
}

u8 SaveDataFile::fn_80078238(void) {
    u8 count = 0;

    for (u8 i = 0; i < ARRAY_LENGTH(mLevelStates); i++)
        if (static_cast<ELevelState>(mLevelStates[i]) >= eLevelState_Perfect)
            count++;

    return count;
}

u8 SaveDataFile::fn_800782F4(void) {
    u8 count = 0;

    for (u8 i = 0; i < ARRAY_LENGTH(mLevelStates); i++)
        if (static_cast<ELevelState>(mLevelStates[i]) >= eLevelState_Perfect)
            count++;

    return 50 - count;
}

void SaveDataFile::fn_800783B4(void) {
    unkA7 = -1;
    unkA8 = -1;
    unkA9 = 0;
    unkAA = 0;

    for (u8 i = 0; i < ARRAY_LENGTH(unkAB); i++)
        unkAB[i] = i;

    for (u8 i = 0; i < ARRAY_LENGTH(unkAB); i++) {
        u8 rand = sRandom.nextU32(ARRAY_LENGTH(unkAB));

        u8 temp = unkAB[i];
        unkAB[i] = unkAB[rand];
        unkAB[rand] = temp;
    }
}

s8 SaveDataFile::fn_80078548(void) {
    if (static_cast<ELevelState>(mLevelStates[4]) < eLevelState_CompleteOK)
        return -1;

    for (u8 i = 0; i < ARRAY_LENGTH(unkAB); i++) {
        u8 level = unkAB[i];
        if (static_cast<ELevelState>(mLevelStates[level]) != eLevelState_CompleteHI)
            continue;

        s8 foo = unkA7;
        if (fn_8000EBBC(level) == foo)
            continue;

        for (u8 j = i; j < ARRAY_LENGTH(unkAB) - 1; j++)
            unkAB[j] = unkAB[j + 1];
        unkAB[ARRAY_LENGTH(unkAB) - 1] = level;

        for (u8 j = 0; j < ARRAY_LENGTH(unkAB) - 3; j++) {
            u8 index = sRandom.nextU32(ARRAY_LENGTH(unkAB) - 3);

            u8 temp = unkAB[j];
            unkAB[j] = unkAB[index];
            unkAB[index] = temp;
        }

        return level;
    }

    return -1;
}

void SaveDataFile::fn_800786E4(void) {
    if (unkA9 != 0) {
        unkA9--;
    }
}

void SaveDataFile::fn_800786FC(void) {
    s32 count = 0;

    for (u8 i = 0; i < ARRAY_LENGTH(mLevelStates); i++)
        if (static_cast<ELevelState>(mLevelStates[i]) >= eLevelState_CompleteHI)
            count++;

    unkAA = 0;
    if (count <= 49)
        unkAA = 1;
    if (count <= 46)
        unkAA = 2;
    if (count <= 40)
        unkAA = 3;
    if (count <= 30)
        unkAA = 4;
    if (count <= 20)
        unkAA = 5;
    if (count <= 10)
        unkAA = 6;
}

void SaveDataFile::fn_80078808(void) {
    if (unkA9 != 0)
        return;
    if (unkA8 == -1)
        return;
    if (static_cast<ELevelState>(mLevelStates[4]) < eLevelState_CompleteOK)
        return;

    if (unkAA > 0)
        unkAA--;
}

bool SaveDataFile::fn_80078848(void) {
    s8 level = fn_80078548();
    if (level >= 0) {
        unkA8 = fn_8000EBBC(level);
        return true;
    } else {
        unkA8 = -1;
        return false;
    }
}

bool SaveDataFile::fn_80078898(void) {
    if (unkA9 != 0)
        return false;
    if (unkAA != 0)
        return false;

    return unkA8 != -1;
}

bool SaveDataFile::fn_800788DC(s32 giftIndex) {
    if (giftIndex == 0)
        return true;

    u8 count = 0;
    for (u8 i = 0; i < ARRAY_LENGTH(mLevelStates); i++)
        if (static_cast<ELevelState>(mLevelStates[i]) >= eLevelState_Perfect)
            count++;

    u8 remaining = 50 - count;
    if (remaining == 0)
        return true;

    for (u8 i = 0; i < ARRAY_LENGTH(mLevelStates); i++) {
        if (static_cast<ELevelState>(mLevelStates[i]) != eLevelState_Perfect)
            continue;

        const GameTableEntry *entry = fn_80079554(fn_80079540(i));
        if (entry->giftType != eGiftType_Music)
            continue;
        if (static_cast<s32>(entry->giftIndex) != giftIndex)
            continue;

        return true;
    }

    return false;
}

bool SaveDataFile::fn_80078A40(s32 giftIndex) {
    if (giftIndex == 0)
        return true;

    u8 count = 0;
    for (u8 i = 0; i < ARRAY_LENGTH(mLevelStates); i++)
        if (static_cast<ELevelState>(mLevelStates[i]) >= eLevelState_Perfect)
            count++;

    u8 remaining = 50 - count;
    if (remaining == 0)
        return true;

    for (u8 i = 0; i < ARRAY_LENGTH(mLevelStates); i++) {
        if (static_cast<ELevelState>(mLevelStates[i]) != eLevelState_Perfect)
            continue;

        const GameTableEntry *entry = fn_80079554(fn_80079540(i));
        if (entry->giftType != eGiftType_ReadingMaterial)
            continue;
        if (static_cast<s32>(entry->giftIndex) != giftIndex)
            continue;

        return true;
    }

    return false;
}

static inline bool isMusicGiftUnlocked(SaveDataFile *file, s32 giftIndex) {
    if (giftIndex == 0)
        return true;

    u8 count = 0;
    for (u8 i = 0; i < ARRAY_LENGTH(file->mLevelStates); i++)
        if (static_cast<ELevelState>(file->mLevelStates[i]) >= eLevelState_Perfect)
            count++;

    u8 remaining = 50 - count;
    if (remaining == 0)
        return true;

    for (u8 i = 0; i < ARRAY_LENGTH(file->mLevelStates); i++) {
        if (static_cast<ELevelState>(file->mLevelStates[i]) != eLevelState_Perfect)
            continue;

        const GameTableEntry *entry = fn_80079554(fn_80079540(i));
        if (entry->giftType != eGiftType_Music)
            continue;
        if (static_cast<s32>(entry->giftIndex) != giftIndex)
            continue;

        return true;
    }

    return false;
}

u8 SaveDataFile::fn_80078BA4(s32 arg) {
    u8 giftCount = 0;

    for (s32 i = 0; i < 88; i++)
        if (isMusicGiftUnlocked(this, i))
            giftCount++;

    return giftCount;
}

CSaveData::CSaveData(void) {}

void CSaveData::_08(void) {}

CSaveData::~CSaveData(void) {}

void CSaveData::initialize(void) {
    setSelectedSaveIndex(0);
    updateSnapshot();
}

void CSaveData::resetAllSaveFiles(void) {
    for (s32 saveIndex = 0; saveIndex < SAVE_COUNT; saveIndex++)
        mSaveFiles[saveIndex].fn_80077908();

    updateSnapshot();
}

void CSaveData::setSelectedSaveIndex(u8 saveIndex) {
    mSelectedSaveIndex = saveIndex;
}

u8 CSaveData::getSelectedSaveIndex(void) {
    return mSelectedSaveIndex;
}

SaveDataFile *CSaveData::getSaveFile(s32 saveIndex) {
    return &mSaveFiles[saveIndex];
}

SaveDataFile *CSaveData::getSelectedSaveFile(void) {
    return &mSaveFiles[mSelectedSaveIndex];
}

u8 *CSaveData::getUnkA04(void) {
    return unkA04;
}

void CSaveData::updateSnapshot(void) {
    for (s32 saveIndex = 0; saveIndex < SAVE_COUNT; saveIndex++)
        memcpy(&mSaveFilesSnapshots[saveIndex], &mSaveFiles[saveIndex], sizeof(SaveDataFile));

    memcpy(&unk1424, &unkA04, sizeof(unkA04));
}
