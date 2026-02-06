#include <nw4r/ut.h>

namespace nw4r {
namespace ut {

template <typename T>
TagProcessorBase<T>::TagProcessorBase() {}

template <typename T>
TagProcessorBase<T>::~TagProcessorBase() {}

template <typename T>
typename TagProcessorBase<T>::Operation
TagProcessorBase<T>::Process(u16 code, ContextType *context) {
    switch (code) {
    case '\n':
        ProcessLinefeed(context);
        return OPERATION_NEXT_LINE;

    case '\t':
        ProcessTab(context);
        return OPERATION_NO_CHAR_SPACE;
    }

    return OPERATION_DEFAULT;
}

template <typename T>
typename TagProcessorBase<T>::Operation
TagProcessorBase<T>::CalcRect(Rect* pRect, u16 code, ContextType *context) {
    switch (code) {
    case '\n': {
        const TextWriterBase<T> &writer = *context->writer;

        pRect->right = writer.GetCursorX();
        pRect->top = writer.GetCursorY();
        ProcessLinefeed(context);
        pRect->left = writer.GetCursorX();
        pRect->bottom = writer.GetCursorY() + context->writer->GetFontHeight();
        pRect->Normalize();
    } return OPERATION_NEXT_LINE;

    case '\t': {
        const TextWriterBase<T> &writer = *context->writer;

        pRect->left = writer.GetCursorX();
        ProcessTab(context);
        pRect->right = writer.GetCursorX();
        pRect->top = writer.GetCursorY();
        pRect->bottom = pRect->top + writer.GetFontHeight();
        pRect->Normalize();
    } return OPERATION_NO_CHAR_SPACE;
    }

    return OPERATION_DEFAULT;
}

template <typename T>
void TagProcessorBase<T>::ProcessLinefeed(ContextType *context) {
    TextWriterBase<T> &writer = *context->writer;

    f32 x = context->x;
    f32 y = writer.GetCursorY() + writer.GetLineHeight();

    writer.SetCursorX(x);
    writer.SetCursorY(y);
}

template <typename T>
void TagProcessorBase<T>::ProcessTab(ContextType *context) {
    TextWriterBase<T> &writer = *context->writer;

    int tabWidth = writer.GetTabWidth();
    if (tabWidth <= 0) {
        return;
    }

    f32 aCharWidth = writer.IsWidthFixed() ? writer.GetFixedWidth() : writer.GetFontWidth();

    f32 dx = writer.GetCursorX() - context->x;
    f32 tabPixel = tabWidth * aCharWidth;
    int numTab = static_cast<int>(dx / tabPixel) + 1;
    f32 x = context->x + (tabPixel * numTab);

    writer.SetCursorX(x);
}

template class TagProcessorBase<char>;
template class TagProcessorBase<wchar_t>;

} // namespace ut
} // namespace nw4r
