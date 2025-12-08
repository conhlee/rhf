#ifndef GAME_ICONSELECTLAYOUT_HPP
#define GAME_ICONSELECTLAYOUT_HPP

#include <revolution/types.h>

#include "Layout.hpp"

#include "SaveData.hpp"

class CIconSelectLayout : public CLayout {
public:
    virtual ~CIconSelectLayout(void);
    virtual void _10(void);
    virtual void _14(void);

    CIconSelectLayout(void) {
        setAnimationCount(116);
    }

    void fn_800C1C10(bool);

    bool fn_800C1DE0(void);

    void fn_800C1DF4(bool *, u8 *, SaveDataFile_Sub00 *, bool *, u8 *, SaveDataFile_Sub00 *);

private:
    // TODO: members
};

#endif
