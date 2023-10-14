#pragma once

/* The way how "handedness" is decided (which half is which),
see https://docs.qmk.fm/#/feature_split_keyboard?id=setting-handedness
for more options.
*/
// Unfortunately, some applications drop or misorder fast key events. This is a
// partial fix to slow down the rate at which macros are sent.
// #define TAP_CODE_DELAY 12

// Auto Shift
#define AUTO_SHIFT_TIMEOUT 150  // should be 135-150ms, but we start longer at 175 to get used to it
#define AUTO_SHIFT_TIMEOUT_PER_KEY
#define NO_AUTO_SHIFT_TAB

// Tap-Hold Key Decision
// #define TAPPING_TERM 175
#define TAPPING_TERM_PER_KEY // tap-hold time decision

// Tap Toggle Layer Switching
#define ONESHOT_TAP_TOGGLE 2  /* Tapping this number of times holds the key until tapped once again. */
#define TAPPING_TOGGLE 2  // Used for TT layer switching

// When idle, turn off Caps Word after 5 seconds.
#define CAPS_WORD_IDLE_TIMEOUT 5000