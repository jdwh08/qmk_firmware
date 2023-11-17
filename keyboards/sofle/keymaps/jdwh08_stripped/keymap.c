#include QMK_KEYBOARD_H

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
    KC_ZMIN,  // zoom in w/ LCTL(KC_EQL) 
    KC_ZMOT,  // zoom out w/ LCTL(KC_MINS)
};

// ----------------------------------------------------------
// Tap Hold: Custom Keys
// #define C_BKSP LT(0, KC_BSPC)  // custom backspace; KC_BSPC on tap, control backspace on hold
// #define C_SRCH LT(0, KC_SRCH)  // custom web search key; browser search on tap, open new browser + search on hold
// #define C_ENTR LT(0, KC_ENT)    // custom enter key; enter on tap, control enter on hold

// ----------------------------------------------------------------------
// LAYERS

enum sofle_layers {
    /* _M_XYZ = Mac Os, _W_XYZ = Win/Linux */
    _QWERTY,
    _STRDY,
    _EXTND,
    _RAISE,
    _ADJUST,
};

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
  KC_GRV,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_GRV,
  KC_ESC,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,  KC_BSPC,
  KC_TAB,   KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                     KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN,  KC_QUOT,
  KC_LSFT,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B, KC_MUTE,     XXXXXXX,KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_RSFT,
                 KC_LGUI,KC_LALT,KC_LCTL, MO(_EXTND), KC_ENT,      KC_SPC,  MO(_RAISE), KC_RCTL, KC_RALT, KC_RGUI
),
/*
 * STRDY
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  -   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  `   |   V  |   M  |   L  |   C  |   P  |                    |   B  | MAGIC|   U  |   O  |   Q  |  /   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   S  |   T  |   R  |   D  |   Y  |-------.    ,-------|   F  |   N  |   E  |   A  |   I  |  '   |
 * |------+------+------+------+------+------|  MUTE |    | PAUSE |------+------+------+------+------+------|
 * |LShift|   X  |   K  |   J  |   G  |   W  |-------|    |-------|   Z  |   H  |   ,  |   .  |   ;  |  +   |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LCTR |EXTND2|Enter | /Space  /       \Bkspc \  |REPEAT|RAISE | RAlt | Caps |
 *            |      |      |      |      |/       /         \      \ |      |      |      | Word |
 *            `----------------------------------'            '------''---------------------------'
 */

