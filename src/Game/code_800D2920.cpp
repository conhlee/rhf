#include "code_800D2920.hpp"

#include "rev_tengoku.rsid"

// Pitch multiplier of -2 semitones (2 ** (-2 / 12))
static const f32 MIN_2_SEMITONES = 0.89090383f;

void fn_800D2920(u16 sfx1P, u16 sfx2P_P1, u16 sfx2P_P2, BOOL set2P, BOOL setP1, SNDHandle *soundHandle) {
    if (!set2P) {
        gSoundManager->play(sfx1P, 0.0f, soundHandle);
    }
    else {
        if (setP1) {
            gSoundManager->play(sfx2P_P1, 0.0f, soundHandle);
        }
        else {
            gSoundManager->play(sfx2P_P2, 0.0f, soundHandle);
            if ((sfx2P_P2 != SE_ROCKET_BELL_2P) && (sfx2P_P2 != SE_KARATE_MANY_OBJECT_2P)) {
                gSoundManager->tune_pitch(MIN_2_SEMITONES, soundHandle);
            }
        }
    }
}

void fn_800D29C8(u16 sfx, BOOL set2P, BOOL setP1, SNDHandle *soundHandle) {
    if (!set2P) {
        gSoundManager->play(sfx, 0.0f, soundHandle);
    }
    else {
        if (setP1) {
            gSoundManager->play(sfx, 0.0f, soundHandle);
        }
        else {
            gSoundManager->play(sfx, 0.0f, soundHandle);
            if ((sfx != SE_ROCKET_BELL_2P) && (sfx != SE_KARATE_MANY_OBJECT_2P)) {
                gSoundManager->tune_pitch(MIN_2_SEMITONES, soundHandle);
            }
        }
    }
}
