#include QMK_KEYBOARD_H

extern keymap_config_t keymap_config;

#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
#endif

#ifdef OLED_ENABLE
static uint32_t oled_timer = 0;
#endif


//NOTE
//To flash for Corne R2G use:
//qmk flash -kb crkbd/r2g -km clarity


////for fix on computer cold start to work - possibly remove once integrated into qmk
// add to your keymap.c
#include <avr/wdt.h>
#include "transactions.h"

static bool watchdog_ping_done = false;
static uint32_t watchdog_timer = 0;

void watchdog_slave_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) { watchdog_ping_done = true; }

void keyboard_post_init_kb(void) {
    wdt_disable();
    keyboard_post_init_user();
    transaction_register_rpc(WATCHDOG_SYNC, watchdog_slave_handler);
    watchdog_timer = timer_read32();
}

void housekeeping_task_kb(void) {
    if (!watchdog_ping_done) {
        if (is_keyboard_master()) {
            if (timer_elapsed32(watchdog_timer) > 100) {
                uint8_t any_data = 1;
                if (transaction_rpc_send(WATCHDOG_SYNC, sizeof(any_data), &any_data)) {
                    watchdog_ping_done = true;  // successful ping
                } else {
                    dprint("Watchdog ping failed!\n");
                }
                watchdog_timer = timer_read32();
            }
        } else {
            if (timer_elapsed32(watchdog_timer) > SPLIT_USB_TIMEOUT + 1000) {
                wdt_enable(WDTO_250MS);
                while (1) {
                }
            }
        }
    }
}
////end of fix







// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum layers {
  _CMAK,
  _QWERTY,
  _OUTER_L,
  _OUTER_L_LK,
  _OUTER_R,
  _OUTER_R_LK,
  //_OUTER_R_LK_MAC,
  _INNER_L,
  _INNER_L1,
  _INNER_L2,
  _INNER_R,
  _INNER_R1,
  _INNER_R2,
  _OUTER_ADJ,
  _STINGS,
  _GAME1,
  _GAME2
};

//home row modifiers
// Left-hand home row mods
#define HM_A LALT_T(KC_A)
#define HM_S LGUI_T(KC_S)
#define HM_D LCTL_T(KC_D)
#define HM_F LSFT_T(KC_F)

// Left-hand home row mods Colemak
#define CM_A LALT_T(KC_A)
#define CM_R LGUI_T(KC_R)
#define CM_S LCTL_T(KC_S)
#define CM_T LSFT_T(KC_T)


// Right-hand home row mods
#define HM_J RSFT_T(KC_J)
#define HM_K RCTL_T(KC_K)
#define HM_L RGUI_T(KC_L)
//Note use alt modifier as P or semicolon depending on what you put there
//use lalt instead of actual ralt because ralt does unexpected things on some languages
#define HM_SCLN LALT_T(KC_SCLN)
#define HM_P LALT_T(KC_P)

// Left-hand home row mods Colemak
#define CM_N RSFT_T(KC_N)
#define CM_E RCTL_T(KC_E)
#define CM_I RGUI_T(KC_I)
#define CM_O LALT_T(KC_O)


// one tap for additional layers
#define OT_LALT OSM(MOD_LALT)
#define OT_LGUI OSM(MOD_LGUI)
#define OT_LCTL OSM(MOD_LCTL)
#define OT_LSFT OSM(MOD_LSFT)

//#define OT_RALT OSM(MOD_RALT)
#define OT_RGUI OSM(MOD_RGUI)
#define OT_RCTL OSM(MOD_RCTL)
#define OT_RSFT OSM(MOD_RSFT)



// Custom keycodes for layer keys
// Dual function escape with left command
#define KC_LGESC LGUI_T(KC_ESC)

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  CMAK,
  OUTER_L,
  OUTER_L_LK,
  OUTER_R,
  OUTER_R_LK,
  //OUTER_R_LK_MAC,
  INNER_L,
  INNER_L1,
  INNER_L2,
  INNER_R,
  INNER_R1,
  INNER_R2,
  OUTER_ADJ,
  STINGS,
  GAME1,
  GAME2,
  RGBRST,
  HLD_BTN1,
  KC_RACL, // right alt / colon
  TM_C,
  TM_AMPR,
  TM_P,
  TM_N,
  TM_S,
  TM_V,
  TM_X,
  TM_SPC,
  TM_MINS,
  TM_PLUS,
  TM_LT,
  TM_GT,
  TM_ON,
  TM_SCON
};