[_STRDY] = LAYOUT(
  KC_ESC,   QK_AREP, ALTREP2,  ALTREP3,  ALTREP4,  QK_REP,               KC_6,     KC_7,    KC_8,     KC_9,    KC_0,  KC_MINUS,
  KC_GRV,   KC_V,   KC_M,    KC_L,    KC_C,    KC_P,                       KC_B,  QK_AREP,    KC_U,    KC_O,    KC_Q,  KC_SLSH,
  KC_TAB,   KC_S,   KC_T,    KC_R,    KC_D,    KC_Y,                       KC_F,     KC_N,    KC_E,    KC_A,    KC_I,  KC_QUOT,
  KC_LSFT,  KC_X,   KC_K,    KC_J,    KC_G,    KC_W, KC_MUTE,     KC_MPLY, KC_Z,     KC_H, KC_COMM,  KC_DOT, KC_SCLN,  KC_EQUAL,
    KC_LGUI,KC_LCTL,TT(_EXTND),KC_ENT, KC_SPC,                    KC_BSPC, QK_REP, TT(_RAISE), KC_RALT, CW_TOGG
),
/* LOWER
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |  F1  |  F2  |  F3  |  F4  |  F5  |                    |  F6  |  F7  |  F8  |  F9  | F10  | F11  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  `   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   !  |   @  |   #  |   $  |   %  |-------.    ,-------|   ^  |   &  |   *  |   (  |   )  |   |  |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * | Shift|  =   |  -   |  +   |   {  |   }  |-------|    |-------|   [  |   ]  |   ;  |   :  |   \  | Shift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |LOWER | /Enter  /       \Space \  |RAISE | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[_EXTND] = LAYOUT(
  _______,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                       KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,
  KC_GRV,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                       KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_F12,
  _______, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,                       KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PIPE,
  _______,  KC_EQL, KC_MINS, KC_PLUS, KC_LCBR, KC_RCBR, _______,       _______, KC_LBRC, KC_RBRC, KC_SCLN, KC_COLN, KC_BSLS, _______,
                       _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______
),
/* RAISE
 * ,----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Esc  | Ins  | Pscr | Menu |      |      |                    |      | PWrd |  Up  | NWrd | DLine| Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  | LAt  | LCtl |LShift|      | Caps |-------.    ,-------|      | Left | Down | Rigth|  Del | Bspc |
 * |------+------+------+------+------+------|  MUTE  |    |       |------+------+------+------+------+------|
 * |Shift | Undo |  Cut | Copy | Paste|      |-------|    |-------|      | LStr |      | LEnd |      | Shift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |LOWER | /Enter  /       \Space \  |RAISE | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[_RAISE] = LAYOUT(
  _______, _______ , _______ , _______ , _______ , _______,                           _______,  _______  , _______,  _______ ,  _______ ,_______,
  _______,  KC_INS,  KC_PSCR,   KC_APP,  XXXXXXX, XXXXXXX,                        KC_PGUP, KC_PRVWD,   KC_UP, KC_NXTWD,KC_DLINE, KC_BSPC,
  _______, KC_LALT,  KC_LCTL,  KC_LSFT,  XXXXXXX, KC_CAPS,                       KC_PGDN,  KC_LEFT, KC_DOWN, KC_RGHT,  KC_DEL, KC_BSPC,
  _______,KC_UNDO, KC_CUT, KC_COPY, KC_PASTE, XXXXXXX,  _______,       _______,  XXXXXXX, KC_LSTRT, XXXXXXX, KC_LEND,   XXXXXXX, _______,
                         _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______
),
/* ADJUST
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | QK_BOOT|      |QWERTY|COLEMAK|      |      |                    |      |      |      |      |      |      |
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
  XXXXXXX , XXXXXXX,  XXXXXXX ,  XXXXXXX , XXXXXXX, XXXXXXX,                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
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
    // uint8_t saved_mods = 0;
    // If Caps Word is on, save the mods and hold Shift.
    // if (is_caps_word_on()) {
    //     saved_mods = get_mods();
    //     register_mods(MOD_BIT(KC_LSFT));
    // }

    send_string_with_delay_P(str, TAP_CODE_DELAY);  // Send the string.
    set_last_keycode(repeat_keycode);

    // If Caps Word is on, restore the mods.
    // if (is_caps_word_on()) {
    //     set_mods(saved_mods);
    // }
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
        case KC_D: magic_length = 1; return KC_Y;         // D -> Y
        case KC_E: magic_length = 1; return KC_U;         // E -> U
        case KC_F: magic_length = 2; MAGIC_STRING(/*f*/"rom", KC_M); return KC_NO;        // F -> ROM
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
        // case KC_Q: magic_length = 1; return M_qU;      // Q -> U  NOT DONE; we will have a combo for qu -> q
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
        // case KC_9PRC: magic_length = 1; return KC_DOT;       // 9 -> .

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
        case KC_D: MAGIC_STRING(/*d*/"ge", KC_E); return 2;         // D -> GE
        case KC_E: MAGIC_STRING(/*e*/"nce", KC_E); return 3;        // E -> NCE
        case KC_F: MAGIC_STRING(/*f*/"ound", KC_D); return 4;       // F -> OUND
        case KC_G: tap_code(KC_L); return 1;                        // G -> L
        case KC_H: MAGIC_STRING(/*h*/"ere", KC_E); return 3;        // H -> ERE
        case KC_I: MAGIC_STRING(/*h*/"ze", KC_E); return 2;         // I -> ZE
        case KC_J: MAGIC_STRING(/*j*/"ust", KC_T); return 3;        // J -> UST  // no other j's, so this repeats for now.
        case KC_K: MAGIC_STRING(/*k*/"now", KC_W); return 3;        // K -> NOW
        case KC_L: tap_code(KC_L); return 1;                        // L -> Y
        case KC_M: MAGIC_STRING(/*m*/"ent", KC_T); return 3;        // M -> ENT
        case KC_N: MAGIC_STRING(/*n*/"ess", KC_S); return 3;        // N -> ESS
        case KC_O: MAGIC_STRING(/*o*/"ut", KC_T); return 2;         // O -> UT
        case KC_P: tap_code(KC_Y); return 1;                        // P -> Y
        // case KC_Q: return M_qU;      // Q -> U  NOT DONE; we will have a combo for qu -> q
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
        case KC_D: MAGIC_STRING(/*d*/"ge", KC_E); return 2;         // D -> GE  // no other d's, so this repeats for now.
        case KC_E: MAGIC_STRING(/*e*/"xp", KC_P); return 2;         // E -> XP
        case KC_F: MAGIC_STRING(/*f*/"alse", KC_E); return 4;       // F -> ALSE
        case KC_G: tap_code(KC_Y); return 1;                        // G -> Y
        case KC_H: MAGIC_STRING(/*h*/"aving", KC_G); return 5;      // H -> AVING
        case KC_I: MAGIC_STRING(/*i*/"ous", KC_S); return 3;        // I -> OUS
        case KC_J: MAGIC_STRING(/*j*/"ust", KC_T); return 3;        // J -> UST  // no other j's, so this repeats for now.
        case KC_K: MAGIC_STRING(/*k*/"now", KC_W); return 3;        // K -> NOW  // no other k's, so this repeats for now.
        case KC_L: MAGIC_STRING(/*l*/"at", KC_T); return 2;         // L -> AT
        case KC_M: MAGIC_STRING(/*m*/"ore", KC_E); return 3;        // M -> ORE
        case KC_N: MAGIC_STRING(/*n*/"ext", KC_T); return 3;        // N -> EXT
        case KC_O: MAGIC_STRING(/*o*/"ver", KC_R); return 3;        // O -> VER
        case KC_P: MAGIC_STRING(/*p*/"lease", KC_E); return 5;      // P -> LEASE
        // case KC_Q: return M_qU;      // Q -> U  NOT DONE; we will have a combo for qu -> q
        case KC_R: MAGIC_STRING(/*r*/"ight", KC_T); return 4;       // R -> IGHT
        case KC_S: MAGIC_STRING(/*s*/"how", KC_W); return 3;        // S -> HOW
        case KC_T: MAGIC_STRING(/*t*/"ment", KC_T); return 4;       // T -> MENT
        case KC_U: MAGIC_STRING(/*u*/"se", KC_E); return 2;         // U -> SE
        case KC_V: MAGIC_STRING(/*v*/"iew", KC_W); return 3;        // V -> IEW  // no other v's, so this repeats for now.
        case KC_W: MAGIC_STRING(/*w*/"ork", KC_K); return 3;        // W -> ORK
        case KC_X: MAGIC_STRING(/*x*/"eno", KC_O); return 3;        // X -> ENO
        case KC_Y: tap_code(KC_C); return 1;                        // Y -> C
        case KC_Z: MAGIC_STRING(/*z*/"oom", KC_M); return 3;        // Z -> OOM
        case KC_SPC: MAGIC_STRING(/* */"from", KC_M); return 4;     // spc -> FROM
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
        case KC_D: MAGIC_STRING(/*d*/"ge", KC_E); return 2;         // D -> GE  // no other d's, so this repeats for now.
        case KC_E: MAGIC_STRING(/*e*/"very", KC_Y); return 4;       // E -> VERY
        case KC_F: MAGIC_STRING(/*f*/"urther", KC_R); return 6;     // F -> URTHER
        case KC_G: MAGIC_STRING(/*g*/"eneral", KC_L); return 6;     // G -> ENERAL
        case KC_H: MAGIC_STRING(/*h*/"owever", KC_R); return 6;     // H -> OWEVER
        case KC_I: MAGIC_STRING(/*i*/"ble", KC_E); return 3;        // I -> BLE
        case KC_J: MAGIC_STRING(/*j*/"ust", KC_T); return 3;        // J -> UST  // no other j's, so this repeats for now.
        case KC_K: MAGIC_STRING(/*k*/"now", KC_W); return 3;        // K -> NOW  // no other k's, so this repeats for now.
        case KC_L: MAGIC_STRING(/*l*/"ike", KC_E); return 3;        // L -> IKE
        case KC_M: MAGIC_STRING(/*m*/"anage", KC_E); return 5;      // M -> ANAGE
        case KC_N: MAGIC_STRING(/*n*/"othing", KC_G); return 6;     // N -> OTHING
        case KC_O: MAGIC_STRING(/*o*/"riginal", KC_L); return 7;    // O -> RIGINAL
        case KC_P: MAGIC_STRING(/*p*/"revious", KC_S); return 8;    // P -> REVIOUS
        // case KC_Q: return M_qU;      // Q -> U  NOT DONE; we will have a combo for qu -> q
        case KC_R: MAGIC_STRING(/*r*/"eview", KC_W); return 5;      // R -> EVIEW
        case KC_S: MAGIC_STRING(/*s*/"hould", KC_D); return 5;      // S -> HOULD
        case KC_T: MAGIC_STRING(/*t*/"hrough", KC_H); return 6;     // T -> HROUGH
        case KC_U: MAGIC_STRING(/*u*/"sual", KC_L); return 4;       // U -> SUAL
        case KC_V: MAGIC_STRING(/*v*/"ersion", KC_N); return 6;     // V -> ERSION
        case KC_W: MAGIC_STRING(/*w*/"hich", KC_H); return 4;       // W -> HICH
        case KC_X: MAGIC_STRING(/*x*/"eno", KC_O); return 3;        // X -> ENO  // no other x's, so this repeats for now.
        case KC_Y: MAGIC_STRING(/*y*/"esterday", KC_Y); return 8;   // Y -> ESTERDAY
        case KC_Z: MAGIC_STRING(/*z*/"oom", KC_M); return 3;        // Z -> OOM  // no other z's, so this repeats for now.
        case KC_SPC: MAGIC_STRING(/* */"your", KC_R); return 4;      // spc -> YOUR
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
};

