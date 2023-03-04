#pragma once
#include "quantum.h"
#include QMK_KEYBOARD_H

enum keycodes {
    OS_LCTL = SAFE_RANGE,
    OS_LALT,
    OS_RALT,
    OS_LGUI,
    OS_LSFT,
    POINTER_DEFAULT_DPI_FORWARD,
    POINTER_DEFAULT_DPI_REVERSE,
    SNIPING_MODE,
    SNIPING_MODE_TOGGLE,
    DRAGSCROLL_MODE,
    DRAGSCROLL_MODE_TOGGLE,
    ACCELERATION_TOGGLE,
    UNDO,
    REDO,
    REPEAT,
    SW_WIN,
    SW_CG,
    COPY,
    CUT,
    PASTE,
    SALL,
    DWRD,
    QUIT,
    SAVE,
    COLEMAK,
    QWERTY,
};
