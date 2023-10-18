#include QMK_KEYBOARD_H
#include "os_detection.h"

// NOTE: This firmware is too big for the default Atmega32u4/Pro-Micro controller. I'm using RP2040/Elite-Pi.
// TODO: Make backspace delete both brackets if the prior key was a bracket key?
// TODO: Bind Q to QU, register last key to U, and fix anything that this breaks

// TODO: Sentence Case from https://getreuer.info/posts/keyboards/macros/index.html (though change this to use esc to undo the capitalization? w/ https://getreuer.info/posts/keyboards/triggers/index.html#based-on-previously-typed-keys)
// TODO: Add WPM and Luna https://github.com/HellSingCoder/qmk_firmware/tree/master/keyboards/sofle/keymaps/helltm and BongoCat https://github.com/nwii/oledbongocat
// TODO: Make deleting () or <> all at once when cursor is inside them.

// BUGS:
// Browser search button is busted (typing insert)

// ---------------------------------------------------------------------------------------------------
// States used in multiple functions

// ----------------------------------------------------------------------------------------------------
// CUSTOM KEYCODES
enum custom_keycodes {
    KC_STRDY = SAFE_RANGE,  // to STRDY
    KC_QWERTY,  // to qwerty

    KC_PRVWD, // prevword
    KC_NXTWD, // next word
    KC_LSTRT, // start of line
    KC_LEND,  // end of line
    KC_DLINE, // delete line

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

    KC_9PRC,  // type 9 or () if shifted
    // C_LPARN,  // helper for ( macro
    KC_SBRC,  // type [] [squarebracket]
    KC_CBRC,  // type {} {curlybracket}
    KC_CANG,  // type , or <> if shifted
    // C_LANGB,  // helper for < macro

    // New phrases for magic layer
    UPDIR,  // ../
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

// ----------------------------------------------------------
// Tap Hold: Custom Keys
#define C_BKSP LT(0, KC_BSPC)  // custom backspace; KC_BSPC on tap, control backspace on hold
#define C_SRCH LT(0, KC_SRCH)  // custom web search key; browser search on tap, open new browser + search on hold

// ----------------------------------------------------------
// MAGIC KEY: Blob of info
// https://github.com/getreuer/qmk-keymap/blob/main/keymap.c
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

// ----------------------------------------------------------------------
// LAYERS
enum sofle_layers {
    _STRDY,  // strdy base
    _QWERTY,  // qwerty base
    _EXTND,  // extend functions
    _RAISE,  // TODO
    _ADJUST,  // settings changes
};
layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _EXTND, _RAISE, _ADJUST);  // activate _ADJ if both are active
}
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
 * |LShift|   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   ;  |  =   |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI |EXTND2| LCTR |Enter | /Space  /       \Bkspc \  |RAISE | RCTR | RAlt | Caps |
 *            |      |      |      |LFUNC |/       /         \      \ |      |      |      | Word |
 *            `----------------------------------'            '------''---------------------------'
 */
[_QWERTY] = LAYOUT(
  KC_ESC,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                     KC_6,    KC_7,    KC_8, KC_9PRC,    KC_0,  KC_MINUS,
  KC_GRV,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,  KC_QUOT,
  KC_TAB,   KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                     KC_H,    KC_J,    KC_K,    KC_L, KC_BSLS,  KC_SLSH,
  KC_LSFT,   KC_Z,   KC_X,    KC_C,    KC_V,    KC_B, KC_MUTE,KC_MEDIA_PLAY_PAUSE,KC_N,    KC_M,  KC_CANG,  KC_DOT, KC_SCLN,  KC_EQUAL,
                 KC_LGUI, TT(_EXTND),KC_LCTL, KC_ENT, KC_SPC,                   C_BKSP, QK_REP, KC_RALT, TT(_RAISE), CW_TOGG
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
 * |LShift|   X  |   K  |   J  |   G  |   W  |-------|    |-------|   Z  |   H  |   ,  |   .  |   ;  |  +   |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI |EXTND2| LCTR |Enter | /Space  /       \Bkspc \  |REPEAT| RAlt |RAISE | Caps |
 *            |      |Layer |      |      |/       /         \      \ |      |      |Layer | Word |
 *            `----------------------------------'           '------''---------------------------'
 */

