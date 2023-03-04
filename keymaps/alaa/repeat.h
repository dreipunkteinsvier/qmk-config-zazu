#pragma once
#include QMK_KEYBOARD_H

// Initialize variables holding the bitfield
// representation of active modifiers.
uint8_t mod_state;
uint8_t oneshot_mod_state;

void process_repeat_key(uint16_t keycode, keyrecord_t *record);