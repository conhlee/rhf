#ifndef NW4R_DB_DBGPRINTBASE_H
#define NW4R_DB_DBGPRINTBASE_H

#include <nw4r/types_nw4r.h>

#include <nw4r/ut.h>

#include <cstdarg>

namespace nw4r {
namespace db {

template <typename T>
class DbgPrintBase {
public:
    static DbgPrintBase<T> *GetInstance();

    static void Initialize(void *buffer, u32 size, const ut::Font &font, ut::Color textColor);

    void SetTextColor(ut::Color color);
    ut::Color GetTextColor(void);

    void SetFontSize(f32 size);
    f32 GetFontSize(void);

    void SetVisible(bool visible);
    bool IsVisible(void);

    void Registerf(int x, int y, const char *format, ...);
    void Registerf(int x, int y, int time, const char *format, ...);

    void VRegisterf(int x, int y, int time, const char *format, std::va_list args);
};

} // namespace db
} // namespace nw4r

#endif