#if defined __has_include
#if __has_include("lvgl.h")
#include "lvgl.h"
#elif __has_include("lvgl/lvgl.h")
#include "lvgl/lvgl.h"
#else
#include "lvgl.h"
#endif
#else
#include "lvgl.h"
#endif

#include <stdint.h>

LV_FONT_DECLARE(rb_24);
LV_FONT_DECLARE(rb_18);
LV_IMG_DECLARE(window);
LV_IMG_DECLARE(apple);
LV_IMG_DECLARE(hash);
LV_IMG_DECLARE(tools);
LV_IMG_DECLARE(cmd);
LV_IMG_DECLARE(shift);
LV_IMG_DECLARE(caret);
LV_IMG_DECLARE(alt);

uint8_t USER_EVENT_CPI_UPDATE;
uint8_t USER_EVENT_ACTIVE_LAYER_CHANGE;
uint8_t USER_EVENT_CAPS_WORD_UPDATE;
uint8_t USER_EVENT_DRAGSCROLL_UPDATE;
uint8_t USER_EVENT_ACC_UPDATE;
uint8_t USER_EVENT_RGBMODE_UPDATE;
uint8_t USER_EVENT_ALTMOD;
uint8_t USER_EVENT_CMDMOD;
uint8_t USER_EVENT_SHIFTMOD;
uint8_t USER_EVENT_CTRLMOD;

extern lv_obj_t * mbox1;
extern lv_obj_t * mbox1_title;
extern lv_obj_t * mbox1_text;
extern lv_obj_t * ui_Screen1;
extern lv_obj_t * ui_Screen1_Label_CPI;
extern lv_obj_t * ui_Screen1_deflayer;
extern lv_obj_t * ui_Screen1_Label_ACC;
extern lv_obj_t * ui_Screen1_Label_RGB;
extern lv_obj_t * ui_Screen1_Label_ALTMOD;
extern lv_obj_t * ui_Screen1_Label_CMDMOD;
extern lv_obj_t * ui_Screen1_Label_SHIFTMOD;
extern lv_obj_t * ui_Screen1_Label_CTRLMOD;
extern lv_obj_t * ui_Layer_Indicator;
extern lv_obj_t * ui_Screen1_deflayer_list;

void ui_event_dflayer_dropdown(lv_event_t * e);
void ui_df_layer_change(lv_event_t * e);
void ui_event_dflayer_dropdown(lv_event_t * e);

void set_cpi_text_value(lv_obj_t* lbl);
void ui_render_cpi(lv_event_t * e);

void qmk_lv_set_layer(uint8_t layer);
uint8_t qmk_lv_active_layer(void);
uint16_t qmk_lv_get_cpi(void);
void ui_active_layer_change(lv_event_t * e);
void ui_render_msg_box(lv_event_t * e);

void dragscroll_set_user(bool active);
void sniping_set_user(bool active);

void lvgl_event_triggers(void);

void display_init(void);