//NOTE the rare weird space keypress on trying to trigger left thumb, minus and quote, on OUTER_R
//Is because outer R key is backspace, so pressing it once and then holding, triggers repeated backspace
//instead of a layer change, so end up pressing space and tab with left thumbs - it's not really a
//problem, just wanted to get to the bottom of it.

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT_split_3x6_3(
  //,-----------------------------------------.                ,---------------------------------------------.
     KC_NO, KC_Q,  KC_W,  KC_E,  KC_R,  KC_T,                   KC_Y,  KC_U,  KC_I,  KC_O,  KC_P,   KC_NO,
  //|------+------+------+------+------+------|                |------+------+-------+------+-------+--------|
     KC_Q,     HM_A,  HM_S,  HM_D,  HM_F,  KC_G,                KC_H,  HM_J,  HM_K,  HM_L,  HM_P, KC_SCLN,
  //|------+------+------+------+------+------|                |------+------+-------+------+-------+--------|
     KC_NO,  KC_Z,  KC_X,  KC_C,  KC_V,  KC_B,                  KC_N,  KC_M,KC_COMM,KC_DOT,KC_SLSH, KC_NO,
  //|------+------+------+------+------+------+------|  |------+------+------+-------+------+-------+--------|
     KC_NO, LT(_OUTER_L,KC_SPC), LT(_INNER_L,KC_TAB),  LT(_INNER_R,KC_ENT), LT(_OUTER_R,KC_BSPC), KC_NO
                              //`--------------------'  `--------------------'
  ),

  [_CMAK] = LAYOUT_split_3x6_3(
  //,-----------------------------------------.                ,---------------------------------------------.
         KC_Q,  KC_W,  KC_F,  KC_P,  KC_B,                KC_J,  KC_L,  KC_U,  KC_Y,  KC_SCLN,
  //|+------+------+------+------+------|                |------+------+-------+------+----------|
         CM_A,  CM_R,  CM_S,  CM_T,  KC_G,                KC_M,  CM_N,  CM_E,  CM_I,  CM_O,
  //|+------+------+------+------+------|                |------+------+-------+------+----------|
         KC_Z,  KC_X,  KC_C,  KC_D,  KC_V,                KC_K,  KC_H,KC_COMM,KC_DOT,KC_SLSH,
  //|+------+------+------+------+------+------|  |------+------+------+-------+------+----------|
      LT(_OUTER_L,KC_SPC), LT(_INNER_L,KC_TAB),  LT(_INNER_R,KC_ENT), LT(_OUTER_R,KC_BSPC),
                              //`--------------------'  `--------------------'
  ),


  [_OUTER_L] = LAYOUT_split_3x6_3(
  //,---------------------------------------------.                ,-----------------------------------------.
      KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,                   KC_NO, KC_DEL,KC_CAPS,KC_INS, KC_PSCR,
  //|+------+-------+-------+-------+-------|                |------+------+------+------+------+-
      KC_LALT, KC_LGUI, KC_LCTL, KC_LSFT, KC_NO,           KC_NO, KC_LEFT,KC_DOWN,KC_UP,KC_RIGHT,
  //|+------+-------+-------+-------+-------|                |------+------+------+------+------+-
      KC_NO, KC_NO, KC_NO, TG(_OUTER_L_LK), KC_NO,         KC_NO, KC_HOME,KC_PGDN,KC_PGUP,KC_END,
  //|------+------+-------+-------+-------+-------+------|  |------+------+------+------+------+------+------|
                                KC_TRNS, KC_TRNS,        KC_TILD, LT(_OUTER_R,KC_PAST)
                                  //`--------------------'  `--------------------'
  ),


  //can't use KC_TRNS since by letting go of the lower layer mod key, the KC_TRNS will refer to the base layer
  [_OUTER_L_LK] = LAYOUT_split_3x6_3(
  //,---------------------------------------------.                ,-----------------------------------------.
      KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,                   KC_NO, KC_DEL,KC_CAPS,KC_INS, KC_PSCR,
  //|+------+-------+-------+-------+-------|                |------+------+------+------+------+
      KC_LALT, KC_LGUI, KC_LCTL, KC_LSFT, KC_NO,           KC_NO, KC_LEFT,KC_DOWN,KC_UP,KC_RIGHT,
  //|+------+-------+-------+-------+-------|                |------+------+------+------+------+
      KC_NO, KC_NO, KC_NO, TG(_OUTER_L_LK), KC_NO,         KC_NO, KC_HOME,KC_PGDN,KC_PGUP,KC_END,
  //|------+------+-------+-------+-------+-------+------|  |------+------+------+------+------+------+------|
                                        KC_NO, KC_NO,        KC_NO, TO(0)
                                  //`--------------------'  `--------------------'
  ),


  [_OUTER_R] = LAYOUT_split_3x6_3(
  //,---------------------------------------------.                ,-----------------------------------------.
     KC_BTN2, KC_BTN3, HLD_BTN1, KC_BTN1, KC_V,       KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,
  //-+------+-------+-------+-------+-------|                |------+------+------+------+-------|
     KC_MS_L, KC_MS_U, KC_MS_D, KC_MS_R, KC_C,        KC_NO,KC_RSFT,KC_RCTL,KC_RGUI, KC_LALT,
  //-+------+-------+-------+-------+-------|                |------+------+------+------+-------|
     KC_WH_L, KC_WH_U, KC_WH_D, KC_WH_R, KC_X,        KC_NO,TG(_OUTER_R_LK),KC_NO,KC_NO, KC_NO,
  //|------+------+-------+-------+-------+-------+------|  |------+------+------+------+------+------+------|
                        LT(_OUTER_L,KC_ESC), KC_COLN,           KC_TRNS, KC_TRNS
                                  //`--------------------'  `--------------------'
  ),


  //can't use KC_TRNS since by letting go of the lower layer mod key, the KC_TRNS will refer to the base layer
  [_OUTER_R_LK] = LAYOUT_split_3x6_3(
  //,---------------------------------------------.                ,-----------------------------------------.
    KC_BTN2, KC_BTN3, HLD_BTN1, KC_BTN1, KC_V,        KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,
  //-+------+-------+-------+-------+-------|                |------+------+------+------+-------|
     KC_MS_L, KC_MS_U, KC_MS_D, KC_MS_R, KC_C,        KC_NO,KC_RSFT,KC_RCTL,KC_RGUI, KC_LALT,
  //-+------+-------+-------+-------+-------|                |------+------+------+------+-------|
     KC_WH_L, KC_WH_U, KC_WH_D, KC_WH_R, KC_X,        KC_NO,TG(_OUTER_R_LK),KC_NO,KC_NO, KC_NO,
  //|------+------+-------+-------+-------+-------+------|  |------+------+------+------+------+------+------|
                                    KC_NO, KC_NO,        KC_NO, TO(0)
                                  //`--------------------'  `--------------------'
  ),


  [_INNER_L] = LAYOUT_split_3x6_3(
  //,---------------------------------------------.                ,-----------------------------------------.
      KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,               KC_AT,  KC_HASH,KC_LCBR, KC_RCBR, KC_GRAVE,
  //|+-------+----+------+------+-------|           +------+--+------------+----+------+------++------|
      KC_LALT, KC_LGUI, KC_LCTL, KC_LSFT, KC_NO,   KC_EXLM,KC_DQUO,KC_LPRN, KC_RPRN, KC_UNDS,
  //|+-------+----+------+------+-------|           +------+--+------------+----+------+------++------|
      KC_NO,KC_NO,KC_NO,KC_NO,KC_NO,               KC_AMPR,KC_QUOT,KC_LBRC, KC_RBRC, KC_MINS,
  //|------+------+-------+-------+-------+-------+------|  |------+------+------+------+------+------+------|
                                        KC_TRNS, KC_TRNS,     KC_NO, KC_NO,
                                  //`--------------------'  `--------------------'
  ),


  [_INNER_R] = LAYOUT_split_3x6_3(
  //,---------------------------------------------.                ,-----------------------------------------.
     KC_GRAVE, KC_1, KC_2, KC_3, KC_COMM,                 KC_NO,  KC_NO,  KC_NO,  KC_NO,  KC_NO,
  //--+------+------+------+------+----------|                |------+------+------+------+------+-
     KC_SPC,  KC_4, KC_5, KC_6, KC_0,                     KC_NO, KC_RSFT,KC_RCTL,KC_RGUI, KC_LALT,
  //--+------+------+------+------+----------|                |------+------+------+------+------+-
     KC_BSLS, KC_7, KC_8, KC_9, KC_DOT,                   KC_NO,KC_NO,KC_NO,KC_NO, KC_NO,
  //|------+------+-------+-------+-------+-------+------|  |------+------+------+------+------+------+------|
                        OSL(_INNER_R2), OSL(_INNER_R1),       KC_TRNS, KC_TRNS
                                  //`--------------------'  `--------------------'
  ),


  [_INNER_R1] = LAYOUT_split_3x6_3(
  //,---------------------------------------------.        ,-----------------------------------------.
      KC_F1, KC_F2, KC_F3, KC_F4,KC_NUM,             KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,
  //|+-----+-------+-------+----+------|             +------+-------+-------+-------+---------
      KC_F5, KC_F6, KC_F7, KC_F8,KC_PAUS,            KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,
  //|+-----+-------+-------+----+------|             +------+-------+-------+-------+---------
      KC_F9, KC_F10,KC_F11,KC_F12,KC_SCRL,            KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,
  //|------+------+-------+-------+-------+-------+------|  |------+------+------+------+------+------+------|
                                    KC_TRNS, KC_TRNS,         KC_TRNS, TO(0)
                                  //`--------------------'  `--------------------'
  ),


  [_INNER_R2] = LAYOUT_split_3x6_3(
  //,---------------------------------------------.                ,-----------------------------------------.
      KC_CIRC, KC_PERC, KC_DLR, KC_HASH, KC_AT,            KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,
  //|+------+-------+-------+-------+-------|              +------+-------+-------+-------+--------
      KC_ASTR, KC_MINS, KC_PLUS, KC_EQL, KC_EXLM,          KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,
  //|+------+-------+-------+-------+-------|              +------+-------+-------+-------+--------
      KC_SLSH, KC_PIPE, KC_LT, KC_GT, KC_AMPR,             KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,KC_TRNS,
  //|------+------+-------+-------+-------+-------+------|  |------+------+------+------+------+------+------|
                                    KC_TRNS, KC_TRNS,     KC_TRNS, TO(0)
                                  //`--------------------'  `--------------------'
  ),


  [_OUTER_ADJ] = LAYOUT_split_3x6_3(
  //,-----------------------------------------.                ,-----------------------------------------.
    TG(_STINGS),TG(_GAME1),DF(_QWERTY),DF(_CMAK),KC_NO,  KC_NO, TM_SPC, TM_S, TM_V, TM_X,
  //-+------+------+------+------+------|                |------+------+------+------+------+------|
     KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,                  TM_PLUS, TM_ON, TM_SCON, TM_C, TM_AMPR,
  //-+------+------+------+------+------|                |------+------+------+------+------+------|
     KC_MUTE, KC_VOLD, KC_VOLU, KC_MPLY, KC_NO,          TM_MINS, TM_LT, TM_P, TM_N, TM_GT,
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                        KC_TRNS, KC_NO,  KC_NO, KC_TRNS
                              //`--------------------'  `--------------------'
  ),


  [_STINGS] = LAYOUT_split_3x6_3(
  //,-----------------------------------------.                ,-----------------------------------------.
     RESET,RGBRST, KC_NO, KC_NO, KC_NO,                     KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
  //-+------+------+------+------+------|                |------+------+------+------+------+--
     RGB_TOG,RGB_HUI,RGB_SAI,RGB_VAI,RGB_SPI,               KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
  //-+------+------+------+------+------|                |------+------+------+------+------+------|
     RGB_MOD,RGB_HUD,RGB_SAD,RGB_VAD,RGB_SPD,               KC_NO, KC_NO, KC_NO, KC_NO, RGB_RMOD,
  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                        KC_NO, KC_NO,    KC_NO, TO(0)
                              //`--------------------'  `--------------------'
  ),


  [_GAME1] = LAYOUT_split_3x6_3(
  //,-----------------------------------------.                ,---------------------------------------------.
      KC_Q,  KC_W,  KC_E,  KC_R,  KC_T,                KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
  //|+------+------+------+------+------|                |------+------+-------+------+---------|
      KC_A,  KC_S,  KC_D,  KC_F,  KC_G,                KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
  //|+------+------+------+------+------|                |------+------+-------+------+---------|
      KC_Z,  KC_X,  KC_C,  KC_V,  KC_B,               KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
  //|------+------+------+------+------+------+------|  |------+------+------+-------+------+-------+--------|
                                KC_LCTL, TT(_GAME2),  KC_NO, TO(0)
                              //`--------------------'  `--------------------'
  ),


  [_GAME2] = LAYOUT_split_3x6_3(
  //,-----------------------------------------.                ,---------------------------------------------.
      KC_NO,  KC_1,  KC_2,  KC_3,  KC_NO,                KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
  //|+------+------+------+------+------|                |------+------+-------+------+---------|
      KC_NO,  KC_4,  KC_5,  KC_6,  KC_0,                 KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
  //|+------+------+------+------+------|                |------+------+-------+------+---------|
      KC_NO, KC_7,  KC_8,  KC_9,  KC_NO,                KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
  //|------+------+------+------+------+------+------|  |------+------+------+-------+------+-------+--------|
                                    KC_LCTL, KC_TRNS,    KC_NO, TO(0),
                              //`--------------------'  `--------------------'
  )


};



