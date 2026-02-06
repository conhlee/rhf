#include <nw4r/g3d.h>
#include <nw4r/ut.h>

#include <revolution/OS.h>
#include <revolution/VI.h>
#include <revolution/GX.h>

namespace {

static const char * NW4R_G3D_Version_ =
    "<< NW4R    - G3D \tfinal   build: Apr  2 2009 04:10:47 (0x4302_145) >>";

} // namespace

namespace nw4r {
namespace g3d {

void G3dInit(bool bLCEnable) {
    OSRegisterVersion(NW4R_G3D_Version_);

    if (bLCEnable) {
        ut::LC::Enable();
    }
    else {
        ut::LC::Disable();
    }

    InitFastCast();

    GXRenderModeObj *rmode;

    u32 tvFormat = VIGetTvFormat(); // TODO: fake temp to generate unsigned compare
    switch (tvFormat) {
    case VI_TVFORMAT_NTSC:
        rmode = &GXNtsc480IntDf;
        break;
    case VI_TVFORMAT_PAL:
        rmode = &GXPal528IntDf;
        break;
    case VI_TVFORMAT_EURGB60:
        rmode = &GXEurgb60Hz480IntDf;
        break;
    case VI_TVFORMAT_MPAL:
        rmode = &GXMpal480IntDf;
        break;
    default:
        rmode = &GXNtsc480IntDf;
        break;
    }

    G3DState::SetRenderModeObj(*rmode);
}

void G3dReset() {
    G3DState::Invalidate(0x7ff);
}

} // namespace g3d
} // namespace nw4r
