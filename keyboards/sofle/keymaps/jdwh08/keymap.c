#include QMK_KEYBOARD_H
#include "os_detection.h"  // OS Types
#include "sentence_case.h"

// NOTE: This firmware is too big for the default Atmega32u4/Pro-Micro controller. I'm using RP2040/Elite-Pi.

// TODO:? Change sentence case to use backspace to undo the capitalization? w/ (https://getreuer.info/posts/keyboards/triggers/index.html#based-on-previously-typed-keys)
// TODO: Add dynamic macros https://docs.qmk.fm/#/feature_dynamic_macros
// TODO: Add Steno
// TODO: Add button for control-alt-del

// TODO: Add button to switch between tabs in same window (control tab) for H
// TODO: Holding down U/Q in the EXTND layer should jump to start and end of line.
// TODO: Mouse jiggler? https://github.com/DIYCharles/MouseJiggler
// TODO: Mouse cursor layer on RAISE VMLSTR? https://docs.qmk.fm/#/feature_mouse_keys
// TODO: Lock? https://docs.qmk.fm/#/feature_secure
// TODO: Aussie/Cursive/ZALGO/etc. https://github.com/drashna/qmk_userspace/blob/04579e566c5b038c2027a1def733b870f532f11a/users/drashna/keyrecords/unicode.c#L4

// BUGS:
// Sentence case disables capsword.
// Backspace won't delete the pair for any keys that type matched pairs (), <>, [], {}, ""
// Luna Jump doesn't work.

// ----------------------------------------------------------------------------------------------------
// CUSTOM KEYCODES

enum custom_keycodes {
    // Built in Defaults
    KC_QWERTY = SAFE_RANGE,
    KC_STRDY,
    KC_PRVWD,
    KC_NXTWD,
    KC_LSTRT,
    KC_LEND,
    KC_DLINE,

    // MAGIC KEYS
    ALTREP2,
    ALTREP3,
    ALTREP4,

    // Q -> QU
    KC_QU,

    // Extra Functions
    KC_SLWD,  // select word
    KC_SRCH,  // search selection in browser (assume browser open)
    KC_SRCHB, // search selection in browser (opens browser first)

    KC_REDO,  // redo LCTL(KC_Y)
    KC_SNIP,  // snip portion of screen
    KC_PRNT,  // print LCTL(KC_P)
    KC_SLAL,  // select all LCTL(KC_A)
    KC_CNEW,  // new LCTL(KC_N)
    KC_TNEW,  // new tab LCTL(KC_T)
    KC_CLSE,  // close LCTL(KC_W)
    KC_RFSH,  // refresh LCTL(KC_R)
    KC_SAVE,  // save LCTL(KC_S)
    KC_ATAB,  // alt tab LALT(KC_TAB)
    KC_CTAB,  // control tab LCTL(KC_TAB)
    KC_ZMIN,  // zoom in w/ LCTL(KC_EQL) 
    KC_ZMOT,  // zoom out w/ LCTL(KC_MINS)

    // Brackets Doubling
    KC_9PRC,  // type 9 or () if shifted
    // C_LPARN,  // helper for ( macro
    KC_SBRC,  // type [] [squarebracket]
    KC_CBRC,  // type {} {curlybracket}
    KC_CANG,  // type , or <> if shifted
    // C_LANGB,  // helper for < macro
    KC_DQOT,  // type ""
};

// ----------------------------------------------------------
// Tap Hold: Custom Keys
#define C_BKSP LT(0, KC_BSPC)  // custom backspace; KC_BSPC on tap, control backspace on hold
#define C_SRCH LT(0, KC_SRCH)  // custom web search key; browser search on tap, open new browser + search on hold
#define C_ENTR LT(0, KC_ENT)    // custom enter key; enter on tap, control enter on hold
#define C_PRVWD LT(0, KC_PRVWD)  // previous word on tap; LSTART on hold
#define C_NXTWD LT(0, KC_NXTWD)  // next word on tap; LEND on hold
#define C_BRC LT(0, KC_SBRC)  // square bracket on tap; curly bracket on hold

/////////////////////////////////////////////////////////////////////////
// LAYERS

enum sofle_layers {
    /* _M_XYZ = Mac Os, _W_XYZ = Win/Linux */
    _QWERTY,
    _STRDY,
    _EXTND,
    _RAISE,
    _ADJUST,
};

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _EXTND, _RAISE, _ADJUST);
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * QWERTY
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |  `   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  `   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | ESC  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |RShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |LOWER | /Enter  /       \Space \  |RAISE | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */

[_QWERTY] = LAYOUT(
  KC_ESC,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_MINUS,
  KC_GRV,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,  KC_SLSH,
  KC_TAB,   KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                     KC_H,    KC_J,    KC_K,    KC_L, KC_BSLS,  KC_QUOT,
  KC_LSFT,   KC_Z,   KC_X,    KC_C,    KC_V,    KC_B, KC_MUTE,KC_MEDIA_PLAY_PAUSE,KC_N,    KC_M,  KC_COMM,  KC_DOT, KC_SCLN,  KC_EQUAL,
                KC_LGUI, KC_LCTL, TT(_EXTND), C_ENTR, KC_SPC,                   C_BKSP, QK_REP, TT(_RAISE), KC_RALT, CW_TOGG
),
/*
 * STRDY
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  -   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  `   |   V  |   M  |   L  |   C  |   P  |                    |   B  |   H  |   U  |   O  |   Q  |  /   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   S  |   T  |   R  |   D  |   Y  |-------.    ,-------|   F  | MAGIC|   E  |   A  |   I  |  '   |
 * |------+------+------+------+------+------|  MUTE |    | PAUSE |------+------+------+------+------+------|
 * |LShift|   X  |   K  |   J  |   G  |   W  |-------|    |-------|   Z  |   N  |   ,  |   .  |   ;  |  +   |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LCTR |EXTND2|Enter | /Space  /       \Bkspc \  |REPEAT|RAISE | RAlt | Caps |
 *            |      |      |      |      |/       /         \      \ |      |      |      | Word |
 *            `----------------------------------'            '------''---------------------------'
 */

[_STRDY] = LAYOUT(
  KC_ESC,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                       KC_6,     KC_7,    KC_8, KC_9PRC,    KC_0,  KC_MINUS,
  KC_GRV,   KC_V,   KC_M,    KC_L,    KC_C,    KC_P,                       KC_B,     KC_H,    KC_U,    KC_O,    KC_QU,  KC_SLSH,
  KC_TAB,   KC_S,   KC_T,    KC_R,    KC_D,    KC_Y,                       KC_F,  QK_AREP,    KC_E,    KC_A,    KC_I,  KC_DQOT,
  KC_LSFT,  KC_X,   KC_K,    KC_J,    KC_G,    KC_W, KC_MUTE,     KC_MPLY, KC_Z,     KC_N, KC_COMM,  KC_DOT, KC_SCLN,  KC_EQUAL,
    KC_LGUI,KC_LCTL,TT(_EXTND),C_ENTR, KC_SPC,                    C_BKSP, QK_REP, TT(_RAISE), KC_RALT, CW_TOGG
),
/* EXTEND2
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |xBASEx|  F1  |  F2  |  F3  |  F4  |  F5  |                    |  F6  |  F7  |  F8  |  F9  | F10  | F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | `ESC |Print |BSrcSl|Close |Refrsh| SNIP |                    | CNEW | CTAB |PrvWd |UpArrw| NxtWd|   /  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  | Save |SrchSl| Del  |Selwrd|  [{  |-------.    ,-------| Find |MAGIC |LArrw |DnArrw| RArrw|   '  |
 * |------+------+------+------+------+------|  MUTE |    | PAUSE |------+------+------+------+------+------|
 * |LShift| Undo | Cut  | Copy | Paste| Redo |-------|    |-------|SlctAl| ATAB |   ,  |   .  |   ;  |   +  |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI |      | LCTR |Enter | /Space  /       \Bkspc \  |REPEAT|ADJUST| RAlt | Caps |
 *            |      |      |      |      |/       /         \      \ |      |      |      | Word |
 *            `----------------------------------'            '------''---------------------------'
 */
