#ifndef NW4R_UT_CHAR_STRM_READER_H
#define NW4R_UT_CHAR_STRM_READER_H
#include <revolution/types.h>

#include <nw4r/db/assert.h>

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
        NW4R_ASSERT_PTR(this, 75);
        return (this->*mReadFunc)();
    }

    const void* GetCurrentPos() const {
        NW4R_ASSERT_PTR(this, 69);
        return mCharStrm;
    }

    void Set(const char* stream) {
        NW4R_ASSERT_PTR(this, 50);
        NW4R_ASSERT_PTR(stream, 51);


        NW4R_ASSERT(mReadFunc == ReadNextCharUTF8 || mReadFunc == ReadNextCharCP1252 || mReadFunc == ReadNextCharSJIS, 54);

        mCharStrm = stream;
    }
    void Set(const wchar_t* stream) {
        mCharStrm = stream;
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
