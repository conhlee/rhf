#include "SaveData.hpp"

CSaveData::CSaveData(void) {}

void CSaveData::_08(void) {}

CSaveData::~CSaveData(void) {}

void CSaveData::fn_80078E04(void) {
    fn_80078F2C(0);
    fn_80078F68();
}

void CSaveData::fn_80078E8C(void) {
    for (s32 saveIndex = 0; saveIndex < SAVE_COUNT; saveIndex++)
        unk4[saveIndex].fn_80077908();

    fn_80078F68();
}

void CSaveData::fn_80078F2C(u8 arg) {
    unk1444 = arg;
}

u8 CSaveData::fn_80078F34(void) {
    return unk1444;
}

SaveDataFile *CSaveData::fn_80078F3C(s32 saveIndex) {
    return &unk4[saveIndex];
}

SaveDataFile *CSaveData::fn_80078F4C(void) {
    return &unk4[unk1444];
}

u8 *CSaveData::fn_80078F60(void) {
    return unkA04;
}

void CSaveData::fn_80078F68(void) {
    for (s32 saveIndex = 0; saveIndex < SAVE_COUNT; saveIndex++)
        memcpy(&unkA24[saveIndex], &unk4[saveIndex], sizeof(SaveDataFile));

    memcpy(&unk1424, &unkA04, 0x20);
}