[_EXTND] = LAYOUT(
  TG(_EXTND),  KC_F1,   KC_F2,     KC_F3,       KC_F4,     KC_F5,                       KC_F6,      KC_F7,    KC_F8,   KC_F9,   KC_F10,   KC_F12,
  QK_GESC,   KC_PRNT, KC_SRCHB,  KC_RFSH,     KC_CLSE,   KC_SNIP,                     KC_CNEW,    KC_CTAB,   C_PRVWD,  KC_UP,  C_NXTWD,  _______,
  _______,   KC_SAVE,  KC_SRCH,  KC_SLWD,      KC_DEL,    C_BRC,                      KC_FIND,    _______,  KC_LEFT, KC_DOWN,  KC_RGHT,  _______,
  _______,   KC_UNDO,   KC_CUT,  KC_COPY,    KC_PASTE,   KC_REDO, _______,   _______, KC_SLAL,    KC_ATAB,  _______, _______,  _______,  _______,
                    _______, _______, _______, _______, _______,                       _______, _______, _______, _______, _______
),
/* RAISE
 * ,----------------------------------------.                     ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Esc  | Ins  | Pscr | Menu |      |      |                    |      | PWrd |  Up  | NWrd | DLine| Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  | LAt  | LCtl |LShift|      | Caps |-------.    ,-------|      | Left | Down | Rigth|  Del | Bspc |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * |Shift | Undo |  Cut | Copy | Paste|      |-------|    |-------|      | LStr |      | LEnd |      | Shift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |LOWER | /Enter  /       \Space \  |RAISE | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[_RAISE] = LAYOUT(
  TG(_RAISE), _______ , _______ , _______ , _______ , _______,                   _______,  _______  , _______,  _______ ,  _______ ,_______,
  QK_GESC,  KC_INS,  KC_PSCR,   KC_APP,  XXXXXXX, XXXXXXX,                        KC_PGUP, KC_PRVWD,   KC_UP, KC_NXTWD,KC_DLINE, KC_BSPC,
  _______, KC_LALT,  KC_LCTL,  KC_LSFT,  XXXXXXX, KC_CAPS,                       KC_PGDN,  KC_LEFT, KC_DOWN, KC_RGHT,  KC_DEL, KC_BSPC,
  _______,KC_UNDO, KC_CUT, KC_COPY, KC_PASTE, XXXXXXX,  _______,       _______,  XXXXXXX, KC_LSTRT, XXXXXXX, KC_LEND,   XXXXXXX, _______,
                         _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______
),
/* ADJUST
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |QK_BOOT|     |QWERTY| STRDY|      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |MACWIN|      |      |      |-------.    ,-------|      | VOLDO| MUTE | VOLUP|      |      |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|      | PREV | PLAY | NEXT |      |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |LOWER | /Enter  /       \Space \  |RAISE | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
  [_ADJUST] = LAYOUT(
  TO(_STRDY) , XXXXXXX,  XXXXXXX ,  XXXXXXX , XXXXXXX, XXXXXXX,                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  QK_BOOT  , XXXXXXX,KC_QWERTY,KC_STRDY,CG_TOGG,XXXXXXX,                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX , XXXXXXX,CG_TOGG, XXXXXXX,    XXXXXXX,  XXXXXXX,                     XXXXXXX, KC_VOLD, KC_MUTE, KC_VOLU, XXXXXXX, XXXXXXX,
  XXXXXXX , XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX,  XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, KC_MPRV, KC_MPLY, KC_MNXT, XXXXXXX, XXXXXXX,
                   _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______
  )
};
////////////////////////////////////////////////////////////////////////////
// MAGIC KEY
uint8_t magic_length = 0;  // length of the string sent out by magic key. used for magic backspace.
// ----------------------------------------------------------------------------
// ALT REP KEYS:
bool remember_last_key_user(uint16_t keycode, keyrecord_t* record,
                            uint8_t* remembered_mods) {
    switch (keycode) {
        case ALTREP2:
        case ALTREP3:
        case ALTREP4:
            return false;  // Ignore ALTREP keys.
    }

    return true;  // Other keys can be repeated.
}
// -------------------------------------------------------------------------
// An enhanced version of SEND_STRING: if Caps Word is active, the Shift key is
// held while sending the string. Additionally, the last key is set such that if
// the Repeat Key is pressed next, it produces `repeat_keycode`.
#define MAGIC_STRING(str, repeat_keycode) \
    magic_send_string_P(PSTR(str), (repeat_keycode))
static void magic_send_string_P(const char* str, uint16_t repeat_keycode) {
    uint8_t saved_mods = 0;
    // If Caps Word is on, save the mods and hold Shift.
    if (is_caps_word_on()) {
        saved_mods = get_mods();
        register_mods(MOD_BIT(KC_LSFT));
    }

    send_string_with_delay_P(str, TAP_CODE_DELAY);  // Send the string.
    set_last_keycode(repeat_keycode);

    // If Caps Word is on, restore the mods.
    if (is_caps_word_on()) {
        set_mods(saved_mods);
    }
}

// DEFAULT: (1tap) - Send the alternate repeat keycode.
uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods) {
    if ((mods & ~MOD_MASK_SHIFT) == 0) {
        switch (keycode) {
        // For navigating next/previous search results in Vim:
        // N -> Shift + N, Shift + N -> N.
        // case KC_N: return KC_N;

        // Behavior for Magic Sturdy's "magic" key -- TAP
        case KC_A: magic_length = 1; return KC_O;         // A -> O
        case KC_B: magic_length = 1; return KC_E;         // B -> E
        case KC_C: magic_length = 1; return KC_K;         // C -> K
        case KC_D: magic_length = 1; MAGIC_STRING(/*d*/"ge", KC_E); return KC_NO;        // D -> GE
        case KC_E: magic_length = 1; return KC_U;         // E -> U
        case KC_F: magic_length = 2; MAGIC_STRING(/*f*/"rom", KC_M); return KC_NO;       // F -> ROM
        case KC_G: magic_length = 1; return KC_R;         // G -> R
        case KC_H: magic_length = 2; MAGIC_STRING(/*h*/"en", KC_N); return KC_NO;        // H -> EN
        case KC_I: magic_length = 2; MAGIC_STRING(/*i*/"on", KC_N); return KC_NO;        // I -> ON
        case KC_J: magic_length = 3; MAGIC_STRING(/*j*/"ust", KC_T); return KC_NO;       // J -> UST
        case KC_K: magic_length = 1; return KC_S;         // K -> S
        case KC_L: magic_length = 1; return KC_K;         // L -> K
        case KC_M: magic_length = 1; return KC_S;         // M -> S
        case KC_N: magic_length = 1; return KC_H;         // N -> H
        case KC_O: magic_length = 1; return KC_A;         // O -> A
        case KC_P: magic_length = 1; return KC_R;         // P -> R
        case KC_Q: magic_length = 1; return KC_I;         // Q -> I
        case KC_QU: magic_length = 1; return KC_I;        // QU -> I
        case KC_R: magic_length = 1; return KC_L;         // R -> L
        case KC_S: magic_length = 1; return KC_K;         // S -> K
        case KC_T: magic_length = 3; MAGIC_STRING(/*t*/"hat", KC_T); return KC_NO;       // T -> HAT
        case KC_U: magic_length = 1; return KC_N;         // U -> N
        case KC_V: magic_length = 1; MAGIC_STRING(/*v*/"er", KC_R); return KC_NO;        // V -> ER
        case KC_W: magic_length = 3; MAGIC_STRING(/*w*/"hen", KC_N); return KC_NO;       // W -> HEN
        case KC_X: magic_length = 1; return KC_P;         // X -> P
        case KC_Y: magic_length = 1; return KC_P;         // Y -> P
        case KC_Z: magic_length = 1; return KC_E;         // Z -> E
        case KC_SPC: magic_length = 3; MAGIC_STRING(/* */"the", KC_E); return KC_NO;      // spc -> THE

        case KC_1 ... KC_0: magic_length = 1; return KC_DOT; // 1-0 -> .
        case KC_9PRC: magic_length = 1; return KC_DOT;       // 9 -> .

        case KC_DOT: magic_length = 1; MAGIC_STRING(/*.*/"/", KC_NO); return KC_NO;    // . -> ./
        case KC_COMM:                   // ! -> ==
            if ((mods & MOD_MASK_SHIFT) == 0) {
            return KC_NO;
            }
            // Fall through intended.
        case KC_EQL: magic_length = 2; MAGIC_STRING(/**/"==", KC_NO); return KC_NO;     // = -> ==
        case KC_HASH: magic_length = 7; MAGIC_STRING(/*#*/"include", KC_NO); return KC_NO; // # -> include
        case KC_QUOT:
            // Fall through intended.
        // case KC_DQOT:
            // Shouldn't happen because of auto shift but...
            if ((mods & MOD_MASK_SHIFT) != 0) {
            // magic_length = 1;  // Not done.
            return KC_NO;  // " -> ""<cursor>"""
            }
            else {
            magic_length = 2;
            MAGIC_STRING(/*'*/"ll", KC_L); return KC_NO;
            }
        case KC_GRV:  // ` -> ``<cursor>``` (for Markdown code)
            magic_length = 1;  // Not done.
            // return M_MKGRVS;
            return KC_NO;
        case KC_LABK: magic_length = 1; return KC_MINS;   // < -> - (for Haskell)
        }
    } else if ((mods & MOD_MASK_CTRL)) {
        switch (keycode) {
        case KC_A:  // Ctrl+A -> Ctrl+C
            magic_length = 0;
            return C(KC_C);
        }
    }
    magic_length = 0;
    return KC_TRNS;
}
// Option 2: (1hold)
// https://github.com/Ikcelaks/keyboard_layouts/blob/main/magic_sturdy/QMK_Layout/Moonlander/magic_sturdy/keymap.c
// https://www.ngrams.info/ for ngrams
static int process_magic_key_2(uint16_t keycode, uint8_t mods) {
    if ((mods & ~MOD_MASK_SHIFT) == 0) {
        switch (keycode) {
        // Behavior for Magic Sturdy's "magic" key -- HOLD
        case KC_A: MAGIC_STRING(/*a*/"ck", KC_K); return 2;         // A -> CK
        case KC_B: MAGIC_STRING(/*b*/"an", KC_N); return 2;         // B -> AN
        case KC_C: tap_code(KC_Y); return 1;                        // C -> Y
        case KC_D: tap_code(KC_Y); return 1;                        // D -> Y
        case KC_E: MAGIC_STRING(/*e*/"nce", KC_E); return 3;        // E -> NCE
        case KC_F: MAGIC_STRING(/*f*/"ound", KC_D); return 4;       // F -> OUND
        case KC_G: tap_code(KC_L); return 1;                        // G -> L
        case KC_H: MAGIC_STRING(/*h*/"ere", KC_E); return 3;        // H -> ERE
        case KC_I: MAGIC_STRING(/*h*/"ze", KC_E); return 2;         // I -> ZE
        case KC_J: MAGIC_STRING(/*j*/"oin", KC_N); return 3;        // J -> OIN
        case KC_K: MAGIC_STRING(/*k*/"now", KC_W); return 3;        // K -> NOW
        case KC_L: tap_code(KC_Y); return 1;                        // L -> Y
        case KC_M: MAGIC_STRING(/*m*/"ent", KC_T); return 3;        // M -> ENT
        case KC_N: MAGIC_STRING(/*n*/"ess", KC_S); return 3;        // N -> ESS
        case KC_O: MAGIC_STRING(/*o*/"ut", KC_T); return 2;         // O -> UT
        case KC_P: tap_code(KC_Y); return 1;                        // P -> Y
        case KC_Q: tap_code(KC_I); return 1;                        // Q -> I
        case KC_QU: tap_code(KC_I); return 1;                       // QU -> I
        case KC_R: tap_code(KC_K); return 1;                        // R -> K
        case KC_S: MAGIC_STRING(/*s*/"ome", KC_E); return 3;        // S -> OME
        case KC_T: MAGIC_STRING(/*t*/"hen", KC_N); return 3;        // T -> HEN
        case KC_U: tap_code(KC_E); return 1;                        // U -> E
        case KC_V: MAGIC_STRING(/*v*/"alue", KC_E); return 4;       // V -> ALUE
        case KC_W: MAGIC_STRING(/*w*/"ith", KC_H); return 3;        // W -> ITH
        case KC_X: tap_code(KC_T); return 1;                        // X -> T
        case KC_Y: MAGIC_STRING(/*y*/"ear", KC_R); return 3;        // Y -> EAR
        case KC_Z: MAGIC_STRING(/*w*/"one", KC_E); return 3;        // Z -> ONE
        case KC_SPC: MAGIC_STRING(/* */"and", KC_D); return 3;      // spc -> AND
        case KC_QUOT:
        // case KC_DQOT:
            // Shouldn't happen because of auto shift but...
            if ((mods & MOD_MASK_SHIFT) != 0) {
            return 0;
            }
            else {
            MAGIC_STRING(/*'*/"re", KC_E); return 2;                // ' -> 'RE
            }
        }
    }
    return 0;
}