//NOTE - to flash my keyboard since my corne has Elite-C chips I need
//to use the following commands
//qmk flash -kb crkbd -km clarity -bl dfu-split-left
//qmk flash -kb crkbd -km clarity -bl dfu-split-right
//note when flashing you should not have the TRRS cable plugged in
//soundmonster layout seems broken with OLEDs for new functionality,
//I tried the steps here but can't get it to show oled updates on both sides
//same as this
//https://www.reddit.com/r/olkb/comments/qvo5cx/oled_glitch_when_i_use_soundmonsters_keymap_for/
//will just have to wait for updates I guess

layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, _OUTER_L, _OUTER_R, _OUTER_ADJ);
}

/*
bool get_tapping_force_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_LALT:
        case KC_LGUI:
        case KC_LCTL:
        case KC_LSFT:
        case HM_A:
        case HM_S:
        case HM_D:
        case HM_F:
        case HM_J:
        case HM_K:
        case HM_L:
        case HM_SCLN:
        case HM_P:
            return false;
        default:
            return false;
    }
}
*/


bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        //case KC_LALT:
        //case KC_LGUI:
        //case KC_LCTL:
        //case KC_LSFT:
        case HM_A:
        case HM_S:
        case HM_D:

        //case CM_A:  //don't include this case, is same as HM_A
        case CM_R:
        case CM_S:

        //case HM_F:
        //case HM_J:

        case HM_K:
        case HM_L:
        case HM_SCLN:
        case HM_P:

        case CM_E:
        case CM_I:
        case CM_O:
            return false;
        default:
            return true;
    }
}

