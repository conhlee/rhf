#ifndef MSL_STDBOOL_H
#define MSL_STDBOOL_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef bool
typedef unsigned char bool;
#define false 0
#define true 1
#endif

#ifdef __cplusplus
}
#endif
#endif