// Option 3: (2tap)
static int process_magic_key_3(uint16_t keycode, uint8_t mods) {
    if ((mods & ~MOD_MASK_SHIFT) == 0) {
        switch (keycode) {
        // For navigating next/previous search results in Vim:
        // N -> Shift + N, Shift + N -> N.
        // case KC_N: return KC_N;

        // Behavior for Magic Sturdy's "magic" key -- HOLD
        case KC_A: MAGIC_STRING(/*a*/"ble", KC_E); return 3;        // A -> BLE
        case KC_B: MAGIC_STRING(/*a*/"efore", KC_E); return 5;      // B -> EFORE
        case KC_C: tap_code(KC_R); return 1;                        // C -> R
        case KC_D: MAGIC_STRING(/*d*/"uring", KC_G); return 5;      // D -> URING
        case KC_E: MAGIC_STRING(/*e*/"xp", KC_P); return 2;         // E -> XP
        case KC_F: MAGIC_STRING(/*f*/"alse", KC_E); return 4;       // F -> ALSE
        case KC_G: tap_code(KC_Y); return 1;                        // G -> Y
        case KC_H: MAGIC_STRING(/*h*/"aving", KC_G); return 5;      // H -> AVING
        case KC_I: MAGIC_STRING(/*i*/"ous", KC_S); return 3;        // I -> OUS
        case KC_J: MAGIC_STRING(/*j*/"ob", KC_B); return 2;         // J -> OB
        case KC_K: MAGIC_STRING(/*k*/"new", KC_W); return 3;        // K -> new
        case KC_L: MAGIC_STRING(/*l*/"at", KC_T); return 2;         // L -> AT
        case KC_M: MAGIC_STRING(/*m*/"ore", KC_E); return 3;        // M -> ORE
        case KC_N: MAGIC_STRING(/*n*/"ext", KC_T); return 3;        // N -> EXT
        case KC_O: MAGIC_STRING(/*o*/"ver", KC_R); return 3;        // O -> VER
        case KC_P: MAGIC_STRING(/*p*/"lease", KC_E); return 5;      // P -> LEASE
        case KC_Q: tap_code(KC_I); return 1;                        // Q -> I
        case KC_QU: tap_code(KC_I); return 1;                       // QU -> I
        case KC_R: MAGIC_STRING(/*r*/"ight", KC_T); return 4;       // R -> IGHT
        case KC_S: MAGIC_STRING(/*s*/"how", KC_W); return 3;        // S -> HOW
        case KC_T: MAGIC_STRING(/*t*/"ment", KC_T); return 4;       // T -> MENT
        case KC_U: MAGIC_STRING(/*u*/"se", KC_E); return 2;         // U -> SE
        case KC_V: MAGIC_STRING(/*v*/"iew", KC_W); return 3;        // V -> IEW
        case KC_W: MAGIC_STRING(/*w*/"ork", KC_K); return 3;        // W -> ORK
        case KC_X: MAGIC_STRING(/*x*/"eno", KC_O); return 3;        // X -> ENO
        case KC_Y: tap_code(KC_C); return 1;                        // Y -> C
        case KC_Z: MAGIC_STRING(/*z*/"oom", KC_M); return 3;        // Z -> OOM
        case KC_SPC: MAGIC_STRING(/* */"of", KC_F); return 2;       // spc -> OF
        case KC_QUOT:
        // case KC_DQOT:
            // Shouldn't happen because of auto shift but...
            if ((mods & MOD_MASK_SHIFT) != 0) {
            return 0;
            }
            else {
            MAGIC_STRING(/*'*/"ve", KC_E); return 2;                // ' -> 'VE
            }
        }
    }
    return 0;
}
// Option 4 (2Hold)
static int process_magic_key_4(uint16_t keycode, uint8_t mods) {
    if ((mods & ~MOD_MASK_SHIFT) == 0) {
        switch (keycode) {
        // For navigating next/previous search results in Vim:
        // N -> Shift + N, Shift + N -> N.
        // case KC_N: return KC_N;

        // Behavior for Magic Sturdy's "magic" key -- HOLD
        case KC_A: MAGIC_STRING(/*a*/"bout", KC_T); return 4;       // A -> BOUT
        case KC_B: MAGIC_STRING(/*b*/"etween", KC_N); return 6;     // B -> ETWEEN
        case KC_C: MAGIC_STRING(/*c*/"or", KC_R); return 2;         // C -> OR
        case KC_D: MAGIC_STRING(/*d*/"ifferent", KC_E); return 8;   // D -> IFFERENT
        case KC_E: MAGIC_STRING(/*e*/"very", KC_Y); return 4;       // E -> VERY
        case KC_F: MAGIC_STRING(/*f*/"urther", KC_R); return 6;     // F -> URTHER
        case KC_G: MAGIC_STRING(/*g*/"eneral", KC_L); return 6;     // G -> ENERAL
        case KC_H: MAGIC_STRING(/*h*/"owever", KC_R); return 6;     // H -> OWEVER
        case KC_I: MAGIC_STRING(/*i*/"ble", KC_E); return 3;        // I -> BLE
        case KC_J: MAGIC_STRING(/*j*/"udge", KC_T); return 4;       // J -> UDGE
        case KC_K: MAGIC_STRING(/*k*/"eep", KC_P); return 3;        // K -> EEP
        case KC_L: MAGIC_STRING(/*l*/"ike", KC_E); return 3;        // L -> IKE
        case KC_M: MAGIC_STRING(/*m*/"anage", KC_E); return 5;      // M -> ANAGE
        case KC_N: MAGIC_STRING(/*n*/"othing", KC_G); return 6;     // N -> OTHING
        case KC_O: MAGIC_STRING(/*o*/"riginal", KC_L); return 7;    // O -> RIGINAL
        case KC_P: MAGIC_STRING(/*p*/"revious", KC_S); return 8;    // P -> REVIOUS
        case KC_Q: tap_code(KC_I); return 1;                        // Q -> I
        case KC_QU: tap_code(KC_I); return 1;                       // QU -> I
        case KC_R: MAGIC_STRING(/*r*/"eview", KC_W); return 5;      // R -> EVIEW
        case KC_S: MAGIC_STRING(/*s*/"hould", KC_D); return 5;      // S -> HOULD
        case KC_T: MAGIC_STRING(/*t*/"hrough", KC_H); return 6;     // T -> HROUGH
        case KC_U: MAGIC_STRING(/*u*/"sual", KC_L); return 4;       // U -> SUAL
        case KC_V: MAGIC_STRING(/*v*/"ersion", KC_N); return 6;     // V -> ERSION
        case KC_W: MAGIC_STRING(/*w*/"hich", KC_H); return 4;       // W -> HICH
        case KC_X: MAGIC_STRING(/*ex*/"ample", KC_E); return 5;     // X -> AMPLE  // TODO: make this trigger off ex instead of X?
        case KC_Y: MAGIC_STRING(/*y*/"esterday", KC_Y); return 8;   // Y -> ESTERDAY
        case KC_Z: MAGIC_STRING(/*z*/"ealous", KC_S); return 6;     // Z -> EALOUS
        case KC_SPC: MAGIC_STRING(/* */"you", KC_U); return 3;      // spc -> YOU
        case KC_QUOT:
        // case KC_DQOT:
            // Shouldn't happen because of auto shift but...
            if ((mods & MOD_MASK_SHIFT) != 0) {
            return 0;
            }
            else {
            MAGIC_STRING(/*'*/"ve", KC_E); return 2;                // ' -> 'VE  // no other apostrophes longer than 2?
            }
        }
    }
    return 0;
}

// ----------------------------------------------------------------------------
// MAGIC COMBOS
enum combos {
    REP_TAP_C,
    REP_HOLD_C,
    MAGIC_TAP_C,
    MAGIC_HOLD_C,
    // MAGIC_TWOTAP_C,
    // MAGIC_REP_TAP_C,
    MAGIC_REP_HOLD_C,

    DOUBLE_Q_C,
};

const uint16_t PROGMEM magic_tap[] = {QK_AREP, COMBO_END};
const uint16_t PROGMEM magic_hold[] = {QK_AREP, COMBO_END};
const uint16_t PROGMEM rep_tap[] = {QK_REP, COMBO_END};
const uint16_t PROGMEM rep_hold[] = {QK_REP, COMBO_END};
// const uint16_t PROGMEM magic_rep_tap[] = {QK_AREP, QK_REP, COMBO_END};
const uint16_t PROGMEM magic_rep_hold[] = {QK_AREP, QK_REP, COMBO_END};

const uint16_t PROGMEM double_q_tap[] = {KC_QU, QK_REP, COMBO_END};

combo_t key_combos[] = {
    [REP_TAP_C] = COMBO(rep_tap, QK_REP),
    [MAGIC_TAP_C] = COMBO(magic_tap, QK_AREP),
    [MAGIC_HOLD_C] = COMBO(magic_hold, ALTREP2),
    // [MAGIC_REP_TAP_C] = COMBO(magic_rep_tap, ALTREP3),
    // [MAGIC_TWOTAP_C] = COMBO(magic_twotap, ALTREP3),
    [REP_HOLD_C] = COMBO(rep_hold, ALTREP3),
    [MAGIC_REP_HOLD_C] = COMBO(magic_rep_hold, ALTREP4),
    // [MAGIC_REP_TAP_C] = COMBO(magic_rep_tap, ALTREP4)

    [DOUBLE_Q_C] = COMBO(double_q_tap, KC_Q),
};

bool get_combo_must_tap(uint16_t index, combo_t *combo) {
    switch (index) {
        case MAGIC_TAP_C:  // fallthrough intended obvs
        case REP_TAP_C:
        // case MAGIC_REP_TAP_C:
            return true;
    }
    return false;
};

bool get_combo_must_hold(uint16_t index, combo_t *combo) {
    switch (index) {
        case REP_HOLD_C:  // fallthrough intended obvs
        case MAGIC_HOLD_C:
        case MAGIC_REP_HOLD_C:
            return true;
    }
    return false;
}

uint16_t get_combo_term(uint16_t index, combo_t *combo) {
    switch (index) {
        case MAGIC_REP_HOLD_C:
            return COMBO_TERM;
        case MAGIC_HOLD_C:
            return COMBO_TERM+50;
        case REP_HOLD_C:
            return COMBO_TERM+200;
    }

    return COMBO_TERM;
}

bool get_combo_must_press_in_order(uint16_t index, combo_t *combo) {
    return false;
}

// void process_combo_event(uint16_t combo_index, bool pressed) {
//     int rep_keycode = get_alt_repeat_key_keycode();
//     int rep_mods = get_last_mods();
    
//     switch(combo_index) {
//         case MAGIC_REP_TAP_C:
//             if (pressed) {
//                 register_code16(QK_AREP);
//                 SS_DELAY(200);
//                 unregister_code16(QK_AREP);
//             }
//             break;
//         case MAGIC_REP_HOLD_C:
//             if (pressed) {
//                 process_magic_key_2(rep_keycode, rep_mods);
//             }
//     }
// }

