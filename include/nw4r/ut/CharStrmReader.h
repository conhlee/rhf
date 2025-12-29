#ifndef NW4R_UT_CHAR_STRM_READER_H
#define NW4R_UT_CHAR_STRM_READER_H
#include <revolution/types.h>

#include <nw4r/db.h>

namespace nw4r {
namespace ut {

class CharStrmReader {
public:
    typedef u16 (CharStrmReader::*ReadFunc)();

public:
    explicit CharStrmReader(ReadFunc pFunc)
        : mCharStrm(NULL), mReadFunc(pFunc) {}

    ~CharStrmReader() {}

    u16 ReadNextCharUTF8();
    u16 ReadNextCharUTF16();
    u16 ReadNextCharCP1252();
    u16 ReadNextCharSJIS();

    u16 Next() {
        return (this->*mReadFunc)();
    }

    const void* GetCurrentPos() const {
        return mCharStrm;
    }

    void Set(const char* stream) {
        NW4R_ASSERT_PTR(this, 50);
        NW4R_ASSERT_PTR(stream, 51);


        NW4R_ASSERT(mReadFunc == ReadNextCharUTF8 || mReadFunc == ReadNextCharCP1252 || mReadFunc == ReadNextCharSJIS, 54);

        mCharStrm = stream;
    }
    void Set(const wchar_t* pStrm) {
        mCharStrm = pStrm;
    }

private:
    template <typename T> T GetChar(int offset) const {
        return static_cast<const T*>(mCharStrm)[offset];
    }

    template <typename T> void StepStrm(int offset) {
        static_cast<const T*>(mCharStrm) += offset;
    }

private:
    const void* mCharStrm; // at 0x0
    ReadFunc mReadFunc;    // at 0x4
};

} // namespace ut
} // namespace nw4r

#endif
