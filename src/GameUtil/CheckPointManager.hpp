#ifndef GAMEUTIL_CHECKPOINTMANAGER_HPP
#define GAMEUTIL_CHECKPOINTMANAGER_HPP

#include <revolution/types.h>
#include "Singleton.hpp"

class CCheckPointManager : public TSingleton<CCheckPointManager> {
public:

    virtual void _08(void);
    virtual ~CCheckPointManager(void);
    virtual void _10(void);
    virtual void _14(void);

    CCheckPointManager(void);

    void fn_801EAE20();
    void fn_801EAF2C(u8, u32);
    void fn_801EB00C(u8, u32);

    void setUnk2FC(u32 arg1) {
        unk2FC = arg1;
    }
    u8 getUnk78(void) {
        return unk78;
    }

    void setUnkDD(u8 unk) { unkDD = unk; }
    u8 getUnkDD(void) const { return unkDD; }
private:

    u8 pad04[0x78 - 4];
    u8 unk78;
    u8 pad79[0xdd - 0x79];
    u8 unkDD;
    u8 padDE[0x2fc - 0xde];
    u32 unk2FC;
};

extern CCheckPointManager *gCheckPointManager;

#endif