///////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
// ----------------------------------------------------------------------------
// OS DETECTION HANDLING
// https://docs.qmk.fm/#/feature_os_detection
bool hasos = false;
os_variant_t hostos;
static void get_os(void) {
    if (!hasos) {
        hostos = detected_host_os();
        hasos = true;
    }
    // Auto switch mode
    // BUG: this presses the y key on windows.
    if (hostos == OS_MACOS) {
        keymap_config.swap_lctl_lgui = true;
        // process_magic(CG_SWAP, record);  // start off by swapping control and GUI(command)
    }
    else {
        keymap_config.swap_lctl_lgui = false;
        // process_magic(CG_NORM, record);  // keep control instead of gui
    }
}
// ----------------------------------------------------------------------
// AUTO SHIFT
// Prevent repeating for these characters:
bool get_auto_shift_no_auto_repeat(uint16_t keycode, keyrecord_t *record) {
    return true;
}
// Timeout
uint16_t get_autoshift_timeout(uint16_t keycode, keyrecord_t *record) {
    // https://docs.qmk.fm/#/feature_auto_shift
    switch (keycode) {
        case AUTO_SHIFT_NUMERIC:
            return get_generic_autoshift_timeout();
        case AUTO_SHIFT_SPECIAL:
            return get_generic_autoshift_timeout();
        case AUTO_SHIFT_ALPHA:
            return get_generic_autoshift_timeout();
        default:
            return get_generic_autoshift_timeout();
    }
}
// Get AutoShift eligiblity
bool get_auto_shifted_key(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    // AutoShift Block List:
        case KC_LBRC:  // we have our own { key, no shift
        case C_BKSP:  // don't shift backspace.
        case QK_AREP:  // don't shift magic
        case ALTREP2:
        case ALTREP3:
        case ALTREP4:
        case QK_REP:  // don't shift repeat
            return false;

    // - - - - - - - - - - - - - -
    // AutoShift Allow List:

    // Custom symbols processing (to avoid clash with KC_LBRC)
        case KC_MINUS ... KC_EQUAL:
        case KC_BACKSLASH ... KC_SLASH:

    // Custom characters processing
        // case KC_UNDO:  // analogue for redo/C(KC_Y)
        case KC_PRNT:  // C_S(P), e.g. VSCode, Private Mode
        // case KC_SLAL:  // C_S(A)  https://defkey.com/what-means/ctrl-shift-a
        case KC_CNEW:  // C_S(N)  https://defkey.com/what-means/ctrl-shift-n
        // case KC_RFSH:  // C_S(R)
        case KC_SAVE:  // C_S(S) save as
        case KC_ATAB: // Alt tab the other way I guess

        case KC_9PRC:  // 9 vs ( macro
        case KC_CANG: // , vs < macro

        case KC_QU:  // QU vs Q

    // Default behaviour:
#    ifndef NO_AUTO_SHIFT_ALPHA
        case KC_A ... KC_Z:
#    endif
#    ifndef NO_AUTO_SHIFT_NUMERIC
        case KC_1 ... KC_0:
#    endif
#    ifndef NO_AUTO_SHIFT_SPECIAL
#    ifndef NO_AUTO_SHIFT_TAB
        case KC_TAB:
#    endif
#    ifndef NO_AUTO_SHIFT_SYMBOLS
        // Not used, we have our own symbol shift above.
        case AUTO_SHIFT_SYMBOLS:
#    endif
#    endif
#    ifdef AUTO_SHIFT_ENTER
        case KC_ENT:
#    endif
        return true;
    }
    return get_custom_auto_shifted_key(keycode, record);
}
// Custom Keys
bool get_custom_auto_shifted_key(uint16_t keycode, keyrecord_t *record) {
    switch(keycode) {
        case KC_9PRC:  // 9 and ( but ( should activate a parentheses macro
            return true;
        case KC_CANG:  // , and < but < should activate a bracket macro
            return true;
        case KC_DQOT:  // ' and " but " should activate a double quote macro ""
            return true;
        default:
            return false;
    }
}
void autoshift_press_user(uint16_t keycode, bool shifted, keyrecord_t *record) {
    switch(keycode) {
        case KC_9PRC:
            if (!shifted) {
                register_code16(KC_9);
            }
            else {
                // BRACKET MACRO GOES HERE FOR SOME REASON
                // WON'T WORK IN NORMAL SPOT
                // register_code16(C_LPARN);
                // unregister_code16(C_LPARN);
                clear_oneshot_mods();  // Temporarily disable mods.
                unregister_mods(MOD_MASK_CSAG);
                SEND_STRING("()");
                tap_code(KC_LEFT);  // move cursor back into ()
                set_last_keycode(KC_9PRC);
            }
            break;
        case KC_CANG:
            if (!shifted) {
                register_code16(KC_COMM);
            }
            else {
                // BRACKET MACRO GOES HERE FOR SOME REASON
                // WON'T WORK IN NORMAL SPOT
                // register_code16(C_LANGB);
                // unregister_code16(C_LANGB);
                clear_oneshot_mods();  // Temporarily disable mods.
                unregister_mods(MOD_MASK_CSAG);
                SEND_STRING("<>");
                tap_code(KC_LEFT);  // move cursor back into ()
                set_last_keycode(KC_LABK);
            }
            break;
        case KC_DQOT:
            if (!shifted) {
                register_code16(KC_QUOT);
            }
            else {
                clear_oneshot_mods();  // Temporarily disable mods.
                unregister_mods(MOD_MASK_CSAG);
                SEND_STRING("\"\"");
                tap_code(KC_LEFT);  // move cursor back into ""
                set_last_keycode(KC_DQOT);
            }
            break;
        case KC_QU:
            if (!shifted) {
                MAGIC_STRING("qu", KC_QU);
            }
            else {
                MAGIC_STRING("Qu", KC_QU);
            }
            break;
        default:
            if (shifted) {
                add_weak_mods(MOD_BIT(KC_LSFT));
            }
            // & 0xFF gets the Tap key for Tap Holds, required when using Retro Shift
            register_code16((IS_RETRO(keycode)) ? keycode & 0xFF : keycode);
    }
}
void autoshift_release_user(uint16_t keycode, bool shifted, keyrecord_t *record) {
    switch(keycode) {
        case KC_9PRC:
            if (!shifted) {
                unregister_code16(KC_9);
            }
            else {
                register_mods(get_mods());
                set_last_keycode(KC_9PRC);
            }
            break;
        case KC_CANG:
            if (!shifted) {
                unregister_code16(KC_COMM);
            }
            else {
                register_mods(get_mods());
                set_last_keycode(KC_LABK);
            }
            break;
        case KC_DQOT:
            if (!shifted) {
                unregister_code16(KC_QUOT);
            }
            else {
                register_mods(get_mods());
            }
            break;
        default:
            // & 0xFF gets the Tap key for Tap Holds, required when using Retro Shift
            // The IS_RETRO check isn't really necessary here, always using
            // keycode & 0xFF would be fine.
            unregister_code16((IS_RETRO(keycode)) ? keycode & 0xFF : keycode);
    }
}
// ----------------------------------------------------------------------
// Custom Alt-Tab Key
bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;

// Custom Control-Tab Key
bool is_ctrl_tab_active = false;
uint16_t ctrl_tab_timer = 0;
// ----------------------------------------------------------------------
// Tap Hold: Tap vs Long-Press Processing
// https://getreuer.info/posts/keyboards/triggers/index.html#tap-vs.-long-press
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case C_BKSP:  // custom backspace
            return TAPPING_TERM + 100;
        case C_ENTR:   // custom enter
            return TAPPING_TERM + 150;
        default:
            return TAPPING_TERM + 100;  // 100 ms by default? we add 100 to the TAPPING_TERM
    }
} 
// Given a tap-hold key event, replaces the hold function with `long_press_keycode`.
static bool process_tap_or_long_press_key(
    keyrecord_t* record, uint16_t long_press_keycode) {
    if (record->tap.count == 0) {  // Key is being held.
        if (record->event.pressed) {
            set_last_keycode(long_press_keycode);
            // BUGFIX: will this allow me to process keys within keys?
            tap_code16(long_press_keycode);
            // register_code16(long_press_keycode);
            // unregister_code16(long_press_keycode);
        }
        return false;  // Skip default handling.
    }
    return true;  // Continue default handling.
}
// ----------------------------------------------------------------------
// CAPS WORD
bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // DISABLE CAPS WORD WITH THE TOGGLE FFS
        case CW_TOGG:
            return false;

        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
        case KC_MINS:
        add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to the next key.
        return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case KC_9PRC:  // custom 9()
        case C_BKSP:  // custom backspace
        case KC_BSPC:
        case KC_SPC:  // space
        case KC_DEL:
        case KC_UNDS:
        case QK_AREP:
        case QK_REP:
        case ALTREP2:
        case ALTREP3:
        case ALTREP4:
        case KC_ENT:
        case C_ENTR:
            return true;

        default:
        return false;  // Deactivate Caps Word.
    }
}
// void caps_word_set_user(bool active) {
    // NOTE: I don't think this function has ever worked.
//     // #ifdef OLED_ENABLE
//     // if (active) {
//     //     oled_write_P(PSTR("CAPWD"), true);
//     // } 
//     // else {
//     //     // Do something when Caps Word deactivates.
//     //     oled_write_P(PSTR("CAPWD"), false);
//     // }
//     // # endif // OLED_ENABLE

//     // Set caps lock to ON if caps word is enabled.
//     // if (active) {
//     //     led_usb_state.caps_lock = true;
//     // }
//     // else {
//     //     led_usb_state.caps_lock = false;
//     // }
// }
// -------------------------------------------------------------------------
// SELECT WORD

// https://getreuer.info/posts/keyboards/select-word/index.html
// Copyright 2021-2023 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

enum selwd_states {
    SELWD_STATE_NONE,        // No selection.
    SELWD_STATE_SELECTED,    // Macro released with something selected.
    SELWD_STATE_WORD,        // Macro held with word(s) selected.
    SELWD_STATE_FIRST_LINE,  // Macro held with one line selected.
    SELWD_STATE_LINE         // Macro held with multiple lines selected.
};
static uint8_t selwd_state = SELWD_STATE_NONE;

// Idle timeout timer to disable Select Word after a period of inactivity.
#if SELECT_WORD_TIMEOUT > 0
static uint16_t idle_timer = 0;
void select_word_task(void) {
    if (selwd_state && timer_expired(timer_read(), idle_timer)) {
        selwd_state = SELWD_STATE_NONE;
    }
}
#endif  // SELECT_WORD_TIMEOUT > 0

// Process Word Selection
bool process_select_word(uint16_t keycode, keyrecord_t* record,
                         uint16_t sel_keycode) {
    if (keycode == KC_LSFT || keycode == KC_RSFT) {
        return true;
    }

    #if SELECT_WORD_TIMEOUT > 0
    idle_timer = record->event.time + SELECT_WORD_TIMEOUT;
    #endif  // SELECT_WORD_TIMEOUT > 0

    if (keycode == sel_keycode && record->event.pressed) {  // On key press.
        const uint8_t mods = get_mods();
    #ifndef NO_ACTION_ONESHOT
        const bool shifted = (mods | get_oneshot_mods()) & MOD_MASK_SHIFT;
        clear_oneshot_mods();
    #else
        const bool shifted = mods & MOD_MASK_SHIFT;
    #endif  // NO_ACTION_ONESHOT

        if (!shifted) {  // Select word.
            get_os();
            if (keymap_config.swap_lctl_lgui) {
                set_mods(MOD_BIT(KC_LALT));  // Hold Left Alt (Option).
                // set_mods(mod_config(MOD_LALT));
            }
            else {
                set_mods(MOD_BIT(KC_LCTL));  // Hold Left Ctrl.
                // set_mods(mod_config(MOD_LCTL));
            }
            if (selwd_state == SELWD_STATE_NONE) {
                // On first use, tap Ctrl+Right then Ctrl+Left (or with Alt on Mac) to
                // ensure the cursor is positioned at the beginning of the word.
                send_keyboard_report();
                tap_code(KC_RGHT);
                tap_code(KC_LEFT);
            }
            register_mods(MOD_BIT(KC_LSFT));
            register_code(KC_RGHT);
            selwd_state = SELWD_STATE_WORD;
        } else {  // Select line.
            if (selwd_state == SELWD_STATE_NONE) {
                if (keymap_config.swap_lctl_lgui) {
                    // MAC:
                    // Tap GUI (Command) + Left, then Shift + GUI + Right.
                    set_mods(MOD_BIT(KC_LGUI));
                    send_keyboard_report();
                    tap_code(KC_LEFT);
                    register_mods(MOD_BIT(KC_LSFT));
                    tap_code(KC_RGHT);
                }
                else {
                    // Tap Home, then Shift + End.
                    clear_mods();
                    send_keyboard_report();
                    tap_code(KC_HOME);
                    register_mods(MOD_BIT(KC_LSFT));
                    tap_code(KC_END);
                }
                set_mods(mods);
                selwd_state = SELWD_STATE_FIRST_LINE;
            } else {
                register_code(KC_DOWN);
                selwd_state = SELWD_STATE_LINE;
            }
        }
        return false;
    }

    // `sel_keycode` was released, or another key was pressed.
    switch (selwd_state) {
        case SELWD_STATE_WORD:
            unregister_code(KC_RGHT);
            if (keymap_config.swap_lctl_lgui) {
                unregister_mods(MOD_BIT(KC_LSFT) | MOD_BIT(KC_LALT));
            }
            else {
                unregister_mods(MOD_BIT(KC_LSFT) | MOD_BIT(KC_LCTL));
            }
            selwd_state = SELWD_STATE_SELECTED;
            break;

        case SELWD_STATE_FIRST_LINE:
            selwd_state = SELWD_STATE_SELECTED;
            break;

        case SELWD_STATE_LINE:
            unregister_code(KC_DOWN);
            selwd_state = SELWD_STATE_SELECTED;
            break;

        case SELWD_STATE_SELECTED:
            if (keycode == KC_ESC) {
                tap_code(KC_RGHT);
                selwd_state = SELWD_STATE_NONE;
                return false;
            }
            // Fallthrough intended.
        default:
            selwd_state = SELWD_STATE_NONE;
    }

    return true;
}
// -----------------------------------------------------------------------------
// SENTENCE CASE

// https://getreuer.info/posts/keyboards/sentence-case/index.html
// Copyright 2022 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


enum sentence_case_state {
    SNCSE_STATE_INIT,     /**< Initial enabled state. */
    SNCSE_STATE_WORD,     /**< Within a word. */
    SNCSE_STATE_ABBREV,   /**< Within an abbreviation like "e.g.". */
    SNCSE_STATE_ENDING,   /**< Sentence ended. */
    SNCSE_STATE_PRIMED,   /**< "Primed" state, in the space following an ending. */
    SNCSE_STATE_DISABLED, /**< Sentence Case is disabled. */
};
#if SENTENCE_CASE_TIMEOUT > 0
static uint16_t idle_timer = 0;
#endif  // SENTENCE_CASE_TIMEOUT > 0
#if SENTENCE_CASE_BUFFER_SIZE > 1
static uint16_t key_buffer[SENTENCE_CASE_BUFFER_SIZE] = {0};
#endif  // SENTENCE_CASE_BUFFER_SIZE > 1
static uint8_t state_history[SENTENCE_CASE_STATE_HISTORY_SIZE];
static uint16_t suppress_key = KC_NO;
static uint8_t sentence_state = SNCSE_STATE_INIT;