[_STRDY] = LAYOUT(
  KC_ESC,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                       KC_6,    KC_7,    KC_8, KC_9PRC,    KC_0,  KC_MINUS,
  KC_GRV,   KC_V,   KC_M,    KC_L,    KC_C,    KC_P,                       KC_B, C_MAGIC,    KC_U,    KC_O,    KC_Q,  KC_QUOT,
  KC_TAB,   KC_S,   KC_T,    KC_R,    KC_D,    KC_Y,                       KC_F,    KC_N,    KC_E,    KC_A,    KC_I,  KC_SLSH,
  KC_LSFT,  KC_X,   KC_K,    KC_J,    KC_G,    KC_W, KC_MUTE,     KC_MPLY, KC_Z,    KC_H, KC_CANG,  KC_DOT, KC_SCLN,  KC_EQUAL,
    KC_LGUI,TT(_EXTND),KC_LCTL, KC_ENT, KC_SPC,                    C_BKSP, QK_REP, KC_RALT, TT(_RAISE), CW_TOGG
),
/* EXTEND2
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |xBASEx|  F1  |  F2  |  F3  |  F4  |  F5  |                    |  F6  |  F7  |  F8  |  F9  | F10  | F11  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | `ESC |Print |BSrcSl|Close |Refrsh|  {   |                    | Snip | MAGIC|PrvWd |UpArrw| NxtWd| F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  | Save |SrchSl| Del  |Selwrd|  [   |-------.    ,-------| Find |AltTab|LArrw |DnArrw| RArrw|   /  |
 * |------+------+------+------+------+------|  MUTE |    | PAUSE |------+------+------+------+------+------|
 * |LShift| Undo | Cut  | Copy | Paste| Redo |-------|    |-------|SlctAl|C(New)|   ,  |   .  |   ;  |   +  |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI |      | LCTR |Enter | /Space  /       \Bkspc \  |REPEAT| RAlt |ADJUST| Caps |
 *            |      |      |      |      |/       /         \      \ |      |      |Layer | Word |
 *            `----------------------------------'           '------''---------------------------'
 */
[_EXTND] = LAYOUT(
  TG(_EXTND),  KC_F1,   KC_F2,     KC_F3,       KC_F4,     KC_F5,                       KC_F6,      KC_F7,    KC_F8,   KC_F9,   KC_F10,   KC_F11,
  QK_GESC,   KC_PRNT, KC_SRCHB,  KC_RFSH,     KC_CLSE,   KC_CBRC,                     KC_SNIP,    _______, KC_PRVWD,   KC_UP, KC_NXTWD,   KC_F12,
  _______,   KC_SAVE,  KC_SRCH,  KC_SLWD,      KC_DEL,   KC_SBRC,                     KC_FIND,    KC_ATAB,  KC_LEFT, KC_DOWN,  KC_RGHT,  _______,
  _______,   KC_UNDO,   KC_CUT,  KC_COPY,    KC_PASTE,   KC_REDO, _______,   _______, KC_SLAL,    KC_CNEW,  _______, _______,  _______,  _______,
                    _______, _______, _______, _______, _______,                       _______, _______, _______, _______, _______
),
/* RAISE
// TODO: Numpad? Browser functions? Mouse keys? Previous/Next Line Navigation? Dynamic Macros? Hypermagic keys?
 * ,----------------------------------------.                     ,-----------------------------------------.
 * |xBASEx|      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | `Esc | Ins  | Pscr | Menu |      |      |                    |      | PWrd |  Up  | NWrd | DLine| Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  | LAt  | LCtl |LShift|      | Caps |-------.    ,-------|      | Left | Down | Rigth|  Del | Bspc |
 * |------+------+------+------+------+------|  MUTE |    | PAUSE |------+------+------+------+------+------|
 * |LShift| Undo |  Cut | Copy | Paste|      |-------|    |-------|      | LStr |      | LEnd |      | Shift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI |ADJUST| LCTR |Enter | /Space  /       \Bkspc \  |Repeat| Ralt |      | Caps |
 *            |      |Layer |      |      |/       /         \      \ |      |      |      | Word |
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
 * |xBasex|      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |QKBOOT|      |STRDY | QWRTY|      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |MACWIN| ACorr|      |      |-------.    ,-------|      | VOLDO| MUTE | VOLUP|      |      |
 * |------+------+------+------+------+------|  MUTE |    | PAUSE |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|      | PREV | PLAY | NEXT |      |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |LOWER | /Space  /       \Bkspc \  |RAISE | RCTR | RAlt | Caps |
 *            |      |      |      |      |/       /         \      \ |      |      |      | Word |
 *            `----------------------------------'           '------''---------------------------'
 */
  [_ADJUST] = LAYOUT(
  TO(_STRDY) , XXXXXXX,  XXXXXXX ,  XXXXXXX , XXXXXXX, XXXXXXX,                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  QK_BOOT  , XXXXXXX,KC_STRDY,KC_QWERTY,XXXXXXX,XXXXXXX,                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX , XXXXXXX,CG_TOGG, AC_TOGG,    XXXXXXX,  XXXXXXX,                     XXXXXXX, KC_VOLD, KC_MUTE, KC_VOLU, XXXXXXX, XXXXXXX,
  XXXXXXX , XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX,  XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, KC_MPRV, KC_MPLY, KC_MNXT, XXXXXXX, XXXXXXX,
                   _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______
  )
};

