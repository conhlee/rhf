#ifndef GAME_SAVEDATA_HPP
#define GAME_SAVEDATA_HPP

#include <revolution/types.h>
#include "Singleton.hpp"

struct SaveDataFile_Sub00 {
    u32 unk00;
    u32 unk04;
};

struct SaveDataFile {
    u8 pad00[0x280];

    
    void fn_80077C0C(s8);

    void fn_80077C5C(s32, s32);

    void fn_80777C6C(s32, s8);
    void fn_80077C78(s32, s8);

    void fn_80077C84(s32);
    void fn_80077C94(s32);

    void fn_80077CA4(s32, SaveDataFile_Sub00 *);
    void fn_80077CC0(s32, SaveDataFile_Sub00 *);

    s8 fn_80077DF8(void);
    u8 fn_80077E08(void);
    void fn_800786E4(void);

    s32 fn_80077E40(s32);
};

class CSaveData : public TSingleton<CSaveData> {
public:
    virtual void _08(void);
    virtual ~CSaveData(void);

    CSaveData();

    void fn_80078E04(void);
    void fn_80078E8C(void);

    SaveDataFile *fn_80078F4C(void);
    void fn_80078F68(void);

private:
    char _pad[0x1448 - 0x4];
};

extern CSaveData *gSaveData;

#endif