// Sets the current state to `new_state`.
static void set_sentence_state(uint8_t new_state) {
#ifndef NO_DEBUG
    if (debug_enable && sentence_state != new_state) {
        static const char* state_names[] = {
            "INIT", "WORD", "ABBREV", "ENDING", "PRIMED", "DISABLED",
        };
        dprintf("Sentence case: %s\n", state_names[new_state]);
    }
#endif  // NO_DEBUG

    const bool primed = (new_state == SNCSE_STATE_PRIMED);
    if (primed != (sentence_state == SNCSE_STATE_PRIMED)) {
        sentence_case_primed(primed);
    }
    sentence_state = new_state;
}

static void clear_state_history(void) {
#if SENTENCE_CASE_TIMEOUT > 0
    idle_timer = 0;
#endif  // SENTENCE_CASE_TIMEOUT > 0
    memset(state_history, SNCSE_STATE_INIT, sizeof(state_history));
    if (sentence_state != SNCSE_STATE_DISABLED) {
        set_sentence_state(SNCSE_STATE_INIT);
    }
}

void sentence_case_clear(void) {
    clear_state_history();
    suppress_key = KC_NO;
#if SENTENCE_CASE_BUFFER_SIZE > 1
    memset(key_buffer, 0, sizeof(key_buffer));
#endif  // SENTENCE_CASE_BUFFER_SIZE > 1
}

void sentence_case_on(void) {
    if (sentence_state == SNCSE_STATE_DISABLED) {
        sentence_state = SNCSE_STATE_INIT;
        sentence_case_clear();
    }
}

void sentence_case_off(void) {
    if (sentence_state != SNCSE_STATE_DISABLED) {
        set_sentence_state(SNCSE_STATE_DISABLED);
    }
}

void sentence_case_toggle(void) {
    if (sentence_state != SNCSE_STATE_DISABLED) {
        sentence_case_off();
    } else {
        sentence_case_on();
    }
}

bool is_sentence_case_on(void) { return sentence_state != SNCSE_STATE_DISABLED; }

#if SENTENCE_CASE_TIMEOUT > 0
#if SENTENCE_CASE_TIMEOUT < 100 || SENTENCE_CASE_TIMEOUT > 30000
// Constrain timeout to a sensible range. With the 16-bit timer, the longest
// representable timeout is 32768 ms, rounded here to 30000 ms = half a minute.
#error "sentence_case: SENTENCE_CASE_TIMEOUT must be between 100 and 30000 ms"
#endif

void sentence_case_task(void) {
    if (idle_timer && timer_expired(timer_read(), idle_timer)) {
        clear_state_history();  // Timed out; clear all state.
    }
}
#endif  // SENTENCE_CASE_TIMEOUT > 0

bool process_sentence_case(uint16_t keycode, keyrecord_t* record) {
    // Only process while enabled, and only process press events.
    if (sentence_state == SNCSE_STATE_DISABLED || !record->event.pressed) {
        return true;
    }

    #if SENTENCE_CASE_TIMEOUT > 0
    idle_timer = (record->event.time + SENTENCE_CASE_TIMEOUT) | 1;
    #endif  // SENTENCE_CASE_TIMEOUT > 0

    switch (keycode) {
    #ifndef NO_ACTION_TAPPING
        case QK_MOD_TAP ... QK_MOD_TAP_MAX:
        if (record->tap.count == 0) {
            return true;
        }
        keycode = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
        break;
    #ifndef NO_ACTION_LAYER
        case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
    #endif  // NO_ACTION_LAYER
        if (record->tap.count == 0) {
            return true;
        }
        keycode = QK_LAYER_TAP_GET_TAP_KEYCODE(keycode);
        break;
    #endif  // NO_ACTION_TAPPING

    #ifdef SWAP_HANDS_ENABLE
        case QK_SWAP_HANDS ... QK_SWAP_HANDS_MAX:
        if (IS_SWAP_HANDS_KEYCODE(keycode) || record->tap.count == 0) {
            return true;
        }
        keycode = QK_SWAP_HANDS_GET_TAP_KEYCODE(keycode);
        break;
    #endif  // SWAP_HANDS_ENABLE
    }

    if (keycode == KC_BSPC) {
        // Backspace key pressed. Rewind the state and key buffers.
        // BUG: I don't think sentence case will work with control backspace...
        set_sentence_state(state_history[SENTENCE_CASE_STATE_HISTORY_SIZE - 1]);

        memmove(state_history + 1, state_history, SENTENCE_CASE_STATE_HISTORY_SIZE - 1);
        state_history[0] = SNCSE_STATE_INIT;
    #if SENTENCE_CASE_BUFFER_SIZE > 1
        memmove(key_buffer + 1, key_buffer,
                (SENTENCE_CASE_BUFFER_SIZE - 1) * sizeof(uint16_t));
        key_buffer[0] = KC_NO;
    #endif  // SENTENCE_CASE_BUFFER_SIZE > 1
        return true;
    }

    const uint8_t mods = get_mods() | get_weak_mods() | get_oneshot_mods();
    uint8_t new_state = SNCSE_STATE_INIT;

    // We search for sentence beginnings using a simple finite state machine. It
    // matches things like "a. a" and "a.  a" but not "a.. a" or "a.a. a". The
    // state transition matrix is:
    //
    //             'a'       '.'      ' '      '\''
    //           +-------------------------------------
    //   INIT    | WORD      INIT     INIT     INIT
    //   WORD    | WORD      ENDING   INIT     WORD
    //   ABBREV  | ABBREV    ABBREV   INIT     ABBREV
    //   ENDING  | ABBREV    INIT     PRIMED   ENDING
    //   PRIMED  | match!    INIT     PRIMED   PRIMED
    char code = sentence_case_press_user(keycode, record, mods);
    dprintf("Sentence Case: code = '%c' (%d)\n", code, (int)code);
    switch (code) {
        case '\0':  // Current key should be ignored.
        return true;

        case 'a':  // Current key is a letter.
        switch (sentence_state) {
            case SNCSE_STATE_ABBREV:
            case SNCSE_STATE_ENDING:
            new_state = SNCSE_STATE_ABBREV;
            break;

            case SNCSE_STATE_PRIMED:
            // This is the start of a sentence.
            if (keycode != suppress_key) {
                suppress_key = keycode;
                set_oneshot_mods(MOD_BIT(KC_LSFT));  // Shift mod to capitalize.
                new_state = SNCSE_STATE_WORD;
            }
            break;

            default:
            new_state = SNCSE_STATE_WORD;
        }
        break;

        case '.':  // Current key is sentence-ending punctuation.
        switch (sentence_state) {
            case SNCSE_STATE_WORD:
            new_state = SNCSE_STATE_ENDING;
            break;

            default:
            new_state = SNCSE_STATE_ABBREV;
        }
        break;

        case ' ':  // Current key is a space.
        if (sentence_state == SNCSE_STATE_PRIMED ||
            (sentence_state == SNCSE_STATE_ENDING
    #if SENTENCE_CASE_BUFFER_SIZE > 1
            && sentence_case_check_ending(key_buffer)
    #endif  // SENTENCE_CASE_BUFFER_SIZE > 1
                )) {
            new_state = SNCSE_STATE_PRIMED;
            suppress_key = KC_NO;
        }
        break;

        case '\'':  // Current key is a quote.
        new_state = sentence_state;
        break;
    }

    // Slide key_buffer and state_history buffers one element to the left.
    // Optimization note: Using manual loops instead of memmove() here saved
    // ~100 bytes on AVR.
    #if SENTENCE_CASE_BUFFER_SIZE > 1
    for (int8_t i = 0; i < SENTENCE_CASE_BUFFER_SIZE - 1; ++i) {
        key_buffer[i] = key_buffer[i + 1];
    }
    #endif  // SENTENCE_CASE_BUFFER_SIZE > 1
    for (int8_t i = 0; i < SENTENCE_CASE_STATE_HISTORY_SIZE - 1; ++i) {
        state_history[i] = state_history[i + 1];
    }

    #if SENTENCE_CASE_BUFFER_SIZE > 1
    key_buffer[SENTENCE_CASE_BUFFER_SIZE - 1] = keycode;
    if (new_state == SNCSE_STATE_ENDING && !sentence_case_check_ending(key_buffer)) {
        dprintf("Not a real ending.\n");
        new_state = SNCSE_STATE_INIT;
    }
    #endif  // SENTENCE_CASE_BUFFER_SIZE > 1
    state_history[SENTENCE_CASE_STATE_HISTORY_SIZE - 1] = sentence_state;

    set_sentence_state(new_state);
    return true;
}

bool sentence_case_just_typed_P(const uint16_t* buffer, const uint16_t* pattern,
                                int8_t pattern_len) {
    #if SENTENCE_CASE_BUFFER_SIZE > 1
    buffer += SENTENCE_CASE_BUFFER_SIZE - pattern_len;
    for (int8_t i = 0; i < pattern_len; ++i) {
        if (buffer[i] != pgm_read_word(pattern + i)) {
        return false;
        }
    }
    return true;
    #else
    return false;
    #endif  // SENTENCE_CASE_BUFFER_SIZE > 1
}

__attribute__((weak)) bool sentence_case_check_ending(const uint16_t* buffer) {
    #if SENTENCE_CASE_BUFFER_SIZE >= 5
    // Don't consider the abbreviations "vs." and "etc." to end the sentence.
    if (SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_V, KC_S, KC_DOT) ||
        SENTENCE_CASE_JUST_TYPED(KC_SPC, KC_E, KC_T, KC_C, KC_DOT)) {
        return false;  // Not a real sentence ending.
    }
    #endif  // SENTENCE_CASE_BUFFER_SIZE >= 5
    return true;  // Real sentence ending; capitalize next letter.
}

__attribute__((weak)) char sentence_case_press_user(uint16_t keycode,
                                                    keyrecord_t* record,
                                                    uint8_t mods) {
    if ((mods & ~(MOD_MASK_SHIFT | MOD_BIT(KC_RALT))) == 0) {
        const bool shifted = mods & MOD_MASK_SHIFT;
        switch (keycode) {
        case KC_LCTL ... KC_RGUI:  // Mod keys.
            return '\0';  // These keys are ignored.

        case KC_A ... KC_Z:
            return 'a';  // Letter key.

        case KC_DOT:  // . is punctuation, Shift . is a symbol (>)
            return !shifted ? '.' : '#';
        case KC_1:
        case KC_SLSH:
            return shifted ? '.' : '#';
        case KC_2 ... KC_0:  // 2 3 4 5 6 7 8 9 0
        case KC_MINS ... KC_SCLN:  // - = [ ] ; backslash
        case KC_GRV:
        case KC_COMM:
            return '#';  // Symbol key.

        case KC_SPC:
            return ' ';  // Space key.

        case KC_QUOT:
            return '\'';  // Quote key.
        case KC_DQOT:
            return '\'';  // Quote key alternate
        }
    }

    // Otherwise clear Sentence Case to initial state.
    sentence_case_clear();
    return '\0';
}

__attribute__((weak)) void sentence_case_primed(bool primed) {}

