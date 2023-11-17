#pragma once

#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET true

/* The way how "handedness" is decided (which half is which),
see https://docs.qmk.fm/#/feature_split_keyboard?id=setting-handedness
for more options.
*/
// Unfortunately, some applications drop or misorder fast key events. This is a
// partial fix to slow down the rate at which macros are sent.
// #define TAP_CODE_DELAY 12

// ------------------------------------------------------------------------------------------------
// Auto Shift
#define AUTO_SHIFT_TIMEOUT 160  // should be 135-150ms, but we start longer at 175 to get used to it
#define AUTO_SHIFT_TIMEOUT_PER_KEY true  // allow for repeating characters if held down
#define NO_AUTO_SHIFT_TAB true // but don't shift tab.
#define NO_AUTO_SHIFT_SYMBOLS true  // don't shift symbols; we control this ourselves.

// ------------------------------------------------------------------------------------------------
// Alt Tab
#define ALT_TAB_TIMEOUT 350

// ------------------------------------------------------------------------------------------------
// Select Word
#define SELECT_WORD_TIMEOUT 2000  // When idle, clear state after 2 seconds.

// ------------------------------------------------------------------------------------------------
// Tap-Hold Key Decision
// #define TAPPING_TERM 175
#define TAPPING_TERM_PER_KEY true // tap-hold time decision

// ------------------------------------------------------------------------------------------------
// Tap Toggle Layer Switching
#define ONESHOT_TAP_TOGGLE 2  /* Tapping this number of times holds the key until tapped once again. */
#define TAPPING_TOGGLE 2  // Used for TT layer switching

// ------------------------------------------------------------------------------------------------
// Caps Word
#define CAPS_WORD_IDLE_TIMEOUT 5000  // When idle, turn off Caps Word after 5 seconds.
#define CAPS_WORD_INVERT_ON_SHIFT true  // shift reverses caps

// -------------------------------------------------------------------
// COMBOS
// #define COMBO_TERM_PER_COMBO
#define COMBO_MUST_HOLD_PER_COMBO
#define COMBO_MUST_TAP_PER_COMBO
#define COMBO_MUST_PRESS_IN_ORDER_PER_COMBO

// ------------------------------------------------------------------------------------------------
// Previous Key Behaviour
#define PREVKEY_TIMEOUT_MS 5000  // Timeout in milliseconds.
#define PREVKEY_RECENT_SIZE 8   // Number of keys in `recent` buffer.

// ------------------------------------------------------------------------------------------------
// Sentence Case
#define SENTENCE_CASE_BUFFER_SIZE 8
#define SENTENCE_CASE_STATE_HISTORY_SIZE 6
// #define SENTENCE_CASE_TIMEOUT 1000

// ------------------------------------------------------------------------------------------------
// WPM Calculation
#define WPM_ALLOW_COUNT_REGRESSION  // factor backspace
#define WPM_LAUNCH_CONTROL  // calculate wpm only during typing

// ------------------------------------------------------------------------------------------------
// Split Keyboard
// #define SPLIT_KEYBOARD = yes
// #define MASTER_LEFT
#define SPLIT_WPM_ENABLE