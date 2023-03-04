#include "nshot_mod.h"
#include "keys.h"
#include "keycodes.h"

nshot_state_t  nshot_states[] = {
    {OS_LSFT, MOD_BIT(KC_LSFT), MOD_BIT(KC_LSFT), 1, true,  os_up_unqueued, 0, false},
    {OS_LCTL, MOD_BIT(KC_LCTL), MOD_BIT(KC_LGUI), 1, true,  os_up_unqueued, 0, false},
    {OS_LALT, MOD_BIT(KC_LALT), MOD_BIT(KC_LALT), 1, true,  os_up_unqueued, 0, false},
    {OS_RALT, MOD_BIT(KC_RALT), MOD_BIT(KC_RALT), 1, true,  os_up_unqueued, 0, false},
    {OS_LGUI, MOD_BIT(KC_LGUI), MOD_BIT(KC_LCTL), 1, true,  os_up_unqueued, 0, false}
};

uint8_t NUM_NSHOT_STATES = sizeof(nshot_states) / sizeof(nshot_state_t);

void process_nshot_state(uint16_t keycode, keyrecord_t *record) {
    nshot_state_t *curr_state = NULL;

    for (int i = 0; i < NUM_NSHOT_STATES; ++i) {
        curr_state = &nshot_states[i];
        uint8_t max_count = curr_state->max_count * 2;
        uint8_t current_modbit = keymap_config.swap_lctl_lgui ? curr_state->modbit : curr_state->modbit_mac;

        if (keycode == curr_state->trigger) {
            if (record->event.pressed) {
                // Trigger keydown
                if (curr_state->state == os_up_unqueued) {
                    register_mods(current_modbit);
                }
                curr_state->state = os_down_unused;
                curr_state->count = 0;
                curr_state->had_keydown = curr_state->active_on_rolls;
                curr_state->timer = timer_read();
            } else {
                // Trigger keyup
                switch (curr_state->state) {
                    case os_down_unused:
                        // If we didn't use the mod while trigger was held, queue it.
                        if((timer_elapsed(curr_state->timer)) < TAPPING_TERM) {
                        curr_state->state = os_up_queued;
                        break;
                        }
                        curr_state->state = os_up_unqueued;
                            unregister_mods(current_modbit);
                            break;
                    case os_down_used:
                        // If we did use the mod while trigger was held, unregister it.
                        curr_state->state = os_up_unqueued;
                        unregister_mods(current_modbit);
                        break;
                    default:
                        break;
                }
            }
        } else {
            if (record->event.pressed) {
                if (is_nshot_cancel_key(keycode) && curr_state->state != os_up_unqueued) {
                    // Cancel oneshot on designated cancel keydown.
                    curr_state->state = os_up_unqueued;
                    curr_state->count = 0;
                    curr_state->had_keydown = curr_state->active_on_rolls;
                    unregister_mods(current_modbit);
                }

                // Check for oneshot completion on sequential keys while rolling.
                // curr_state->state will only be os_up_queued after the n-shot has been triggered.
                if (curr_state->state == os_up_queued && !is_nshot_ignored_key(keycode)) {
                    // Increment on sequential key press.
                    curr_state->count = curr_state->count + 1;
                    curr_state->had_keydown = true;

                    // If count > max_count, the previous key hit maxed out the n-shot.
                    // Complete the n-shot; this current keycode will be pressed sans mod.
                    if (curr_state->count == max_count) {
                        curr_state->state = os_up_unqueued;
                        curr_state->count = 0;
                        curr_state->had_keydown = curr_state->active_on_rolls;
                        unregister_mods(current_modbit);
                    }
                }

                if (curr_state->state == os_down_unused){
                    curr_state->had_keydown = true;
                }
            } else {
                if (!is_nshot_ignored_key(keycode)) {
                    // On non-ignored keyup, consider the oneshot used.
                    switch (curr_state->state) {
                        case os_down_unused:
                            // if there's been a keydown, the mod is being used normally. Mark as used.
                            if(curr_state->had_keydown){
                                curr_state->state = os_down_used;
                            }
                            // if there has *not* been a keydown, the mod is being rolled into
                            // and this first keyup is not part of the mod behavior.
                            break;
                        case os_up_queued:
                            // If there's been a keydown, the mod key is being used as an n-shot.
                            // Increment the keycount.
                            if (curr_state->had_keydown){
                                curr_state->count = curr_state->count + 1;
                            }

                            // If the n-shot max has been reached, complete the n-shot.
                            if (curr_state->count == max_count) {
                                curr_state->state = os_up_unqueued;
                                curr_state->count = 0;
                                curr_state->had_keydown = curr_state->active_on_rolls;
                                unregister_mods(current_modbit);
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }
}