///////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// Process Record
bool process_record_user(uint16_t keycode, keyrecord_t *record) {

    // MACROS OVERWRITING BEHAVIOUR:
    // Sentence Case
    if (!process_sentence_case(keycode, record)) { return false; }

    // Select Word
    if (!process_select_word(keycode, record, KC_SLWD)) { return false; }

    // int rep_keycode = get_alt_repeat_key_keycode();
    // int rep_mods = get_mods();

    // If alt repeating a key A-Z with no mods other than Shift, set the last key
    // to KC_N. Above, alternate repeat of KC_N is defined to be again KC_N. This
    // way, either tapping alt repeat and then repeat (or double tapping alt
    // repeat) is useful to type certain patterns without SFBs:
    //
    //   O <altrep> <rep> -> OAN (as in "loan")
    //   D <altrep> <rep> -> DYN (as in "dynamic")

    if (get_repeat_key_count() < 0 && KC_A <= keycode && keycode <= KC_Z && keycode != KC_N && (get_last_mods() & ~MOD_MASK_SHIFT) == 0) {
        set_last_keycode(KC_N);
        set_last_mods(0);
    }

    // Clear alt-tab behaviour
    if (is_alt_tab_active) {
        if (timer_elapsed(alt_tab_timer) > ALT_TAB_TIMEOUT) {
            if (keymap_config.swap_lctl_lgui) {
                unregister_mods(MOD_LGUI);
            }
            else {
                unregister_mods(MOD_LALT);
            }
            is_alt_tab_active = false;
        }
    }

    // Clear control-tab behaviour
    if (is_ctrl_tab_active) {
        if (timer_elapsed(ctrl_tab_timer) > ALT_TAB_TIMEOUT) {
            unregister_mods(MOD_LCTL);
            is_ctrl_tab_active = false;
        }
    }

    switch (keycode) {
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // Base Layer Switchers
        case KC_QWERTY:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_QWERTY);
            }
            return false;
        case KC_STRDY:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_STRDY);
            }
            return false;

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // MAGIC
        case ALTREP2: 
            if (record->event.pressed) {
                process_magic_key_2(get_last_keycode(), get_last_mods());
            }
            return false;
        case ALTREP3:
            if (record->event.pressed) {
                process_magic_key_3(get_last_keycode(), get_last_mods());
            }
            return false;
        case ALTREP4:
            if (record->event.pressed) {
                process_magic_key_4(get_last_keycode(), get_last_mods());
            }
            return false;

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // New Keycodes
        // case KC_QU:
        //     if (record->event.pressed) {
        //         register_code(KC_Q);
        //         register_code(KC_U);
        //     }
        //     else {
        //         unregister_code(KC_Q);
        //         unregister_code(KC_U);
        //     }
        //     return false;

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // TAP HOLD
        case C_BKSP:
            if (record->event.pressed) {
                // Custom backspace
                // Delete both brackets when backspacing afterwards
                // DOES NOT WORK.
                switch (get_last_keycode()) {
                    case KC_9PRC:
                    case KC_LPRN:
                    case KC_CANG:
                    case KC_LABK:
                    case KC_CBRC:
                    case KC_SBRC:
                        tap_code(KC_DEL);
                }
                return process_tap_or_long_press_key(record, LCTL(KC_BSPC));  // on long press
            } else {
                return true;  // THIS MUST BE TRUE OTHERWISE SHORT BACKSPACE WON'T STOP
            }
        case C_ENTR:
            if (record->event.pressed) {
                return process_tap_or_long_press_key(record, LCTL(KC_ENT));  // on long press, control enter
            }
            break;
        case C_PRVWD:
            if (record->event.pressed) {
                return process_tap_or_long_press_key(record, KC_LSTRT);  // on long press, line start
            }
            break;
        case C_NXTWD:
            if (record->event.pressed) {
                return process_tap_or_long_press_key(record, KC_LEND);  // on long press, line end
            }
            break;
        case C_BRC:
            if (record->event.pressed) {
                return process_tap_or_long_press_key(record, KC_CBRC);  // on long press, curly brackets
            }
            break;
        case C_SRCH:
            if (record->event.pressed) {
                // Custom search
                return process_tap_or_long_press_key(record, KC_SRCHB);  // on long press, open browser before searching
            }
            break;

        // - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // New Shortcuts and Commands
        case KC_PRVWD:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    register_mods(mod_config(MOD_LALT));
                    register_code(KC_LEFT);
                } else {
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_LEFT);
                }
                return false;
                break;
            }
            else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LALT));
                    unregister_code(KC_LEFT);
                } else {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_LEFT);
                }
                return false;
                break;
            }
        case KC_NXTWD:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                register_mods(mod_config(MOD_LALT));
                register_code(KC_RIGHT);
                } else {
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_RIGHT);
                }
                return false;
                break;
            }
            else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LALT));
                    unregister_code(KC_RIGHT);
                } else {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_RIGHT);
                }
                return false;
                break;
            }
        case KC_LSTRT:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    //CMD-arrow on Mac, but we have CTL and GUI swapped
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_LEFT);
                } else {
                    register_code(KC_HOME);
                }
                return false;
                break;
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_LEFT);
                } else {
                    unregister_code(KC_HOME);
                }
                return false;
                break;
            }
        case KC_LEND:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    //CMD-arrow on Mac, but we have CTL and GUI swapped
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_RIGHT);
                } else {
                    register_code(KC_END);
                }
                return false;
                break;
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_RIGHT);
                } else {
                    unregister_code(KC_END);
                }
                return false;
                break;
            }
        case KC_DLINE:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_BSPC);
                return false;
                break;
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_BSPC);
                return false;
                break;
            }
        case KC_COPY:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_C);
                return false;
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_C);
                return false;
            }
        case KC_PASTE:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_V);
                return false;
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_V);
                return false;
            }
        case KC_CUT:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_X);
                return false;
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_X);
                return false;
            }
        case KC_UNDO:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_Z);
                return false;
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_Z);
                return false;
            }
        case KC_REDO:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_Y);
                return false;
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_Y);
                return false;
            }
        case KC_FIND:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_F);
                return false;
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_F);
                return false;
            }
        case KC_PRNT:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_P);
                return false;
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_P);
                return false;
            }
        case KC_SLAL:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_A);
                return false;
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_A);
                return false;
            }
        case KC_CNEW:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_N);
                return false;
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_N);
                return false;
            }
        case KC_TNEW:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_T);
                return false;
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_T);
                return false;
            }
        case KC_CLSE:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_W);
                return false;
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_W);
                return false;
            }
        case KC_RFSH:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_R);
                return false;
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_R);
                return false;
            }
        case KC_SAVE:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_S);
                return false;
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_S);
                return false;
            }
        case KC_ATAB:
            // Special Alt-Tab key behaviour:
            // https://docs.splitkb.com/hc/en-us/articles/360010513760-How-can-I-use-a-rotary-encoder-
            // Start by pressing ALT(CMD)-TAB
            // Hold ALT(CMD), and press TAB to cycle
            // Stop holding ALT(CMD) after finished
            if (record->event.pressed) {
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
                    get_os();

                    if (keymap_config.swap_lctl_lgui) {
                        register_mods(MOD_LGUI);
                    }
                    else {
                        register_mods(MOD_LALT);
                    }
                }
                alt_tab_timer = timer_read();
                register_code(KC_TAB);
                return false;
                break;
            } else {
                unregister_code(KC_TAB);
                return false;
            }
        case KC_CTAB:
            // Control tab key behaviour:
            if (record->event.pressed) {
                if (!is_ctrl_tab_active) {
                    is_ctrl_tab_active = true;
                    register_mods(MOD_LCTL);
                }
                ctrl_tab_timer = timer_read();
                register_code(KC_TAB);
                return false;
                break;
            } else {
                unregister_code(KC_TAB);
                return false;
            }
        case KC_ZMIN:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_EQL);
                return false;
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_EQL);
                return false;
            }
        case KC_ZMOT:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_MINS);
                return false;
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_MINS);
                return false;
            }

        // customized brackets and parens macros
        case KC_CBRC:
            if (record->event.pressed) {
                // Type {} and put cursor in middle
                MAGIC_STRING("{}", KC_CBRC);  // send {} with { as repeat
                tap_code(KC_LEFT);
                set_last_keycode(KC_CBRC);
                return false;
                break;
            }
        case KC_SBRC:
            if (record->event.pressed) {
                // Type [] and put cursor in middle
                // Shifting handled by autoshift.
                MAGIC_STRING("[]", KC_SBRC);  // send <> with < as repeat
                tap_code(KC_LEFT);
                set_last_keycode(KC_SBRC);
                return false;
                break;
            }
        case KC_9PRC:
            if (record->event.pressed) {
                set_last_keycode(KC_9PRC);
                return true;
                break;
            }
        // I've decided not to do <> because we often type less than or greater than by itself.
        // case KC_LABK:
        //     if (record->event.pressed) {
        //         // Type <> and put cursor in middle
        //         MAGIC_STRING("<>", KC_LABK);  // send <> with < as repeat
        //         tap_code(KC_LEFT);
        //         set_last_keycode(KC_LABK);
        //         return false;
        //         break;
        //     }

        // customized snipping tool macro
        case KC_SNIP:
            if (record->event.pressed) {
                get_os();
                // Using OS Detection
                // BUG: this ignores the mac/windows key.
                switch (hostos) {
                    case OS_MACOS: 
                        register_mods(MOD_LGUI);
                        register_mods(MOD_LSFT);
                        register_code(KC_4);
                        break;
                    case OS_WINDOWS:
                        register_mods(MOD_LGUI);
                        register_mods(MOD_LSFT);
                        register_code(KC_S);
                        break;
                    case OS_LINUX:
                        register_mods(MOD_LSFT);
                        register_code(KC_PSCR);
                        break;
                    case OS_UNSURE:
                        // could be android?
                        register_code(KC_PSCR);
                        break;
                    case OS_IOS:
                        // iPad has command+shift+4?
                        register_mods(MOD_LGUI);
                        register_mods(MOD_LSFT);
                        register_code(KC_4);
                        break;
                }
                return false;
                break;
            } else {
                switch (hostos) {
                    case OS_MACOS: 
                        unregister_mods(MOD_LGUI);
                        unregister_mods(MOD_LSFT);
                        unregister_code(KC_4);
                        return false;
                        break;
                    case OS_WINDOWS:
                        unregister_mods(MOD_LGUI);
                        unregister_mods(MOD_LSFT);
                        unregister_code(KC_S);
                        return false;
                        break;
                    case OS_LINUX:
                        unregister_mods(MOD_LSFT);
                        unregister_code(KC_PSCR);
                        return false;
                        break;
                    case OS_UNSURE:
                        // could be android?
                        unregister_code(KC_PSCR);
                        return false;
                        break;
                    case OS_IOS:
                        // iPad has command+shift+4?
                        unregister_mods(MOD_LGUI);
                        unregister_mods(MOD_LSFT);
                        unregister_code(KC_4);
                        return false;
                        break;
                }
                break;
            }

        // Browser search key
        case KC_SRCHB:
            if (record->event.pressed) {
                get_os();
                // copy
                if (keymap_config.swap_lctl_lgui) {
                    // MacOS Users (bleh)
                    SEND_STRING(SS_LGUI("c"));
                }
                else {
                    SEND_STRING(SS_LCTL("c"));
                }
                // open browser
                // https://gist.github.com/panzerstadt/51bce7f94859a0fda728fae1cbfbb963
                SEND_STRING(SS_DELAY(100) SS_TAP(X_LGUI) SS_DELAY(200) "browser" SS_DELAY(750) SS_TAP(X_ENT) SS_DELAY(1250));
                // open new tab and search
                if (keymap_config.swap_lctl_lgui) {
                    // MacOS Users (bleh)
                    SEND_STRING(SS_LGUI("v") SS_TAP(X_ENTER));
                }
                else {
                    SEND_STRING(SS_LCTL("v") SS_TAP(X_ENTER));
                }
                return false;
                break;
            } else {
                return false;
            }

        case KC_SRCH:
            if (record->event.pressed) {
                get_os();
                // 1. copy highlighted selection
                // register_code16(KC_COPY);  // reusing this doesn't seem to work
                if (keymap_config.swap_lctl_lgui) {
                    // MacOS Users (bleh)
                    SEND_STRING(SS_LGUI("ct") SS_DELAY(200) SS_LGUI("v") SS_TAP(X_ENTER));
                }
                else {
                    SEND_STRING(SS_LCTL("ct") SS_DELAY(200) SS_LCTL("v") SS_TAP(X_ENTER));
                }
                return false;
                break;
            }
    }
    return process_autocorrect(keycode, record);
}
/////////////////////////////////////////////////////////////////////////////////////////
// -------------------------------------------------------
// MATRIX SCAN
void matrix_scan_user(void) {
    // Check for end of alt-tab behaviour
    if (is_alt_tab_active) {
        if (timer_elapsed(alt_tab_timer) > ALT_TAB_TIMEOUT) {
            if (keymap_config.swap_lctl_lgui) {
                unregister_mods(MOD_LGUI);
            }
            else {
                unregister_mods(MOD_LALT);
            }
            is_alt_tab_active = false;
        }
    }
    if (is_ctrl_tab_active) {
        if (timer_elapsed(ctrl_tab_timer) > ALT_TAB_TIMEOUT) {
            unregister_mods(MOD_LCTL);
            is_ctrl_tab_active = false;
        }
    }
    #if SELECT_WORD_TIMEOUT > 0
    // Check for end of select word behaviour
    select_word_task();
    #endif
}
/////////////////////////////////////////////////////////////////////////////////////////
// ---------------------------------------------------------------------------------------------------------
// ENCODERS
#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
          // Mappings for 1st Encoder          // Mappings for 2nd Encoder
    [0] = { ENCODER_CCW_CW(KC_VOLU, KC_VOLD),  ENCODER_CCW_CW(KC_WH_D, KC_WH_U)  }, // QWERTY
    [1] = { ENCODER_CCW_CW(KC_VOLU, KC_VOLD),  ENCODER_CCW_CW(KC_WH_D, KC_WH_U)  }, // STRDY
    [2] = { ENCODER_CCW_CW(KC_WH_R, KC_WH_L),  ENCODER_CCW_CW(KC_WH_D, KC_WH_U)  }, // Xtnd2
    [3] = { ENCODER_CCW_CW(KC_REDO, KC_UNDO), ENCODER_CCW_CW(KC_NXTWD, KC_PRVWD) }, // Raise
    [4] = { ENCODER_CCW_CW(KC_ZMOT, KC_ZMIN), ENCODER_CCW_CW(KC_BRIU, KC_BRID) }, // Adj
    // You can add more layers here if you need them, or you can also delete lines for layers you are not using
};
#endif  // ENCODER_MAP_ENABLE

