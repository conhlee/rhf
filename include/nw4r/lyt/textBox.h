#ifndef NW4R_LYT_TEXTBOX_H
#define NW4R_LYT_TEXTBOX_H

#include <revolution/types.h>
#include <nw4r/lyt/common.h>
#include <nw4r/lyt/pane.h>

namespace nw4r {
namespace lyt {

// Forward declarations
struct ResBlockSet;

enum TextColor {
    TEXTCOLOR_TOP,
    TEXTCOLOR_BOTTOM,

    TEXTCOLOR_MAX
};

class TextBox : public Pane {
public:
    NW4R_UT_RUNTIME_TYPEINFO;

public:
    TextBox(const void* pRes, const ResBlockSet& rBlockSet);
    virtual ~TextBox(); // at 0x8

    virtual void DrawSelf(const DrawInfo& rInfo); // at 0x18

    virtual const ut::Color GetVtxColor(u32 idx) const;       // at 0x24
    virtual void SetVtxColor(u32 idx, ut::Color color); // at 0x28

    virtual u8 GetVtxColorElement(u32 idx) const;       // at 0x34
    virtual void SetVtxColorElement(u32 idx, u8 value); // at 0x38

    virtual void AllocStringBuffer(u16 len); // at 0x64
    virtual void FreeStringBuffer();         // at 0x68
    u16 GetStringBufferLength() const;

    virtual u16 SetString(const wchar_t* pStr, u16 pos = 0);          // at 0x6C
    virtual u16 SetString(const wchar_t* pStr, u16 pos, u16 len); // at 0x70

    ut::Rect GetTextDrawRect(const DrawInfo& rInfo) const;
    ut::Rect GetTextDrawRect(ut::WideTextWriter* pWriter) const;

    const wchar_t* GetString() const {
        return mTextBuf;
    }
    const wchar_t* GetStringBuffer() const {
        return mTextBuf;
    }

    const ut::Font* GetFont() const;
    void SetFont(const ut::Font* pFont);

    ut::Color GetTextColor(u32 idx) const {
        return mTextColors[idx];
    }
    void SetTextColor(u32 idx, ut::Color color) {
        mTextColors[idx] = color;
    }

    const Size& GetFontSize() const {
        return mFontSize;
    }
    void SetFontSize(const Size& rFontSize) {
        mFontSize = rFontSize;
    }

    f32 GetLineSpace() const {
        return mLineSpace;
    }
    void SetLineSpace(f32 space) {
        mLineSpace = space;
    }

    f32 GetCharSpace() const {
        return mCharSpace;
    }
    void SetCharSpace(f32 space) {
        mCharSpace = space;
    }

    ut::WideTagProcessor* GetTagProcessor() const {
        return mpTagProcessor;
    }
    void SetTagProcessor(ut::WideTagProcessor* pProcessor) {
        mpTagProcessor = pProcessor;
    }

    u8 GetTextPositionH() const {
        return detail::GetHorizontalPosition(mTextPosition);
    }
    void SetTextPositionH(u8 value) {
        detail::SetHorizontalPosition(&mTextPosition, value);
    }

    u8 GetTextPositionV() const {
        return detail::GetVerticalPosition(mTextPosition);
    }
    void SetTextPositionV(u8 value) {
        detail::SetVerticalPosition(&mTextPosition, value);
    }

    f32 GetTextMagH() const;
    f32 GetTextMagV() const;

    u32 MakeDrawFlag() const;

protected:
    wchar_t* mTextBuf;                    // at 0xD4
    ut::Color mTextColors[TEXTCOLOR_MAX]; // at 0xD8

    const ut::Font* mpFont; // at 0xE0
    Size mFontSize;         // at 0xE4
    f32 mLineSpace;         // at 0xEC
    f32 mCharSpace;         // at 0xF0

    ut::WideTagProcessor* mpTagProcessor; // at 0xF4
    u16 mTextBufBytes;                    // at 0xF8
    u16 mTextLen;                         // at 0xFA
    u8 mTextPosition;                     // at 0xFC

    struct {
        u8 bAllocFont : 1;
    } mBits; // at 0xFD
};

} // namespace lyt
} // namespace nw4r

#endif