int RGB_current_mode;

// Setting ADJUST layer RGB back to default
void update_tri_layer_RGB(uint8_t layer1, uint8_t layer2, uint8_t layer3) {
  if (IS_LAYER_ON(layer1) && IS_LAYER_ON(layer2)) {
    layer_on(layer3);
  } else {
    layer_off(layer3);
  }
}

void matrix_init_user(void) {
    #ifdef RGBLIGHT_ENABLE
      RGB_current_mode = rgblight_config.mode;
    #endif
}



#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_270; }



/* KEYBOARD PET START */

/* settings */
#    define MIN_WALK_SPEED      10
#    define MIN_RUN_SPEED       40

/* advanced settings */
#    define ANIM_FRAME_DURATION 200  // how long each frame lasts in ms
#    define ANIM_SIZE           96   // number of bytes in array. If you change sprites, minimize for adequate firmware size. max is 1024

/* timers */
uint32_t anim_timer = 0;
uint32_t anim_sleep = 0;

/* current frame */
uint8_t current_frame = 0;

/* status variables */
int   current_wpm = 0;
led_t led_usb_state;

bool isBarking = false;
bool isSneaking = false;
bool isJumping  = false;
bool showedJump = true;

bool capson = false;

bool led_update_user(led_t led_state) {
    if (led_state.caps_lock) {
        capson = true;
    }
    else {
        capson = false;
    }
    return true;
}