// ----------------------------------------------------------------------
// OS DETECTION HANDLING
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
// Custom Alt-Tab Key
bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;
// ----------------------------------------------------------------------
// Tap Hold: Tap vs Long-Press Processing
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case C_BKSP:  // custom backspace
            return 150;
        default:
            return TAPPING_TERM;
    }
} 
// Given a tap-hold key event, replaces the hold function with `long_press_keycode`.
static bool process_tap_or_long_press_key(
    keyrecord_t* record, uint16_t long_press_keycode) {
    if (record->tap.count == 0) {  // Key is being held.
        if (record->event.pressed) {
            set_last_keycode(long_press_keycode);
            // BUGFIX: will this allow me to process keys within keys?
            // tap_code16(long_press_keycode);
            register_code16(long_press_keycode);
            unregister_code16(long_press_keycode);
        }
        return false;  // Skip default handling.
    }
    return true;  // Continue default handling.
}
// ----------------------------------------------------------------------
// AUTOCORRECT
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
        case C_MAGIC:  // don't shift magic
        case QK_REP:  // don't shift repeat
            return false;

    // ----------------------------------------------------------
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
            }
            break;
        case KC_CANG:
            if (!shifted) {
                unregister_code16(KC_COMM);
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
        case C_MAGIC:
        case QK_REP:
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
    STATE_NONE,        // No selection.
    STATE_SELECTED,    // Macro released with something selected.
    STATE_WORD,        // Macro held with word(s) selected.
    STATE_FIRST_LINE,  // Macro held with one line selected.
    STATE_LINE         // Macro held with multiple lines selected.
};
static uint8_t selwd_state = STATE_NONE;

// Idle timeout timer to disable Select Word after a period of inactivity.
#if SELECT_WORD_TIMEOUT > 0
static uint16_t idle_timer = 0;
void select_word_task(void) {
    if (selwd_state && timer_expired(timer_read(), idle_timer)) {
        selwd_state = STATE_NONE;
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
            if (selwd_state == STATE_NONE) {
                // On first use, tap Ctrl+Right then Ctrl+Left (or with Alt on Mac) to
                // ensure the cursor is positioned at the beginning of the word.
                send_keyboard_report();
                tap_code(KC_RGHT);
                tap_code(KC_LEFT);
            }
            register_mods(MOD_BIT(KC_LSFT));
            register_code(KC_RGHT);
            selwd_state = STATE_WORD;
        } else {  // Select line.
            if (selwd_state == STATE_NONE) {
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
                selwd_state = STATE_FIRST_LINE;
            } else {
                register_code(KC_DOWN);
                selwd_state = STATE_LINE;
            }
        }
        return false;
    }

    // `sel_keycode` was released, or another key was pressed.
    switch (selwd_state) {
        case STATE_WORD:
            unregister_code(KC_RGHT);
            if (keymap_config.swap_lctl_lgui) {
                unregister_mods(MOD_BIT(KC_LSFT) | MOD_BIT(KC_LALT));
            }
            else {
                unregister_mods(MOD_BIT(KC_LSFT) | MOD_BIT(KC_LCTL));
            }
            selwd_state = STATE_SELECTED;
            break;

        case STATE_FIRST_LINE:
            selwd_state = STATE_SELECTED;
            break;

        case STATE_LINE:
            unregister_code(KC_DOWN);
            selwd_state = STATE_SELECTED;
            break;

        case STATE_SELECTED:
            if (keycode == KC_ESC) {
                tap_code(KC_RGHT);
                selwd_state = STATE_NONE;
                return false;
            }
            // Fallthrough intended.
        default:
            selwd_state = STATE_NONE;
    }

    return true;
}

// -------------------------------------------------------------------------
// MAGIC KEY
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

