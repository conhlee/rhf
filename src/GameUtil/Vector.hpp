#ifndef GAMEUTIL_VECTOR_HPP
#define GAMEUTIL_VECTOR_HPP

/*
    NOTE: this vector class is utilised solely by DebugPrint.
*/

#include <revolution/types.h>

#include <cstring>

template <typename T>
class CVector {
public:
    virtual ~CVector(void) {
        delete[] mArray;
    }

    CVector(void) {
        mSize = 1;
        mArray = new T[mSize];
        memset(mArray, 0x00, mSize * sizeof(T));
    }

private:
    s32 mSize;
    T *mArray;
};

#endif