/* logic */
static void render_luna(int LUNA_X, int LUNA_Y,uint8_t modifiers) {

    bool isBarking = (modifiers & MOD_MASK_SHIFT) || capson ;
    bool isSneaking = (modifiers & MOD_MASK_CTRL);
    bool isJumping = (modifiers & MOD_MASK_GUI);
    if (isJumping){
        showedJump = false;
    }
    bool isRunning = (modifiers & MOD_MASK_ALT);


    /* Sit */
    //static const char PROGMEM sit[2][ANIM_SIZE] = {/* 'sit1', 32x22px */
    //    {
    //        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1c, 0x02, 0x05, 0x02, 0x24, 0x04, 0x04, 0x02, 0xa9, 0x1e, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x08, 0x68, 0x10, 0x08, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x06, 0x82, 0x7c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x0c, 0x10, 0x10, 0x20, 0x20, 0x20, 0x28, 0x3e, 0x1c, 0x20, 0x20, 0x3e, 0x0f, 0x11, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    //    },

        /* 'sit2', 32x22px */
    //    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1c, 0x02, 0x05, 0x02, 0x24, 0x04, 0x04, 0x02, 0xa9, 0x1e, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x90, 0x08, 0x18, 0x60, 0x10, 0x08, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x0e, 0x82, 0x7c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x0c, 0x10, 0x10, 0x20, 0x20, 0x20, 0x28, 0x3e, 0x1c, 0x20, 0x20, 0x3e, 0x0f, 0x11, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

    /* Walk */
    static const char PROGMEM walk[2][ANIM_SIZE] = {/* 'walk1', 32x22px */
        {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x90, 0x90, 0x90, 0xa0, 0xc0, 0x80, 0x80, 0x80, 0x70, 0x08, 0x14, 0x08, 0x90, 0x10, 0x10, 0x08, 0xa4, 0x78, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x08, 0xfc, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x18, 0xea, 0x10, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1c, 0x20, 0x20, 0x3c, 0x0f, 0x11, 0x1f, 0x03, 0x06, 0x18, 0x20, 0x20, 0x3c, 0x0c, 0x12, 0x1e, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        },

        /* 'walk2', 32x22px */
        {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x20, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x28, 0x10, 0x20, 0x20, 0x20, 0x10, 0x48, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x20, 0xf8, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x30, 0xd5, 0x20, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x20, 0x30, 0x0c, 0x02, 0x05, 0x09, 0x12, 0x1e, 0x02, 0x1c, 0x14, 0x08, 0x10, 0x20, 0x2c, 0x32, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        }};

    /* Run */
    static const char PROGMEM run[2][ANIM_SIZE] = {/* 'run1', 32x22px */
        {
            0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x08, 0x08, 0xc8, 0xb0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x3c, 0x14, 0x04, 0x08, 0x90, 0x18, 0x04, 0x08, 0xb0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0xc4, 0xa4, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc8, 0x58, 0x28, 0x2a, 0x10, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x09, 0x04, 0x04, 0x04, 0x04, 0x02, 0x03, 0x02, 0x01, 0x01, 0x02, 0x02, 0x04, 0x08, 0x10, 0x26, 0x2b, 0x32, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
        },

        /* 'run2', 32x22px */
        {
            0x00, 0x00, 0x00, 0xe0, 0x10, 0x10, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x78, 0x28, 0x08, 0x10, 0x20, 0x30, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x08, 0x10, 0x11, 0xf9, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x10, 0xb0, 0x50, 0x55, 0x20, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x0c, 0x10, 0x20, 0x28, 0x37, 0x02, 0x1e, 0x20, 0x20, 0x18, 0x0c, 0x14, 0x1e, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        }};

    /* Bark */
    static const char PROGMEM bark[2][ANIM_SIZE] = {/* 'bark1', 32x22px */
        {
            0x00, 0xc0, 0x20, 0x10, 0xd0, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0x3c, 0x14, 0x04, 0x08, 0x90, 0x18, 0x04, 0x08, 0xb0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x08, 0x10, 0x11, 0xf9, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc8, 0x48, 0x28, 0x2a, 0x10, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x0c, 0x10, 0x20, 0x28, 0x37, 0x02, 0x02, 0x04, 0x08, 0x10, 0x26, 0x2b, 0x32, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        },

        /* 'bark2', 32x22px */
        {
            0x00, 0xe0, 0x10, 0x10, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0x40, 0x2c, 0x14, 0x04, 0x08, 0x90, 0x18, 0x04, 0x08, 0xb0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x08, 0x10, 0x11, 0xf9, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0x48, 0x28, 0x2a, 0x10, 0x0f, 0x20, 0x4a, 0x09, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x0c, 0x10, 0x20, 0x28, 0x37, 0x02, 0x02, 0x04, 0x08, 0x10, 0x26, 0x2b, 0x32, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        }};

    /* Sneak */
    static const char PROGMEM sneak[2][ANIM_SIZE] = {/* 'sneak1', 32x22px */
        {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x40, 0x40, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x40, 0x40, 0x80, 0x00, 0x80, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x21, 0xf0, 0x04, 0x02, 0x02, 0x02, 0x02, 0x03, 0x02, 0x02, 0x04, 0x04, 0x04, 0x03, 0x01, 0x00, 0x00, 0x09, 0x01, 0x80, 0x80, 0xab, 0x04, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1c, 0x20, 0x20, 0x3c, 0x0f, 0x11, 0x1f, 0x02, 0x06, 0x18, 0x20, 0x20, 0x38, 0x08, 0x10, 0x18, 0x04, 0x04, 0x02, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00,
        },

        /* 'sneak2', 32x22px */
        {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x40, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xa0, 0x20, 0x40, 0x80, 0xc0, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x41, 0xf0, 0x04, 0x02, 0x02, 0x02, 0x03, 0x02, 0x02, 0x02, 0x04, 0x04, 0x02, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x40, 0x40, 0x55, 0x82, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x20, 0x30, 0x0c, 0x02, 0x05, 0x09, 0x12, 0x1e, 0x04, 0x18, 0x10, 0x08, 0x10, 0x20, 0x28, 0x34, 0x06, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
        }
    };

    /* animation */
    void animate_luna(void) {
        /* jump */
        if (isJumping){ //|| !showedJump) {
            /* clear */
            oled_set_cursor(LUNA_X, LUNA_Y + 2);
            oled_write("     ", false);

            oled_set_cursor(LUNA_X, LUNA_Y - 1);

            showedJump = true;
        } else {
            /* clear */
            oled_set_cursor(LUNA_X, LUNA_Y - 1);
            oled_write("     ", false);

            oled_set_cursor(LUNA_X, LUNA_Y);
        }

        /* switch frame */
        current_frame = (current_frame + 1) % 2;

        /* current status */
        //if (led_usb_state.caps_lock) {
        if (isBarking) {
            oled_write_raw_P(bark[abs(1 - current_frame)], ANIM_SIZE);

        } else if (isSneaking) {
            oled_write_raw_P(sneak[abs(1 - current_frame)], ANIM_SIZE);

        } else if (isRunning) {
            oled_write_raw_P(run[abs(1 - current_frame)], ANIM_SIZE);

        //} else if (current_wpm <= MIN_WALK_SPEED) {
        //    oled_write_raw_P(sit[abs(1 - current_frame)], ANIM_SIZE);
        //
        } else {
            oled_write_raw_P(walk[abs(1 - current_frame)], ANIM_SIZE);
        }
        /*
        } else if (current_wpm <= MIN_RUN_SPEED) {
            oled_write_raw_P(walk[abs(1 - current_frame)], ANIM_SIZE);

        } else {
            oled_write_raw_P(run[abs(1 - current_frame)], ANIM_SIZE);
        }
        */
    }

    /* animation timer */
    if (timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
        anim_timer = timer_read32();
        animate_luna();
    }

    //TJ I commented this out because I have own power manager below
    /* this fixes the screen on and off bug */
    //if (current_wpm > 0) {
    //    oled_on();
    //    anim_sleep = timer_read32();
    //} else if (timer_elapsed32(anim_sleep) > OLED_TIMEOUT) {
    //    oled_off();
    //}
}

/* KEYBOARD PET END */



void render_space(void) {
    oled_write_P(PSTR("     "), false);
}

