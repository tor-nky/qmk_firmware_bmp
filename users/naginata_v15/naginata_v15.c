/* Copyright 2018-2022 eswai <@eswai>
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
#include "twpair_on_jis.h"
#include "naginata.h"

#include <string.h>

#define NGBUFFER 10 // キー入力バッファのサイズ

// カーソル定義
// あとで縦書き横書きに合わせて置換するための仮の定義
#define NGUP X_LANG6
#define NGDN X_LANG7
#define NGLT X_LANG8
#define NGRT X_LANG9
#define KAKT X_OUT    // 確定

static uint8_t ng_chrcount = 0; // 文字キー入力のカウンタ
static bool is_naginata = false; // 薙刀式がオンかオフか
static uint8_t naginata_layer = 0; // NG_*を配置しているレイヤー番号
static uint32_t keycomb = 0UL; // 同時押しの状態を示す。32bitの各ビットがキーに対応する。
static uint16_t ngon_keys[2]; // 薙刀式をオンにするキー(通常HJ)
static uint16_t ngoff_keys[2]; // 薙刀式をオフにするキー(通常FG)

// 31キーを32bitの各ビットに割り当てる
#define B_Q    (1UL<<0)
#define B_W    (1UL<<1)
#define B_E    (1UL<<2)
#define B_R    (1UL<<3)
#define B_T    (1UL<<4)

#define B_Y    (1UL<<5)
#define B_U    (1UL<<6)
#define B_I    (1UL<<7)
#define B_O    (1UL<<8)
#define B_P    (1UL<<9)

#define B_A    (1UL<<10)
#define B_S    (1UL<<11)
#define B_D    (1UL<<12)
#define B_F    (1UL<<13)
#define B_G    (1UL<<14)

#define B_H    (1UL<<15)
#define B_J    (1UL<<16)
#define B_K    (1UL<<17)
#define B_L    (1UL<<18)
#define B_SCLN (1UL<<19)

#define B_Z    (1UL<<20)
#define B_X    (1UL<<21)
#define B_C    (1UL<<22)
#define B_V    (1UL<<23)
#define B_B    (1UL<<24)

#define B_N    (1UL<<25)
#define B_M    (1UL<<26)
#define B_COMM (1UL<<27)
#define B_DOT  (1UL<<28)
#define B_SLSH (1UL<<29)

#define B_SHFT (1UL<<30)

// 文字入力バッファ
static uint16_t ninputs[NGBUFFER];

// キーコードとキービットの対応
// メモリ削減のため配列はNG_Qを0にしている
const uint32_t ng_key[] = {
  [NG_Q    - NG_Q] = B_Q,
  [NG_W    - NG_Q] = B_W,
  [NG_E    - NG_Q] = B_E,
  [NG_R    - NG_Q] = B_R,
  [NG_T    - NG_Q] = B_T,

  [NG_Y    - NG_Q] = B_Y,
  [NG_U    - NG_Q] = B_U,
  [NG_I    - NG_Q] = B_I,
  [NG_O    - NG_Q] = B_O,
  [NG_P    - NG_Q] = B_P,

  [NG_A    - NG_Q] = B_A,
  [NG_S    - NG_Q] = B_S,
  [NG_D    - NG_Q] = B_D,
  [NG_F    - NG_Q] = B_F,
  [NG_G    - NG_Q] = B_G,

  [NG_H    - NG_Q] = B_H,
  [NG_J    - NG_Q] = B_J,
  [NG_K    - NG_Q] = B_K,
  [NG_L    - NG_Q] = B_L,
  [NG_SCLN - NG_Q] = B_SCLN,

  [NG_Z    - NG_Q] = B_Z,
  [NG_X    - NG_Q] = B_X,
  [NG_C    - NG_Q] = B_C,
  [NG_V    - NG_Q] = B_V,
  [NG_B    - NG_Q] = B_B,

  [NG_N    - NG_Q] = B_N,
  [NG_M    - NG_Q] = B_M,
  [NG_COMM - NG_Q] = B_COMM,
  [NG_DOT  - NG_Q] = B_DOT,
  [NG_SLSH - NG_Q] = B_SLSH,

  [NG_SHFT - NG_Q] = B_SHFT,
  [NG_SHFT2 - NG_Q] = B_SHFT,
};

// カナ変換テーブル
typedef struct {
  uint32_t key;
  char kana[6];
} naginata_keymap;

// ロング
typedef struct {
  uint32_t key;
  char kana[16];
} naginata_keymap_long;

typedef struct {
  uint32_t key;
} naginata_keymap_macro;

// UNICODE
typedef struct {
  uint32_t key;
  char win[7];
  char mac[9];
} naginata_keymap_unicode;

const PROGMEM naginata_keymap ngmap[] = {
  // 清音
  {.key = B_J                      , .kana = "a"       }, // あ
  {.key = B_K                      , .kana = "i"       }, // い
  {.key = B_L                      , .kana = "u"       }, // う
  {.key = B_SHFT|B_O               , .kana = "e"       }, // え
  {.key = B_SHFT|B_N               , .kana = "o"       }, // お
  {.key = B_F                      , .kana = "ka"      }, // か
  {.key = B_W                      , .kana = "ki"      }, // き
  {.key = B_H                      , .kana = "ku"      }, // く
  {.key = B_S                      , .kana = "ke"      }, // け
  {.key = B_V                      , .kana = "ko"      }, // こ
  {.key = B_SHFT|B_U               , .kana = "sa"      }, // さ
  {.key = B_R                      , .kana = "si"      }, // し
  {.key = B_O                      , .kana = "su"      }, // す
  {.key = B_SHFT|B_A               , .kana = "se"      }, // せ
  {.key = B_B                      , .kana = "so"      }, // そ
  {.key = B_N                      , .kana = "ta"      }, // た
  {.key = B_SHFT|B_G               , .kana = "ti"      }, // ち
  {.key = B_SHFT|B_L               , .kana = "tu"      }, // つ
  {.key = B_E                      , .kana = "te"      }, // て
  {.key = B_D                      , .kana = "to"      }, // と
  {.key = B_M                      , .kana = "na"      }, // な
  {.key = B_SHFT|B_D               , .kana = "ni"      }, // に
  {.key = B_SHFT|B_W               , .kana = "nu"      }, // ぬ
  {.key = B_SHFT|B_R               , .kana = "ne"      }, // ね
  {.key = B_SHFT|B_J               , .kana = "no"      }, // の
  {.key = B_C                      , .kana = "ha"      }, // は
  {.key = B_X                      , .kana = "hi"      }, // ひ
  {.key = B_SHFT|B_X               , .kana = "hi"      }, // ひ
  {.key = B_SHFT|B_SCLN            , .kana = "hu"      }, // ふ
  {.key = B_P                      , .kana = "he"      }, // へ
  {.key = B_Z                      , .kana = "ho"      }, // ほ
  {.key = B_SHFT|B_Z               , .kana = "ho"      }, // ほ
  {.key = B_SHFT|B_F               , .kana = "ma"      }, // ま
  {.key = B_SHFT|B_B               , .kana = "mi"      }, // み
  {.key = B_SHFT|B_COMM            , .kana = "mu"      }, // む
  {.key = B_SHFT|B_S               , .kana = "me"      }, // め
  {.key = B_SHFT|B_K               , .kana = "mo"      }, // も
  {.key = B_SHFT|B_H               , .kana = "ya"      }, // や
  {.key = B_SHFT|B_P               , .kana = "yu"      }, // ゆ
  {.key = B_SHFT|B_I               , .kana = "yo"      }, // よ
  {.key = B_DOT                    , .kana = "ra"      }, // ら
  {.key = B_SHFT|B_E               , .kana = "ri"      }, // り
  {.key = B_I                      , .kana = "ru"      }, // る
  {.key = B_SLSH                   , .kana = "re"      }, // れ
  {.key = B_SHFT|B_SLSH            , .kana = "re"      }, // れ
  {.key = B_A                      , .kana = "ro"      }, // ろ
  {.key = B_SHFT|B_DOT             , .kana = "wa"      }, // わ
  {.key = B_SHFT|B_C               , .kana = "wo"      }, // を
  {.key = B_COMM                   , .kana = "nn"      }, // ん
  {.key = B_SCLN                   , .kana = "-"       }, // ー
  {.key = B_Q                      , .kana = "vu"      }, // ヴ
  {.key = B_SHFT|B_Q               , .kana = "vu"      }, // ヴ

  // 濁音
  {.key = B_J|B_F                  , .kana = "ga"      }, // が
  {.key = B_J|B_W                  , .kana = "gi"      }, // ぎ
  {.key = B_F|B_H                  , .kana = "gu"      }, // ぐ
  {.key = B_J|B_S                  , .kana = "ge"      }, // げ
  {.key = B_J|B_V                  , .kana = "go"      }, // ご
  {.key = B_F|B_U                  , .kana = "za"      }, // ざ
  {.key = B_J|B_R                  , .kana = "zi"      }, // じ
  {.key = B_F|B_O                  , .kana = "zu"      }, // ず
  {.key = B_J|B_A                  , .kana = "ze"      }, // ぜ
  {.key = B_J|B_B                  , .kana = "zo"      }, // ぞ
  {.key = B_F|B_N                  , .kana = "da"      }, // だ
  {.key = B_J|B_G                  , .kana = "di"      }, // ぢ
  {.key = B_F|B_L                  , .kana = "du"      }, // づ
  {.key = B_J|B_E                  , .kana = "de"      }, // で
  {.key = B_J|B_D                  , .kana = "do"      }, // ど
  {.key = B_J|B_C                  , .kana = "ba"      }, // ば
  {.key = B_J|B_X                  , .kana = "bi"      }, // び
  {.key = B_F|B_SCLN               , .kana = "bu"      }, // ぶ
  {.key = B_F|B_P                  , .kana = "be"      }, // べ
  {.key = B_J|B_Z                  , .kana = "bo"      }, // ぼ

  // 半濁音
  {.key = B_M|B_C                  , .kana = "pa"      }, // ぱ
  {.key = B_M|B_X                  , .kana = "pi"      }, // ぴ
  {.key = B_V|B_SCLN               , .kana = "pu"      }, // ぷ
  {.key = B_V|B_P                  , .kana = "pe"      }, // ぺ
  {.key = B_M|B_Z                  , .kana = "po"      }, // ぽ

  // 小書き
  {.key = B_Q|B_H                  , .kana = "xya"     }, // ゃ
  {.key = B_Q|B_P                  , .kana = "xyu"     }, // ゅ
  {.key = B_Q|B_I                  , .kana = "xyo"     }, // ょ
  {.key = B_Q|B_J                  , .kana = "xa"      }, // ぁ
  {.key = B_Q|B_K                  , .kana = "xi"      }, // ぃ
  {.key = B_Q|B_L                  , .kana = "xu"      }, // ぅ
  {.key = B_Q|B_O                  , .kana = "xe"      }, // ぇ
  {.key = B_Q|B_N                  , .kana = "xo"      }, // ぉ
  {.key = B_Q|B_DOT                , .kana = "xwa"     }, // ゎ
  {.key = B_G                      , .kana = "xtu"     }, // っ

  // 清音拗音 濁音拗音 半濁拗音
  {.key = B_R|B_H                  , .kana = "sya"     }, // しゃ
  {.key = B_R|B_P                  , .kana = "syu"     }, // しゅ
  {.key = B_R|B_I                  , .kana = "syo"     }, // しょ
  {.key = B_J|B_R|B_H              , .kana = "ja"      }, // じゃ
  {.key = B_J|B_R|B_P              , .kana = "ju"      }, // じゅ
  {.key = B_J|B_R|B_I              , .kana = "jo"      }, // じょ
  {.key = B_W|B_H                  , .kana = "kya"     }, // きゃ
  {.key = B_W|B_P                  , .kana = "kyu"     }, // きゅ
  {.key = B_W|B_I                  , .kana = "kyo"     }, // きょ
  {.key = B_J|B_W|B_H              , .kana = "gya"     }, // ぎゃ
  {.key = B_J|B_W|B_P              , .kana = "gyu"     }, // ぎゅ
  {.key = B_J|B_W|B_I              , .kana = "gyo"     }, // ぎょ
  {.key = B_G|B_H                  , .kana = "tya"     }, // ちゃ
  {.key = B_G|B_P                  , .kana = "tyu"     }, // ちゅ
  {.key = B_G|B_I                  , .kana = "tyo"     }, // ちょ
  {.key = B_J|B_G|B_H              , .kana = "dya"     }, // ぢゃ
  {.key = B_J|B_G|B_P              , .kana = "dyu"     }, // ぢゅ
  {.key = B_J|B_G|B_I              , .kana = "dyo"     }, // ぢょ
  {.key = B_D|B_H                  , .kana = "nya"     }, // にゃ
  {.key = B_D|B_P                  , .kana = "nyu"     }, // にゅ
  {.key = B_D|B_I                  , .kana = "nyo"     }, // にょ
  {.key = B_X|B_H                  , .kana = "hya"     }, // ひゃ
  {.key = B_X|B_P                  , .kana = "hyu"     }, // ひゅ
  {.key = B_X|B_I                  , .kana = "hyo"     }, // ひょ
  {.key = B_J|B_X|B_H              , .kana = "bya"     }, // びゃ
  {.key = B_J|B_X|B_P              , .kana = "byu"     }, // びゅ
  {.key = B_J|B_X|B_I              , .kana = "byo"     }, // びょ
  {.key = B_M|B_X|B_H              , .kana = "pya"     }, // ぴゃ
  {.key = B_M|B_X|B_P              , .kana = "pyu"     }, // ぴゅ
  {.key = B_M|B_X|B_I              , .kana = "pyo"     }, // ぴょ
  {.key = B_B|B_H                  , .kana = "mya"     }, // みゃ
  {.key = B_B|B_P                  , .kana = "myu"     }, // みゅ
  {.key = B_B|B_I                  , .kana = "myo"     }, // みょ
  {.key = B_E|B_H                  , .kana = "rya"     }, // りゃ
  {.key = B_E|B_P                  , .kana = "ryu"     }, // りゅ
  {.key = B_E|B_I                  , .kana = "ryo"     }, // りょ

  // 清音外来音 濁音外来音
  {.key = B_M|B_E|B_K              , .kana = "thi"     }, // てぃ
  {.key = B_M|B_E|B_P              , .kana = "thu"     }, // てゅ
  {.key = B_J|B_E|B_K              , .kana = "dhi"     }, // でぃ
  {.key = B_J|B_E|B_P              , .kana = "dhu"     }, // でゅ
  {.key = B_M|B_D|B_L              , .kana = "twu"     }, // とぅ
  {.key = B_J|B_D|B_L              , .kana = "dwu"     }, // どぅ
  {.key = B_M|B_R|B_O              , .kana = "sye"     }, // しぇ
  {.key = B_M|B_G|B_O              , .kana = "tye"     }, // ちぇ
  {.key = B_J|B_R|B_O              , .kana = "zye"     }, // じぇ
  {.key = B_J|B_G|B_O              , .kana = "dye"     }, // ぢぇ
  {.key = B_V|B_SCLN|B_J           , .kana = "fa"      }, // ふぁ
  {.key = B_V|B_SCLN|B_K           , .kana = "fi"      }, // ふぃ
  {.key = B_V|B_SCLN|B_O           , .kana = "fe"      }, // ふぇ
  {.key = B_V|B_SCLN|B_N           , .kana = "fo"      }, // ふぉ
  {.key = B_V|B_SCLN|B_P           , .kana = "fyu"     }, // ふゅ
  {.key = B_V|B_K|B_O              , .kana = "ye"      }, // いぇ
  {.key = B_V|B_L|B_K              , .kana = "wi"      }, // うぃ
  {.key = B_V|B_L|B_O              , .kana = "we"      }, // うぇ
  {.key = B_V|B_L|B_N              , .kana = "who"     }, // うぉ
  {.key = B_M|B_Q|B_J              , .kana = "va"      }, // ヴぁ
  {.key = B_M|B_Q|B_K              , .kana = "vi"      }, // ヴぃ
  {.key = B_M|B_Q|B_O              , .kana = "ve"      }, // ヴぇ
  {.key = B_M|B_Q|B_N              , .kana = "vo"      }, // ヴぉ
  {.key = B_M|B_Q|B_P              , .kana = "vyu"     }, // ヴゅ
  {.key = B_V|B_H|B_J              , .kana = "qa"      }, // くぁ
  {.key = B_V|B_H|B_K              , .kana = "qi"      }, // くぃ
  {.key = B_V|B_H|B_O              , .kana = "qe"      }, // くぇ
  {.key = B_V|B_H|B_N              , .kana = "qo"      }, // くぉ
  {.key = B_V|B_H|B_DOT            , .kana = "kuxwa"   }, // くゎ
  {.key = B_F|B_H|B_J              , .kana = "gwa"     }, // ぐぁ
  {.key = B_F|B_H|B_K              , .kana = "gwi"     }, // ぐぃ
  {.key = B_F|B_H|B_O              , .kana = "gwe"     }, // ぐぇ
  {.key = B_F|B_H|B_N              , .kana = "gwo"     }, // ぐぉ
  {.key = B_F|B_H|B_DOT            , .kana = "guxwa"   }, // ぐゎ
  {.key = B_V|B_L|B_J              , .kana = "tsa"     }, // つぁ

  // 追加
  {.key = B_SHFT            , .kana = " "},
  {.key = B_V|B_SHFT        , .kana = ",\n"},  // 、{Enter}
  {.key = B_M|B_SHFT        , .kana = ".\n"},  // 。{Enter}
  {.key = B_U               , .kana = "\b"},

  // enter
  {.key = B_V|B_M           , .kana = "\n"},
  // enter+シフト(連続シフト)
  {.key = B_SHFT|B_V|B_M    , .kana = "\n"},
};

const PROGMEM naginata_keymap_long ngmapl[] = {
// 編集モード Win/Linux
  {.key = B_J|B_K|B_Q		, .kana = SS_LCTL(SS_TAP(X_END))}, // ^{End}
//{.key = B_J|B_K|B_W		, .macro , // 《》{改行}{↑}
  {.key = B_J|B_K|B_R		, .kana = SS_LCTL("s")}, // ^s
  {.key = B_J|B_K|B_T		, .kana = "/"}, // ・
//{.key = B_J|B_K|B_A		, .win   = "……",		.mac = "nagitete"}, // ……{改行}
//{.key = B_J|B_K|B_S		, .macro , // (){改行}{↑}
  {.key = B_J|B_K|B_D		, .kana = "?\n"}, // ？{改行}
//{.key = B_J|B_K|B_F		, .macro , // 「」{改行}{↑}
//{.key = B_J|B_K|B_G		, .macro , // 『』{改行}{↑}
//{.key = B_J|B_K|B_Z		, .macro , // ││{改行}
//{.key = B_J|B_K|B_X		, .macro , // 【】{改行}{↑}
  {.key = B_J|B_K|B_C		, .kana = "!\n"}, // ！{改行}
  {.key = B_J|B_K|B_V		, .kana = SS_TAP(KAKT)SS_TAP(NGDN)}, // {改行}{↓}
//{.key = B_J|B_K|B_B		, .win   = "／",		.mac = "naginame"}, // ／{改行}
  {.key = B_D|B_F|B_Y		, .kana = SS_TAP(X_HOME)}, // {Home}
  {.key = B_D|B_F|B_U		, .kana = SS_LSFT(SS_TAP(X_END))"\b"}, // +{End}{BS}
  {.key = B_D|B_F|B_I		, .kana = SS_LGUI(SS_TAP(X_SLSH))}, // {vk1Csc079}
  {.key = B_D|B_F|B_O		, .kana = "\x7F"}, // {Del}
  {.key = B_D|B_F|B_P		, .kana = "\x1B\x1B\x1B"}, // {Esc 3}
  {.key = B_D|B_F|B_H		, .kana = SS_TAP(KAKT)SS_TAP(X_END)}, // {Enter}{End}
//{.key = B_D|B_F|B_J		, .macro , // {↑}
//{.key = B_D|B_F|B_K		, .macro , // +{↑}
//{.key = B_D|B_F|B_L		, .macro , // +{↑ 7}
  {.key = B_D|B_F|B_SCLN		, .kana = SS_TAP(X_F7)}, // ^i
  {.key = B_D|B_F|B_N		, .kana = SS_TAP(X_END)}, // {End}
//{.key = B_D|B_F|B_M		, .macro , // {↓}
//{.key = B_D|B_F|B_COMM		, .macro , // +{↓}
//{.key = B_D|B_F|B_DOT		, .macro , // +{↓ 7}
  {.key = B_D|B_F|B_SLSH		, .kana = SS_TAP(X_F6)}, // ^u
  {.key = B_M|B_COMM|B_Q		, .kana = SS_TAP(X_HOME)"\x7F\x7F\x7F\b"SS_TAP(NGLT)}, // {Home}{Del 3}{BS}{←}
//{.key = B_M|B_COMM|B_W		, .macro , // ^x｜{改行}^v《》{改行}{↑}
  {.key = B_M|B_COMM|B_E		, .kana = SS_TAP(X_HOME)"\n   "SS_TAP(NGLT)}, // {Home}{改行}{Space 3}{←}
  {.key = B_M|B_COMM|B_R		, .kana = "   "}, // {Space 3}
//{.key = B_M|B_COMM|B_T		, .win   = "〇",		.mac = "nagimaru"}, // 〇{改行}
  {.key = B_M|B_COMM|B_A		, .kana = SS_TAP(X_HOME)"\x7F\b"SS_TAP(NGLT)}, // {Home}{Del 1}{BS}{←}
//{.key = B_M|B_COMM|B_S		, .macro , // ^x(^v){改行}{Space}+{↑}^x
  {.key = B_M|B_COMM|B_D		, .kana = SS_TAP(X_HOME)"\n "SS_TAP(NGLT)}, // {Home}{改行}{Space 1}{←}
//{.key = B_M|B_COMM|B_F		, .macro , // ^x「^v」{改行}{Space}+{↑}^x
//{.key = B_M|B_COMM|B_G		, .macro , // ^x『^v』{改行}{Space}+{↑}^x
//{.key = B_M|B_COMM|B_Z		, .macro , // 　　　×　　　×　　　×{改行 2}
//{.key = B_M|B_COMM|B_X		, .macro , // ^x【^v】{改行}{Space}+{↑}^x
  {.key = B_M|B_COMM|B_C		, .kana = SS_TAP(KAKT)SS_TAP(X_END)"\n"}, // {改行}{End}{改行}}
//{.key = B_M|B_COMM|B_V		, .macro , // {改行}{End}{改行}「」{改行}{↑}
  {.key = B_M|B_COMM|B_B		, .kana = SS_TAP(X_END)"\n"}, // {End}{改行}
  {.key = B_C|B_V|B_Y		, .kana = SS_LSFT(SS_TAP(X_HOME))}, // +{Home}
  {.key = B_C|B_V|B_U		, .kana = SS_LCTL("x")}, // ^x
  {.key = B_C|B_V|B_I		, .kana = SS_LCTL("z")}, // ^z
  {.key = B_C|B_V|B_O		, .kana = SS_LCTL("y")}, // ^y
  {.key = B_C|B_V|B_P		, .kana = SS_LCTL("v")}, // ^v
  {.key = B_C|B_V|B_H		, .kana = SS_LCTL("c")}, // ^c
//{.key = B_C|B_V|B_J		, .macro , // {→ 5}
//{.key = B_C|B_V|B_K		, .macro , // +{→}
//{.key = B_C|B_V|B_L		, .macro , // +{→ 5}
//{.key = B_C|B_V|B_SCLN		, .macro , // +{→ 20}
  {.key = B_C|B_V|B_N		, .kana = SS_LSFT(SS_TAP(X_END))}, // +{End}
//{.key = B_C|B_V|B_M		, .macro , // {← 5}
//{.key = B_C|B_V|B_COMM		, .macro , // +{←}
//{.key = B_C|B_V|B_DOT		, .macro , // +{← 5}
//{.key = B_C|B_V|B_SLSH		, .macro , // +{← 20}
};

const PROGMEM naginata_keymap_long ngmapl_mac[] = {
// 編集モード Mac
  {.key = B_J|B_K|B_Q		, .kana = SS_TAP(KAKT)SS_LCMD(SS_TAP(X_LEFT)SS_TAP(X_DOWN))}, // ^{End}
  {.key = B_J|B_K|B_R		, .kana = SS_LCMD("s")}, // ^s
  {.key = B_D|B_F|B_I		, .kana = SS_TAP(X_LANG1)SS_TAP(X_LANG1)}, // {vk1Csc079}
  {.key = B_D|B_F|B_P		, .kana = "\x1B"SS_TAP(X_NUMLOCK)}, // {Esc 3}
  {.key = B_C|B_V|B_U		, .kana = SS_LCMD("x")}, // ^x
  {.key = B_C|B_V|B_I		, .kana = SS_LCMD("z")}, // ^z
  {.key = B_C|B_V|B_O		, .kana = SS_LCMD(SS_LSFT("z"))}, // ^y
  {.key = B_C|B_V|B_P		, .kana = SS_LCMD("v")}, // ^v
  {.key = B_C|B_V|B_H		, .kana = SS_LCMD("c")}, // ^c

  // 追加
  {.key = B_J|B_K|B_A		, .kana = SS_LALT(";;")SS_TAP(X_ENTER)}, // ……{改行}
  {.key = B_J|B_K|B_B		, .kana = SS_LALT("/")SS_TAP(X_ENTER)}, // ／{改行}
};

const PROGMEM naginata_keymap_unicode ngmapu[] = {
// 編集モード unicode
  {.key = B_J|B_K|B_A		, .win   = "……",		.mac = "nagitete"}, // ……{改行}
  {.key = B_J|B_K|B_B		, .win   = "／",		.mac = "naginame"}, // ／{改行}
  {.key = B_M|B_COMM|B_T		, .win   = "〇",		.mac = "nagimaru"}, // 〇{改行}
};

const PROGMEM naginata_keymap_macro ngmapm[] = {
  {.key = B_J|B_K|B_W}, // 《》{改行}{↑}
  {.key = B_J|B_K|B_S}, // (){改行}{↑}
  {.key = B_J|B_K|B_F}, // 「」{改行}{↑}
  {.key = B_J|B_K|B_G}, // 『』{改行}{↑}
  {.key = B_J|B_K|B_Z}, // ││{改行}
  {.key = B_J|B_K|B_X}, // 【】{改行}{↑}
  {.key = B_D|B_F|B_J}, // {↑}
  {.key = B_D|B_F|B_K}, // +{↑}
  {.key = B_D|B_F|B_L}, // +{↑ 7}
  {.key = B_D|B_F|B_M}, // {↓}
  {.key = B_D|B_F|B_COMM}, // +{↓}
  {.key = B_D|B_F|B_DOT}, // +{↓ 7}
  {.key = B_M|B_COMM|B_W}, // ^x｜{改行}^v《》{改行}{↑}
  {.key = B_M|B_COMM|B_S}, // ^x(^v){改行}{Space}+{↑}^x
  {.key = B_M|B_COMM|B_F}, // ^x「^v」{改行}{Space}+{↑}^x
  {.key = B_M|B_COMM|B_G}, // ^x『^v』{改行}{Space}+{↑}^x
  {.key = B_M|B_COMM|B_Z}, // 　　　×　　　×　　　×{改行 2}
  {.key = B_M|B_COMM|B_X}, // ^x【^v】{改行}{Space}+{↑}^x
  {.key = B_M|B_COMM|B_V}, // {改行}{End}{改行}「」{改行}{↑}
  {.key = B_C|B_V|B_J}, // {→ 5}
  {.key = B_C|B_V|B_K}, // +{→}
  {.key = B_C|B_V|B_L}, // +{→ 5}
  {.key = B_C|B_V|B_SCLN}, // +{→ 20}
  {.key = B_C|B_V|B_M}, // {← 5}
  {.key = B_C|B_V|B_COMM}, // +{←}
  {.key = B_C|B_V|B_DOT}, // +{← 5}
  {.key = B_C|B_V|B_SLSH}, // +{← 20}
};

// 薙刀式のレイヤー、オンオフするキー
void set_naginata(uint8_t layer, uint16_t *onk, uint16_t *offk) {
  naginata_layer = layer;
  ngon_keys[0] = *onk;
  ngon_keys[1] = *(onk+1);
  ngoff_keys[0] = *offk;
  ngoff_keys[1] = *(offk+1);

  naginata_config.raw = eeconfig_read_user();
  if (naginata_config.os != NG_WIN && naginata_config.os != NG_MAC && naginata_config.os != NG_LINUX) {
    naginata_config.os = NG_WIN;
    naginata_config.live_conv = 1;
    naginata_config.tategaki = 1;
    naginata_config.kouchi_shift = 0;
    eeconfig_update_user(naginata_config.raw);
  }
  ng_set_unicode_mode(naginata_config.os);
}

// 薙刀式をオン
void naginata_on(void) {
  is_naginata = true;
  keycomb = 0UL;
  naginata_clear();
  layer_on(naginata_layer);

  if (naginata_config.os != NG_LINUX) {
    tap_code(KC_LANG1); // (Mac)かな
  } else {
    tap_code(KC_HENK);  // 変換
  }
}

// 薙刀式をオフ
void naginata_off(void) {
  is_naginata = false;
  keycomb = 0UL;
  naginata_clear();
  layer_off(naginata_layer);

  if (naginata_config.os != NG_LINUX) {
    tap_code(KC_LANG2); // (Mac)英数
  } else {
    tap_code(KC_MHEN);  // 無変換
  }
}

// 薙刀式のon/off状態を返す
bool naginata_state(void) {
  return is_naginata;
}

// バッファから先頭n文字を削除する
void compress_buffer(uint_fast8_t n) {
  if (ng_chrcount == 0)
    return;
  if (n > ng_chrcount)
    n = ng_chrcount;
  memmove(ninputs, ninputs + n, sizeof(ninputs[0]) * (ng_chrcount - n));
  ng_chrcount -= n;
}

void switchOS(uint8_t os) {
  naginata_config.os = os;
  eeconfig_update_user(naginata_config.raw);
  ng_set_unicode_mode(naginata_config.os);
}

void ng_set_unicode_mode(uint8_t os) {
  switch (os) {
    case NG_WIN:
      set_unicode_input_mode(UC_WINC);
      break;
    case NG_MAC:
      set_unicode_input_mode(UC_MAC);
      break;
    case NG_LINUX:
      set_unicode_input_mode(UC_LNX);
      break;
  }
}

void mac_live_conversion_toggle() {
  naginata_config.live_conv ^= 1;
  eeconfig_update_user(naginata_config.raw);
}

// 出典 https://programming-place.net/ppp/contents/c/rev_res/string014.html
char* replace(char* s, const char* before, const char* after)
{
  const size_t before_len = strlen(before);
  if (before_len == 0) {
    return s;
  }

  const size_t after_len = strlen(after);
  char* p = s;

  for (;;) {
    p = strstr(p, before);
    if (p == NULL) {
      break;
    }
    const char* p2 = p + before_len;
    memmove(p + after_len, p2, strlen(p2) + 1);
    memcpy(p, after, after_len);
    p += after_len;
  }

  return s;
}

void tategaki_toggle() {
  naginata_config.tategaki ^= 1;
  eeconfig_update_user(naginata_config.raw);
}

void ty_send_string(char *s) {
  if (naginata_config.os == NG_MAC) {
    replace(s, SS_TAP(X_HOME), SS_LGUI(SS_TAP(NGUP)));  // 6バイト増える
    replace(s, SS_TAP(X_END), SS_LGUI(SS_TAP(NGDN)));
    replace(s, SS_TAP(KAKT), SS_TAP(X_LANG2)SS_TAP(X_LANG1)); // 3バイト増える
  } else {
    replace(s, SS_TAP(KAKT), "\n");
  }

  if (naginata_config.tategaki) {
    replace(s, SS_TAP(NGUP), SS_TAP(X_UP));
    replace(s, SS_TAP(NGDN), SS_TAP(X_DOWN));
    replace(s, SS_TAP(NGLT), SS_TAP(X_LEFT));
    replace(s, SS_TAP(NGRT), SS_TAP(X_RIGHT));
  } else {
    replace(s, SS_TAP(NGUP), SS_TAP(X_LEFT));
    replace(s, SS_TAP(NGDN), SS_TAP(X_RIGHT));
    replace(s, SS_TAP(NGLT), SS_TAP(X_DOWN));
    replace(s, SS_TAP(NGRT), SS_TAP(X_UP));
  }

  send_string(s);
}

// 方向キー
void cursor_move(bool shift, uint8_t code, uint_fast8_t count) {
  if (shift) {
    register_code(KC_LSFT);
  }
  if (!naginata_config.tategaki) {
    switch (code) {
      case KC_LEFT:   code = KC_DOWN;   break;
      case KC_DOWN:   code = KC_RIGHT;  break;
      case KC_RIGHT:  code = KC_UP;     break;
      case KC_UP:     code = KC_LEFT;   break;
      default:    break;
    }
  }
  for (uint_fast8_t i = 0; i < count; i++) {
    tap_code(code);
  }
  if (shift) {
    unregister_code(KC_LSFT);
  }
}

// （）{改行}{↑}
void send_nagimaka() {
  if (us2jis_state()) {
    send_string(SS_LSFT("89"));
  } else {
    send_string(SS_LSFT("90"));
  }
  tap_code(KC_ENT);
  send_back();
}

// 「」{改行}{↑}
void send_nagikagi() {
  if (us2jis_state()) {
    send_string("]"SS_TAP(X_NUHS));
  } else {
    send_string("[]");
  }
  tap_code(KC_ENT);
  send_back();
}

// 1文字戻る
void send_back() {
  if (naginata_config.tategaki) {
    tap_code(KC_UP);
  } else {
    tap_code(KC_LEFT);
  }
}

void paste_and_clear_clipboard() {
  if (naginata_config.os != NG_MAC) {
    send_string(SS_LCTL("v"));
  } else {
    send_string(SS_LCMD("v"));
  }

  if (naginata_config.tategaki) {
    send_string(SS_TAP(X_DOWN)" "SS_LSFT(SS_TAP(X_UP)));
  } else {
    send_string(SS_TAP(X_RIGHT)" "SS_LSFT(SS_TAP(X_LEFT)));
  }

  if (naginata_config.os != NG_MAC) {
    send_string(SS_LCTL("x"));
  } else {
    send_string(SS_LCMD("x"));
  }
}

void kouchi_shift_toggle() {
  naginata_config.kouchi_shift ^= 1;
  eeconfig_update_user(naginata_config.raw);
}

void ng_show_os(void) {
  switch (naginata_config.os) {
    case NG_WIN:
      send_string("win");
      break;
    case NG_MAC:
      send_string("mac");
      if (naginata_config.live_conv) {
        send_string("/:lc");
      } else {
        send_string("/-lc");
      }
      break;
    case NG_LINUX:
      send_string("linux");
      break;
  }
  if (naginata_config.tategaki) {
    send_string("/tate");
  } else {
    send_string("/yoko");
  }
  if (naginata_config.kouchi_shift) {
    send_string("/:kouchi");
  } else {
    send_string("/-kouchi");
  }
}

void mac_send_string(const char *str) {
  send_string(str);
  if (!naginata_config.live_conv) tap_code(KC_SPC);
  tap_code(KC_ENT);
}

void ng_send_unicode_string(const char *str) {
  switch (naginata_config.os) {
    case NG_LINUX:
      tap_code(KC_MHEN);
      send_unicode_string(str);
      tap_code(KC_HENK);
      break;
    case NG_WIN:
    case NG_MAC:
      send_unicode_string(str);
      tap_code(KC_ENT);
      break;
  }
}

// modifierが押されたら薙刀式レイヤーをオフしてベースレイヤーに戻す
// get_mods()がうまく動かない
static int n_modifier = 0;

bool process_modifier(uint16_t keycode, keyrecord_t *record) {
  // 参考： ./quantum/keycode.h, ./quantum/quantum_keycodes.h
  if (IS_MOD(keycode) || (QK_MOD_TAP <= keycode && keycode <= QK_MOD_TAP_MAX)) {
    if (record->event.pressed) {
      n_modifier++;
      layer_off(naginata_layer);
    } else {
      n_modifier--;
      if (n_modifier <= 0) {
        n_modifier = 0;
        layer_on(naginata_layer);
      }
    }
    return true;
  }
  return false;
}

static uint16_t fghj_buf = 0; // 押しているJかKのキーコード

// 薙刀式の起動処理(COMBOを使わない)
bool enable_naginata(uint16_t keycode, keyrecord_t *record) {
  // キープレス
  if (record->event.pressed) {
    // 1キー目、JKの前に他のキーを押していないこと
    if (fghj_buf == 0) {
      // かなオンキーの場合
      if (keycode == ngon_keys[0] || keycode == ngon_keys[1] || keycode == ngoff_keys[0] || keycode == ngoff_keys[1]) {
        fghj_buf = keycode;
        return false;
      }
    // ２キー目
    } else {
      // ２キー目、１キー目、両方ともかなオンキー
      if ((keycode == ngon_keys[0] && fghj_buf == ngon_keys[1]) ||
          (keycode == ngon_keys[1] && fghj_buf == ngon_keys[0])) {
        naginata_on();
        fghj_buf = 0;
        return false;
      } else if ((keycode == ngoff_keys[0] && fghj_buf == ngoff_keys[1]) ||
          (keycode == ngoff_keys[1] && fghj_buf == ngoff_keys[0])) {
        naginata_off();
        fghj_buf = 0;
        return false;
      // ２キー目はかなオンキーではない
      } else {
        tap_code(fghj_buf); // 1キー目を出力
        fghj_buf = 0;
        return true; // 2キー目はQMKにまかせる
      }
    }
  // J/K単押しだった
  } else if (fghj_buf != 0) {
    tap_code(fghj_buf);
    fghj_buf = 0;

    // Shift + Jで、先にShiftを外した場合にShiftがリリースされない不具合対策
    // 参考： qmk_firmware/quantum/keycode.h, /quantum/quantum_keycodes.h,
    //       qmk_firmware/quantum/action_code.h
    if (IS_MOD(keycode)) {
      unregister_code(keycode);
    } else if (QK_MOD_TAP <= keycode && keycode <= QK_MOD_TAP_MAX) {
      if (keycode & (MOD_LCTL << 8))
        unregister_code(KC_LCTRL);
      if (keycode & (MOD_LSFT << 8))
        unregister_code(KC_LSHIFT);
      if (keycode & (MOD_LALT << 8))
        unregister_code(KC_LALT);
      if (keycode & (MOD_LGUI << 8))
        unregister_code(KC_LGUI);
      if (keycode & (MOD_RCTL << 8))
        unregister_code(KC_RCTRL);
      if (keycode & (MOD_RSFT << 8))
        unregister_code(KC_RSHIFT);
      if (keycode & (MOD_RALT << 8))
        unregister_code(KC_RALT);
      if (keycode & (MOD_RGUI << 8))
        unregister_code(KC_RGUI);
    }
    return false;
  }
  return true;
}

// バッファをクリアする
void naginata_clear(void) {
  for (uint_fast8_t i = 0; i < NGBUFFER; i++) {
    ninputs[i] = 0;
  }
  ng_chrcount = 0;
  n_modifier = 0;
  fghj_buf = 0;
}

// 薙刀式の入力処理
bool process_naginata(uint16_t keycode, keyrecord_t *record) {
  // OS切り替え(UNICODE出力)
  if (record->event.pressed) {
    switch (keycode) {
      case NG_ON:
        naginata_on();
        return false;
      case NG_OFF:
        naginata_off();
        return false;
      case NG_CLR:
        naginata_clear();
        return false;
      case NGSW_WIN:
        switchOS(NG_WIN);
        return false;
      case NGSW_MAC:
        switchOS(NG_MAC);
        return false;
      case NGSW_LNX:
        switchOS(NG_LINUX);
        return false;
      case NG_MLV:
        mac_live_conversion_toggle();
        return false;
      case NG_SHOS:
        ng_show_os();
        return false;
      case NG_TAYO:
        tategaki_toggle();
        return false;
      case NG_KOTI:
        kouchi_shift_toggle();
        return false;
    }
  }

  if (!is_naginata)
    // return true;
    return enable_naginata(keycode, record);

  if (process_modifier(keycode, record))
    return true;

  if (record->event.pressed) {
    switch (keycode) {
      case NG_SHFT ... NG_SHFT2:
        if (!naginata_config.kouchi_shift) {
          if (ng_chrcount >= 1) {
            naginata_type();
            keycomb = 0UL;
          }
        }
        //fallthrouth
      case NG_Q ... NG_SLSH:  // 後置シフトの NG_SHFT, NG_SHFT2 を含む
        ninputs[ng_chrcount] = keycode; // キー入力をバッファに貯める
        ng_chrcount++;
        keycomb |= ng_key[keycode - NG_Q]; // キーの重ね合わせ
        // 変換候補が絞られるか、バッファが一杯になったら処理を開始
        int_fast8_t nc = number_of_candidates();
        if (nc == 0 || nc == 1 || ng_chrcount >= NGBUFFER) {
          naginata_type();
        }
        return false;
    }
  } else { // key release
    switch (keycode) {
      case NG_Q ... NG_SHFT2:
        // どれかキーを離したら処理を開始する
        if (ng_chrcount > 0) {
          naginata_type();
        }
        keycomb &= ~ng_key[keycode - NG_Q]; // キーの重ね合わせ
        return false;
    }
  }
  return true;
}

// キー入力を文字に変換して出力する
void naginata_type(void) {
  // バッファの最初からnt文字目までを検索キーにする。
  // 一致する組み合わせがなければntを減らして=最後の1文字を除いて再度検索する。
  uint_fast8_t nt = ng_chrcount;

  while (nt > 0) {
    // 連続シフト有効で探し、なかったら連続シフト無効でも探す
    if (naginata_lookup(nt, true) || naginata_lookup(nt, false)) {
      compress_buffer(nt);
      return;
    }
    nt--; // 最後の1キーを除いて、もう一度仮名テーブルを検索する
  }
  compress_buffer(1);
}

// 組み合わせが複数ある > 1: 変換しない
// 組み合わせが一つしかない = 1: 変換を開始する
// 組み合わせが一つしかない、ただしキーを全て押していない =-1: 変換しない
// 組み合わせがない = 0: 変換を開始する
int_fast8_t number_of_candidates() {
  uint32_t key; // PROGMEM buffer
  int_fast8_t c = 0;
  uint32_t hit = 0;

  // keycomb_bufはバッファ内のキーの組み合わせ、keycombはリリースしたキーを含んでいない
  uint32_t keycomb_buf = 0UL;

  // バッファ内のキーを組み合わせる
  for (uint_fast8_t i = 0; i < ng_chrcount; i++) {
    keycomb_buf |= ng_key[ninputs[i] - NG_Q];
  }

  // 通常の仮名
  for (int i = 0; i < sizeof ngmap / sizeof ngmap[0]; i++) {
    memcpy_P(&key, &ngmap[i].key, sizeof(key));
    if ((keycomb_buf & key) == keycomb_buf) {
      if (++c >= 2)
        return c;
      hit = key;
    }
  }
  // 編集モード 共通
  for (int i = 0; i < sizeof ngmapl / sizeof ngmapl[0]; i++) {
    memcpy_P(&key, &ngmapl[i].key, sizeof(key));
    if ((keycomb_buf & key) == keycomb_buf) {
      if (++c >= 2)
        return c;
      hit = key;
    }
  }
  // 編集モード マクロ
  for (int i = 0; i < sizeof ngmapm / sizeof ngmapm[0]; i++) {
    memcpy_P(&key, &ngmapm[i].key, sizeof(key));
    if ((keycomb_buf & key) == keycomb_buf) {
      if (++c >= 2)
        return c;
      hit = key;
    }
  // 編集モード UNICODE文字
  for (int i = 0; i < sizeof ngmapu / sizeof ngmapu[0]; i++) {
    memcpy_P(&key, &ngmapu[i].key, sizeof(key));
    if ((keycomb_buf & key) == keycomb_buf) {
      if (++c >= 2)
        return c;
      hit = key;
    }
  }
  }

  if (c == 1 && ng_chrcount < bitpop32(hit)) {
    return -1;
  }
  return c;
}

// バッファの頭からnt文字の範囲を検索キーにしてテーブル検索し、文字に変換して出力する
// 検索に成功したらtrue、失敗したらfalseを返す
bool naginata_lookup(uint_fast8_t nt, bool shifted) {
  uint32_t key; // PROGMEM buffer
  char kana[sizeof(ngmapl[0].kana) + 6];  // PROGMEM buffer

  // keycomb_bufはバッファ内のキーの組み合わせ、keycombはリリースしたキーを含んでいない
  uint32_t keycomb_buf = 0UL;

  // バッファ内のキーを組み合わせる
  for (uint_fast8_t i = 0; i < nt; i++) {
    keycomb_buf |= ng_key[ninputs[i] - NG_Q];
  }

  // NG_SHFT2はスペースの代わりにエンターを入力する
  if (keycomb_buf == B_SHFT && ninputs[0] == NG_SHFT2) {
    tap_code(KC_ENT);
    return true;
  }

  // 連続シフトを有効にする
  if ((keycomb & B_SHFT) == B_SHFT) keycomb_buf |= B_SHFT;

  if (shifted) {
    // 編集モードを連続する
    if ((keycomb & (B_D | B_F))    == (B_D | B_F))    keycomb_buf |= (B_D | B_F);
    if ((keycomb & (B_C | B_V))    == (B_C | B_V))    keycomb_buf |= (B_C | B_V);
    if ((keycomb & (B_J | B_K))    == (B_J | B_K))    keycomb_buf |= (B_J | B_K);
    if ((keycomb & (B_M | B_COMM)) == (B_M | B_COMM)) keycomb_buf |= (B_M | B_COMM);

    // 濁音、半濁音を連続する
    if ((keycomb & B_F) == B_F) keycomb_buf |= B_F;
    if ((keycomb & B_J) == B_J) keycomb_buf |= B_J;
    if ((keycomb & B_V) == B_V) keycomb_buf |= B_V;
    if ((keycomb & B_M) == B_M) keycomb_buf |= B_M;
  }

  switch (keycomb_buf) {
    // send_stringできないキー、長すぎるマクロはここで定義
    case B_H|B_J:
      naginata_on();
      break;
    case B_F|B_G:
      naginata_off();
      break;
    case B_T: // {←}
      cursor_move(false, KC_LEFT, 1);
      break;
    case B_Y: // {→}
      cursor_move(false, KC_RIGHT, 1);
      break;
/*  case B_SHFT|B_T:  // +{←}
      cursor_move(true, KC_LEFT, 1);
      break;
    case B_SHFT|B_Y:  // +{→}
      cursor_move(true, KC_RIGHT, 1);
      break;
*/