const uint16_t PROGMEM magic_tap[] = {QK_AREP, COMBO_END};
const uint16_t PROGMEM magic_hold[] = {QK_AREP, COMBO_END};

const uint16_t PROGMEM rep_tap[] = {QK_REP, COMBO_END};
const uint16_t PROGMEM rep_hold[] = {QK_REP, COMBO_END};
// const uint16_t PROGMEM magic_rep_tap[] = {QK_AREP, QK_REP, COMBO_END};
const uint16_t PROGMEM magic_rep_hold[] = {QK_AREP, QK_REP, COMBO_END};

combo_t key_combos[] = {
    [REP_TAP_C] = COMBO(rep_tap, QK_REP),
    [MAGIC_TAP_C] = COMBO(magic_tap, QK_AREP),
    [MAGIC_HOLD_C] = COMBO(magic_hold, ALTREP2),
    // [MAGIC_REP_TAP_C] = COMBO(magic_rep_tap, ALTREP3),
    // [MAGIC_TWOTAP_C] = COMBO(magic_twotap, ALTREP3),
    [REP_HOLD_C] = COMBO(rep_hold, ALTREP3),
    [MAGIC_REP_HOLD_C] = COMBO(magic_rep_hold, ALTREP4)
    // [MAGIC_REP_TAP_C] = COMBO(magic_rep_tap, ALTREP4)
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
// ----------------------------------------------------------------------------
// OLED

#ifdef OLED_ENABLE

static void render_logo(void) {
    static const char PROGMEM qmk_logo[] = {
        0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
        0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
        0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0
    };

    oled_write_P(qmk_logo, false);
}

static void print_status_narrow(void) {
    // Print current mode
    oled_write_P(PSTR("\n\n"), false);
    oled_write_ln_P(PSTR("MODE"), false);
    oled_write_ln_P(PSTR(""), false);
    if (keymap_config.swap_lctl_lgui) {
        oled_write_ln_P(PSTR("MAC"), false);
    } else {
        oled_write_ln_P(PSTR("WIN"), false);
    }

    switch (get_highest_layer(default_layer_state)) {
        case _QWERTY:
            oled_write_ln_P(PSTR("Qwrt"), false);
            break;
        case _STRDY:
            oled_write_ln_P(PSTR("Clmk"), false);
            break;
        default:
            oled_write_P(PSTR("Undef"), false);
    }
    oled_write_P(PSTR("\n\n"), false);
    // Print current layer
    oled_write_ln_P(PSTR("LAYER"), false);
    switch (get_highest_layer(layer_state)) {
        case _STRDY:
        case _QWERTY:
            oled_write_P(PSTR("Base\n"), false);
            break;
        case _RAISE:
            oled_write_P(PSTR("Raise"), false);
            break;
        case _EXTND:
            oled_write_P(PSTR("Lower"), false);
            break;
        case _ADJUST:
            oled_write_P(PSTR("Adj\n"), false);
            break;
        default:
            oled_write_ln_P(PSTR("Undef"), false);
    }
    oled_write_P(PSTR("\n\n"), false);
    led_t led_usb_state = host_keyboard_led_state();
    oled_write_ln_P(PSTR("CPSLK"), led_usb_state.caps_lock);
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return OLED_ROTATION_270;
    }
    return rotation;
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        print_status_narrow();
    } else {
        render_logo();
    }
    return false;
}

