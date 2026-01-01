#include "TPL.hpp"

#include <TPL/TPL.h>

void UserTPLBind(void *pal) {
    TPLPalette *palette = static_cast<TPLPalette *>(pal);

    // @bug Might have forgotten a zero or two on that address ..
    if (palette->descriptors > reinterpret_cast<TPLDescriptor *>(0x800000)) {
        return;
    }

    TPLBind(palette);
}
