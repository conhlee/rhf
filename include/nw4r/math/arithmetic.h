#ifndef NW4R_MATH_ARITHMETIC_H
#define NW4R_MATH_ARITHMETIC_H

#include <revolution/types.h>

#include <nw4r/db/assert.h>

namespace nw4r {
namespace math {

f32 FrSqrt(f32);

inline f32 FSqrt(f32 x) {
    if (!(x >= 0.0f)) {
        NW4R_WARNING(627, "FSqrt: Input is out of the domain.");
    }
    return (x <= 0) ? 0.0f : x * FrSqrt(x);
}

} // namespace math
} // namespace nw4r

#endif
