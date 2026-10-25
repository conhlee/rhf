#include "MessageManager.hpp"

#include <revolution/SC.h>

#include <nw4r/ut.h>

#include "CheckPointManager.hpp"
#include "BackupManager.hpp"
#include "FileManager.hpp"

#include "SaveData.hpp"

#include "Random.hpp"

#include "MyTagProcessor.hpp"

#define BMG_MAKE_ID(char1, char2, char3, char4) \
    (((u32)(char1) << 24) | ((u32)(char2) << 16) | ((u32)(char3) << 8) | (u32)(char4))

enum {
    BMG_BLOCK_INF1 = BMG_MAKE_ID('I','N','F','1'),
    BMG_BLOCK_DAT1 = BMG_MAKE_ID('D','A','T','1'),
    BMG_BLOCK_STR1 = BMG_MAKE_ID('S','T','R','1'),
    BMG_BLOCK_MID1 = BMG_MAKE_ID('M','I','D','1'),
};

struct BMGBlockHeader {
    u32 type;
    u32 size;

    BMGBlockHeader *getNext(void) {
        return reinterpret_cast<BMGBlockHeader *>(
            reinterpret_cast<u8 *>(this) + size
        );
    }
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

struct MyMessageAttr {
    u16 fontScale;
    u8 textAlign; // nw4r::lyt::HorizontalPosition
    u8 fontScaleOver;
};
static_assert(sizeof(MyMessageAttr) == 4, "");

struct BMGMessageInfo {
    struct Entry {
        u32 dataOffset;
        MyMessageAttr attr;
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

CBMGRes::CBMGRes(void) {
    mData = NULL;

    mFileHeader = NULL;
    mMessageInfo = NULL;
    mMessageData = NULL;
    mStrAttributeInfo = NULL;
    mMessageIDInfo = NULL;
}

CBMGRes::~CBMGRes(void) {}

void CBMGRes::init(u32 arcIndex, const char *pathInArc) {
    mData = gFileManager->arcGetFileAddr(arcIndex, pathInArc);

    mFileHeader = static_cast<BMGFileHeader *>(mData);

    BMGBlockHeader *currentBlock = mFileHeader->mFirstBlock;
    for (u32 i = 0; i < mFileHeader->numBlocks; i++) {
        void *blockContent = reinterpret_cast<u8 *>(currentBlock) + sizeof(BMGBlockHeader);

        switch (currentBlock->type) {
        case BMG_BLOCK_INF1:
            mMessageInfo = static_cast<BMGMessageInfo *>(blockContent);
            break;
        case BMG_BLOCK_DAT1:
            mMessageData = static_cast<BMGMessageData *>(blockContent);
            break;
        case BMG_BLOCK_STR1:
            mStrAttributeInfo = static_cast<BMGStringAttributeInfo *>(blockContent);
            break;
        case BMG_BLOCK_MID1:
            mMessageIDInfo = static_cast<BMGMessageIDInfo *>(blockContent);
            break;
        }

        currentBlock = currentBlock->getNext();
    }
}

// NOTE: Inlined
u32 CBMGRes::mesgIDToIndex(u32 id) {
    u32 i = 0;
    for (i = 0; i < mMessageIDInfo->numEntries; i++) {
        if (id == mMessageIDInfo->entries[i]) {
            break;
        }
    }
    return i;
}

static const wchar_t lbl_80325130[] = L"XXX";

static const wchar_t lbl_80325138[] = L"???";

// TODO: match (regswaps)
// NOTE: Inlined
const wchar_t *CBMGRes::getString(u32 id) {
    if (id == MESG_ID_INVALID) {
        return lbl_80325130;
    }
    else {
        u32 i = mesgIDToIndex(id);
        if (i < mMessageIDInfo->numEntries) {
            const wchar_t *string = &mMessageData->data[mMessageInfo->entries[i].dataOffset / sizeof(wchar_t)];
            if (wcslen(string) != 0) {
                return string;
            }
            else {
                return lbl_80325138;
            }
        }
        else {
            return lbl_80325138;
        }
    }
}

// NOTE: Inlined
u32 CBMGRes::getStringSize(u32 id) {
    u32 mesgIndex = mesgIDToIndex(id);
    return mMessageInfo->entries[mesgIndex + 1].dataOffset -
           mMessageInfo->entries[mesgIndex + 0].dataOffset;
}

// NOTE: Inlined
u16 CBMGRes::getFontScale(u32 id) {
    if (id == MESG_ID_INVALID) {
        return 100;
    }
    else {
        u32 i = mesgIDToIndex(id);
        if (i < mMessageIDInfo->numEntries) {
            u16 fontScale = mMessageInfo->entries[i].attr.fontScaleOver;
            if (fontScale == 0) {
                fontScale = mMessageInfo->entries[i].attr.fontScale;
            }
            return fontScale;
        }
        else {
            return 100;
        }
    }
}

// NOTE: Inlined
u16 CBMGRes::getFontScaleOrig(u32 id) {
    if (id == MESG_ID_INVALID) {
        return 100;
    }
    else {
        u32 i = mesgIDToIndex(id);
        if (i < mMessageIDInfo->numEntries) {
            u16 fontScale = mMessageInfo->entries[i].attr.fontScale;
            return fontScale;
        }
        else {
            return 100;
        }
    }
}

// NOTE: Inlined
u8 CBMGRes::getTextAlign(u32 id) {
    if (id == MESG_ID_INVALID) {
        return nw4r::lyt::HORIZONTALPOSITION_MAX;
    }
    else {
        u32 i = mesgIDToIndex(id);
        if (i < mMessageIDInfo->numEntries) {
            return mMessageInfo->entries[i].attr.textAlign;
        }
        else {
            return nw4r::lyt::HORIZONTALPOSITION_MAX;
        }
    }
}

// NOTE: Not inlined
s32 CMessageManager::fn_80087708(const char *string) {
    s32 num = 0;

    if (string[0] < '0' || string[0] > '9') {
        return MESG_ID_INVALID;
    }
    num += (string[0] - '0');

    for (s32 i = 1; i < 7; i++) {
        num *= 10;
        if ((string[i] < '0') || (string[i] > '9')) {
            return MESG_ID_INVALID;
        }
        num += (string[i] - '0');
    }
    
    return num;
}

static bool mesgIDStrCheck(const char *string) {
    for (s32 i = 0; i < 7; i++) {
        if ((string[i] < '0') || (string[i] > '9')) {
            return false;
        }
    }
    return true;
}

u8 CBMGRes::fn_80087854(u32 id) {
    const u8 *stream = reinterpret_cast<const u8 *>(getString(id));
    const u8 *streamEnd = stream + getStringSize(id);

    u8 i = 0;
    while (stream < streamEnd) {
        wchar_t ch = *reinterpret_cast<const wchar_t *>(stream);
        if (ch == 0x001A) {
            s32 len = *(stream + 2);
            if (*(stream + 3) == 0x02) {
                i++;
            }

            for (s32 i = 0; i < len; i++) {
                stream++;
            }
        }
        else {
            stream += sizeof(wchar_t);
        }
    }

    return i + 1;
}

u16 CBMGRes::fn_80087A30(u32 id, u8 param) {
    const u8 *stream = reinterpret_cast<const u8 *>(getString(id));
    const u8 *streamEnd = stream + + getStringSize(id);

    u16 k = 0;

    u8 j = 0;
    
    while (stream < streamEnd) {
        wchar_t ch = *reinterpret_cast<const wchar_t *>(stream);
        if (ch == 0x001A) {
            s32 len = *(stream + 2);
            if (*(stream + 3) == 0x02) {
                if (j == param) {
                    return k + 2;
                }
                j++;

                k = 0;

                for (s32 i = 0; i < len; i++) {
                    stream++;
                }
            }
            else {
                for (s32 i = 0; i < len; i++) {
                    stream++;
                    k++;
                }
            }
        }
        else {
            stream += sizeof(wchar_t);
            k += (u16)sizeof(wchar_t);
        }
    }

    return k;
}

// TODO: reg alloc bullshit
const wchar_t *CBMGRes::fn_80087CB0(u32 id, u8 param, wchar_t *param2) {
    const u8 *stream = reinterpret_cast<const u8 *>(getString(id));
    const u8 *streamEnd = stream + getStringSize(id);

    u8 j = 0;
    u8 *subStream = (u8 *)param2;
    
    while (stream < streamEnd) {
        wchar_t ch = *reinterpret_cast<const wchar_t *>(stream);
        if (ch == 0x001A) {
            s32 len = *(stream + 2);
            if (*(stream + 3) == 0x02) {
                if (j == param) {
                    *(wchar_t *)subStream = L'\0';
                    return reinterpret_cast<const wchar_t *>(stream);
                }
                j++;

                for (s32 i = 0; i < len; i++) {
                    stream++;
                }
            }
            else {
                for (s32 i = 0; i < len; i++) {
                    u8 byte = *stream++;
                    if (j == param) {
                        *subStream++ = byte;
                    }
                }
            }
        }
        else {
            stream += sizeof(wchar_t);
            if (j == param) {
                *(wchar_t *)subStream = ch;
                subStream += sizeof(wchar_t);
            }
        }
    }

    return NULL;
}

CMessageManager::CMessageManager(void) {}

CMessageManager::~CMessageManager(void) {}

void CMessageManager::fn_80088030(void) {}

void CMessageManager::fn_80088034(void) {
    switch (SCGetLanguage()) {
    default:
        gFileManager->startArchive(0, "content2/message/riq_E.szs");
        gFileManager->waitArc(0);
        break;
    }
}

void CMessageManager::fn_8008807C(void) {
    gFileManager->endArchive(0);
}

// TODO: getString trouble
void CMessageManager::fn_80088088(void) {
    switch (SCGetLanguage()) {
    default:
        mBMG[eMesgSource_Main].init(0, "riq_E.bmg");
        break;
    }
    switch (SCGetLanguage()) {
    default:
        mBMG[eMesgSource_Title].init(0, "riq_title_E.bmg");
        break;
    }
    switch (SCGetLanguage()) {
    default:
        mBMG[eMesgSource_Comment].init(0, "riq_comment_E.bmg");
        break;
    }
    switch (SCGetLanguage()) {
    default:
        mBMG[eMesgSource_Bonus].init(0, "riq_bonus_E.bmg");
        break;
    }
    switch (SCGetLanguage()) {
    default:
        mBMG[eMesgSource_Cafe].init(0, "riq_cafe_E.bmg");
        break;
    }

    wchar_t *str = (wchar_t *)gMessageManager->getString("0000101", eMesgSource_Comment);
    gCheckPointManager->fn_801EC250(1, str);

    str = (wchar_t *)gMessageManager->getString("0000102", eMesgSource_Comment);
    gCheckPointManager->fn_801EC250(2, str);

    str = (wchar_t *)gMessageManager->getString("0000103", eMesgSource_Comment);
    gCheckPointManager->fn_801EC260(str);

    str = (wchar_t *)gMessageManager->getString("0000100", eMesgSource_Comment);
    gCheckPointManager->fn_801EC268(str);
}

void CMessageManager::fn_800883F4(void) {
    for (s32 i = 0; i < ARRAY_LENGTH(mBMG); i++) {
        mBMG[i].mData = NULL;
        mBMG[i].mFileHeader = NULL;
        mBMG[i].mMessageInfo = NULL;
        mBMG[i].mMessageData = NULL;
        mBMG[i].mStrAttributeInfo = NULL;
        mBMG[i].mMessageIDInfo = NULL;
    }
}

void CMessageManager::fn_80088474(nw4r::lyt::Pane *pane) {
    const char *mesgIDStr = pane->GetUserData();

    nw4r::lyt::TextBox *textBox = nw4r::ut::DynamicCast<nw4r::lyt::TextBox *>(pane);
    if (textBox != NULL) {
        textBox->AllocStringBuffer(0x100);

        nw4r::ut::Font *font = const_cast<nw4r::ut::Font *>(textBox->GetFont());
        font->SetAlternateChar(0xE06B);

        if (mesgIDStrCheck(mesgIDStr)) {
            u32 mesgID = fn_80087708(mesgIDStr);

            const wchar_t *str = mBMG[eMesgSource_Main].getString(mesgID);
            u32 strSize = mBMG[eMesgSource_Main].getStringSize(mesgID);
            s32 fontScale = mBMG[eMesgSource_Main].getFontScale(mesgID);
            u8 textAlign = mBMG[eMesgSource_Main].getTextAlign(mesgID);

            fn_800888E0(textBox, str, strSize, fontScale, textAlign);
        }
        else {
            textBox->SetString(L"");
        }
    }

    nw4r::lyt::Pane::PaneList &paneList = textBox->GetChildList();
    for (
        nw4r::lyt::Pane::PaneList::Iterator it = paneList.GetBeginIter();
        it != paneList.GetEndIter(); it++
    ) {
        nw4r::lyt::Pane &pane = *it;
        fn_80088474(&pane);
    }
}

void CMessageManager::fn_800888E0(nw4r::lyt::TextBox *textBox, const wchar_t *src, s32 size, f32 scale, s32 textAlign) {
    static wchar_t buffer[0x100];
    // @bug Should be sizeof(buffer) instead of ARRAY_LENGTH(buffer)
    memset(buffer, 0x00, ARRAY_LENGTH(buffer));

    u32 len = fn_800892A8(src, buffer, size);
    textBox->SetString(buffer, 0, (len / sizeof(wchar_t)));

    const nw4r::ut::Font *font = textBox->GetFont();

    f32 fw = scale * font->GetWidth() / 100.0f;
    f32 fh = scale * font->GetHeight() / 100.0f;
    textBox->SetFontSize(nw4r::lyt::Size(fw, fh));

    if ((u32)textAlign <= nw4r::lyt::HORIZONTALPOSITION_MAX) {
        textBox->SetTextPositionH(textAlign);
    }
    else if (textAlign == 4) {
        CMyTagProcessor tagProcessor;
        nw4r::ut::WideTextWriter textWriter;

        textWriter.SetFont(*font);
        textWriter.SetFontSize(fw, fh);
        textWriter.SetLineSpace(textBox->GetLineSpace());
        textWriter.SetCharSpace(textBox->GetCharSpace());
        textWriter.SetTagProcessor(&tagProcessor);

        f32 width = textWriter.CalcStringWidth(buffer, (len / sizeof(wchar_t)));
        textBox->SetSize(nw4r::lyt::Size(width, textBox->GetSize().height));
        textBox->SetTextPositionH(0);
    }
}

/*
 * TODO: I think this is functionally matching but it has some useless code (inline?)
 *       that I can't seem to replicate ...
 */
void CMessageManager::fn_80088FEC(nw4r::lyt::TextBox *textBox, s32 mesgID, s32 sourceID) {
    const wchar_t *string = mBMG[sourceID].getString(mesgID);
    u32 stringSize = mBMG[sourceID].getStringSize(mesgID);
    u16 fontScale = mBMG[sourceID].getFontScale(mesgID);
    u8 textAlign = mBMG[sourceID].getTextAlign(mesgID);

    fn_800888E0(textBox, string, stringSize, fontScale, textAlign);
}

static const wchar_t lbl_80325158[] = L"\xE042"; // button A
static const wchar_t lbl_8032515C[] = L"\xE043"; // button B
static const wchar_t lbl_80325160[] = L"\xE044"; // button HOME
static const wchar_t lbl_80325164[] = L"\xE045"; // button PLUS
static const wchar_t lbl_80325168[] = L"\xE046"; // button MINUS
static const wchar_t lbl_8032516C[] = L"\xE047"; // button 1
static const wchar_t lbl_80325170[] = L"\xE048"; // button 2

static const char lbl_80325178[] = "0998000";
static const char lbl_80325180[] = "0998001";
static const char lbl_80325188[] = "0998002";
static const char lbl_80325190[] = "0998003";

static const char *lbl_802ECB70[] = {
    lbl_80325178,
    lbl_80325180,
    lbl_80325188,
    lbl_80325190,
};

static const char lbl_80325198[] = "0993000";
static const char lbl_803251A0[] = "0993001";
static const char lbl_803251A8[] = "0993002";
static const char lbl_803251B0[] = "0993003";

static const char *lbl_802ECB80[] = {
    lbl_80325198,
    lbl_803251A0,
    lbl_803251A8,
    lbl_803251B0
};

u32 CMessageManager::fn_800892A8(const wchar_t *src, wchar_t *dest, s32 srcSize) {
    const u8 *stream = reinterpret_cast<const u8 *>(src);
    const u8 *streamEnd = stream + srcSize;
    u32 destLen = 0;
    while (stream < streamEnd) {
        wchar_t ch = *reinterpret_cast<const wchar_t *>(stream);
        if (ch == 0x001A) {
            s32 len = *(stream + 2);
            if (*(stream + 3) == 0x01) {
                wchar_t tag = *reinterpret_cast<const wchar_t *>(stream + 4);

                const wchar_t *output;
                switch (tag) {
                case 0x0001:
                    output = lbl_80325158;
                    break;
                case 0x0002:
                    output = lbl_8032515C;
                    break;
                case 0x0003:
                    output = lbl_80325160;
                    break;
                case 0x0004:
                    output = lbl_80325164;
                    break;
                case 0x0005:
                    output = lbl_80325168;
                    break;
                case 0x0006:
                    output = lbl_8032516C;
                    break;
                case 0x0007:
                    output = lbl_80325170;
                    break;
                case 0x000A:
                    output = mBMG[eMesgSource_Title].getString(mFormatIDTitle);
                    break;
                case 0x0014: {
                    static wchar_t buffer[] = L"0";
                    u32 num = gBackupManager->getBlocksRequired() / 8;
                    if ((gBackupManager->getBlocksRequired() % 8) != 0) {
                        num++;
                    }
                    buffer[0] = L'0' + num;
                    output = buffer;
                } break;
                case 0x0015: {
                    static wchar_t buffer[] = L"0";
                    buffer[0] = L'0' + gBackupManager->getFilesRequired();
                    output = buffer;
                } break;
                case 0x001E:
                    output = mBMG[eMesgSource_Bonus].getString(mFormatIDBonus);
                    break;
                case 0x001F: {
                    static wchar_t buffer[4];
                    u8 perfectsLeft = gSaveData->fn_80078F4C()->fn_800782F4() - 1;
                    swprintf(buffer, ARRAY_LENGTH(buffer) - 1, L"%d", perfectsLeft);
                    output = buffer;
                } break;
                case 0x0028:
                case 0x0029:
                case 0x002A:
                case 0x002B: {
                    static wchar_t buffer[4];
                    u32 num;
                    if (tag == 0x0028) {
                        num = gSaveData->fn_80078F4C()->fn_800780E0();
                    }
                    else if (tag == 0x0029) {
                        num = gSaveData->fn_80078F4C()->fn_8007819C();
                    }
                    else {
                        num = mMedalCount;
                    }

                    if ((tag == 0x0028) || (tag == 0x002A)) {
                        swprintf(buffer, 3, L"%02d", (u8)num);
                    }
                    else {
                        swprintf(buffer, 2, L"%d", (u8)num);
                    }

                    output = buffer;
                } break;
                case 0x003C: {
                    static wchar_t buffer[4];
                    swprintf(buffer, ARRAY_LENGTH(buffer), L"%d", mUnk86);
                    output = buffer;
                } break;
                case 0x003D: {
                    static wchar_t buffer[4];
                    swprintf(buffer, ARRAY_LENGTH(buffer), L"%02d", mUnk88);
                    output = buffer;
                } break;
                case 0x0032: {
                    u32 index = sRandom.nextU32(ARRAY_LENGTH(lbl_802ECB70));
                    output = gMessageManager->getString(lbl_802ECB70[index], eMesgSource_Cafe);
                } break;
                case 0x0033: {
                    u32 index = sRandom.nextU32(ARRAY_LENGTH(lbl_802ECB80));
                    output = gMessageManager->getString(lbl_802ECB80[index], eMesgSource_Cafe);
                } break;
                }

                for (; *output != L'\0'; output++) {
                    *dest++ = *output;
                    destLen += sizeof(wchar_t);
                }

                stream += len;
            }
            else if (*(stream + 3) == 0x02) {
                for (s32 i = 0; i < len; i++) {
                    stream++;
                }
            }
            else {
                for (s32 i = 0; i < len; i++) {
                    *((u8 *)dest)++ = *stream++;
                    destLen++;
                }
            }
        }
        else {
            *dest++ = *((const wchar_t *)stream)++;
            destLen += sizeof(wchar_t);
        }
    }
}

CUnkMessage::CUnkMessage(const char *mesgIDStr, s32 sourceID, BOOL unk) {
    s32 mesgID = CMessageManager::fn_80087708(mesgIDStr);
    fn_80089934(mesgID, sourceID, unk);
}

void CUnkMessage::fn_80089934(s32 mesgID, s32 sourceID, BOOL unk) {
    mMessageID = mesgID;
    mSubCount = gMessageManager->mBMG[sourceID].fn_80087854(mMessageID);
    mUnk0C = gMessageManager->mBMG[sourceID].getFontScale(mMessageID);
    mUnk0E = gMessageManager->mBMG[sourceID].getFontScaleOrig(mMessageID);
    mUnk10 = gMessageManager->mBMG[sourceID].getTextAlign(mMessageID);

    mSub = new Sub[mSubCount];
    for (u8 i = 0; i < mSubCount; i++) {
        u32 dataSize = gMessageManager->mBMG[sourceID].fn_80087A30(mMessageID, i);
        mSub[i].dataSize = dataSize;
        mSub[i].unk04 = reinterpret_cast<wchar_t *>(new u8[mSub[i].dataSize]);
        
        const wchar_t *end = gMessageManager->mBMG[sourceID].fn_80087CB0(mMessageID, i, mSub[i].unk04);
        if (end == NULL) {
            mSub[i].unk08 = L'\0';
        }
        else {
            mSub[i].unk08 = *(end + 2);
        }

        if (mSub[i].unk08 == L'\x02') {
            mSub[i].unk0C = *(const u32 *)(end + 3);
        }

        if (unk && (mSub[i].unk04[0] == L'\n')) {
            mSub[i].dataSize -= 1 * sizeof(wchar_t);

            wchar_t *newBuf = reinterpret_cast<wchar_t *>(new u8[mSub[i].dataSize]);
            memcpy(newBuf, mSub[i].unk04 + 1, mSub[i].dataSize);
            delete[] mSub[i].unk04;
            mSub[i].unk04 = newBuf;
        }
    }
}

CUnkMessage::~CUnkMessage(void) {
    for (u8 i = 0; i < mSubCount; i++) {
        delete[] mSub[i].unk04;
    }
    delete[] mSub;
}

u32 CUnkMessage::fn_80089CEC(void) {
    return mMessageID;
}

u8 CUnkMessage::fn_80089CF4(void) {
    return mSubCount;
}

u16 CUnkMessage::fn_80089CFC(s32 index) {
    return mSub[index].dataSize;
}

wchar_t *CUnkMessage::fn_80089D0C(s32 index) {
    return mSub[index].unk04;
}

bool CUnkMessage::fn_80089D20(s32 index) {
    return (mSub[index].unk08 - 1) == 0;
}

u16 CUnkMessage::fn_80089D88(s32 foo, s32 bar) {
    return mSub[1 + foo + bar].dataSize;
}

wchar_t *CUnkMessage::fn_80089DA0(s32 foo, s32 bar) {
    return mSub[1 + foo + bar].unk04;
}

u32 CUnkMessage::fn_80089DBC(s32 foo, s32 bar) {
    return mSub[1 + foo + bar].unk0C;
}

u16 CUnkMessage::fn_80089DD8(void) {
    return mUnk0C;
}

u16 CUnkMessage::fn_80089DE0(void) {
    return mUnk0E;
}

u32 CUnkMessage::fn_80089DE8(void) {
    return mUnk10;
}