// ------------------------------------------------------------------
// PROCESSING
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // MACROS OVERWRITING BEHAVIOUR:
    // Select Word
    if (!process_select_word(keycode, record, KC_SLWD)) { return false; }

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
                return false;
                break;
            case KC_NXTWD:
                if (keymap_config.swap_lctl_lgui) {
                    register_mods(mod_config(MOD_LALT));
                    register_code(KC_RIGHT);
                } else {
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_RIGHT);
                }
                return false;
                break;
            case KC_LSTRT:
                if (keymap_config.swap_lctl_lgui) {
                    //CMD-arrow on Mac, but we have CTL and GUI swapped
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_LEFT);
                } else {
                    register_code(KC_HOME);
                }
                return false;
                break;
            case KC_LEND:
                if (keymap_config.swap_lctl_lgui) {
                    //CMD-arrow on Mac, but we have CTL and GUI swapped
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_RIGHT);
                } else {
                    register_code(KC_END);
                }
                return false;
                break;
            case KC_DLINE:
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_BSPC);
                return false;
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
                break;
            case KC_CNEW:
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_N);
                return false;
                break;
            case KC_TNEW:
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_T);
                return false;
                break;
            case KC_CLSE:
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_W);
                return false;
                break;
            case KC_RFSH:
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_R);
                return false;
                break;
            case KC_SAVE:
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_S);
                return false;
                break;
            case KC_ATAB:
                // Special Alt-Tab key behaviour:
                // https://docs.splitkb.com/hc/en-us/articles/360010513760-How-can-I-use-a-rotary-encoder-
                // Start by pressing ALT(CMD)-TAB
                // Hold ALT(CMD), and press TAB to cycle
                // Stop holding ALT(CMD) after finished
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
            case KC_ZMIN:
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_EQL);
                return false;
                break;
            case KC_ZMOT:
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_MINS);
                return false;
                break;

            // customized brackets and parens macros
            case KC_CBRC:
                // Type {} and put cursor in middle
                MAGIC_STRING("{}", KC_CBRC);  // send {} with { as repeat
                tap_code(KC_LEFT);
                return false;
                break;
            case KC_SBRC:
                // Type [] and put cursor in middle
                // Shifting handled by autoshift.
                MAGIC_STRING("[]", KC_SBRC);  // send <> with < as repeat
                tap_code(KC_LEFT);
                return false;
                break;

            // customized snipping tool macro
            case KC_SNIP:
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

            // TAP HOLD
            case C_BKSP:
                // Custom backspace
                // Delete both brackets when backspacing afterwards
                switch (get_last_keycode()) {
                    case KC_9PRC:
                    case KC_CANG:
                    case KC_CBRC:
                    case KC_SBRC:
                        register_code(KC_DEL);
                }
                return process_tap_or_long_press_key(record, LCTL(KC_BSPC));  // on long press
                return false;
                break;
            case C_SRCH:
                // Custom search
                return process_tap_or_long_press_key(record, KC_SRCHB);  // on long press, open browser before searching
                return false;
                break;

            // Browser search key
            case KC_SRCHB:
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

            case KC_SRCH:
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

            // MAGIC
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
                return false;
                break;
            case KC_NXTWD:
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LALT));
                    unregister_code(KC_RIGHT);
                } else {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_RIGHT);
                }
                return false;
                break;
            case KC_LSTRT:
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_LEFT);
                } else {
                    unregister_code(KC_HOME);
                }
                return false;
                break;
            case KC_LEND:
                if (keymap_config.swap_lctl_lgui) {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_RIGHT);
                } else {
                    unregister_code(KC_END);
                }
                return false;
                break;
            case KC_DLINE:
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_BSPC);
                return false;
                break;

            // Commands
            case KC_COPY:
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_C);
                return false;
                break;
            case KC_PASTE:
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_V);
                return false;
                break;
            case KC_CUT:
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_X);
                return false;
                break;
            case KC_UNDO:
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_Z);
                return false;
                break;
            case KC_REDO:
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_Y);
                return false;
                break;
            case KC_FIND:
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_F);
                return false;
                break;
            case KC_PRNT:
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_P);
                return false;
                break;
            case KC_SLAL:
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_A);
                return false;
                break;
            case KC_CNEW:
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_N);
                return false;
                break;
            case KC_CLSE:
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_W);
                return false;
                break;
            case KC_RFSH:
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_R);
                return false;
                break;
            case KC_SAVE:
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_S);
                return false;
                break;

            // Special Alt Tab
            case KC_ATAB:
                // Special Alt-Tab key behaviour.
                // Stop typing tab, Keep the ALT(CMD). 
                // let matrix_scan_user handle dropping ALT(CMD) 
                // after specified time has passed
                unregister_code(KC_TAB);
                return false;
                break;

            // Zoom in and out
            case KC_ZMIN:
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_EQL);
                return false;
                break;
            case KC_ZMOT:
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_MINS);
                return false;
                break;

            // customized brackets and parens macros
            // case C_LPARN:
            //     // Type () and put cursor in middle
            //     // Shifting handled by autoshift.
            //     unregister_code16(KC_LPRN);
            //     unregister_code16(KC_RPRN);
            //     return false;
            //     break;
            // case C_LANGB:
            //     // Type <> and put cursor in middle
            //     // Shifting handled by autoshift.
            //     unregister_code16(KC_LABK);
            //     unregister_code16(KC_RABK);
            //     return false;
            //     break;

            // customized snipping tool macro
            case KC_SNIP:
                // Using OS Detection
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

            // Tap Hold
            case C_BKSP:
                switch (get_last_keycode()) {
                    case KC_9PRC:
                    case KC_CANG:
                    case KC_CBRC:
                    case KC_SBRC:
                        // unregister delete trailing parenthesis
                        unregister_code(KC_DEL);
                }
                return false;
                break;
            // Browser search Tap Hold
            // case KC_SRCH:
            //     unregister_mods(mod_config(MOD_LCTL));
            //     return false;  //BUG: I dunno man, is this right or wrong?
            //     break;
            case KC_SRCHB:
                return false;
                break;
        }
    }
    return true;
}
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
    // Check for end of select word behaviour
    select_word_task();
}
// -------------------------------------------------------
// ENCODERS
#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
          // Mappings for 1st Encoder          // Mappings for 2nd Encoder
    [0] = { ENCODER_CCW_CW(KC_VOLU, KC_VOLD),  ENCODER_CCW_CW(KC_WH_D, KC_WH_U)  }, // QWERTY
    [1] = { ENCODER_CCW_CW(KC_VOLU, KC_VOLD),  ENCODER_CCW_CW(KC_WH_D, KC_WH_U)  }, // STRDY
    [2] = { ENCODER_CCW_CW(KC_WH_R, KC_WH_L),  ENCODER_CCW_CW(KC_WH_D, KC_WH_U)  }, // Xtnd2
    [3] = { ENCODER_CCW_CW(KC_REDO, KC_UNDO), ENCODER_CCW_CW(KC_PRVWD, KC_NXTWD) }, // Raise
    [4] = { ENCODER_CCW_CW(KC_ZMOT, KC_ZMIN), ENCODER_CCW_CW(KC_BRIU, KC_BRID) }, // Adj
    // You can add more layers here if you need them, or you can also delete lines for layers you are not using
};
#endif  // ENCODER_MAP_ENABLE
// ----------------------------------------------------------------------
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
#    define MIN_RUN_SPEED       40

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

