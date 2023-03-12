#include "swapper.h"
// #include "mab_haptic.h"
#include "mab_pointing.h"
#include "lvgl.h"
#include "display.h"
#include "keys.h"

enum layers {
    _QWERTY = 0,
    _COLEMAK,
    _NAVIGATION,
    _MEDIA,
    _NUMBERS,
    _FUNCTION,
};

// pointing
#define DPI_MOD POINTER_DEFAULT_DPI_FORWARD
#define DPI_RMOD POINTER_DEFAULT_DPI_REVERSE
#define SNIPING SNIPING_MODE
#define SNP_TOG SNIPING_MODE_TOGGLE
#define DRGSCRL DRAGSCROLL_MODE
#define DRG_TOG DRAGSCROLL_MODE_TOGGLE
#define ACC_TOG ACCELERATION_TOGGLE


// QWERTY HOME ROW MODS
#define HM_A	LGUI_T(KC_A)
#define HM_S	LALT_T(KC_S)
#define HM_D	LCTL_T(KC_D)
#define HM_F	LSFT_T(KC_F)
#define HM_G	RALT_T(KC_G)
#define HM_H	RALT_T(KC_H)
#define HM_J	RSFT_T(KC_J)
#define HM_K	LCTL_T(KC_K)
#define HM_L	LALT_T(KC_L)
#define HM_SCLN LGUI_T(KC_SCLN)

// COLEMAK HOME ROW MODS
#define CM_A    LGUI_T(KC_A)
#define CM_R    LALT_T(KC_R)
#define CM_S    LCTL_T(KC_S)
#define CM_T    LSFT_T(KC_T)
#define CM_G    RALT_T(KC_G)
#define CM_M    RALT_T(KC_M)
#define CM_N    RSFT_T(KC_N)
#define CM_E    LCTL_T(KC_E)
#define CM_I    LALT_T(KC_I)
#define CM_O    LGUI_T(KC_O)

// CTRL + ARROWS
#define CT_LEFT LCTL(KC_LEFT)
#define CT_DOWN LCTL(KC_DOWN)
#define CT_UP LCTL(KC_UP)
#define CT_RGHT LCTL(KC_RGHT)

// THUMB KEY LAYER TAPS
#define TB_TAB LT(_MEDIA, KC_TAB)
#define TB_ENT LT(_NAVIGATION, KC_ENT)
#define TB_SPC LT(_NUMBERS, KC_SPC)
#define TB_BSPC LT(_FUNCTION, KC_BSPC)

// POINTING DEVICE MAGIC
#define PD_RCSCR LT(0, KC_BTN2)

// vertical combos for umlauts
const uint16_t PROGMEM ae_combo[] = {KC_Q, HM_A, COMBO_END};
const uint16_t PROGMEM ss_combo[] = {KC_W, HM_S, COMBO_END};
const uint16_t PROGMEM ue_combo[] = {KC_U, HM_J, COMBO_END};
const uint16_t PROGMEM oe_combo[] = {KC_O, HM_L, COMBO_END};
// horizontal combos for mods
const uint16_t PROGMEM del_combo[] = {HM_H, HM_J, COMBO_END};
const uint16_t PROGMEM bsp_combo[] = {HM_J, HM_K, COMBO_END};
const uint16_t PROGMEM ent_combo[] = {HM_K, HM_L, COMBO_END};
const uint16_t PROGMEM tab_combo[] = {HM_F, HM_D, COMBO_END};
const uint16_t PROGMEM esc_combo[] = {HM_D, HM_S, COMBO_END};

combo_t key_combos[COMBO_COUNT] = {
    COMBO(ae_combo, RALT(KC_Q)),
    COMBO(ss_combo, RALT(KC_S)),
    COMBO(ue_combo, RALT(KC_Y)),
    COMBO(oe_combo, RALT(KC_P)),
    COMBO(del_combo, KC_DEL),
    COMBO(bsp_combo, KC_BSPC),
    COMBO(ent_combo, KC_ENT),
    COMBO(tab_combo, KC_TAB),
    COMBO(esc_combo, KC_ESC)
};

