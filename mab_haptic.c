/* Copyright 2022 Alaa Mansour
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mab_haptic.h"
#include "keys.h"
#include "drivers/haptic/DRV2605L.h"

bool mab_process_record_haptic(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case G(KC_Q):
        case A(KC_F4):
        case UNDO:
        case REDO:
        case ACCELERATION_TOGGLE:
        case DRAGSCROLL_MODE_TOGGLE:
        case SNIPING_MODE_TOGGLE:
        case KC_AUDIO_VOL_UP:
        case KC_AUDIO_VOL_DOWN:
        case KC_BTN1:
        case KC_BTN3:
        case C(KC_V):
        case C(KC_C):
        case C(KC_X):
        case G(KC_V):
        case G(KC_C):
        case G(KC_X):
            if (record->event.pressed) {
                DRV_pulse(medium_click1);
            }
            break;
        case DF(0):
        case DF(1):
        case SW_CG:
        case CG_TOGG:
        case POINTER_DEFAULT_DPI_FORWARD:
        case EE_CLR:
        case QK_BOOT:
            if (record->event.pressed) {
                DRV_pulse(strong_click1);
            }
            break;
        case KC_ESC:
            if (record->event.pressed) {
                /* DRV_pulse(sharp_tick1); */
                DRV_pulse(medium_click1);
            }
            break;
        case KC_BTN2:
            if (record->event.pressed) {
                DRV_pulse(sh_dblclick_str);
            }
            break;
        case KC_S: // save
            if (record->event.pressed) {
                if ((get_mods() & MOD_MASK_GUI) || (get_oneshot_mods() & MOD_MASK_GUI)) {
                    DRV_pulse(medium_click1);
                }
                if ((get_mods() & MOD_MASK_CTRL) || (get_oneshot_mods() & MOD_MASK_CTRL)) {
                    DRV_pulse(medium_click1);
                }
            }
            break;
        default:
            break;
    }

    return true;
}

__attribute__((weak)) bool get_haptic_enabled_key_user(uint16_t keycode, keyrecord_t *record) { return false; }

bool get_haptic_enabled_key(uint16_t keycode, keyrecord_t *record) {
    bool returnValue = false;

    // Note to self: Be careful when setting anything to true in this switch statement, as it cannot be overridden in the
    // get_haptic_enabled_key_user call, since it's an ||
    switch (keycode) {
        default:
            returnValue = false;
            break;
    }

    return (returnValue || get_haptic_enabled_key_user(keycode, record));
}
