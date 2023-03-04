#include "mab_pointing.h"
#include "keys.h"

#ifdef QUANTUM_PAINTER_ENABLE
  #include "lvgl.h"
  #include "display.h"
#endif

#define MAB_SNIPING_DPI_CONFIG_STEP 100
#define MAB_DRAGSCROLL_DPI 100
#define MAB_DRAGSCROLL_BUFFER_SIZE 6
#define MAB_DEFAULT_DPI_CONFIG_STEP 100
#define MAB_DEFAULT_DPI 400
#define MAB_MINIMUM_SNIPING_DPI 100

typedef union {
    uint8_t raw;
    struct {
        uint8_t pointer_default_dpi : 4; // 16 steps available.
        uint8_t pointer_sniping_dpi : 2; // 4 steps available.
        bool    is_dragscroll_enabled : 1;
        bool    is_sniping_enabled : 1;
        bool    is_acceleration_enabled: 1;
    } __attribute__((packed));
} mab_config_t;

static mab_config_t g_mab_config = {0};

static uint16_t get_pointer_default_dpi(mab_config_t* config) {
    return (uint16_t)config->pointer_default_dpi * MAB_DEFAULT_DPI_CONFIG_STEP + MAB_DEFAULT_DPI;
}

static uint16_t get_pointer_sniping_dpi(mab_config_t* config) {
    return (uint16_t)config->pointer_sniping_dpi * MAB_SNIPING_DPI_CONFIG_STEP + MAB_MINIMUM_SNIPING_DPI;
}

static void write_mab_config_to_eeprom(mab_config_t* config) {
    eeconfig_update_kb(config->raw);
}

/** \brief Set the appropriate DPI for the input config. */
static void maybe_update_pointing_device_cpi(mab_config_t* config) {
    if (config->is_dragscroll_enabled) {
        pointing_device_set_cpi(MAB_DRAGSCROLL_DPI);
    } else if (config->is_sniping_enabled) {
        pointing_device_set_cpi(get_pointer_sniping_dpi(config));
    } else {
        pointing_device_set_cpi(get_pointer_default_dpi(config));
    }
}

/**
 * \brief Update the pointer's default DPI to the next or previous step.
 *
 * Increases the DPI value if `forward` is `true`, decreases it otherwise.
 * The increment/decrement steps are equal to MAB_DEFAULT_DPI_CONFIG_STEP.
 */
static void step_pointer_default_dpi(mab_config_t* config, bool forward) {
    config->pointer_default_dpi += forward ? 1 : -1;
    maybe_update_pointing_device_cpi(config);
}

void mab_cycle_pointer_default_dpi(bool forward) {
    step_pointer_default_dpi(&g_mab_config, forward);
    write_mab_config_to_eeprom(&g_mab_config);
}

uint16_t mab_get_pointer_default_dpi(void) {
    return get_pointer_default_dpi(&g_mab_config);
}

uint16_t mab_get_pointer_sniping_dpi(void) {
    return get_pointer_sniping_dpi(&g_mab_config);
}

bool mab_get_pointer_sniping_enabled(void) {
    return g_mab_config.is_sniping_enabled;
}

void mab_set_pointer_sniping_enabled(bool enable) {
    g_mab_config.is_sniping_enabled = enable;
    maybe_update_pointing_device_cpi(&g_mab_config);
}

bool mab_get_pointer_dragscroll_enabled(void) {
    return g_mab_config.is_dragscroll_enabled;
}

void mab_set_pointer_dragscroll_enabled(bool enable) {
    g_mab_config.is_dragscroll_enabled = enable;
    maybe_update_pointing_device_cpi(&g_mab_config);
}

bool mab_get_pointer_acceleration_enabled(void) {
    return g_mab_config.is_acceleration_enabled;
}

void mab_set_pointer_acceleration_enabled(bool enable) {
    g_mab_config.is_acceleration_enabled = enable;
    maybe_update_pointing_device_cpi(&g_mab_config);
}

