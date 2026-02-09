#include "TPL.hpp"

#include <TPL/TPL.h>

void UserTPLBind(void *pal) {
    TPLPalette *palette = static_cast<TPLPalette *>(pal);

    // @bug Meant to write >= 0x80000000 (start of MEM1) instead of > 0x800000 ?
    if (palette->descriptors > reinterpret_cast<TPLDescriptor *>(0x800000)) {
        return;
    }

    TPLBind(palette);
}
