/* Copyright 2018-2019 eswai <@eswai> / Satoru NAKAYA <@tor-nky>
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

// *************** 薙刀式の設定 ***************
// 有効にするとスペースキーを離しても文字キーシフト全復活
// 無効にするとDvorakJ風になる
// #define NG_USE_SHIFT_WHEN_SPACE_UP

// 後置シフトの時間制限(ms) ※ 無効にすれば時間無制限
// ※ 有効にするなら薙刀式v15のかな定義を変更しない
//  (Shift+2キー以上の同時押し、いわゆる冗長シフトをつけないこと)
//  (1キー単独で出力確定する定義がないこと、つまり編集モードを削らないこと)
#define NG_KOUCHI_SHIFT_MS 60
// ********************************************


// 互換性
#if !(defined(IS_QK_KB) || defined(KC_LNG9))
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
#   define QK_MODS_GET_BASIC_KEYCODE(kc) ((kc)&0xFF)
#   define IS_QK_MOD_TAP(code) ((code) >= QK_MOD_TAP && (code) <= QK_MOD_TAP_MAX)
#   define IS_MODIFIER_KEYCODE(code) ((code) >= KC_LEFT_CTRL && (code) <= KC_RIGHT_GUI)
# ifdef NG_BMP
#   include "bmp_custom_keycode.h" // For BLE Miro Pro old firmware(<1.0.0)
# endif
#elif (defined(NG_BMP))
#   define NG_BMP_VIAL
#   include "bmp_custom_keycodes.h" // For BLE Miro Pro new firmware(>=1.0.0)
#endif

void naginata_clear(void);

bool process_modifier(uint16_t, keyrecord_t *);

void naginata_on(void);
void naginata_off(void);
bool naginata_state(void);

void switchOS(uint8_t);
void ng_set_unicode_mode(uint8_t);
void ng_show_os(void);
void ng_send_unicode_string_P(const char *);
void mac_live_conversion_toggle(void);
void tategaki_toggle(void);
void kouchi_shift_toggle(void);

bool process_naginata(uint16_t, keyrecord_t *);
// void set_naginata(uint8_t);
void set_naginata(uint8_t, uint16_t *, uint16_t *);

typedef struct {
  uint16_t keycode;
  uint16_t pressTime;
  uint16_t releaseTime;
} Keystroke;

// 同時押し等の状態を示す、各ビットがキーに対応する型
typedef uint32_t Ngkey;
// かな定義配列 ngmap[] の添字の型
typedef uint_fast16_t Ngmap_num;

bool naginata_type(uint16_t, keyrecord_t *);
void end_repeating_key(void);

void ng_space_or_enter(void);
void ng_backspace_with_repeat(void);  // {BS}
void ng_delete_with_repeat(void); // {Del}
void ng_cut(void);
void ng_copy(void);
void ng_paste(void);
void ng_move_cursor_with_repeat(bool, uint8_t, uint8_t);
void ng_up(uint8_t);
void ng_down(uint8_t);
void ng_left(uint8_t);
void ng_right(uint8_t);
void ng_home(void);
void ng_end(void);
void ng_katakana(void);
void ng_save(void);
void ng_hiragana(void);
void ng_redo(void);
void ng_undo(void);
void ng_saihenkan(void);
void ng_eof(void);
void ng_ime_cancel(void);
void ng_ime_complete(void);
void dic_send_string(const char *);
void dic_send_string_with_cut_paste(const char *);
void ng_send_kana(const char *str);

#define NG_SEND_KANA(string) ng_send_kana(PSTR(string))


// なぜKC_キーコードを使わず、NG_キーコードを定義するのか
// 1. 英字レイアウトがQWERTYでない場合でもOK
// 2. 薙刀式レイヤーでもKC_を定義すれば、かな変換せず出力できる
typedef enum naginata_keycodes {
#if !(defined(NG_BMP))
  NG_Q = SAFE_RANGE,  // 薙刀式シフトキー
#elif (defined(NG_BMP_VIAL))
  NG_Q = BMP_SAFE_RANGE - 4,  // 薙刀式シフトキー // For BLE Miro Pro new firmware(>=1.0.0)
#else  // 参照: https://github.com/sekigon-gonnoc/vial-qmk/blob/dev/ble-micro-pro/tmk_core/protocol/bmp/bmp_custom_keycodes.h
  NG_Q = BMP_SAFE_RANGE + 2,  // 薙刀式シフトキー // For BLE Miro Pro old firmware(<1.0.0)
#endif
  NG_W,
  NG_E,
  NG_R,
  NG_T,
  NG_Y,
  NG_U,
  NG_I,
  NG_O,
  NG_P,

  NG_A,
  NG_S,
  NG_D,
  NG_F,
  NG_G,
  NG_H,
  NG_J,
  NG_K,
  NG_L,
  NG_SCLN,

  NG_Z,
  NG_X,
  NG_C,
  NG_V,
  NG_B,
  NG_N,
  NG_M,
  NG_COMM,
  NG_DOT,
  NG_SLSH,

  NG_SHFT,
  NG_SHFT2,

  NG_ON,
  NG_OFF,
#ifndef NG_BMP
  NG_CLR,
#endif
  NGSW_WIN,
  NGSW_MAC,
  NGSW_LNX,
#ifdef NG_BMP
  NGSW_IOS,
#else
  NG_MLV,
#endif
  NG_SHOS,
  NG_TAYO,
  NG_KOTI,
} NGKEYS;

// EEPROMに保存する設定
typedef union {
  uint32_t raw;
  struct {
    uint8_t os;
    bool live_conv :1;
    bool tategaki :1;
    bool kouchi_shift :1;
  };
} user_config_t;

user_config_t naginata_config;

#define NG_SAFE_RANGE NG_KOTI + 1

#define NG_WIN 1
#define NG_MAC 2
#define NG_LINUX 3
#define NG_WIN_DIC  251
#define NG_MAC_DIC  252
#define NG_LINUX_DIC 253
#define NG_IOS      254
