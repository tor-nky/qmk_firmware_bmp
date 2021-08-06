
#pragma once

#undef MATRIX_ROWS
#undef MATRIX_COLS

 // Set 8 buttons + 4 gesture
#define MATRIX_ROWS 2
#define MATRIX_COLS 8
#define MATRIX_MSWHEEL_ROW 1
#define MATRIX_MSWHEEL_COL 4

// For gesture recognition trick
#define TAPPING_TERM_PER_KEY

// Use kemap_key_to_keycode in keymap.c
#define OVERRIDE_KEYMAP_KEY_TO_KEYCODE

#define DYNAMIC_KEYMAP_EEPROM_MAX_ADDR 4096
#define DYNAMIC_KEYMAP_LAYER_COUNT 6

#undef PRODUCT_ID
#define PRODUCT_ID   0x3999

#undef PRODUCT
#define PRODUCT mouse_quantizer

#define USB_MOUSE_POLLING_INTERVAL_MS 1