///////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// OLED: LUNA
#ifdef OLED_ENABLE
/* 32 * 32 logo */
// static void render_logo(void) {
//     static const char PROGMEM hell_logo[] = {0x00, 0x80, 0xc0, 0xc0, 0x60, 0x60, 0x30, 0x30, 0x18, 0x1c, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x80, 0xe0, 0x78, 0x1e, 0x06, 0x00, 0x0c, 0x1c, 0x18, 0x30, 0x30, 0x60, 0x60, 0xc0, 0xc0, 0x80, 0x00, 0x01, 0x03, 0x07, 0x06, 0x0c, 0x0c, 0x18, 0x18, 0x30, 0x70, 0x60, 0x00, 0xc0, 0xf0, 0x3c, 0x0f, 0x03, 0x00, 0x00, 0x00, 0x00, 0x60, 0x70, 0x30, 0x18, 0x18, 0x0c, 0x0c, 0x06, 0x07, 0x03, 0x01, 0x00, 0xf8, 0xf8, 0x80, 0x80, 0x80, 0xf8, 0xf8, 0x00, 0x80, 0xc0, 0xc0, 0x40, 0xc0, 0xc0, 0x80, 0x00, 0xf8, 0xf8, 0x00, 0xf8, 0xf8, 0x00, 0x08, 0x38, 0x08, 0x00, 0x38, 0x08, 0x30, 0x08, 0x38, 0x00, 0x1f, 0x1f, 0x01, 0x01, 0x01, 0x1f, 0x1f, 0x00, 0x0f, 0x1f, 0x1a, 0x12, 0x1a, 0x1b, 0x0b, 0x00, 0x1f, 0x1f, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//     oled_write_raw_P(hell_logo, sizeof(hell_logo));
// }

