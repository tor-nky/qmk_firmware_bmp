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

#include "bmp_custom_keycode.h" // BMP用

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

void ng_cut(void);
void ng_copy(void);
void ng_paste(void);
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
#ifdef ENABLE_NG_IOS
void ios_send_string(const char *);
void ios_send_string_with_cut_paste(const char *);
#endif

// かな定義配列 ngmap[] の添字の型
typedef uint_fast16_t Ngmap_num;

bool naginata_type(uint16_t, bool);
bool ng_search_and_send(uint32_t);
Ngmap_num ng_search_with_rest_key(uint32_t, uint32_t);
int8_t number_of_candidates(uint32_t);

// bool enable_naginata(uint16_t, keyrecord_t *);

// なぜKC_キーコードを使わず、NG_キーコードを定義するのか
// 1. 英字レイアウトがQWERTYでない場合でもOK
// 2. 薙刀式レイヤーでもKC_を定義すれば、かな変換せず出力できる
typedef enum naginata_keycodes {
  NG_Q = BMP_SAFE_RANGE + 2, // 薙刀式シフトキー    // BMP用
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
  NG_CLR,
  NGSW_WIN,
  NGSW_MAC,
  NGSW_LNX,
  NGSW_IOS,
  NG_MLV,
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

#define NG_SAFE_RANGE NG_Q + 43

#define NG_WIN 1
#define NG_MAC 2
#define NG_LINUX 3
#define NG_IOS 254
