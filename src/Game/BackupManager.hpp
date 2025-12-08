#ifndef GAME_BACKUPMANAGER_HPP
#define GAME_BACKUPMANAGER_HPP

#include <revolution/types.h>
#include "Singleton.hpp"

class CBackupManager : public TSingleton<CBackupManager> {
public:
    CBackupManager();

    void fn_80084054(void);
    void fn_8008414C(void);
    s32 fn_800843E4(void);

    s32 fn_8008452C(void);
    s32 fn_80084A20(void);
    s32 fn_80084D1C(void);
    void waitAsync(void);
    s32 fn_8008516C(void);

    //TODO
    void fn_80084FC8(u32);

    s32 getUnk4C(void) const { return mUnk4C; }

private:
    u8 _pad04[0x4C - 4];
    s32 mUnk4C;
    u8 _pad50[0x2B78 - 0x50];
};

extern CBackupManager *gBackupManager;

#endif
