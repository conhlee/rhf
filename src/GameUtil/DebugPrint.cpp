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

// non matching: data
// ptmfs are misplaced, may have something
// to do with the pool data hack below
// (needed to match fn_801EC850)
#pragma pool_data off

struct MyVec2 {
    f32 x, y;

    f32 max(void) const {
        f32 _y = y;
        f32 _x = x;
        return _x < _y ? _y : _x;
    }
};

Vec2i CDebugPrint::fn_801EC850(const char *format, std::va_list args) {
    if (nw4r::db::DbgPrint::GetInstance() != NULL) {
        nw4r::ut::Font *font = gDebugFont->getFont();
    
        char buffer[256];
        s32 charCount = vsnprintf(buffer, sizeof(buffer), format, args);
    
        nw4r::ut::CharStrmReader charStrmReader = font->GetCharStrmReader();
        charStrmReader.Set(buffer);
    
        
        f32 fontHeight = font->GetHeight();
        f32 dpFontSize = nw4r::db::DbgPrint::GetInstance()->GetFontSize();
        
        f32 fontSizeRatio = dpFontSize / fontHeight;

        MyVec2 local_18 = { 0 };
    
        s32 lineFeed = font->GetLineFeed();
        
        f32 temp_f28 = fontSizeRatio * lineFeed;
    
        u16 lastChar = charStrmReader.Next();
    
        while ((static_cast<const char *>(charStrmReader.GetCurrentPos()) - buffer) <= charCount) {
            if (lastChar == '\n') {
                local_18.y = local_18.max();
                local_18.x = 0.0f;
    
                temp_f28 += fontSizeRatio * font->GetLineFeed();
            }
            else {
                local_18.x += fontSizeRatio * font->GetCharWidth(lastChar);
            }
    
            lastChar = charStrmReader.Next();
        }

        local_18.y = local_18.max();
        return (Vec2i) { local_18.y, temp_f28 };
    }
    return (Vec2i) { 0, 0 };

}

void CDebugPrint::_18(void) {}
