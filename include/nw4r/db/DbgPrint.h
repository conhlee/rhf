#ifndef NW4R_UT_DBGPRINT_H
#define NW4R_UT_DBGPRINT_H

#include <nw4r/types_nw4r.h>

#include <nw4r/db/DbgPrintBase.h>

namespace nw4r {
namespace db {

typedef DbgPrintBase<char> DbgPrint;

typedef DbgPrintBase<wchar_t> WideDbgPrint;

} // namespace db
} // namespace nw4r

#endif