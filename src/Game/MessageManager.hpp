#ifndef GAME_MESSAGEMANAGER_HPP
#define GAME_MESSAGEMANAGER_HPP

#include <revolution/types.h>

#include <nw4r/lyt.h>

#include "Singleton.hpp"

enum { MESG_ID_INVALID = 10000001 };
namespace {

struct BMGBlockHeader {
    u32 type;
    u32 dataSize;

    BMGBlockHeader *getNext(void) {
        return reinterpret_cast<BMGBlockHeader *>(
            reinterpret_cast<u8 *>(this) + dataSize
        );
    }
};

enum {
    BMG_BLOCK_INF1 = 0x494e4631,
    BMG_BLOCK_DAT1 = 0x44415431,
    BMG_BLOCK_STR1 = 0x53545231,
    BMG_BLOCK_MID1 = 0x4d494431,
};

struct BMGFileHeader {
    enum EEncoding {
        ENCODING_1BYTE,
        ENCODING_2BYTE,
        ENCODING_SJIS,
        ENCODING_UTF8,
    };

    u32 signature;
    u32 type;
    u32 dataSize;
    u32 numBlocks;

    u8 mEncoding; // EEncoding

    u8 mUnk11[0x20 - 0x11];

    BMGBlockHeader mFirstBlock[1];
};

struct BMGMessageInfo {
    struct Entry {
        u32 dataOffset;
        u32 attribute;
    };

    u16 numEntries;
    u16 entrySize;
    u16 groupID;
    Entry entries[1];
};

struct BMGMessageData {
    wchar_t data[1];
};

struct BMGStringAttributeInfo {};

struct BMGMessageIDInfo {
    u16 numEntries;
    u8 form;
    u8 formSupplement;
    u8 reserved[4];
    u32 entries[];
};

} // namespace

class CMessageManager; // Forward-declaration

class CBMGRes {
    friend class CMessageManager;

public:
    CBMGRes(void);
    ~CBMGRes(void);

private:
    void init(u32 arcIndex, const char *pathInArc);
    wchar_t *getString(u32 id);

private:
    void *mData;

    BMGFileHeader *mFileHeader;
    BMGMessageInfo *mMessageInfo;
    BMGMessageData *mMessageData;
    BMGStringAttributeInfo *mStrAttributeInfo;
    BMGMessageIDInfo *mMessageIDInfo;
};

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

    void setFormatMesgID(const char *mesgIDStr) {
        mFormatMesgID = fn_80087708(mesgIDStr);
    }

    static s32 fn_80087708(const char *mesgIDStr);

private:
    enum { BMG_COUNT = 5 };

private:
    CBMGRes mBMG[BMG_COUNT];
    s32 mUnk7C;
    s32 mFormatMesgID;
    u8 mMedalCount;
    u16 mUnk86;
    u16 mUnk88;
};

extern CMessageManager *gMessageManager;

#endif