/* Copyright 2019 sekigon-gonnoc
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "bmp.h"
#include "bmp_custom_keycode.h"
#include "keycode_str_converter.h"

#include "keymap_jp.h"

// 薙刀式
#include "naginata.h"
NGKEYS naginata_keys;
// 薙刀式

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    LOWER = BMP_SAFE_RANGE,
    RAISE,
};

const key_string_map_t custom_keys_user =
{
    .start_kc = LOWER,
//    .end_kc = RAISE,
//    .key_strings = "LOWER\0RAISE\0"
    .end_kc = NG_KOTI,
    .key_strings = "LOWER\0RAISE\0NG_Q\0NG_W\0NG_E\0NG_R\0NG_T\0NG_Y\0NG_U\0NG_I\0NG_O\0NG_P\0NG_A\0NG_S\0NG_D\0NG_F\0NG_G\0NG_H\0NG_J\0NG_K\0NG_L\0NG_SCLN\0NG_Z\0NG_X\0NG_C\0NG_V\0NG_B\0NG_N\0NG_M\0NG_COMM\0NG_DOT\0NG_SLSH\0NG_SHFT\0NG_SHFT2\0NG_ON\0NG_OFF\0NG_CLR\0NGSW_WIN\0NGSW_MAC\0NGSW_LNX\0NGSW_IOS\0NG_MLV\0NG_SHOS\0NG_TAYO\0NG_KOTI\0"
};

enum layers {
    _BASE,
    _NAGINATA, // 薙刀式入力レイヤー
    _LOWER, _RAISE, _ADJUST,
    _BMP
};

const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    {{
    KC_A, KC_B, KC_C, KC_D, KC_E, KC_F, KC_G, KC_H, KC_I,
        KC_J, KC_K, KC_L, KC_M, KC_N, KC_O, KC_P, KC_Q, KC_R, KC_S
    }}
};

uint32_t keymaps_len() {
  return 19;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  bool continue_process = process_record_user_bmp(keycode, record);
  if (continue_process == false)
  {
    return false;
  }

  switch (keycode) {
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
    default:
      // 薙刀式
      if (!process_naginata(keycode, record))
        return false;
      // 薙刀式
      break;
  }

  return true;
}

void matrix_init_user(void) {
  // 薙刀式
  uint16_t ngonkeys[] = {KC_H, KC_J};
  uint16_t ngoffkeys[] = {KC_F, KC_G};
  set_naginata(_NAGINATA, ngonkeys, ngoffkeys);
  // 薙刀式
}
