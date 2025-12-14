#ifndef GAMEUTIL_DEBUGFONT_HPP
#define GAMEUTIL_DEBUGFONT_HPP

// TODO: change name of this TU as it now contains both CDebugFont & CDebugConsole ..

#include <revolution/types.h>

#include <nw4r/ut.h>

#include "Singleton.hpp"

class CDebugFont : public TSingleton<CDebugFont> {
public:
    virtual void _08(void);
    virtual ~CDebugFont(void);
    virtual void _10(void);

    CDebugFont();

    nw4r::ut::Font *getFont(void);

private:
    nw4r::ut::RomFont mFont;
    u32 mUnk20;
};

extern CDebugFont *gDebugFont;

class CDebugConsole : public TSingleton<CDebugConsole> {
public:
    virtual void _08(void);
    virtual ~CDebugConsole(void);
    virtual void _10(s32);

    CDebugConsole();

private:
    u32 mUnk04;
    u32 mUnk08;
    u32 mUnk0C;
};

extern CDebugConsole *gDebugConsole;

#endif