// 編集モード macro
    case B_J|B_K|B_W: // 《》{改行}{↑}
      if (naginata_config.os != NG_MAC) {
        ng_send_unicode_string("《》");
      } else {
        mac_send_string("naginiya");
      }
      send_back();
      break;
    case B_J|B_K|B_S: // (){改行}{↑}
      send_nagimaka();
      break;
    case B_J|B_K|B_F: // 「」{改行}{↑}
      send_nagikagi();
      break;
    case B_J|B_K|B_G: // 『』{改行}{↑}
      if (naginata_config.os != NG_MAC) {
        ng_send_unicode_string("『』");
      } else {
        mac_send_string("naginika");
      }
      send_back();
      break;
    case B_J|B_K|B_Z: // ││{改行}
      if (naginata_config.os != NG_MAC) {
        if (naginata_config.tategaki) {
          ng_send_unicode_string("││");
        } else {
          ng_send_unicode_string("──");
        }
      } else if (naginata_config.tategaki) {
        mac_send_string("nagitase");
      } else {
        mac_send_string("nagiyose");
      }
      break;
    case B_J|B_K|B_X: // 【】{改行}{↑}
      if (naginata_config.os != NG_MAC) {
        ng_send_unicode_string("【】");
      } else {
        mac_send_string("nagisuka");
      }
      send_back();
      break;
    case B_D|B_F|B_J: // {↑}
      cursor_move(false, KC_UP, 1);
      break;
    case B_D|B_F|B_K: // +{↑}
      cursor_move(true, KC_UP, 1);
      break;
    case B_D|B_F|B_L: // +{↑ 7}
      cursor_move(true, KC_UP, 7);
      break;
    case B_D|B_F|B_M: // {↓}
      cursor_move(false, KC_DOWN, 1);
      break;
    case B_D|B_F|B_COMM: // +{↓}
      cursor_move(true, KC_DOWN, 1);
      break;
    case B_D|B_F|B_DOT: // +{↓ 7}
      cursor_move(true, KC_DOWN, 7);
      break;
    case B_M|B_COMM|B_W: // ^x｜{改行}^v《》{改行}{↑}
      if (naginata_config.os != NG_MAC) {
        send_string(SS_LCTL("x"));
        ng_send_unicode_string("｜");
        send_string(SS_LCTL("v"));
        ng_send_unicode_string("《》");
      } else {
        send_string(SS_LCMD("x"));
        mac_send_string("nagitabo");
        send_string(SS_LCMD("v"));
        mac_send_string("naginiya");
      }
      send_back();
      break;
    case B_M|B_COMM|B_S: // ^x(^v){改行}{Space}+{↑}^x
      if (naginata_config.os != NG_MAC) {
        send_string(SS_LCTL("x"));
      } else {
        send_string(SS_LCMD("x"));
      }
      send_nagimaka();
      paste_and_clear_clipboard();
      break;
    case B_M|B_COMM|B_F: // ^x「^v」{改行}{Space}+{↑}^x
      if (naginata_config.os != NG_MAC) {
        send_string(SS_LCTL("x"));
      } else {
        send_string(SS_LCMD("x"));
      }
      send_nagikagi();
      paste_and_clear_clipboard();
      break;
    case B_M|B_COMM|B_G: // ^x『^v』{改行}{Space}+{↑}^x
      if (naginata_config.os != NG_MAC) {
        send_string(SS_LCTL("x"));
        ng_send_unicode_string("『』");
      } else {
        send_string(SS_LCMD("x"));
        mac_send_string("naginika");
      }
      send_back();
      paste_and_clear_clipboard();
      break;
    case B_M|B_COMM|B_Z: //　　　×　　　×　　　×{改行 2}
      if (naginata_config.os != NG_MAC) {
        ng_send_unicode_string("　　　×　　　×　　　×");
      } else {
        tap_code(KC_SPACE);
        tap_code(KC_SPACE);
        tap_code(KC_SPACE);
        mac_send_string("nagibatu");
      }
      tap_code(KC_ENT);
      break;
    case B_M|B_COMM|B_X: // ^x【^v】{改行}{Space}+{↑}^x
      if (naginata_config.os != NG_MAC) {
        send_string(SS_LCTL("x"));
        ng_send_unicode_string("【】");
      } else {
        send_string(SS_LCMD("x"));
        mac_send_string("nagisuka");
      }
      send_back();
      paste_and_clear_clipboard();
      break;
    case B_M|B_COMM|B_V: // {改行}{End}{改行}「」{改行}{↑}
      if (naginata_config.os != NG_MAC) {
        tap_code(KC_ENT);
        tap_code(KC_END);
        tap_code(KC_ENT);
      } else {
        tap_code(KC_LANG2);
        tap_code(KC_LANG1);
        if (naginata_config.tategaki) {
          send_string(SS_LGUI(SS_TAP(X_DOWN)));
        } else {
          send_string(SS_LGUI(SS_TAP(X_RIGHT)));
        }
        tap_code(KC_ENT);
      }
      send_nagikagi();
      break;
    case B_C|B_V|B_J: // {→ 5}
      cursor_move(false, KC_RIGHT, 5);
      break;
    case B_SHFT|B_Y:  // +{→}
    case B_C|B_V|B_K: // +{→}
      cursor_move(true, KC_RIGHT, 1);
      break;
    case B_C|B_V|B_L: // +{→ 5}
      cursor_move(true, KC_RIGHT, 5);
      break;
    case B_C|B_V|B_SCLN: // +{→ 20}
      cursor_move(true, KC_RIGHT, 20);
      break;
    case B_C|B_V|B_M: // {← 5}
      cursor_move(false, KC_LEFT, 5);
      break;
    case B_SHFT|B_T:  // +{←}
    case B_C|B_V|B_COMM: // +{←}
      cursor_move(true, KC_LEFT, 1);
      break;
    case B_C|B_V|B_DOT: // +{← 5}
      cursor_move(true, KC_LEFT, 5);
      break;
    case B_C|B_V|B_SLSH: // +{← 20}
      cursor_move(true, KC_LEFT, 20);
      break;

    default:
      // キーから仮名に変換して出力する

      // 通常の仮名
      for (int i = 0; i < sizeof ngmap / sizeof ngmap[0]; i++) {
        memcpy_P(&key, &ngmap[i].key, sizeof(key));
        if (keycomb_buf == key) {
          memcpy_P(&kana, &ngmap[i].kana, sizeof(ngmap[0].kana));
          send_string(kana);
          return true;
        }
      }
      // 編集モード Mac
      if (naginata_config.os == NG_MAC) {
        for (int i = 0; i < sizeof ngmapl_mac / sizeof ngmapl_mac[0]; i++) {
          memcpy_P(&key, &ngmapl_mac[i].key, sizeof(key));
          if (keycomb_buf == key) {
            memcpy_P(&kana, &ngmapl_mac[i].kana, sizeof(ngmapl_mac[0].kana));
            ty_send_string(kana);
            return true;
          }
        }
      }
      // 編集モード 共通
      for (int i = 0; i < sizeof ngmapl / sizeof ngmapl[0]; i++) {
        memcpy_P(&key, &ngmapl[i].key, sizeof(key));
        if (keycomb_buf == key) {
          memcpy_P(&kana, &ngmapl[i].kana, sizeof(ngmapl[0].kana));
          ty_send_string(kana);
          return true;
        }
      }
      // 編集モード UNICODE文字
      for (int i = 0; i < sizeof ngmapu / sizeof ngmapu[0]; i++) {
        memcpy_P(&key, &ngmapu[i].key, sizeof(key));
        if (keycomb_buf == key) {
          if (naginata_config.os != NG_MAC) {
            memcpy_P(&kana, &ngmapu[i].win, sizeof(ngmapu[0].win));
            ng_send_unicode_string(kana);
          } else {
            memcpy_P(&kana, &ngmapu[i].mac, sizeof(ngmapu[0].mac));
            mac_send_string(kana);
          }
          return true;
        }
      }
      return false;
  }
  return true;
}
