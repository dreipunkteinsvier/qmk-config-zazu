#pragma once
#include "quantum.h"

uint16_t mab_get_pointer_default_dpi(void);

void mab_cycle_pointer_default_dpi(bool forward);

uint16_t mab_get_pointer_sniping_dpi(void);
bool mab_get_pointer_sniping_enabled(void);
void mab_set_pointer_sniping_enabled(bool enable);

bool mab_get_pointer_dragscroll_enabled(void);
void mab_set_pointer_dragscroll_enabled(bool enable);

bool mab_get_pointer_acceleration_enabled(void);
void mab_set_pointer_acceleration_enabled(bool enable);

bool mab_process_pointing_keys(uint16_t keycode, keyrecord_t *record);

void mab_set_pointer(void);