// KEYMAPS 
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_QWERTY] = LAYOUT_2_3x5_3(
    KC_NO,    KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,                                 KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_NO,
    KC_Q,     HM_A,     HM_S,     HM_D,     HM_F,     HM_G,                                 HM_H,     HM_J,     HM_K,     HM_L,     HM_SCLN,  KC_P,
              KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,                                 KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,
                                            KC_ESC,   TB_TAB,   TB_ENT,           TB_SPC,   TB_BSPC,  KC_DEL,
                                                                KC_BTN3, KC_BTN1, PD_RCSCR,
                                                    //click   ,  right , down   , left   , up
                                                      ACC_TOG , KC_VOLU, SNP_TOG, KC_VOLD, DRG_TOG
    ),

    [_COLEMAK] = LAYOUT_2_3x5_3(
    KC_NO,    KC_Q,     KC_W,     KC_F,     KC_P,     KC_B,                                 KC_J,     KC_L,     KC_U,     KC_Y,    KC_SCLN,   KC_NO,
    KC_Q,     CM_A,     CM_R,     CM_S,     CM_T,     CM_G,                                 CM_M,     CM_N,     CM_E,     CM_I,    CM_O,      KC_SCLN, 
              KC_Z,     KC_X,     KC_C,     KC_D,     KC_V,                                 KC_K,     KC_H,     KC_COMM,  KC_DOT,  KC_SLASH,
                                            KC_ESC,   TB_TAB,   TB_ENT,           TB_SPC,   TB_BSPC,  KC_DEL,
                                                                KC_BTN3, KC_BTN1, PD_RCSCR,
                                                    //click   ,  right , down   , left   , up
                                                      ACC_TOG , KC_VOLU, SNP_TOG, KC_VOLD, DRG_TOG
    ),

    [_NAVIGATION] = LAYOUT_2_3x5_3(
    _______,  _______,  _______,  _______,  _______,  KC_LCBR,                              KC_RCBR,  CT_LEFT,  CT_DOWN,  CT_UP,    CT_RGHT,  _______,
    _______,  KC_LGUI,  KC_LALT,  KC_LCTL,  KC_LSFT,  KC_LPRN,                              KC_RPRN,  KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  _______,
              _______,  _______,  _______,  _______,  KC_LBRC,                              KC_RBRC,  KC_HOME,  KC_PGDN,  KC_PGUP,  KC_END,
                                            _______,  _______,  _______,          _______,  _______,  _______,
                                                                _______, _______, _______,
                                                    //click  ,  right , down    , left   , up
                                                      _______, _______, DPI_RMOD, _______, DPI_MOD
    ),
    [_MEDIA] = LAYOUT_2_3x5_3(
    _______,  KC_UNDS,  KC_AMPR,  KC_ASTR,  KC_LPRN,  KC_RPRN,                              RGB_TOG,  RGB_MOD,  RGB_HUI,  RGB_SAI,  RGB_VAI,  _______,
    _______,  KC_PLUS,  KC_DLR,   KC_PERC,  KC_CIRC,  KC_DQUO,                              RGB_M_P,  RGB_RMOD, RGB_HUD,  RGB_SAD,  RGB_VAD,  _______,
              KC_TILD,  KC_EXLM,  KC_AT,    KC_HASH,  KC_PIPE,                              QWERTY,   COLEMAK,  _______,  _______,  QK_BOOTLOADER,
                                            _______,  _______,  _______,          _______,  _______,  _______,
                                                                _______, _______, _______,
                                                    //click  ,  right , down    , left   , up
                                                      RGB_TOG, RGB_VAI, RGB_RMOD, RGB_VAD, RGB_MOD
    ),

    [_NUMBERS] = LAYOUT_2_3x5_3(
    _______,  KC_MINS,  KC_7,     KC_8,     KC_9,     KC_0,                                 _______,  KC_MPRV,  KC_VOLD,  KC_VOLU,  KC_MNXT,  _______, 
    _______,  KC_EQL,   KC_4,     KC_5,     KC_6,     KC_QUOT,                              KC_RALT,  KC_RSFT,  KC_LCTL,  KC_LALT,  KC_LGUI,  _______, 
              KC_GRV,   KC_1,     KC_2,     KC_3,     KC_BSLS,                              _______,  _______,  _______,  _______,  _______,
                                            _______,  _______,  _______,          _______,  _______,  _______,
                                                                _______, _______, _______,
                                                    //click  ,  right , down    , left   , up
                                                      RGB_TOG, RGB_VAI, RGB_RMOD, RGB_VAD, RGB_MOD
    ),

    [_FUNCTION] = LAYOUT_2_3x5_3(
    _______,  KC_F12,   KC_F7,    KC_F8,    KC_F9,    KC_PSCR,                              _______,  _______,  _______,  _______,  _______,  _______, 
    _______,  KC_F11,   KC_F4,    KC_F5,    KC_F6,    _______,                              KC_RALT,  KC_RSFT,  KC_LCTL,  KC_LALT,  KC_LGUI,   _______, 
              KC_F10,   KC_F1,    KC_F2,    KC_F3,    KC_INS,                               _______,  _______,  _______,  _______,  _______,
                                            _______,  _______,  _______,          _______,  _______,  _______,
                                                                _______, _______, _______,
                                                    //click  ,  right , down    , left   , up
                                                      RGB_TOG, RGB_VAI, RGB_RMOD, RGB_VAD, RGB_MOD
    ),
};

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // case SHT_T:
        //     return TAPPING_TERM - 150; // Recommended
        // case SHT_N:
        //     return TAPPING_TERM - 150; // Recommended
        default:
            return TAPPING_TERM;
    }
}

