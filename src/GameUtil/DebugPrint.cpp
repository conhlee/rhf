#include "DebugPrint.hpp"

#include <cstdio>

#include "DebugFont.hpp"

CDebugPrint::CDebugPrint(void) :
    mUnk04(0), mSBGList(), mUnk14(0), mUnk2C(-1)
{}

CDebugPrint::SBG::SBG(void) {
    unk10 = -1;
}
CDebugPrint::SBG::~SBG(void) {}

CDebugPrint::~CDebugPrint(void) {}

void CDebugPrint::_10(void) {}

void CDebugPrint::_08(void) {}

void CDebugPrint::_14(void) {
    setFontColor(nw4r::ut::Color::WHITE);
    setFontSize(16.0f);
}

void CDebugPrint::fn_801EC674(s32 x, s32 y, s32 frames, const char *format, ...) {
    va_list args;

    if (mEnable) {
        va_start(args, format);

        Vec2i size = fn_801EC850(format, args);
        s32 sizeX = size.x;
        s32 sizeY = size.y;

        va_end(args);

        if (x < mUnk1C) {
            mUnk1C = x;
        }
        if (y < mUnk20) {
            mUnk20 = y;
        }

        if ((x + sizeX) > mUnk24) {
            mUnk24 = (x + sizeX);
        }
        if ((y + sizeY) > mUnk28) {
            mUnk28 = (y + sizeY);
        }
    }

    va_start(args, format);
    doRegister(x, y, frames, format, args);
    va_end(args);
}

// TODO: match
Vec2i CDebugPrint::fn_801EC850(const char *format, std::va_list args) {
    if (nw4r::db::DbgPrint::GetInstance() == NULL) {
        return (Vec2i) { 0, 0 };
    }

    nw4r::ut::Font *font = gDebugFont->getFont();

    char buffer[256];
    s32 charCount = vsnprintf(buffer, sizeof(buffer), format, args);

    nw4r::ut::CharStrmReader charStrmReader = font->GetCharStrmReader();
    charStrmReader.Set(buffer);

    f32 local_1b8 = 0.0f;
    f32 local_1b4 = 0.0f;

    f32 fontHeight = font->GetHeight();
    f32 dpFontSize = nw4r::db::DbgPrint::GetInstance()->GetFontSize();

    f32 fontSizeRatio = dpFontSize / fontHeight;
    s32 lineFeed = font->GetLineFeed();

    u16 lastChar = charStrmReader.Next();

    f32 dVar15 = 0.0f;
    f32 dVar1;
    f32 dVar16;


    for (;;) {
        const char *curPos = static_cast<const char *>(charStrmReader.GetCurrentPos());
        if (charCount < (curPos - buffer)) {
            break;
        }

        if (lastChar == '\n') {
        }
        else {
            f32 charWidth = font->GetCharWidth(lastChar);
            dVar1 = fontHeight * (charWidth) + local_1b8;
        }

        lastChar = charStrmReader.Next();
    }
}

void CDebugPrint::_18(void) {}