/*
void render_mod_status_gui_alt(uint8_t modifiers) {
    static const char PROGMEM gui_off_1[] = {0x85, 0x86, 0};
    static const char PROGMEM gui_off_2[] = {0xa5, 0xa6, 0};
    static const char PROGMEM gui_on_1[] = {0x8d, 0x8e, 0};
    static const char PROGMEM gui_on_2[] = {0xad, 0xae, 0};

    static const char PROGMEM alt_off_1[] = {0x87, 0x88, 0};
    static const char PROGMEM alt_off_2[] = {0xa7, 0xa8, 0};
    static const char PROGMEM alt_on_1[] = {0x8f, 0x90, 0};
    static const char PROGMEM alt_on_2[] = {0xaf, 0xb0, 0};

    // fillers between the modifier icons bleed into the icon frames
    static const char PROGMEM off_off_1[] = {0xc5, 0};
    static const char PROGMEM off_off_2[] = {0xc6, 0};
    static const char PROGMEM on_off_1[] = {0xc7, 0};
    static const char PROGMEM on_off_2[] = {0xc8, 0};
    static const char PROGMEM off_on_1[] = {0xc9, 0};
    static const char PROGMEM off_on_2[] = {0xca, 0};
    static const char PROGMEM on_on_1[] = {0xcb, 0};
    static const char PROGMEM on_on_2[] = {0xcc, 0};

    if(modifiers & MOD_MASK_GUI) {
        oled_write_P(gui_on_1, false);
    } else {
        oled_write_P(gui_off_1, false);
    }

    if ((modifiers & MOD_MASK_GUI) && (modifiers & MOD_MASK_ALT)) {
        oled_write_P(on_on_1, false);
    } else if(modifiers & MOD_MASK_GUI) {
        oled_write_P(on_off_1, false);
    } else if(modifiers & MOD_MASK_ALT) {
        oled_write_P(off_on_1, false);
    } else {
        oled_write_P(off_off_1, false);
    }

    if(modifiers & MOD_MASK_ALT) {
        oled_write_P(alt_on_1, false);
    } else {
        oled_write_P(alt_off_1, false);
    }

    if(modifiers & MOD_MASK_GUI) {
        oled_write_P(gui_on_2, false);
    } else {
        oled_write_P(gui_off_2, false);
    }

    if (modifiers & MOD_MASK_GUI & MOD_MASK_ALT) {
        oled_write_P(on_on_2, false);
    } else if(modifiers & MOD_MASK_GUI) {
        oled_write_P(on_off_2, false);
    } else if(modifiers & MOD_MASK_ALT) {
        oled_write_P(off_on_2, false);
    } else {
        oled_write_P(off_off_2, false);
    }

    if(modifiers & MOD_MASK_ALT) {
        oled_write_P(alt_on_2, false);
    } else {
        oled_write_P(alt_off_2, false);
    }
}

void render_mod_status_ctrl_shift(uint8_t modifiers) {
    static const char PROGMEM ctrl_off_1[] = {0x89, 0x8a, 0};
    static const char PROGMEM ctrl_off_2[] = {0xa9, 0xaa, 0};
    static const char PROGMEM ctrl_on_1[] = {0x91, 0x92, 0};
    static const char PROGMEM ctrl_on_2[] = {0xb1, 0xb2, 0};

    static const char PROGMEM shift_off_1[] = {0x8b, 0x8c, 0};
    static const char PROGMEM shift_off_2[] = {0xab, 0xac, 0};
    static const char PROGMEM shift_on_1[] = {0xcd, 0xce, 0};
    static const char PROGMEM shift_on_2[] = {0xcf, 0xd0, 0};

    // fillers between the modifier icons bleed into the icon frames
    static const char PROGMEM off_off_1[] = {0xc5, 0};
    static const char PROGMEM off_off_2[] = {0xc6, 0};
    static const char PROGMEM on_off_1[] = {0xc7, 0};
    static const char PROGMEM on_off_2[] = {0xc8, 0};
    static const char PROGMEM off_on_1[] = {0xc9, 0};
    static const char PROGMEM off_on_2[] = {0xca, 0};
    static const char PROGMEM on_on_1[] = {0xcb, 0};
    static const char PROGMEM on_on_2[] = {0xcc, 0};

    if(modifiers & MOD_MASK_CTRL) {
        oled_write_P(ctrl_on_1, false);
    } else {
        oled_write_P(ctrl_off_1, false);
    }

    if ((modifiers & MOD_MASK_CTRL) && (modifiers & MOD_MASK_SHIFT)) {
        oled_write_P(on_on_1, false);
    } else if(modifiers & MOD_MASK_CTRL) {
        oled_write_P(on_off_1, false);
    } else if(modifiers & MOD_MASK_SHIFT) {
        oled_write_P(off_on_1, false);
    } else {
        oled_write_P(off_off_1, false);
    }

    if(modifiers & MOD_MASK_SHIFT) {
        oled_write_P(shift_on_1, false);
    } else {
        oled_write_P(shift_off_1, false);
    }

    if(modifiers & MOD_MASK_CTRL) {
        oled_write_P(ctrl_on_2, false);
    } else {
        oled_write_P(ctrl_off_2, false);
    }

    if (modifiers & MOD_MASK_CTRL & MOD_MASK_SHIFT) {
        oled_write_P(on_on_2, false);
    } else if(modifiers & MOD_MASK_CTRL) {
        oled_write_P(on_off_2, false);
    } else if(modifiers & MOD_MASK_SHIFT) {
        oled_write_P(off_on_2, false);
    } else {
        oled_write_P(off_off_2, false);
    }

    if(modifiers & MOD_MASK_SHIFT) {
        oled_write_P(shift_on_2, false);
    } else {
        oled_write_P(shift_off_2, false);
    }
}
*/

void render_logo(void) {
    static const char PROGMEM corne_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0};
    oled_write_P(corne_logo, false);
    oled_write_P(PSTR("corne"), false);
}


