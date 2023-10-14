#include QMK_KEYBOARD_H

#ifdef OS_DETECTION_ENABLE
  #include "os_detection.h"
#endif
// NOTE: This firmware is too big for the default Atmega32u4/Pro-Micro controller. I'm using RP2040/Elite-Pi.
// TODO: Braces Macro, Select Word, Select Line, SearchSelection, JoinLine from https://getreuer.info/posts/keyboards/macros/index.html
// TODO: Activate CapsWord with both GUI buttons https://getreuer.info/posts/keyboards/caps-word/index.html
// TODO: Sentence Case from https://getreuer.info/posts/keyboards/macros/index.html (though change this to use esc to undo the capitalization? w/ https://getreuer.info/posts/keyboards/triggers/index.html#based-on-previously-typed-keys)
// TODO: Select Word from https://getreuer.info/posts/keyboards/select-word/index.html

// Stolen from https://github.com/getreuer/qmk-keymap/blob/main/keymap.c
// This keymap uses Ikcelaks' Magic Sturdy layout for the base layer (see
// https://github.com/Ikcelaks/keyboard_layouts). I've also made some twists of
// my own. The "magic" is a key whose function depends on the last pressed key,
// implemented using the Alternate Repeat Key. This key is used to remove the
// top SFBs and type common n-grams.
//
// The following describes the functionality, where * is the magic key and @ the
// repeat key. For example, tapping A and then the magic key types "ao".
//
// SFB removal and common n-grams:
//
//     A * -> AO     L * -> LK      S * -> SK
//     C * -> CY     M * -> MENT    T * -> TMENT
//     D * -> DY     O * -> OA      U * -> UE
//     E * -> EU     P * -> PY      Y * -> YP
//     G * -> GY     Q * -> QUEN    spc * -> THE
//     I * -> ION    R * -> RL
//
// When the magic key types a letter, following it with the repeat key produces
// "n". This is useful to type certain patterns without SFBs.
//
//     A * @ -> AON             (like "kaon")
//     D * @ -> DYN             (like "dynamic")
//     E * @ -> EUN             (like "reunite")
//     O * @ -> OAN             (like "loan")
//
// Other patterns:
//
//     spc * @ -> THEN
//     I * @ -> IONS            (like "nations")
//     M * @ -> MENTS           (like "moments")
//     Q * @ -> QUENC           (like "frequency")
//     T * @ -> TMENTS          (lite "adjustments")
//     = *   -> ===             (JS code)
//     ! *   -> !==             (JS code)
//     " *   -> """<cursor>"""  (Python code)
//     ` *   -> ```<cursor>```  (Markdown code)
//     # *   -> #include        (C code)
//     < -   -> <-              (Haskell code)
//     . *   -> ../             (shell)
//     . * @ -> ../../
#define C_MAGIC QK_AREP

#define C_GUI_ESC LGUI_T(KC_ESC)
#define C_RSFT_ENT RSFT_T(KC_ENT)
#define C_LALT_ENT LALT_T(KC_ENT)
#define C_RCTL_MINS RCTL_T(KC_MINS)
#define C_LCTL_BSPC LCTL(KC_BSPC)

// Tap-Hold Custom Keys
#define C_BKSP LT(0, KC_BSPC)  // custom backspace

// Can have up to 32 layers(!)
enum sofle_layers {
    /* _M_XYZ = Mac Os, _W_XYZ = Win/Linux */
    _STRDY,
    _QWERTY,
    _EXTND,
    _RAISE,
    _ADJUST,
};

enum custom_keycodes {
    KC_STRDY = SAFE_RANGE,
    KC_QWERTY,
    KC_PRVWD,
    KC_NXTWD,
    KC_LSTRT,
    KC_LEND,
    KC_DLINE,
    KC_REDO,  // LCTL(KC_Y)
    KC_SNIP,  // SGUI(KC_S) // TODO: switch this to SGUI(KC_4) for mac
    KC_PRNT,  // LCTL(KC_P)
    KC_SLAL,  // LCTL(KC_A)
    KC_CNEW,  // LCTL(KC_N)
    KC_CLSE,  // LCTL(KC_W)
    KC_RFSH,  // LCTL(KC_R)
    KC_SAVE,  // LCTL(KC_S)
    KC_ZMIN,  // LCTL(KC_EQL) 
    KC_ZMOT,  // LCTL(KC_MINS)

