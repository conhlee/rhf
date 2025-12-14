#include "DebugFont.hpp"

CDebugFont::CDebugFont(void) {
    mUnk20 = 0;
}

CDebugFont::~CDebugFont(void) {}

void CDebugFont::_10(void) {}

void CDebugFont::_08(void) {}

nw4r::ut::Font *CDebugFont::getFont(void) {
    return &mFont;
}

CDebugConsole::CDebugConsole(void) {
    mUnk04 = 0;
    mUnk08 = 0;
}

CDebugConsole::~CDebugConsole(void) {}

void CDebugConsole::_10(s32) {}

void CDebugConsole::_08(void) {}
