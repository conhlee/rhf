#include "Rap/MyLayout.hpp"

#include "LayoutManager.hpp"

#include "MessageManager.hpp"

const char layoutFile_rap[] = "rap.brlyt";

const char * const layoutFileTable[] = { layoutFile_rap, NULL };

void Rap::CMyLayout::_10(void) {
    nw4r::lyt::MultiArcResourceAccessor *resAccessor = gLayoutManager->getResAccessor();

    u32 layoutBinSize;
    void *layoutBin = resAccessor->GetResource(0, layoutFileTable[0], &layoutBinSize);

    buildLayout(layoutBin, resAccessor);

    gMessageManager->fn_80088474(getLayout()->GetRootPane());

    mNullMessage00 = getLayout()->GetRootPane()->FindPaneByName("N_message_00");
    mNullMissMsg00 = getLayout()->GetRootPane()->FindPaneByName("N_miss_msg_00");
    mNullMessage01 = getLayout()->GetRootPane()->FindPaneByName("N_message_01");
    mNullMessage02 = getLayout()->GetRootPane()->FindPaneByName("N_message_02");

    mTagProcessor = new CMyTagProcessor;

    mTextMessage00 = static_cast<nw4r::lyt::TextBox *>(
        getLayout()->GetRootPane()->FindPaneByName("T_message_00")
    );
    mTextMissMsg00 = static_cast<nw4r::lyt::TextBox *>(
        getLayout()->GetRootPane()->FindPaneByName("T_miss_msg_00")
    );
    mTextMessage01 = static_cast<nw4r::lyt::TextBox *>(
        getLayout()->GetRootPane()->FindPaneByName("T_message_01")
    );
    mTextMessage02 = static_cast<nw4r::lyt::TextBox *>(
        getLayout()->GetRootPane()->FindPaneByName("T_message_02")
    );

    mTextMessage00->SetTagProcessor(mTagProcessor);
    mTextMissMsg00->SetTagProcessor(mTagProcessor);
    mTextMessage01->SetTagProcessor(mTagProcessor);
    mTextMessage02->SetTagProcessor(mTagProcessor);

    getAnimation(0)->fn_801D9E70(getLayout(), mNullMessage00, "rap_show", resAccessor);
    getAnimation(1)->fn_801D9E70(getLayout(), mNullMissMsg00, "rap_show", resAccessor);
    getAnimation(2)->fn_801D9E70(getLayout(), mNullMessage01, "rap_show", resAccessor);
    getAnimation(3)->fn_801D9E70(getLayout(), mNullMessage02, "rap_show", resAccessor);
    getAnimation(4)->fn_801D9E70(getLayout(), mNullMessage00, "rap_hide", resAccessor);
    getAnimation(5)->fn_801D9E70(getLayout(), mNullMissMsg00, "rap_hide", resAccessor);
    getAnimation(6)->fn_801D9E70(getLayout(), mNullMessage01, "rap_hide", resAccessor);
    getAnimation(7)->fn_801D9E70(getLayout(), mNullMessage02, "rap_hide", resAccessor);

    this->CLayout::_10();

    getAnimation(4)->playFromBeginning();
    getAnimation(4)->fn_801DA770();

    getAnimation(6)->playFromBeginning();
    getAnimation(6)->fn_801DA770();

    getAnimation(7)->playFromBeginning();
    getAnimation(7)->fn_801DA770();

    mBaseX = mNullMessage02->GetTranslate().x;
    mBaseY = mNullMessage02->GetTranslate().y;

    mUnk48 = false;
}

void Rap::CMyLayout::_14(void) {
    delete mTagProcessor;
    this->CLayout::_14();
}

// TODO: match ( https://decomp.me/scratch/5jpGg )
void Rap::CMyLayout::fn_80072E94(const char *mesgIDStr, bool miss) {
    if (!miss) {
        nw4r::lyt::TextBox *tb = mTextMessage00;
        gMessageManager->setupTextBox(tb, mesgIDStr, 0);

        getAnimation(4)->setIsPlaying(false);
        getAnimation(0)->playFromBeginning();

        mUnk48 = true;
    }
    else {
        nw4r::lyt::TextBox *tb = mTextMissMsg00;
        gMessageManager->setupTextBox(tb, mesgIDStr, 0);

        getAnimation(5)->setIsPlaying(false);
        getAnimation(1)->playFromBeginning();

        mUnk48 = false;
    }
}

void Rap::CMyLayout::fn_80072F74(const char *mesgID) {
    gMessageManager->setupTextBox(mTextMessage01, mesgID, 0);

    getAnimation(6)->setIsPlaying(false);
    getAnimation(2)->playFromBeginning();
}

void Rap::CMyLayout::fn_8007307C(void) {
    if (mUnk48) {
        getAnimation(0)->setIsPlaying(false);
        getAnimation(4)->playFromBeginning();
    }
    else {
        getAnimation(1)->setIsPlaying(false);
        getAnimation(5)->playFromBeginning();
    }
}

void Rap::CMyLayout::fn_80073100(void) {
    getAnimation(2)->setIsPlaying(false);
    getAnimation(6)->playFromBeginning();
}

void Rap::CMyLayout::fn_8007314C(void) {
    getAnimation(3)->setIsPlaying(false);
    getAnimation(7)->playFromBeginning();
}

void Rap::CMyLayout::fn_80073198(f32 x, f32 y) {
    mNullMessage02->SetTranslate(nw4r::math::VEC2(mBaseX + x, mBaseY - y));
}

Rap::CMyLayout::~CMyLayout(void) {
    _14();
}
