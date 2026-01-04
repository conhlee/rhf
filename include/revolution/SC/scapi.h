#ifndef RVL_SDK_SC_SCAPI_H
#define RVL_SDK_SC_SCAPI_H
#include <types.h>
#include <revolution/WPAD/bte.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { SC_ASPECT_STD, SC_ASPECT_WIDE } SCAspectRatio;

typedef enum { SC_EURGB_50_HZ, SC_EURGB_60_HZ } SCEuRgb60Mode;

typedef enum {
    SC_LANG_JP,
    SC_LANG_EN,
    SC_LANG_DE,
    SC_LANG_FR,
    SC_LANG_SP,
    SC_LANG_IT,
    SC_LANG_NL,
    SC_LANG_ZH_S,
    SC_LANG_ZH_T,
    SC_LANG_KR,
} SCLanguage;

typedef enum { SC_SND_MONO, SC_SND_STEREO, SC_SND_SURROUND } SCSoundMode;

typedef enum { SC_INTERLACED, SC_PROGRESSIVE } SCProgressiveMode;

typedef enum { SC_MOTOR_OFF, SC_MOTOR_ON } SCMotorMode;

typedef enum { SC_SENSOR_BAR_BOTTOM, SC_SENSOR_BAR_TOP } SCSensorBarPos;

typedef struct SCIdleModeInfo {
    u8 wc24;      // at 0x0
    u8 slotLight; // at 0x1
} SCIdleModeInfo;

typedef struct SCBtDeviceInfo {
    u8 mac[6];     // at 0x0
    char name[64]; // at 0x6
} SCBtDeviceInfo;

typedef struct SCBtDeviceInfoArray {
    u8 numRegist;              // at 0x0
    SCBtDeviceInfo regist[10]; // at 0x1
    SCBtDeviceInfo active[6];  // at 0x2BD
} SCBtDeviceInfoArray;

typedef struct SCBtCmpDevInfo {
    BD_ADDR devAddr; // size 0x06, offset 0x00
    struct small_dev_info small; // size 0x40, offset 0x06
    LINK_KEY linkKey; // size 0x10, offset 0x30 // ? or just a buffer
} SCBtCmpDevInfo; // size 0x56

typedef struct SCBtCmpDevInfoArray {
    u8 num; // size 0x001, offset 0x000 // name known from asserts
    SCBtCmpDevInfo devices[6]; // size 0x204, offset 0x001
} SCBtCmpDevInfoArray; // size 0x205

u8 SCGetAspectRatio(void);
s8 SCGetDisplayOffsetH(void);
u8 SCGetEuRgb60Mode(void);
void SCGetIdleMode(SCIdleModeInfo* mode);
u8 SCGetLanguage(void);
u8 SCGetProgressiveMode(void);
u8 SCGetScreenSaverMode(void);
u8 SCGetSoundMode(void);
u32 SCGetCounterBias(void);
void SCGetBtDeviceInfoArray(SCBtDeviceInfoArray* info);
BOOL SCSetBtDeviceInfoArray(const SCBtDeviceInfoArray* info);
void SCGetBtCmpDevInfoArray(SCBtCmpDevInfoArray *array);
BOOL SCSetBtCmpDevInfoArray(const SCBtCmpDevInfoArray *array);
u32 SCGetBtDpdSensibility(void);
u8 SCGetWpadMotorMode(void);
void SCSetWpadMotorMode(u8 mode);
u8 SCGetWpadSensorBarPosition(void);
u8 SCGetWpadSpeakerVolume(void);
void SCSetWpadSpeakerVolume(u8 vol);

#ifdef __cplusplus
}
#endif
#endif
