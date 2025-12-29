#include "Prologue/MyLayout.hpp"

#include "LayoutManager.hpp"

const char layoutFile_prologue[] = "prologue.brlyt";

const char *layoutFileTable[] = { layoutFile_prologue, NULL };

void Prologue::CMyLayout::_10(void) {
    nw4r::lyt::MultiArcResourceAccessor *resAccessor = gLayoutManager->getResAccessor();

    u32 layoutBinSize;
    void *layoutBin = resAccessor->GetResource(0, layoutFileTable[0], &layoutBinSize);

    buildLayout(layoutBin, resAccessor);

    CLayoutAnimation *animation = getAnimation(0);
    animation->fn_801D9E70(getLayout(), NULL, "prologue_anim_00", resAccessor);

    this->CLayout::_10();
}

void Prologue::CMyLayout::fn_8007785C(u32 animIndex) {
    CLayoutAnimation *animation = getAnimation(animIndex);

    animation->fn_801DA20C();

    animation->setIsPlaying(true);
    animation->setSkipNextUpdate(true);
}

Prologue::CMyLayout::~CMyLayout(void) {
    _14();
}
