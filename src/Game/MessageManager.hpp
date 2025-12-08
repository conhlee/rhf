#ifndef GAME_MESSAGEMANAGER_HPP
#define GAME_MESSAGEMANAGER_HPP

#include <revolution/types.h>
#include <nw4r/lyt.h>
#include "Singleton.hpp"

// TODO
extern s32 fn_80087708(const char *);

class CMessageManager : public TSingleton<CMessageManager> {
public:
    CMessageManager(void);

    void fn_80088030(void);
    void fn_80088034(void);
    void fn_8008807C(void);
    void fn_80088088(void);
    void fn_800883F4(void);
    void fn_80088474(nw4r::lyt::Pane *);
    void fn_80088FEC(nw4r::lyt::TextBox *, s32, s32);

    void setupTextBox(nw4r::lyt::TextBox *textBox, const char *mesgIDStr, s32 bmgType) {
        s32 mesgID = fn_80087708(mesgIDStr);
        fn_80088FEC(textBox, mesgID, bmgType);
    }

private:
    u8 _pad[0x8C - 0x4];
};

extern CMessageManager *gMessageManager;

#endif