static void pointing_device_task_mab(report_mouse_t* mouse_report) {
    static int16_t scroll_buffer_x = 0;
    static int16_t scroll_buffer_y = 0;
    if (g_mab_config.is_dragscroll_enabled) {
        scroll_buffer_x += mouse_report->x;
        scroll_buffer_y -= mouse_report->y;
        mouse_report->x = 0;
        mouse_report->y = 0;
        if (abs(scroll_buffer_x) > MAB_DRAGSCROLL_BUFFER_SIZE) {
            mouse_report->h = scroll_buffer_x > 0 ? 1 : -1;
            scroll_buffer_x = 0;
        }
        if (abs(scroll_buffer_y) > MAB_DRAGSCROLL_BUFFER_SIZE) {
            mouse_report->v = scroll_buffer_y > 0 ? 1 : -1;
            scroll_buffer_y = 0;
        }
    }
    if (g_mab_config.is_acceleration_enabled && !g_mab_config.is_dragscroll_enabled && !g_mab_config.is_sniping_enabled) {
        mouse_xy_report_t x = mouse_report->x, y = mouse_report->y;
        mouse_report->x = 0;
        mouse_report->y = 0;

        x = (mouse_xy_report_t)(x > 0 ? x * x / 16 + x : -x * x / 16 + x);
        y = (mouse_xy_report_t)(y > 0 ? y * y / 16 + y : -y * y / 16 + y);
        mouse_report->x = x;
        mouse_report->y = y;
    }
}

report_mouse_t pointing_device_task_kb(report_mouse_t mouse_report) {
    pointing_device_task_mab(&mouse_report);
    mouse_report = pointing_device_task_user(mouse_report);
    return mouse_report;
}

void pointing_device_init_kb(void) {
    maybe_update_pointing_device_cpi(&g_mab_config);
    pointing_device_init_user();
}

void eeconfig_init_kb(void) {
    g_mab_config.raw                 = 0;
    g_mab_config.pointer_default_dpi = 0;
    write_mab_config_to_eeprom(&g_mab_config);
    maybe_update_pointing_device_cpi(&g_mab_config);
    eeconfig_init_user();
}

static bool has_shift_mod(void) {
    return mod_config(get_mods() | get_oneshot_mods()) & MOD_MASK_SHIFT;
}

bool mab_process_pointing_keys(uint16_t keycode, keyrecord_t *record) {
    switch(keycode) {
        case POINTER_DEFAULT_DPI_FORWARD:
            if (record->event.pressed) {
                // Step backward if shifted, forward otherwise.
                mab_cycle_pointer_default_dpi(/* forward= */ !has_shift_mod());
            }
            break;
        case POINTER_DEFAULT_DPI_REVERSE:
            if (record->event.pressed) {
                // Step backward if shifted, forward otherwise.
                mab_cycle_pointer_default_dpi(/* forward= */ has_shift_mod());
            }
            break;
        case SNIPING_MODE:
            if (mab_get_pointer_dragscroll_enabled()) {
              mab_set_pointer_dragscroll_enabled(false);
              #ifdef QUANTUM_PAINTER_ENABLE
                dragscroll_set_user(false);
              #endif
            }
            mab_set_pointer_sniping_enabled(record->event.pressed);
            #ifdef QUANTUM_PAINTER_ENABLE
              sniping_set_user(record->event.pressed);
            #endif
            break;
        case SNIPING_MODE_TOGGLE:
            if (record->event.pressed) {
                if (mab_get_pointer_dragscroll_enabled()) {
                  mab_set_pointer_dragscroll_enabled(false);
                  #ifdef QUANTUM_PAINTER_ENABLE
                    dragscroll_set_user(false);
                  #endif
                }
                mab_set_pointer_sniping_enabled(!mab_get_pointer_sniping_enabled());
                #ifdef QUANTUM_PAINTER_ENABLE
                  sniping_set_user(mab_get_pointer_sniping_enabled());
                #endif
            }
            break;
        case DRAGSCROLL_MODE:
            if (mab_get_pointer_sniping_enabled()) {
              mab_set_pointer_sniping_enabled(false);
              #ifdef QUANTUM_PAINTER_ENABLE
                sniping_set_user(false);
              #endif
            }
            mab_set_pointer_dragscroll_enabled(record->event.pressed);
            #ifdef QUANTUM_PAINTER_ENABLE
              dragscroll_set_user(record->event.pressed);
            #endif
            break;
        case DRAGSCROLL_MODE_TOGGLE:
            if (record->event.pressed) {
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
            }
            break;
        case ACCELERATION_TOGGLE:
            if (record->event.pressed) {
                mab_set_pointer_acceleration_enabled(!mab_get_pointer_acceleration_enabled());
            }
            break;
        default:
            break;
    }

    return true;
}

void mab_set_pointer() {
  if (keymap_config.swap_lctl_lgui) {
    g_mab_config.pointer_default_dpi  = 2;
    maybe_update_pointing_device_cpi(&g_mab_config);
  } else {
    g_mab_config.pointer_default_dpi  = 1;
    maybe_update_pointing_device_cpi(&g_mab_config);
  }
}
