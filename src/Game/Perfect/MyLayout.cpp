#include "Perfect/MyLayout.hpp"

#include "LayoutManager.hpp"

#include "MessageManager.hpp"

#include "SaveData.hpp"

#include "GameTable.hpp"
#include "GiftTable.hpp"

const char layoutFile_perfect[] = "perfect.brlyt";

DECL_SECTION(".sdata") const char * const layoutFileTable[] = { layoutFile_perfect, NULL };

// TODO
extern "C" void fn_801D8A5C(nw4r::lyt::TextBox *, f32, f32);

enum {
    perfect_loop
};

void Perfect::CMyLayout::_10(void) {
    nw4r::lyt::MultiArcResourceAccessor *resAccessor = gLayoutManager->getResAccessor();

    u32 layoutBinSize;
    void *layoutBin = gLayoutManager->getResAccessor()->GetResource(0, layoutFileTable[0], &layoutBinSize);

    buildLayout(layoutBin, resAccessor);

    gMessageManager->fn_80088474(getLayout()->GetRootPane());

    getAnimation(perfect_loop)->fn_801D9E70(getLayout(), NULL, "perfect_loop", resAccessor);
    getAnimation(perfect_loop)->setLoop(true);

    getAnimation(perfect_loop)->playFromBeginForward();

    this->CLayout::_10();

    s32 gameNum = gSaveData->fn_80078F4C()->fn_80077DF8();
    const GameTableEntry *gameEnt = fn_8000EB50(gameNum);

    const char *giftTitle;
    if (gameEnt->giftType == GameTableEntry::eGiftType_Music) {
        giftTitle = fn_800B4338(gameEnt->giftIndex)->titleFormatted;
    }
    else { // GameTableEntry::eGiftType_ReadingMaterial
        giftTitle = fn_800B434C(gameEnt->giftIndex)->titleFormatted;
    }

    gMessageManager->setFormatMesgID(giftTitle);

    u8 perfectsLeft = gSaveData->fn_80078F4C()->fn_800782F4() - 1;

    const char *perfectMesg;
    if (gameEnt->giftType == GameTableEntry::eGiftType_Music) {
        if (perfectsLeft == 0) {
            perfectMesg = "0103003";
        }
        else if (perfectsLeft == 1) {
            perfectMesg = "0103005";
        }
        else {
            perfectMesg = "0103001";
        }
    }
    else { // GameTableEntry::eGiftType_ReadingMaterial
        if (perfectsLeft == 0) {
            perfectMesg = "0103002";
        }
        else if (perfectsLeft == 1) {
            perfectMesg = "0103004";
        }
        else {
            perfectMesg = "0103000";
        }
    }

    nw4r::lyt::TextBox *mesgTextBox = static_cast<nw4r::lyt::TextBox *>(
        getLayout()->GetRootPane()->FindPaneByName("T_pft_00")
    );

    gMessageManager->setupTextBox(mesgTextBox, perfectMesg, 0);

    fn_801D8A5C(mesgTextBox, 25.6f, 32.0f);
}

Perfect::CMyLayout::~CMyLayout(void) {
    _14();
}