bool caps_word_press_user(uint16_t keycode) {
  switch (keycode) {
    // Keycodes that continue Caps Word, with shift applied.
    case KC_A ... KC_Z:
      add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to the next key.
      return true;

    // Keycodes that continue Caps Word, without shifting.
    case KC_1 ... KC_0:
    case KC_BSPC:
    case KC_DEL:
    case KC_MINS:
    case KC_UNDS:
    case KC_LSFT:
    case KC_RSFT:
      return true;

    default:
      return false;  // Deactivate Caps Word.
  }
}


bool sw_win_active = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // mab_process_record_haptic(keycode, record);
    mab_process_pointing_keys(keycode, record);

    // if (keymap_config.swap_lctl_lgui) {
    //         update_swapper(
    //             &sw_win_active, KC_LGUI, KC_TAB, SW_WIN,
    //             keycode, record
    //         );
    // } else {
    //         update_swapper(
    //             &sw_win_active, KC_LALT, KC_TAB, SW_WIN,
    //             keycode, record
    //         );
    // }

    switch (keycode) {

        case COLEMAK:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_COLEMAK);
            }
            return false;
        case QWERTY:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_QWERTY);
            }
            return false;
        case LT(0, KC_BTN2):
            if (!record->tap.count && record->event.pressed) {
                // Intercept hold function to send DRAGSCROLL_MODE_TOGGLE
                if (mab_get_pointer_sniping_enabled()) {
                  mab_set_pointer_sniping_enabled(false);
                  #ifdef QUANTUM_PAINTER_ENABLE
                    sniping_set_user(false);
                  #endif
                }
                mab_set_pointer_dragscroll_enabled(!mab_get_pointer_dragscroll_enabled());
                #ifdef QUANTUM_PAINTER_ENABLE
                  dragscroll_set_user(mab_get_pointer_dragscroll_enabled());
                #endif
                return false;
            }
            return true; 
        default:
            return true;
    }
    return true;
}

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_QWERTY]  =  { ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN)},
    [_COLEMAK]  =  { ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN)},
    [_NAVIGATION]  =  { ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN)},
    [_MEDIA]  =  { ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN)},
    [_NUMBERS]  =  { ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN)},
    [_FUNCTION]  =  { ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN)},
};