#endif

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _EXTND, _RAISE, _ADJUST);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
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
        // Built-in Shortcuts
        case KC_PRVWD:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    register_mods(mod_config(MOD_LALT));
                    register_code(KC_LEFT);
                } else {
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_LEFT);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LALT));
                    unregister_code(KC_LEFT);
                } else {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_LEFT);
                }
            }
            break;
        case KC_NXTWD:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    register_mods(mod_config(MOD_LALT));
                    register_code(KC_RIGHT);
                } else {
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_RIGHT);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LALT));
                    unregister_code(KC_RIGHT);
                } else {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_RIGHT);
                }
            }
            break;
        case KC_LSTRT:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                     //CMD-arrow on Mac, but we have CTL and GUI swapped
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_LEFT);
                } else {
                    register_code(KC_HOME);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_LEFT);
                } else {
                    unregister_code(KC_HOME);
                }
            }
            break;
        case KC_LEND:
            if (record->event.pressed) {
                if (keymap_config.swap_lctl_lgui) {
                    //CMD-arrow on Mac, but we have CTL and GUI swapped
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_RIGHT);
                } else {
                    register_code(KC_END);
                }
            } else {
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_RIGHT);
                } else {
                    unregister_code(KC_END);
                }
            }
            break;
        case KC_DLINE:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_BSPC);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_BSPC);
            }
            break;
        case KC_COPY:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_C);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_C);
            }
            return false;
        case KC_PASTE:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_V);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_V);
            }
            return false;
        case KC_CUT:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_X);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_X);
            }
            return false;
            break;
        case KC_UNDO:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_Z);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_Z);
            }
            return false;
    }
    return true;
}

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