// OLED: QMK Logo
static void render_logo(void) {
    static const char PROGMEM qmk_logo[] = {
        0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
        0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
        0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0
    };

    oled_write_P(qmk_logo, false);
}

// OLED: PRINT
static void print_status_narrow(void) {
    // Print mac vs windows
    if (keymap_config.swap_lctl_lgui) {
        oled_write_ln_P(PSTR("MAC"), false);
    } else {
        oled_write_ln_P(PSTR("WIN"), false);
    }
    oled_write_P(PSTR("-----"), false);
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
        case _STRDY:
            oled_write_ln_P(PSTR("Base\n"), false);
            break;
        case _RAISE:
            oled_write_ln_P(PSTR("Raise"), true);
            break;
        case _EXTND:
            oled_write_ln_P(PSTR("Xtnd2"), true);
            break;
        case _ADJUST:
            oled_write_ln_P(PSTR("Adjst"), true);
            break;
        default:
            oled_write_ln_P(PSTR("uNDEf"), true);
    }
    oled_write_P(PSTR("-----"), false);
    // Statuses: Autocorrect, AltTab, CapsWord
    oled_write_P(PSTR("ACORR"), autocorrect_is_enabled());  // autocorrect
    oled_write_P(PSTR("ALTAB"), is_alt_tab_active);  // alt-tab
    oled_write_P(PSTR("CAPWD"), is_caps_word_on());  // caps word
    oled_write_P(PSTR("SELWD"), selwd_state != STATE_NONE); // select word

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
}
bool oled_task_user(void) {
    // Keyboard Pet Variables
    current_wpm   = get_current_wpm();
    led_usb_state = host_keyboard_led_state();

    if (is_keyboard_master()) {
        print_status_narrow();
    } else {
        // print_logo_narrow();
        render_logo();
    }
    return false;
}
#endif