void render_layer_state(bool _isleft) {
    static const char PROGMEM default_layer[] = {
        0x20, 0x94, 0x95, 0x96, 0x20,
        0x20, 0xb4, 0xb5, 0xb6, 0x20,
        0x20, 0xd4, 0xd5, 0xd6, 0x20, 0};
    static const char PROGMEM raise_layer[] = {
        0x20, 0x97, 0x98, 0x99, 0x20,
        0x20, 0xb7, 0xb8, 0xb9, 0x20,
        0x20, 0xd7, 0xd8, 0xd9, 0x20, 0};
    static const char PROGMEM lower_layer[] = {
        0x20, 0x9a, 0x9b, 0x9c, 0x20,
        0x20, 0xba, 0xbb, 0xbc, 0x20,
        0x20, 0xda, 0xdb, 0xdc, 0x20, 0};
    static const char PROGMEM adjust_layer[] = {
        0x20, 0x9d, 0x9e, 0x9f, 0x20,
        0x20, 0xbd, 0xbe, 0xbf, 0x20,
        0x20, 0xdd, 0xde, 0xdf, 0x20, 0};
    if(layer_state_is(_OUTER_ADJ)) {
        oled_write_P(adjust_layer, false);
    } else if((layer_state_is(_OUTER_L) && _isleft) || (layer_state_is(_OUTER_R) && !_isleft)) {
        oled_write_P(lower_layer, false);
    } else if((layer_state_is(_INNER_L) && _isleft) || (layer_state_is(_INNER_R) && !_isleft)) {
        oled_write_P(raise_layer, false);
    } else {
        oled_write_P(default_layer, false);
    }
}


//doesn't work with new additional layers
/*
void render_layer_state_minimal_l(void) {
    if(layer_state_is(_OUTER_ADJ)) {
        oled_write_P(PSTR(" @ - "), false);
    } else if(layer_state_is(_OUTER_L)) {
        oled_write_P(PSTR(" @ - "), false);
    } else if(layer_state_is(_INNER_L)) {
        oled_write_P(PSTR(" - @ "), false);
    } else {
        oled_write_P(PSTR(" - - "), false);
    }
}

void render_layer_state_minimal_r(void) {
    if(layer_state_is(_OUTER_ADJ)) {
        oled_write_P(PSTR(" - @ "), false);
    } else if(layer_state_is(_OUTER_R)) {
        oled_write_P(PSTR(" - @ "), false);
    } else if(layer_state_is(_INNER_R)) {
        oled_write_P(PSTR(" @ - "), false);
    } else {
        oled_write_P(PSTR(" - - "), false);
    }
}
*/

void render_status_main(void) {
    render_logo();
    render_space();
    render_space();
    render_space();
    //render_layer_state(true);
    //render_layer_state_minimal_l();
    render_space();
    //render_mod_status_gui_alt(get_mods()|get_oneshot_mods());
    //render_mod_status_ctrl_shift(get_mods()|get_oneshot_mods());
    render_luna(0,13,get_mods()|get_oneshot_mods());
}

void render_status_secondary(void) {
    render_logo();
    render_space();
    render_space();
    render_space();
    //render_layer_state(false);
    //render_layer_state_minimal_r();
    render_space();
    //render_mod_status_gui_alt(get_mods()|get_oneshot_mods());
    //render_mod_status_ctrl_shift(get_mods()|get_oneshot_mods());
    render_luna(0,13,get_mods()|get_oneshot_mods());
}

void suspend_power_down_user() {
    oled_off();
}

bool oled_task_user(void) {
    /* KEYBOARD PET VARIABLES START */
    //TJ: disabled wpm functionality to fit in games layers. Luna will just walk now
    //current_wpm   = get_current_wpm();
    current_wpm   = 15;
    
    led_usb_state = host_keyboard_led_state();
    /* KEYBOARD PET VARIABLES END */

    if (is_keyboard_master()) {
        if (timer_elapsed32(oled_timer) > 30000) {
            oled_off();
            return false;
        }
        else { oled_on(); }
    }
    if (is_keyboard_left()) {
        render_status_main();  // Renders the current keyboard state (layer, lock, caps, scroll, etc)
    } else {
        render_status_secondary();
    }
    return false;
}

#endif





bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
#ifdef OLED_ENABLE
        oled_timer = timer_read32();