    // New for magic layer
    UPDIR,
    M_ION,
    M_NION,
    M_MENT,
    M_QUEN,
    M_TMENT,
    M_THE,
    M_UPDIR,
    M_INCLUDE,
    M_DOCSTR,
    M_MKGRVS,
    M_EQEQ,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * QWERTY
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  -   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  `   |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  |  '   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   \  |  /   |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * |Enter |   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   ;  |  =   |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI |EXTND2| LCTR |LShift| /Space  /       \Bkspc \  |RAISE | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'            '------''---------------------------'
 */

[_QWERTY] = LAYOUT(
  KC_ESC,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_MINUS,
  KC_GRV,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,  KC_QUOT,
  KC_TAB,   KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                     KC_H,    KC_J,    KC_K,    KC_L, KC_BSLS,  KC_SLSH,
  KC_ENT,   KC_Z,   KC_X,    KC_C,    KC_V,    KC_B, KC_MUTE,KC_MEDIA_PLAY_PAUSE,KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SCLN,  KC_EQUAL,
                 KC_LGUI, TT(_EXTND),KC_LCTL, KC_LSFT, KC_SPC,                   C_BKSP, QK_REP, KC_RALT, TT(_RAISE), KC_RGUI
),
/*
 * STRDY
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  -   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  `   |   V  |   M  |   L  |   C  |   P  |                    |   B  | MAGIC|   U  |   O  |   Q  |  '   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   S  |   T  |   R  |   D  |   Y  |-------.    ,-------|   F  |   N  |   E  |   A  |   I  |  /   |
 * |------+------+------+------+------+------|  MUTE |    | PAUSE |------+------+------+------+------+------|
 * | Enter|   X  |   K  |   J  |   G  |   W  |-------|    |-------|   Z  |   H  |   ,  |   .  |   ;  |  +   |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI |EXTND2| LCTR |LShift| /Space  /       \Bkspc \  |REPEAT| RAlt |RAISE | RGUI |
 *            |      |Layer |      |      |/       /         \      \ |      |      |Layer |      |
 *            `----------------------------------'           '------''---------------------------'
 */

[_STRDY] = LAYOUT(
  KC_ESC,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                       KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_MINUS,
  KC_GRV,   KC_V,   KC_M,    KC_L,    KC_C,    KC_P,                       KC_B, C_MAGIC,    KC_U,    KC_O,    KC_Q,  KC_QUOT,
  KC_TAB,   KC_S,   KC_T,    KC_R,    KC_D,    KC_Y,                       KC_F,    KC_N,    KC_E,    KC_A,    KC_I,  KC_SLSH,
  KC_ENT,   KC_X,   KC_K,    KC_J,    KC_G,    KC_W, KC_MUTE,     KC_MPLY, KC_Z,    KC_H, KC_COMM,  KC_DOT, KC_SCLN,  KC_EQUAL,
    KC_LGUI,TT(_EXTND),KC_LCTL, KC_LSFT, KC_SPC,                    C_BKSP, QK_REP, KC_RALT, TT(_RAISE), KC_RGUI
),
/* EXTEND2
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |xBASEx|  F1  |  F2  |  F3  |  F4  |  F5  |                    |  F6  |  F7  |  F8  |  F9  | F10  | F11  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | `ESC |Print | Del  |Close | [{   |  ]}  |                    | Snip | MAGIC|PrvWd |UpArrw| NxtWd| F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |Save  |Refrsh|BwSrch| (    |   )  |-------.    ,-------| Find |AltTab|LArrw |DnArrw| RArrw|   /  |
 * |------+------+------+------+------+------|  MUTE |    | PAUSE |------+------+------+------+------+------|
 * | Enter| Undo | Cut  | Copy | Paste| Redo |-------|    |-------|SlctAl|C(New)|   ,  |   .  |   ;  |   +  |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI |      | LCTR |LShift| /Space  /       \Bkspc \  |REPEAT| RAlt |      |ADJUST|
 *            |      |      |      |      |/       /         \      \ |      |      |      | Layer|
 *            `----------------------------------'           '------''---------------------------'
 */
[_EXTND] = LAYOUT(
  TG(_EXTND),  KC_F1,   KC_F2,     KC_F3,      KC_F4,     KC_F5,                        KC_F6,      KC_F7,    KC_F8,   KC_F9,   KC_F10,   KC_F11,
  QK_GESC,   KC_PRNT,   KC_DEL,  KC_CLSE,     KC_LBRC,   KC_RBRC,                     KC_SNIP,    _______, KC_PRVWD,   KC_UP, KC_NXTWD,   KC_F12,
  _______,   KC_SAVE,  KC_RFSH,  KC_WSCH,     KC_LPRN,   KC_RPRN,                     KC_FIND,  A(KC_TAB),  KC_LEFT, KC_DOWN,  KC_RGHT,  _______,
  _______,   KC_UNDO,   KC_CUT,  KC_COPY,     KC_PASTE,  KC_REDO, _______,   _______, KC_SLAL,    KC_CNEW,  _______, _______,  _______,  _______,
                    _______, _______, _______, _______, _______,                       _______, _______, _______, _______, TO(_ADJUST)
),
/* RAISE
 * ,----------------------------------------.                    ,-----------------------------------------.
 * |xBASEx|      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | `Esc | Ins  | Pscr | Menu |      |      |                    |      | PWrd |  Up  | NWrd | DLine| Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  | LAt  | LCtl |LShift|      | Caps |-------.    ,-------|      | Left | Down | Rigth|  Del | Bspc |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * |Enter | Undo |  Cut | Copy | Paste|      |-------|    |-------|      | LStr |      | LEnd |      | Shift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            |Adjust| LAlt | LCTR |LOWER | /Enter  /       \Space \  |RAISE | RCTR | RAlt | RGUI |
 *            |Layer |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[_RAISE] = LAYOUT(
  TG(_RAISE), _______ , _______ , _______ , _______ , _______,                   _______,  _______  , _______,  _______ ,  _______ ,_______,
  QK_GESC,  KC_INS,  KC_PSCR,   KC_APP,  XXXXXXX, XXXXXXX,                        KC_PGUP, KC_PRVWD,   KC_UP, KC_NXTWD,KC_DLINE, KC_BSPC,
  _______, KC_LALT,  KC_LCTL,  KC_LSFT,  XXXXXXX, KC_CAPS,                       KC_PGDN,  KC_LEFT, KC_DOWN, KC_RGHT,  KC_DEL, KC_BSPC,
  _______,KC_UNDO, KC_CUT, KC_COPY, KC_PASTE, XXXXXXX,  _______,       _______,  XXXXXXX, KC_LSTRT, XXXXXXX, KC_LEND,   XXXXXXX, _______,
                         TO(_ADJUST), _______, _______, _______, _______,       _______, _______, _______, _______, _______
),
/* ADJUST
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |xBasex|      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |QKBOOT|      |STRDY | QWRTY|      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |MACWIN| ACorr|      |      |-------.    ,-------|      | VOLDO| MUTE | VOLUP|      |      |
 * |------+------+------+------+------+------|  MUTE |    |       |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|      | PREV | PLAY | NEXT |      |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |LOWER | /Enter  /       \Space \  |RAISE | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
  [_ADJUST] = LAYOUT(
  TG(_ADJUST) , XXXXXXX,  XXXXXXX ,  XXXXXXX , XXXXXXX, XXXXXXX,                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  QK_BOOT  , XXXXXXX,KC_STRDY,KC_QWERTY,XXXXXXX,XXXXXXX,                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX , XXXXXXX,CG_TOGG, AC_TOGG,    XXXXXXX,  XXXXXXX,                     XXXXXXX, KC_VOLD, KC_MUTE, KC_VOLU, XXXXXXX, XXXXXXX,
  XXXXXXX , XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX,  XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, KC_MPRV, KC_MPLY, KC_MNXT, XXXXXXX, XXXXXXX,
                   _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______
  )
};

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
    // Print mac vs windows
    if (keymap_config.swap_lctl_lgui) {
        oled_write_ln_P(PSTR("MAC"), false);
    } else {
        oled_write_ln_P(PSTR("WIN"), false);
    }
    oled_write_ln_P(PSTR("-----"), false);
    // print base layer
    switch (get_highest_layer(default_layer_state)) {
        case _STRDY:
            oled_write_ln_P(PSTR("STRDY"), false);
            break;
        case _QWERTY:
            oled_write_ln_P(PSTR("QWRTY"), false);
            break;
        default:
            oled_write_P(PSTR("Undef"), false);
    }
    oled_write_ln_P(PSTR("-----"), false);
    // Print current layer
    switch (get_highest_layer(layer_state)) {
        case _STRDY:
            oled_write_P(PSTR("Base\n"), false);
            break;
        case _RAISE:
            oled_write_P(PSTR("Raise"), true);
            break;
        case _EXTND:
            oled_write_P(PSTR("XTND"), true);
            break;
        case _ADJUST:
            oled_write_P(PSTR("Adj\n"), true);
            break;
        default:
            oled_write_ln_P(PSTR("Undef"), false);
    }
    oled_write_ln_P(PSTR("-----"), false);
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


// Tap vs Long-Press Keys
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case C_BKSP:  // custom backspace
            return 125;
        default:
            return TAPPING_TERM;
    }
} 
// Given a tap-hold key event, replaces the hold function with `long_press_keycode`.
static bool process_tap_or_long_press_key(
    keyrecord_t* record, uint16_t long_press_keycode) {
    if (record->tap.count == 0) {  // Key is being held.
        if (record->event.pressed) {
            tap_code16(long_press_keycode);
        }
        return false;  // Skip default handling.
    }
    return true;  // Continue default handling.
}

#ifdef AUTOCORRECT_ENABLE
bool apply_autocorrect(uint8_t backspaces, const char* str,
                       char* typo, char* correct) {
    for (uint8_t i = 0; i < backspaces; ++i) {
        tap_code(KC_BSPC);
    }
    send_string_with_delay_P(str, TAP_CODE_DELAY);
    return false;
}
#endif  // AUTOCORRECT_ENABLE

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
        // These magic patterns work with Caps Word.
        case M_ION:
        case M_MENT:
        case M_QUEN:
        case M_TMENT:
        return true;

        default:
        return false;  // Deactivate Caps Word.
    }
}

uint16_t get_alt_repeat_key_keycode_user(uint16_t keycode, uint8_t mods) {
    if ((mods & ~MOD_MASK_SHIFT) == 0) {
        switch (keycode) {
        // For navigating next/previous search results in Vim:
        // N -> Shift + N, Shift + N -> N.
        case KC_N: return KC_N;

        // Behavior for Magic Sturdy's "magic" key.
        case KC_A: return KC_O;         // A -> O
        case KC_C: return KC_Y;         // C -> Y
        case KC_D: return KC_Y;         // D -> Y
        case KC_E: return KC_U;         // E -> U
        case KC_G: return KC_Y;         // G -> Y
        case KC_I: return M_ION;        // I -> ON
        case KC_L: return KC_K;         // L -> K
        case KC_M: return M_MENT;       // M -> ENT
        case KC_O: return KC_A;         // O -> A
        case KC_P: return KC_Y;         // P -> Y
        case KC_Q: return M_QUEN;       // Q -> UEN
        case KC_R: return KC_L;         // R -> L
        case KC_S: return KC_K;         // S -> K
        case KC_T: return M_TMENT;      // T -> TMENT
        case KC_U: return KC_E;         // U -> E
        case KC_Y: return KC_P;         // Y -> P
        case KC_SPC: return M_THE;      // spc -> THE

        case KC_DOT: return M_UPDIR;    // . -> ./
        case KC_COMM:                   // ! -> ==
            if ((mods & MOD_MASK_SHIFT) == 0) {
            return KC_NO;
            }
            // Fall through intended.
        case KC_EQL: return M_EQEQ;     // = -> ==
        case KC_HASH: return M_INCLUDE; // # -> include
        case KC_QUOT:
            if ((mods & MOD_MASK_SHIFT) != 0) {
            return M_DOCSTR;  // " -> ""<cursor>"""
            }
            return KC_NO;
        case KC_GRV:  // ` -> ``<cursor>``` (for Markdown code)
            return M_MKGRVS;
        case KC_LABK: return KC_MINS;   // < -> - (for Haskell)
        }
    } else if ((mods & MOD_MASK_CTRL)) {
        switch (keycode) {
        case KC_A:  // Ctrl+A -> Ctrl+C
            return C(KC_C);
        }
    }
    return KC_TRNS;
}

bool remember_last_key_user(uint16_t keycode, keyrecord_t* record,
                            uint8_t* remembered_mods) {
    // Unpack tapping keycode for tap-hold keys.
    switch (keycode) {
    #ifndef NO_ACTION_TAPPING
        case QK_MOD_TAP ... QK_MOD_TAP_MAX:
        keycode = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
        break;
    #ifndef NO_ACTION_LAYER
        case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
        keycode = QK_LAYER_TAP_GET_TAP_KEYCODE(keycode);
        break;
    #endif  // NO_ACTION_LAYER
    #endif  // NO_ACTION_TAPPING
    }

    // Forget Shift on letters when Shift or AltGr are the only mods.
    // Exceptionally, I want Shift remembered on N and Z for "NN" and "ZZ" in Vim.
    switch (keycode) {
        case KC_A ... KC_M:
        case KC_O ... KC_Y:
        if ((*remembered_mods & ~(MOD_MASK_SHIFT | MOD_BIT(KC_RALT))) == 0) {
            *remembered_mods &= ~MOD_MASK_SHIFT;
        }
        break;
    }

    return true;
}

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

// bool process_magic_key_2(uint16_t prev_keycode, uint8_t prev_mods) {
//     switch (prev_keycode) {
//         case KC_B:
//             SEND_STRING("ecome");
//             return false;
//         case KC_F:
//             SEND_STRING("ollow");
//             return false;
//         case KC_N:
//             SEND_STRING("eighbor");
//             return false;
//         case KC_H:
//             SEND_STRING("owever");
//             return false;
//         case KC_U:
//             SEND_STRING("pgrade");
//             return false;
//         case KC_O:
//             SEND_STRING("ther");
//             return false;
//         case KC_A:
//             SEND_STRING("lready");
//             return false;
//         case KC_P:
//             SEND_STRING("sych");
//             return false;
//         case KC_I:
//             SEND_STRING("'ll");
//             return false;
//         case KC_K:
//             SEND_STRING("now");
//             return false;
//         case KC_T:
//             SEND_STRING("hough");
//             return false;
//         case KC_L:
//             SEND_STRING("ittle");
//             return false;
//         case KC_M:
//         case KC_R:
//             SEND_STRING("ight");
//             return false;
//         case KC_J:
//             SEND_STRING("udge");
//             return false;
//         case KC_C:
//             SEND_STRING("ould");
//             return false;
//         case KC_D:
//             SEND_STRING("evelop");
//             return false;
//         case KC_G:
//             SEND_STRING("eneral");
//             return false;
//         case KC_W:
//             SEND_STRING("here");
//             return false;
//         case KC_S:
//             SEND_STRING("hould");
//             return false;
//         case KC_DOT:
//             SEND_STRING("org");
//             return false;
//         case KC_COMM:
//             SEND_STRING(" however");
//             return false;
//         default:
//             SEND_STRING("'ll");
//             return false;
//     }
// }
// bool process_magic_key_3(uint16_t prev_keycode, uint8_t prev_mods) {
//     switch (prev_keycode) {
//         case KC_B:
//             SEND_STRING("etween");
//             return false;
//         case KC_N:
//             SEND_STRING("umber");
//             return false;
//         case KC_U:
//             SEND_STRING("pdate");
//             return false;
//         case KC_A:
//             SEND_STRING("bout");
//             return false;
//         case KC_W:
//             SEND_STRING("orld");
//             return false;
//         case KC_G:
//             SEND_STRING("overn");
//             return false;
//         case KC_P:
//             SEND_STRING("rogram");
//             return false;
//         case KC_Q:
//             SEND_STRING("uestion");
//             return false;
//         case KC_C:
//             SEND_STRING("rowd");
//             return false;
//         case KC_S:
//             SEND_STRING("chool");
//             return false;
//         case KC_T:
//             SEND_STRING("hrough");
//             return false;
//         case KC_M:
//             SEND_STRING("anage");
//             return false;
//         case KC_O:
//             SEND_STRING("xygen");
//             return false;
//         case KC_I:
//             SEND_STRING("'m");
//             return false;
//         case KC_E:
//             SEND_STRING("'re");
//             return false;
//         case KC_DOT:
//             SEND_STRING("com");
//             return false;
//         case KC_COMM:
//             SEND_STRING(" since");
//             return false;
//         default:
//             return false;
//     }
// }

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
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

    if (record->event.pressed) {
        switch (keycode) {
            // base switch
            case KC_STRDY:
                set_single_persistent_default_layer(_STRDY);
                return false;
            case KC_QWERTY:
                set_single_persistent_default_layer(_QWERTY);
                return false;

            // mac-win switching
            case KC_PRVWD:
                if (keymap_config.swap_lctl_lgui) {
                    register_mods(mod_config(MOD_LALT));
                    register_code(KC_LEFT);
                } else {
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_LEFT);
                }
                break;
            case KC_NXTWD:
                if (keymap_config.swap_lctl_lgui) {
                    register_mods(mod_config(MOD_LALT));
                    register_code(KC_RIGHT);
                } else {
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_RIGHT);
                }
                break;
            case KC_LSTRT:
                if (keymap_config.swap_lctl_lgui) {
                    //CMD-arrow on Mac, but we have CTL and GUI swapped
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_LEFT);
                } else {
                    register_code(KC_HOME);
                }
                break;
            case KC_LEND:
                if (keymap_config.swap_lctl_lgui) {
                    //CMD-arrow on Mac, but we have CTL and GUI swapped
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_RIGHT);
                } else {
                    register_code(KC_END);
                }
                break;
            case KC_DLINE:
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_BSPC);
                break;
            case KC_COPY:
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_C);
                return false;
            case KC_PASTE:
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_V);
                return false;
            case KC_CUT:
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_X);
                return false;
                break;
            case KC_UNDO:
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_Z);
                return false;
            case KC_REDO:
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_Y);
                return false;
            case KC_FIND:
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_F);
                return false;
            case KC_PRNT:
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_P);
                return false;
            case KC_SLAL:
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_A);
                return false;
            case KC_CNEW:
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_N);
                return false;
            case KC_CLSE:
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_W);
                return false;
            case KC_RFSH:
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_R);
                return false;
            case KC_SAVE:
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_S);
                return false;
            case KC_ZMIN:
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_EQL);
                return false;
            case KC_ZMOT:
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_MINS);
                return false;

            // customized snipping tool macro?
            case KC_SNIP:
                // Using OS Detection
                switch (detected_host_os()) {
                    case OS_MACOS: 
                        register_mods(MOD_LGUI);
                        register_mods(MOD_LSFT);
                        register_code(KC_4);
                    case OS_WINDOWS:
                        register_mods(MOD_LGUI);
                        register_mods(MOD_LSFT);
                        register_code(KC_S);
                    case OS_LINUX:
                        register_mods(MOD_LSFT);
                        register_code(KC_PSCR);
                    case OS_UNSURE:
                        // could be android?
                        register_code(KC_PSCR);
                    case OS_IOS:
                        // iPad has command+shift+4?
                        register_mods(MOD_LGUI);
                        register_mods(MOD_LSFT);
                        register_code(KC_4);
                }
                return false;

            // tap-press
            case C_BKSP:
                return process_tap_or_long_press_key(record, KC_DLINE);

            // magic
            // Macros invoked through the MAGIC key.
            case M_ION:     MAGIC_STRING(/*i*/"on", KC_S); break;
            case M_MENT:    MAGIC_STRING(/*m*/"ent", KC_S); break;
            case M_QUEN:    MAGIC_STRING(/*q*/"uen", KC_C); break;
            case M_TMENT:   MAGIC_STRING(/*t*/"ment", KC_S); break;
            case M_THE:     MAGIC_STRING(/* */"the", KC_N); break;
            case M_UPDIR:   MAGIC_STRING(/*.*/"./", UPDIR); break;
            case M_INCLUDE: SEND_STRING_DELAY(/*#*/"include ", TAP_CODE_DELAY); break;
            case M_EQEQ:    SEND_STRING_DELAY(/*=*/"==", TAP_CODE_DELAY); break;
            case M_DOCSTR:
                SEND_STRING_DELAY(/*"*/"\"\"\"\"\""
                    SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT), TAP_CODE_DELAY);
                break;
            case M_MKGRVS:
                SEND_STRING_DELAY(/*`*/"``\n\n```" SS_TAP(X_UP), TAP_CODE_DELAY);
                break;
        }
    }
    else {
        switch (keycode) {
            case KC_PRVWD:
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LALT));
                    unregister_code(KC_LEFT);
                } else {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_LEFT);
                }
                break;
            case KC_NXTWD:
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LALT));
                    unregister_code(KC_RIGHT);
                } else {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_RIGHT);
                }
                break;
            case KC_LSTRT:
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_LEFT);
                } else {
                    unregister_code(KC_HOME);
                }
                break;
            case KC_LEND:
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_RIGHT);
                } else {
                    unregister_code(KC_END);
                }
                break;
            case KC_DLINE:
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_BSPC);
                break;
            case KC_COPY:
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_C);
                return false;
            case KC_PASTE:
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_V);
                return false;
            case KC_CUT:
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_X);
                return false;
                break;
            case KC_UNDO:
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_Z);
                return false;
            case KC_REDO:
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_Y);
                return false;
            case KC_FIND:
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_F);
                return false;
            case KC_PRNT:
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_P);
                return false;
            case KC_SLAL:
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_A);
                return false;
            case KC_CNEW:
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_N);
                return false;
            case KC_CLSE:
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_W);
                return false;
            case KC_RFSH:
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_R);
                return false;
            case KC_SAVE:
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_S);
                return false;

            case KC_SNIP:
                // Using OS Detection
                switch (detected_host_os()) {
                    case OS_MACOS: 
                        unregister_mods(MOD_LGUI);
                        unregister_mods(MOD_LSFT);
                        unregister_code(KC_4);
                    case OS_WINDOWS:
                        unregister_mods(MOD_LGUI);
                        unregister_mods(MOD_LSFT);
                        unregister_code(KC_S);
                    case OS_LINUX:
                        unregister_mods(MOD_LSFT);
                        unregister_code(KC_PSCR);
                    case OS_UNSURE:
                        // could be android?
                        unregister_code(KC_PSCR);
                    case OS_IOS:
                        // iPad has command+shift+4?
                        unregister_mods(MOD_LGUI);
                        unregister_mods(MOD_LSFT);
                        unregister_code(KC_4);
                }
                return false;

            case KC_ZMIN:
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_EQL);
                return false;
            case KC_ZMOT:
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_MINS);
                return false;
        }
    }
    return true;
}

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


#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
          // Mappings for 1st Encoder          // Mappings for 2nd Encoder
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU),  ENCODER_CCW_CW(KC_WH_D, KC_WH_U)  }, // QWERTY
    [1] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU),  ENCODER_CCW_CW(KC_WH_D, KC_WH_U)  }, // STRDY
    [2] = { ENCODER_CCW_CW(KC_WH_L, KC_WH_R),  ENCODER_CCW_CW(KC_WH_D, KC_WH_U)  }, // Xtnd2
    [3] = { ENCODER_CCW_CW(KC_ZMIN, KC_ZMOT), ENCODER_CCW_CW(KC_BRID, KC_BRIU) }, // Raise
    [4] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_WH_D, KC_WH_U) }, // Adj
    // You can add more layers here if you need them, or you can also delete lines for layers you are not using
};
#endif
