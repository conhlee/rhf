#include <revolution/OS.h>
#include <revolution/SC.h>
#include <string.h>

#define XOR_KEY 0x73B5DBFA

typedef struct SCRegion {
    s8 area;      // at 0x0
    char name[4]; // at 0x1
} SCRegion;

typedef struct SCGameRegion {
    s8 region;    // at 0x0
    char name[3]; // at 0x1
} SCGameRegion;

static SCRegion ProductAreaAndStringTbl[] = {
    {SC_AREA_JPN, "JPN"},
    {SC_AREA_USA, "USA"},
    {SC_AREA_EUR, "EUR"},
    {SC_AREA_AUS, "AUS"},
    {SC_AREA_BRA, "BRA"},
    {SC_AREA_TWN, "TWN"},
    {SC_AREA_TWN, "ROC"},
    {SC_AREA_KOR, "KOR"},
    {SC_AREA_HKG, "HKG"},
    {SC_AREA_ASI, "ASI"},
    {SC_AREA_LTN, "LTN"},
    {SC_AREA_SAF, "SAF"},
    {SC_AREA_CHN, "CHN"},
    {SC_AREA_NULL, ""} // End of List
};

static SCGameRegion ProductGameRegionAndStringTbl[] = {
    {SC_PRD_GAME_REG_JP, "JP"},
    {SC_PRD_GAME_REG_US, "US"},
    {SC_PRD_GAME_REG_EU, "EU"},
    {SC_PRD_GAME_REG_KR, "KR"},
    {SC_PRD_GAME_REG_CN, "CN"},
    {SC_PRD_GAME_REG_NULL, ""} // End of List
};

BOOL __SCF1(const char* type, char* buf, u32 sz) {
    u8 ptext;
    BOOL found = FALSE;
    u32 i;
    const u8* settings = (const u8*)OSPhysicalToCached(OS_PHYS_SC_PRDINFO);
    u32 key = XOR_KEY;
    u32 typeOfs = 0;
    u32 bufOfs = 0;

    for (i = 0; i < SC_PRDINFO_SIZE; i++, key = key >> 31 | key << 1) {
        ptext = settings[i];

        if (ptext != 0x00) {
            ptext ^= key;
            if (type[typeOfs] == 0x00 && ptext == '=') {
                found = TRUE;
                break;
            }

            if (((ptext ^ type[typeOfs]) & 0xDF) == 0) {
                typeOfs++;
            }
            else {
                typeOfs = 0;
            }
        }
    }

    if (found) {
        for (i++; i < SC_PRDINFO_SIZE && bufOfs < sz; i++) {
            key = key >> 31 | key << 1;
            ptext = settings[i];

            if (ptext != 0x00) {
                ptext ^= key;
                if (ptext == '\r' || ptext == '\n') {
                    ptext = 0x00;
                }
            }

            buf[bufOfs++] = ptext;

            if (ptext == 0x00) {
                return TRUE;
            }
        }
    }

    return FALSE;
}

BOOL SCGetProductAreaString(char* buf, u32 sz) {
    return __SCF1("AREA", buf, sz);
}

s8 SCGetProductArea(void) {
    s8 area;
    char name[4];
    SCRegion* iter = ProductAreaAndStringTbl;

    if (SCGetProductAreaString(name, sizeof(name))) {
        for (; (area = iter->area) != -1; iter++) {
            if (!strcmp(iter->name, name)) {
                return area;
            }
        }
    }

    return -1;
}

BOOL SCGetProductGameRegionString(char* buf, u32 sz) {
    return __SCF1("GAME", buf, sz);
}

s8 SCGetProductGameRegion(void) {
    s8 region;
    char name[3];
    SCGameRegion* iter = ProductGameRegionAndStringTbl;

    if (SCGetProductGameRegionString(name, sizeof(name))) {
        for (; (region = iter->region) != -1; iter++) {
            if (!strcmp(iter->name, name)) {
                return region;
            }
        }
    }

    return -1;
}