#endif
    // set_timelog();
  }
  
  //Static variables have a property of preserving their value 
  //even after they are out of their scope!
  //static uint16_t my_colon_timer;

  //static uint16_t outer_l_timer;
  //static uint16_t outer_r_timer;
  //static uint16_t inner_l_timer;
  //static uint16_t inner_r_timer;

  static bool is_HLD_BTN1;

  if (  is_HLD_BTN1 &&
        keycode != HLD_BTN1 &&
        keycode != KC_MS_L &&
        keycode != KC_MS_U &&
        keycode != KC_MS_D &&
        keycode != KC_MS_R &&
        keycode != KC_WH_L &&
        keycode != KC_WH_U &&
        keycode != KC_WH_D &&
        keycode != KC_WH_R
  ){
      //de-activate the hold button if it is on
      is_HLD_BTN1 = false;
      unregister_code(KC_BTN1);

  }

  switch (keycode) {

    //I was only doing this to get the tri layer working
    //since I can use LT otherwise
    //and there's a better way to get joint layers, using 
    //layer_state_set_user function, so no need 
    //https://www.reddit.com/r/olkb/comments/8eticz/comment/dxxzz1d/?utm_source=share&utm_medium=web2x&context=3
    /*
    case OUTER_L:
      if (record->event.pressed) {
        layer_on(_OUTER_L);
      } else {
        layer_off(_OUTER_L);
      }
      update_tri_layer_RGB(_OUTER_L, _OUTER_R, _OUTER_ADJ);
      return false;
    case OUTER_R:
      if (record->event.pressed) {
        layer_on(_OUTER_R);
      } else {
        layer_off(_OUTER_R);
      }
      update_tri_layer_RGB(_OUTER_L, _OUTER_R, _OUTER_ADJ);
      return false;
    case OUTER_ADJ:
        if (record->event.pressed) {
          layer_on(_OUTER_ADJ);
        } else {
          layer_off(_OUTER_ADJ);
        }
        return false;

    case INNER_L:
      if (record->event.pressed) {
        inner_l_timer = timer_read();
        layer_on(_INNER_L);
      } else {
        layer_off(_INNER_L);
        if (timer_elapsed(inner_l_timer) < TAPPING_TERM) {
            SEND_STRING(SS_TAP(X_ENT)); // Change the character(s) to be sent on tap here
        }
      }
      update_tri_layer_RGB(_INNER_L, _INNER_R, _INNER_ADJ);
      return false;
    case INNER_R:
      inner_r_timer = timer_read();
      if (record->event.pressed) {
        layer_on(_INNER_R);
      } else {
        layer_off(_INNER_R);
        if (timer_elapsed(inner_r_timer) < TAPPING_TERM) {
            SEND_STRING(SS_TAP(X_SPC)); // Change the character(s) to be sent on tap here
        }
      }
      update_tri_layer_RGB(_INNER_L, _INNER_R, _INNER_ADJ);
      return false;
    case INNER_ADJ:
        if (record->event.pressed) {
          layer_on(_INNER_ADJ);
        } else {
          layer_off(_INNER_ADJ);
        }
        return false;

    */

    //needed to comment out to save space
    /*
    case KC_RACL:
        if (record->event.pressed) {
          my_colon_timer = timer_read();
          register_code(KC_RALT);
        } else {
          unregister_code(KC_RALT);
          if (timer_elapsed(my_colon_timer) < TAPPING_TERM) {
            SEND_STRING(":"); // Change the character(s) to be sent on tap here
          }
        }
        return false;
    */

    case RGBRST:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          eeconfig_update_rgblight_default();
          rgblight_enable();
          RGB_current_mode = rgblight_config.mode;
        }
      #endif
      #ifdef RGB_MATRIX_ENABLE
        if (record->event.pressed) {
          eeconfig_update_rgb_matrix_default();
          rgb_matrix_enable();
        }
      #endif
      break;

    /*
    case KC_BTN1:
        if (record->event.pressed) {
            //handle click as normal
        }
        else {
            //de-activate the hold button if it is on
            is_HLD_BTN1 = false;
        }
        break;
    */

    case HLD_BTN1:
        if (record->event.pressed) {
            is_HLD_BTN1 ^= 1;
            if (is_HLD_BTN1) {
                register_code(KC_BTN1);
            } else {
                unregister_code(KC_BTN1);
            }
        }
        break;
    case TM_C:
        if (record->event.pressed) {
            // when keycode QMKBEST is pressed
            SEND_STRING(SS_LCTL("a")"c");
        } else {
            // when keycode QMKBEST is released
        }
        break;
    case TM_P:
        if (record->event.pressed) {
            // when keycode QMKBEST is pressed
            SEND_STRING(SS_LCTL("a")"p");
        } else {
            // when keycode QMKBEST is released
        }
        break;
    case TM_N:
        if (record->event.pressed) {
            // when keycode QMKBEST is pressed
            SEND_STRING(SS_LCTL("a")"n");
        } else {
            // when keycode QMKBEST is released
        }
        break;
    case TM_AMPR:
        if (record->event.pressed) {
            // when keycode QMKBEST is pressed
            SEND_STRING(SS_LCTL("a")"&");
        } else {
            // when keycode QMKBEST is released
        }
        break;
    case TM_SPC:
        if (record->event.pressed) {
            // when keycode QMKBEST is pressed
            SEND_STRING(SS_LCTL("a")" ");
        } else {
            // when keycode QMKBEST is released
        }
        break;
    case TM_S:
        if (record->event.pressed) {
            // when keycode QMKBEST is pressed
            SEND_STRING(SS_LCTL("a")"s");
        } else {
            // when keycode QMKBEST is released
        }
        break;
    case TM_V:
        if (record->event.pressed) {
            // when keycode QMKBEST is pressed
            SEND_STRING(SS_LCTL("a")"v");
        } else {
            // when keycode QMKBEST is released
        }
        break;
    case TM_X:
        if (record->event.pressed) {
            // when keycode QMKBEST is pressed
            SEND_STRING(SS_LCTL("a")"x");
        } else {
            // when keycode QMKBEST is released
        }
        break;
    case TM_MINS:
        if (record->event.pressed) {
            // when keycode QMKBEST is pressed
            SEND_STRING(SS_LCTL("a")"-");
        } else {
            // when keycode QMKBEST is released
        }
        break;
    case TM_PLUS:
        if (record->event.pressed) {
            // when keycode QMKBEST is pressed
            SEND_STRING(SS_LCTL("a")"+");
        } else {
            // when keycode QMKBEST is released
        }
        break;
    case TM_LT:
        if (record->event.pressed) {
            // when keycode QMKBEST is pressed
            SEND_STRING(SS_LCTL("a")"<");
        } else {
            // when keycode QMKBEST is released
        }
        break;
    case TM_GT:
        if (record->event.pressed) {
            // when keycode QMKBEST is pressed
            SEND_STRING(SS_LCTL("a")">");
        } else {
            // when keycode QMKBEST is released
        }
        break;
    case TM_ON:
        if (record->event.pressed) {
            // when keycode QMKBEST is pressed
            SEND_STRING(SS_LCTL("a"));
        } else {
            // when keycode QMKBEST is released
        }
        break;
    case TM_SCON:
        if (record->event.pressed) {
            // when keycode QMKBEST is pressed
            SEND_STRING(SS_LCTL("a")"[");
        } else {
            // when keycode QMKBEST is released
        }
        break;

    /* KEYBOARD PET STATUS START */
    /*
    case HM_D: //KC_LCTL:
    case HM_F: //KC_RCTL:
    case HM_J: //KC_LCTL:
    case HM_K: //KC_RCTL:
        if (record->event.pressed) {
            isSneaking = true;
        } else {
            isSneaking = false;
        }
        break;
    case HM_S: //KC_SPC:
    case HM_L: //KC_SPC:
        if (record->event.pressed) {
            isJumping  = true;
            showedJump = false;
        } else {
            isJumping = false;
        }
        break;
    */
    /* KEYBOARD PET STATUS END */
  }
  return true;
}

#ifdef RGB_MATRIX_ENABLE

void suspend_power_down_keymap(void) {
    rgb_matrix_set_suspend_state(true);
}

void suspend_wakeup_init_keymap(void) {
    rgb_matrix_set_suspend_state(false);
}

#endif