/* 32 * 14 os logos */
static const char PROGMEM windows_logo[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbc, 0xbc, 0xbe, 0xbe, 0x00, 0xbe, 0xbe, 0xbf, 0xbf, 0xbf, 0xbf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x0f, 0x0f, 0x00, 0x0f, 0x0f, 0x1f, 0x1f, 0x1f, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static const char PROGMEM mac_logo[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xf0, 0xf8, 0xf8, 0xf8, 0xf0, 0xf6, 0xfb, 0xfb, 0x38, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x0f, 0x1f, 0x1f, 0x0f, 0x0f, 0x1f, 0x1f, 0x0f, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* KEYBOARD PET START */
/* settings */
#    define MIN_WALK_SPEED      10
#    define MIN_RUN_SPEED       50

/* advanced settings */
#    define ANIM_FRAME_DURATION 200  // how long each frame lasts in ms
#    define ANIM_SIZE           96   // number of bytes in array. If you change sprites, minimize for adequate firmware size. max is 1024

/* timers */
uint32_t anim_timer = 0;

/* current frame */
uint8_t current_frame = 0;

/* status variables */
int   current_wpm = 0;
led_t led_usb_state;

bool isSneaking = false;
bool isJumping  = false;
bool showedJump = true;

/* logic */
static void render_luna(int LUNA_X, int LUNA_Y) {
    /* Sit */
    static const char PROGMEM sit[2][ANIM_SIZE] = {/* 'sit1', 32x22px */
                                                   {
                                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1c, 0x02, 0x05, 0x02, 0x24, 0x04, 0x04, 0x02, 0xa9, 0x1e, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x08, 0x68, 0x10, 0x08, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x06, 0x82, 0x7c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x0c, 0x10, 0x10, 0x20, 0x20, 0x20, 0x28, 0x3e, 0x1c, 0x20, 0x20, 0x3e, 0x0f, 0x11, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                   },

                                                   /* 'sit2', 32x22px */
                                                   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1c, 0x02, 0x05, 0x02, 0x24, 0x04, 0x04, 0x02, 0xa9, 0x1e, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x90, 0x08, 0x18, 0x60, 0x10, 0x08, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x0e, 0x82, 0x7c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x0c, 0x10, 0x10, 0x20, 0x20, 0x20, 0x28, 0x3e, 0x1c, 0x20, 0x20, 0x3e, 0x0f, 0x11, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

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
                                                     }};

    /* animation */
    void animate_luna(void) {
        /* jump */
        if (isJumping || !showedJump) {
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
        if (is_caps_word_on()) {
            oled_write_raw_P(bark[current_frame], ANIM_SIZE);

        } else if (isSneaking) {
            oled_write_raw_P(sneak[current_frame], ANIM_SIZE);

        } else if (current_wpm <= MIN_WALK_SPEED) {
            oled_write_raw_P(sit[current_frame], ANIM_SIZE);

        } else if (current_wpm <= MIN_RUN_SPEED) {
            oled_write_raw_P(walk[current_frame], ANIM_SIZE);

        } else {
            oled_write_raw_P(run[current_frame], ANIM_SIZE);
        }
    }

#    if OLED_TIMEOUT > 0
    /* the animation prevents the normal timeout from occuring */
    if (last_input_activity_elapsed() > OLED_TIMEOUT && last_led_activity_elapsed() > OLED_TIMEOUT) {
        oled_off();
        return;
    } else {
        oled_on();
    }
#    endif

    /* animation timer */
    if (timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
        anim_timer = timer_read32();
        animate_luna();
    }
}
// ----------------------------------------------------------------------------
// OLED: Bongo cat
// https://github.com/nwii/oledbongocat/blob/main/keymap.c
char wpm_str[10];

#ifdef OLED_ENABLE
// WPM-responsive animation stuff here
#    define BCAT_IDLE_FRAMES 5
#    define BCAT_IDLE_SPEED 20  // below this wpm value your animation will idle

// #define PREP_FRAMES 1 // uncomment if >1

#    define BCAT_TAP_FRAMES 2
#    define BCAT_TAP_SPEED 30  // above this wpm value typing animation to trigger

#    define BCAT_ANIM_FRAME_DURATION 200  // how long each frame lasts in ms
// #define BCAT_SLEEP_TIMER 60000 // should sleep after this period of 0 wpm, needs fixing
#    define BCAT_ANIM_SIZE 636  // number of bytes in array, minimize for adequate firmware size, max is 1024

uint32_t bcat_anim_timer         = 0;
uint32_t bcat_anim_sleep         = 0;
uint8_t  bcat_current_idle_frame = 0;
// uint8_t bcat_current_prep_frame = 0; // uncomment if PREP_FRAMES >1
uint8_t bcat_current_tap_frame = 0;

// Code containing pixel art, contains:
// 5 idle frames, 1 prep frame, and 2 tap frames

// To make your own pixel art:
// save a png/jpeg of an 128x32 image (resource: https://www.pixilart.com/draw )
// follow this guide up to and including "CONVERT YOUR IMAGE" https://docs.splitkb.com/hc/en-us/articles/360013811280-How-do-I-convert-an-image-for-use-on-an-OLED-display-
// replace numbers in brackets with your own
// if you start getting errors when compiling make sure you didn't accedentally delete a bracket
static void bcat_render_anim(void) {
    static const char PROGMEM bcat_idle[BCAT_IDLE_FRAMES][BCAT_ANIM_SIZE] = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0x40, 0x40, 0x20, 0x20, 0x20, 0x20, 0x20, 0x18, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x20, 0x40, 0x40, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xc1, 0x01, 0x01, 0x02, 0x02, 0x04, 0x04, 0x04, 0x04, 0x02, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x40, 0x80, 0x80, 0x40, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x04, 0x08, 0x08, 0x10, 0x20, 0x40, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x20, 0x20, 0x20, 0x20, 0x10, 0x10, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01,
                                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x18, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x83, 0x83, 0x40, 0x40, 0x40, 0x40, 0x20, 0x21, 0x21, 0x20, 0x10, 0x10, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x04, 0x08, 0x30, 0x40, 0x80, 0x80, 0x00, 0x00, 0x01, 0x86, 0x98, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x20, 0x20, 0x20, 0x20, 0x10, 0x10, 0x10, 0x10, 0x08, 0x0f, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x02, 0x04, 0x08, 0x10, 0x10, 0x20, 0x20, 0x40, 0x40, 0x41, 0x42, 0x24, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                                                              {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0x40, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xc1, 0x01, 0x01, 0x02, 0x02, 0x04, 0x04, 0x04, 0x04, 0x02, 0x02, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x60, 0x60, 0x00, 0x00, 0x01, 0x02, 0x04, 0x04, 0x08, 0x08, 0x10, 0x10, 0x20, 0x20, 0x40, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x20, 0x20, 0x20, 0x20, 0x10, 0x10, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01,
                                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x30, 0x0c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x86, 0x86, 0x40, 0x40, 0x40, 0x40, 0x21, 0x22, 0x22, 0x20, 0x11, 0x11, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x04, 0x08, 0x30, 0x40, 0x80, 0x80, 0x00, 0x00, 0x01, 0x86, 0x98, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x20, 0x20, 0x20, 0x20, 0x10, 0x10, 0x10, 0x10, 0x08, 0x0f, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x02, 0x04, 0x08, 0x10, 0x10, 0x20, 0x20, 0x40, 0x40, 0x41, 0x42, 0x24, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                                                              {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x82, 0x02, 0x02, 0x04, 0x04, 0x08, 0x08, 0x08, 0x08, 0x04, 0x04, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x60, 0x60, 0x00, 0x01, 0x01, 0x02, 0x04, 0x04, 0x08, 0x08, 0x10, 0x10, 0x20, 0x20, 0x40, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x20, 0x20, 0x20, 0x20, 0x10, 0x10, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01,
                                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x30, 0x0c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x86, 0x86, 0x40, 0x40, 0x40, 0x40, 0x21, 0x22, 0x22, 0x20, 0x11, 0x11, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x04, 0x08, 0x30, 0x40, 0x80, 0x80, 0x00, 0x00, 0x01, 0x86, 0x98, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x20, 0x20, 0x20, 0x20, 0x10, 0x10, 0x10, 0x10, 0x08, 0x0f, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x02, 0x04, 0x08, 0x10, 0x10, 0x20, 0x20, 0x40, 0x40, 0x41, 0x42, 0x24, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                                                              {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x34, 0xc4, 0x04, 0x04, 0x04, 0x08, 0x08, 0x08, 0x08, 0x08, 0x04, 0x04, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x40, 0x80, 0x80, 0x40, 0x00, 0x00, 0x30, 0x30, 0x00, 0x01, 0x01, 0x02, 0x02, 0x04, 0x04, 0x04, 0x08, 0x08, 0x10, 0x20, 0x40, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x20, 0x20, 0x20, 0x20, 0x10, 0x10, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01,
                                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x18, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x83, 0x83, 0x40, 0x40, 0x40, 0x40, 0x20, 0x21, 0x21, 0x20, 0x10, 0x10, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x04, 0x08, 0x30, 0x40, 0x80, 0x80, 0x00, 0x00, 0x01, 0x86, 0x98, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x20, 0x20, 0x20, 0x20, 0x10, 0x10, 0x10, 0x10, 0x08, 0x0f, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x02, 0x04, 0x08, 0x10, 0x10, 0x20, 0x20, 0x40, 0x40, 0x41, 0x42, 0x24, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
                                                              {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0x40, 0x40, 0x20, 0x20, 0x20, 0x20, 0x20, 0x18, 0x04, 0x02, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x20, 0x40, 0x40, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x0d, 0x31, 0xc1, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x04, 0x04, 0x02, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x40, 0x80, 0x80, 0x40, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x04, 0x08, 0x08, 0x10, 0x20, 0x40, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x20, 0x20, 0x20, 0x20, 0x10, 0x10, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01,
                                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x18, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x83, 0x83, 0x40, 0x40, 0x40, 0x40, 0x20, 0x21, 0x21, 0x20, 0x10, 0x10, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x04, 0x08, 0x30, 0x40, 0x80, 0x80, 0x00, 0x00, 0x01, 0x86, 0x98, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                               0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x20, 0x20, 0x20, 0x20, 0x10, 0x10, 0x10, 0x10, 0x08, 0x0f, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x02, 0x04, 0x08, 0x10, 0x10, 0x20, 0x20, 0x40, 0x40, 0x41, 0x42, 0x24, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
    static const char PROGMEM bcat_prep[][BCAT_ANIM_SIZE]            = {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x40, 0x40, 0x20, 0x20, 0x10, 0x08, 0x04, 0x02, 0x04, 0x08, 0x10, 0x10, 0x20, 0x20, 0x40, 0x40, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xc1, 0x01, 0x01, 0x02, 0x02, 0x04, 0x84, 0x44, 0x44, 0x42, 0x82, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x40, 0x80, 0x80, 0x40, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x64, 0x18, 0x04, 0x12, 0xc2, 0xca, 0x24, 0x88, 0xf0, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x20, 0x20, 0x20, 0x20, 0x10, 0x10, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x18, 0x06, 0x01, 0x00, 0x00, 0x0c, 0x03, 0x00, 0x02, 0x18, 0x19, 0x00, 0x05, 0xfe, 0x80, 0x83, 0x83, 0x40, 0x40, 0x40, 0x40, 0x20, 0x21, 0x21, 0x20, 0x10, 0x10, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x20, 0x20, 0x20, 0x20, 0x10, 0x10, 0x10, 0x10, 0x08, 0x0f, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};
    static const char PROGMEM bcat_tap[BCAT_TAP_FRAMES][BCAT_ANIM_SIZE]   = {
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x40, 0x40, 0x20, 0x20, 0x10, 0x08, 0x04, 0x02, 0x04, 0x08, 0x10, 0x10, 0x20, 0x20, 0x40, 0x40, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xc1, 0x01, 0x01, 0x02, 0x02, 0x04, 0x04, 0x04, 0x04, 0x02, 0x02, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x40, 0x80, 0x80, 0x40, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x64, 0x18, 0x04, 0x12, 0xc2, 0xca, 0x24, 0x88, 0xf0, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x20, 0x20, 0x20, 0x20, 0x10, 0x10, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x18, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x83, 0x83, 0x40, 0x40, 0x40, 0x40, 0x20, 0x21, 0x21, 0x20, 0x10, 0x10, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x20, 0x20, 0x20, 0x20, 0x10, 0x10, 0x10, 0x10, 0x08, 0x0f, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x02, 0x04, 0x08, 0x10, 0x10, 0x20, 0x20, 0x40, 0x40, 0x41, 0x42, 0x24, 0x98, 0xc0, 0x88, 0x88, 0x8c, 0x9c, 0x1c, 0x1e, 0x0e, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x40, 0x40, 0x20, 0x20, 0x10, 0x08, 0x04, 0x02, 0x04, 0x08, 0x10, 0x10, 0x20, 0x20, 0x40, 0x40, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xc1, 0x01, 0x01, 0x02, 0x02, 0x04, 0x84, 0x44, 0x44, 0x42, 0x82, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x40, 0x80, 0x80, 0x40, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x04, 0x08, 0x08, 0x10, 0x20, 0x40, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x20, 0x20, 0x20, 0x20, 0x10, 0x10, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x18, 0x06, 0x01, 0x00, 0x00, 0x0c, 0x03, 0x00, 0x02, 0x18, 0x19, 0x00, 0x05, 0xfe, 0x80, 0x83, 0x83, 0x40, 0x40, 0x40, 0x40, 0x20, 0x21, 0x21, 0x20, 0x10, 0x10, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x04, 0x08, 0x30, 0x40, 0x80, 0x80, 0x00, 0x00, 0x01, 0x86, 0x98, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x20, 0x20, 0x20, 0x20, 0x10, 0x10, 0x10, 0x10, 0x08, 0x0f, 0x08, 0x08, 0x04, 0x04, 0x04, 0x04, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x0f, 0x0f, 0x07, 0x03, 0x03, 0x61, 0xf0, 0xf8, 0xfc, 0x60, 0x01, 0x01, 0x01, 0x3c, 0x78, 0xf8, 0xf0, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    };

    // assumes 1 frame prep stage
    void bcat_animation_phase(void) {
        if (current_wpm <= BCAT_IDLE_SPEED) {
            bcat_current_idle_frame = (bcat_current_idle_frame + 1) % BCAT_IDLE_FRAMES;
            oled_write_raw_P(bcat_idle[abs((BCAT_IDLE_FRAMES - 1) - bcat_current_idle_frame)], BCAT_ANIM_SIZE);
        }
        if (current_wpm > BCAT_IDLE_SPEED && current_wpm < BCAT_TAP_SPEED) {
            // oled_write_raw_P(prep[abs((PREP_FRAMES-1)-current_prep_frame)], ANIM_SIZE); // uncomment if IDLE_FRAMES >1
            oled_write_raw_P(bcat_prep[0], BCAT_ANIM_SIZE);  // remove if IDLE_FRAMES >1
        }
        if (current_wpm >= BCAT_TAP_SPEED) {
            bcat_current_tap_frame = (bcat_current_tap_frame + 1) % BCAT_TAP_FRAMES;
            oled_write_raw_P(bcat_tap[abs((BCAT_TAP_FRAMES - 1) - bcat_current_tap_frame)], BCAT_ANIM_SIZE);
        }
    }
    if (current_wpm != 000) {
        oled_on();  // not essential but turns on animation OLED with any alpha keypress
        if (timer_elapsed32(bcat_anim_timer) > BCAT_ANIM_FRAME_DURATION) {
            bcat_anim_timer = timer_read32();
            bcat_animation_phase();
        }
        bcat_anim_sleep = timer_read32();
    } else {
        if (timer_elapsed32(bcat_anim_sleep) > OLED_TIMEOUT) {
            oled_off();
        } else {
            if (timer_elapsed32(bcat_anim_timer) > BCAT_ANIM_FRAME_DURATION) {
                bcat_anim_timer = timer_read32();
                bcat_animation_phase();
            }
        }
    }
}
#endif // OLED_DRIVER_ENABLE
// ----------------------------------------------------------------------------
// OLED: PRINT
static void print_status_narrow(void) {
    // Print mac vs windows
    if (keymap_config.swap_lctl_lgui) {
        oled_write_ln_P(PSTR("MAC"), false);
    } else {
        oled_write_ln_P(PSTR("WIN"), false);
    }
    // print base layer
    switch (get_highest_layer(default_layer_state)) {
        case _STRDY:
            oled_write_P(PSTR("STRDY"), false);
            break;
        case _QWERTY:
            oled_write_P(PSTR("QWRTY"), false);
            break;
        default:
            oled_write_P(PSTR("uNDEf"), true);
    }
    // Print current layer
    switch (get_highest_layer(layer_state)) {
        case _QWERTY:
        case _STRDY:
            oled_write_P(PSTR("Base "), false);
            break;
        case _RAISE:
            oled_write_P(PSTR("Raise"), true);
            break;
        case _EXTND:
            oled_write_P(PSTR("Xtnd2"), true);
            break;
        case _ADJUST:
            oled_write_P(PSTR("Adjst"), true);
            break;
        default:
            oled_write_P(PSTR("uNDEf"), true);
    }
    oled_write_P(PSTR("-----"), false);
    // Statuses: Autocorrect, AltTab, CapsWord
    oled_write_P(PSTR("ACORR"), autocorrect_is_enabled());  // autocorrect
    oled_write_P(PSTR("ALTAB"), is_alt_tab_active);  // alt-tab
    oled_write_P(PSTR("CAPWD"), is_caps_word_on());  // caps word
    oled_write_P(PSTR("SELWD"), selwd_state != SELWD_STATE_NONE); // select word
    oled_write_P(PSTR("SNCSE"), sentence_state == SNCSE_STATE_ENDING || sentence_state == SNCSE_STATE_PRIMED); // sentence case
    oled_write_P(PSTR("-----"), false);
    oled_write_P(PSTR(get_u16_str(get_last_keycode(), '0')), false);
    // oled_write_P(PSTR(get_u8_str(get_alt_repeat_key_keycode(), '0')), false);
    // QMK
    // render_logo();  // TODO: Get a better logo.
    render_luna(0, 13);
}
// static void print_logo_narrow(void) {
//     // /* wpm counter */
//     uint8_t n = get_current_wpm();
//     char    wpm_str[4];
//     oled_set_cursor(0, 1);
//     wpm_str[3] = '\0';
//     wpm_str[2] = '0' + n % 10;
//     wpm_str[1] = '0' + (n /= 10) % 10;
//     wpm_str[0] = '0' + n / 10;
//     oled_write(wpm_str, false);

//     oled_set_cursor(0, 15);
//     oled_write("wpm", false);

//     // LUNA doesn't work here.
// }
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return OLED_ROTATION_270;
    }
    return rotation;
    // return OLED_ROTATION_270;
    // return OLED_ROTATION_180;
}
bool oled_task_user(void) {
    // Keyboard Pet Variables
    current_wpm   = get_current_wpm();
    led_usb_state = host_keyboard_led_state();

    if (is_keyboard_master()) {
        print_status_narrow();
    } else {
        // print_logo_narrow();
        // render_logo();
        bcat_render_anim();  // renders pixelart
        oled_set_cursor(0, 0);                            // sets cursor to (row, column) using charactar spacing (5 rows on 128x32 screen, anything more will overflow back to the top)
        // sprintf(wpm_str, "WPM:%03d", current_wpm);  // edit the string to change wwhat shows up, edit %03d to change how many digits show up
        oled_write(get_u8_str(get_current_wpm(), '0'), false);  // alternative non-sprintf wpm
        oled_write(wpm_str, false);                       // writes wpm on top left corner of string
    }
    return false;
}
#endif