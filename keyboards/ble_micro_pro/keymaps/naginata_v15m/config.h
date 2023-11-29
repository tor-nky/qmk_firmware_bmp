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

#pragma once

// place overrides here

// 互換性
#ifndef IS_QK_KB
#   define KC_LANGUAGE_1 KC_LANG1
#   define KC_LANGUAGE_2 KC_LANG2
#   define KC_INTERNATIONAL_2 KC_KANA
#   define KC_INTERNATIONAL_4 KC_HENK
#   define KC_INTERNATIONAL_5 KC_MHEN
#   define KC_NUM_LOCK KC_NUMLOCK
#   define KC_LEFT_SHIFT KC_LSFT
#   define KC_LEFT_ALT KC_LALT
#   define KC_LEFT_GUI KC_LGUI
#   define KC_LEFT_CTRL KC_LCTL
#   define KC_RIGHT_CTRL KC_RCTL
#   define KC_RIGHT_SHIFT KC_RSFT
#   define KC_RIGHT_ALT KC_RALT
#   define KC_RIGHT_GUI KC_RGUI
#   define KC_BACKSPACE KC_BSPACE
#   define X_BACKSPACE X_BSPACE
#   define UNICODE_MODE_WINCOMPOSE UC_WINC
#   define UNICODE_MODE_MACOS UC_MAC
#   define UNICODE_MODE_LINUX UC_LNX
#   define IS_QK_MOD_TAP(code) ((code) >= QK_MOD_TAP && (code) <= QK_MOD_TAP_MAX)
#   define IS_MODIFIER_KEYCODE(code) ((code) >= KC_LEFT_CTRL && (code) <= KC_RIGHT_GUI)
#endif


// iOS用を使うとき
#define ENABLE_NG_IOS

//#define MAC_USE_KAWASEMI // Mac汎用 → かわせみ専用

#ifndef MAC_USE_KAWASEMI
// UNICODE入力を安定化
//#define UNICODE_TYPE_DELAY 32     // かわせみ3では不要
#else
// Keycodes used for starting Unicode input on different platforms
#   define UNICODE_KEY_MAC KC_NO   // かわせみ3用
#endif
