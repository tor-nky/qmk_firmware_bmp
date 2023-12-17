/* Copyright eswai <@eswai> / Satoru NAKAYA <@tor-nky>
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
#include "naginata.h"
#include "naginata_parts.h"
#include "unicode_P.h"

#include <string.h>

static bool is_naginata = false; // 薙刀式がオンかオフか
static uint8_t naginata_layer = 0; // NG_*を配置しているレイヤー番号
static uint16_t ngon_keys[2]; // 薙刀式をオンにするキー(通常HJ)
static uint16_t ngoff_keys[2]; // 薙刀式をオフにするキー(通常FG)
static uint32_t pushed_key = 0; // 同時押しの状態を示す。各ビットがキーに対応する。

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

#define NKEYS 3 // 組み合わせにある同時押しするキーの数、薙刀式なら3
                // (最大何キーまでバッファに貯めるか)

// カナ変換テーブル
typedef struct {
  uint32_t key;
  void (*func)(void);
} naginata_keymap;

// かな定義の要素数
#define NGMAP_COUNT (sizeof ngmap / sizeof ngmap[0])

// かな定義
// 3キー同時 → 2キー同時 → センターシフト → 単打 の順
// シフト残り判定は、3キー同時内と2キー同時内の順序で決まる
// 同じ key の定義が複数ある時の動作は不明
const PROGMEM naginata_keymap ngmap[] = {
  // ********** 3キー同時 **********
  // 拗音、外来音
  {.key = B_J|B_W|B_H   , .func = ng_send_gya   },  // ぎゃ
  {.key = B_J|B_R|B_H   , .func = ng_send_ja    },  // じゃ
  {.key = B_J|B_G|B_H   , .func = ng_send_dya   },  // ぢゃ
  {.key = B_J|B_X|B_H   , .func = ng_send_bya   },  // びゃ
  {.key = B_J|B_W|B_P   , .func = ng_send_gyu   },  // ぎゅ
  {.key = B_J|B_R|B_P   , .func = ng_send_ju    },  // じゅ
  {.key = B_J|B_G|B_P   , .func = ng_send_dyu   },  // ぢゅ
  {.key = B_J|B_X|B_P   , .func = ng_send_byu   },  // びゅ
  {.key = B_J|B_W|B_I   , .func = ng_send_gyo   },  // ぎょ
  {.key = B_J|B_R|B_I   , .func = ng_send_jo    },  // じょ
  {.key = B_J|B_G|B_I   , .func = ng_send_dyo   },  // ぢょ
  {.key = B_J|B_X|B_I   , .func = ng_send_byo   },  // びょ
  {.key = B_M|B_X|B_I   , .func = ng_send_pyo   },  // ぴょ
  {.key = B_M|B_X|B_P   , .func = ng_send_pyu   },  // ぴゅ
  {.key = B_M|B_X|B_H   , .func = ng_send_pya   },  // ぴゃ
  {.key = B_M|B_E|B_P   , .func = ng_send_thu   },  // てゅ
  {.key = B_M|B_E|B_K   , .func = ng_send_thi   },  // てぃ
  {.key = B_J|B_E|B_P   , .func = ng_send_dhu   },  // でゅ
  {.key = B_J|B_E|B_K   , .func = ng_send_dhi   },  // でぃ
  {.key = B_M|B_D|B_L   , .func = ng_send_twu   },  // とぅ
  {.key = B_J|B_D|B_L   , .func = ng_send_dwu   },  // どぅ
  {.key = B_M|B_R|B_O   , .func = ng_send_sye   },  // しぇ
  {.key = B_M|B_G|B_O   , .func = ng_send_tye   },  // ちぇ
  {.key = B_J|B_R|B_O   , .func = ng_send_je    },  // じぇ
  {.key = B_J|B_G|B_O   , .func = ng_send_dye   },  // ぢぇ
  {.key = B_V|B_SCLN|B_O, .func = ng_send_fe    },  // ふぇ
  {.key = B_V|B_SCLN|B_P, .func = ng_send_fyu   },  // ふゅ
  {.key = B_V|B_SCLN|B_J, .func = ng_send_fa    },  // ふぁ
  {.key = B_V|B_SCLN|B_K, .func = ng_send_fi    },  // ふぃ
  {.key = B_V|B_SCLN|B_N, .func = ng_send_fo    },  // ふぉ
  {.key = B_M|B_Q|B_O   , .func = ng_send_ve    },  // ヴぇ
  {.key = B_M|B_Q|B_P   , .func = ng_send_vyu   },  // ヴゅ
  {.key = B_M|B_Q|B_J   , .func = ng_send_va    },  // ヴぁ
  {.key = B_M|B_Q|B_K   , .func = ng_send_vi    },  // ヴぃ
  {.key = B_M|B_Q|B_N   , .func = ng_send_vo    },  // ヴぉ
  {.key = B_V|B_L|B_O   , .func = ng_send_we    },  // うぇ
  {.key = B_V|B_L|B_K   , .func = ng_send_wi    },  // うぃ
  {.key = B_V|B_L|B_N   , .func = ng_send_who   },  // うぉ
  {.key = B_V|B_K|B_O   , .func = ng_send_ye    },  // いぇ
  {.key = B_V|B_L|B_J   , .func = ng_send_tsa   },  // つぁ
  {.key = B_V|B_H|B_O   , .func = ng_send_qe    },  // くぇ
  {.key = B_V|B_H|B_J   , .func = ng_send_qa    },  // くぁ
  {.key = B_V|B_H|B_K   , .func = ng_send_qi    },  // くぃ
  {.key = B_V|B_H|B_N   , .func = ng_send_qo    },  // くぉ
  {.key = B_V|B_H|B_DOT , .func = ng_send_kuxwa },  // くゎ
  {.key = B_F|B_H|B_O   , .func = ng_send_gwe   },  // ぐぇ
  {.key = B_F|B_H|B_J   , .func = ng_send_gwa   },  // ぐぁ
  {.key = B_F|B_H|B_K   , .func = ng_send_gwi   },  // ぐぃ
  {.key = B_F|B_H|B_N   , .func = ng_send_gwo   },  // ぐぉ
  {.key = B_F|B_H|B_DOT , .func = ng_send_guxwa },  // ぐゎ
  // 非標準の変換
//  {.key = B_X|B_C|B_M    , .func = ng_send_pyu   },  // ピュ

  // 編集モード
  {.key = B_D|B_F|B_Y   , .func = ng_home}, // {Home}
  {.key = B_D|B_F|B_U   , .func = ng_edit_delete_to_end}, // +{End}{BS}
  {.key = B_D|B_F|B_I   , .func = ng_saihenkan}, // {vk1Csc079}
  {.key = B_D|B_F|B_O   , .func = ng_delete}, // {Del}
  {.key = B_D|B_F|B_P   , .func = ng_ime_cancel}, // {Esc 3}
  {.key = B_D|B_F|B_H   , .func = ng_edit_kakutei_end}, // {Enter}{End}
  {.key = B_D|B_F|B_J   , .func = ng_edit_1_up}, // {↑}
  {.key = B_D|B_F|B_K   , .func = ng_edit_s1_up}, // +{↑}
  {.key = B_D|B_F|B_L   , .func = ng_edit_s7_up}, // +{↑ 7}
  {.key = B_D|B_F|B_SCLN, .func = ng_katakana}, // ^i
  {.key = B_D|B_F|B_N   , .func = ng_end}, // {End}
  {.key = B_D|B_F|B_M   , .func = ng_edit_1_down}, // {↓}
  {.key = B_D|B_F|B_COMM, .func = ng_edit_s1_down}, // +{↓}
  {.key = B_D|B_F|B_DOT , .func = ng_edit_s7_down}, // +{↓ 7}
  {.key = B_D|B_F|B_SLSH, .func = ng_hiragana}, // ^u
  {.key = B_J|B_K|B_Q   , .func = ng_eof}, // ^{End}
  {.key = B_J|B_K|B_W   , .func = ng_edit_nijuu_yama_gakko}, // 『』{改行}{↑}
  {.key = B_J|B_K|B_R   , .func = ng_save}, // ^s
  {.key = B_J|B_K|B_T   , .func = ng_symbol_chuuten}, // ・
  {.key = B_J|B_K|B_A   , .func = ng_edit_tenten}, // ……{改行}
  {.key = B_J|B_K|B_S   , .func = ng_edit_maru_kakko}, // (){改行}{↑}
  {.key = B_J|B_K|B_D   , .func = ng_symbol_question}, // ？{改行}
  {.key = B_J|B_K|B_F   , .func = ng_edit_kagi_kakko}, // 「」{改行}{↑}
  {.key = B_J|B_K|B_G   , .func = ng_edit_nijuu_yama_kakko}, // 《》{改行}{↑}
  {.key = B_J|B_K|B_Z   , .func = ng_symbol_yokobou}, // ――{改行}
  {.key = B_J|B_K|B_X   , .func = ng_edit_sumituki_kakko}, // 【】{改行}{↑}
  {.key = B_J|B_K|B_C   , .func = ng_symbol_exclaim}, // ！{改行}
  {.key = B_J|B_K|B_V   , .func = ng_edit_kakutei_down}, // {改行}{↓}
  {.key = B_J|B_K|B_B   , .func = ng_edit_kakutei_left}, // {改行}{←}
  {.key = B_C|B_V|B_Y   , .func = ng_edit_s_home}, // +{Home}
  {.key = B_C|B_V|B_U   , .func = ng_cut}, // ^x
  {.key = B_C|B_V|B_I   , .func = ng_paste}, // ^v
  {.key = B_C|B_V|B_O   , .func = ng_redo}, // ^y
  {.key = B_C|B_V|B_P   , .func = ng_undo}, // ^z
  {.key = B_C|B_V|B_H   , .func = ng_copy}, // ^c
  {.key = B_C|B_V|B_J   , .func = ng_edit_1_right}, // {→}
  {.key = B_C|B_V|B_K   , .func = ng_edit_s1_right}, // +{→}
  {.key = B_C|B_V|B_L   , .func = ng_edit_s5_right}, // +{→ 5}
  {.key = B_C|B_V|B_SCLN, .func = ng_edit_s20_right}, // +{→ 20}
  {.key = B_C|B_V|B_N   , .func = ng_edit_s_end}, // +{End}
  {.key = B_C|B_V|B_M   , .func = ng_edit_1_left}, // {←}
  {.key = B_C|B_V|B_COMM, .func = ng_edit_s1_left}, // +{←}
  {.key = B_C|B_V|B_DOT , .func = ng_edit_s5_left}, // +{← 5}
  {.key = B_C|B_V|B_SLSH, .func = ng_edit_s20_left}, // +{← 20}
  {.key = B_M|B_COMM|B_Q, .func = ng_edit_togaki_zengyo}, // {Home}{→}{End}{Del 4}{←}
  {.key = B_M|B_COMM|B_W, .func = ng_edit_surround_nijuu_yama_gakko}, // ^x『^v』{改行}{Space}+{↑}^x
  {.key = B_M|B_COMM|B_E, .func = ng_edit_togaki}, // {Home}{改行}{Space 3}{←}
  {.key = B_M|B_COMM|B_R, .func = ng_edit_3_space}, // {Space 3}
  {.key = B_M|B_COMM|B_T, .func = ng_symbol_maru}, // 〇{改行}
  {.key = B_M|B_COMM|B_A, .func = ng_edit_serifu_zengyo}, // {Home}{→}{End}{Del 2}{←}
  {.key = B_M|B_COMM|B_S, .func = ng_edit_surround_maru_kakko}, // ^x(^v){改行}{Space}+{↑}^x
  {.key = B_M|B_COMM|B_D, .func = ng_edit_serifu}, // {Home}{改行}{Space 1}{←}
  {.key = B_M|B_COMM|B_F, .func = ng_edit_surround_kagi_kakko}, // ^x「^v」{改行}{Space}+{↑}^x
  {.key = B_M|B_COMM|B_G, .func = ng_edit_surround_ruby}, // ^x｜{改行}^v《》{改行}{↑}{Space}+{↑}^x
  {.key = B_M|B_COMM|B_Z, .func = ng_edit_separate_line}, // 　　　×　　　×　　　×{改行 2}
  {.key = B_M|B_COMM|B_X, .func = ng_edit_surround_sumituki_kakko}, // ^x【^v】{改行}{Space}+{↑}^x
  {.key = B_M|B_COMM|B_C, .func = ng_symbol_slash}, // ／{改行}
  {.key = B_M|B_COMM|B_V, .func = ng_edit_next_line_kagi_kakko}, // {改行}{End}{改行}「」{改行}{↑}
  {.key = B_M|B_COMM|B_B, .func = ng_edit_next_line_space}, // {改行}{End}{改行}{Space}

  // ********** 2キー同時 **********
  // 濁音
  {.key = B_F|B_U       , .func = ng_send_za    },  // ざ
  {.key = B_F|B_O       , .func = ng_send_zu    },  // ず
  {.key = B_F|B_P       , .func = ng_send_be    },  // べ
  {.key = B_F|B_H       , .func = ng_send_gu    },  // ぐ
  {.key = B_F|B_L       , .func = ng_send_du    },  // づ
  {.key = B_F|B_SCLN    , .func = ng_send_bu    },  // ぶ
  {.key = B_F|B_N       , .func = ng_send_da    },  // だ
  {.key = B_J|B_W       , .func = ng_send_gi    },  // ぎ
  {.key = B_J|B_E       , .func = ng_send_de    },  // で
  {.key = B_J|B_R       , .func = ng_send_zi    },  // じ
  {.key = B_J|B_A       , .func = ng_send_ze    },  // ぜ
  {.key = B_J|B_S       , .func = ng_send_ge    },  // げ
  {.key = B_J|B_D       , .func = ng_send_do    },  // ど
  {.key = B_J|B_F       , .func = ng_send_ga    },  // が
  {.key = B_J|B_G       , .func = ng_send_di    },  // ぢ
  {.key = B_J|B_Z       , .func = ng_send_bo    },  // ぼ
  {.key = B_J|B_X       , .func = ng_send_bi    },  // び
  {.key = B_J|B_C       , .func = ng_send_ba    },  // ば
  {.key = B_J|B_V       , .func = ng_send_go    },  // ご
  {.key = B_J|B_B       , .func = ng_send_zo    },  // ぞ

  // 半濁音
  {.key = B_V|B_P       , .func = ng_send_pe    },  // ぺ
  {.key = B_V|B_SCLN    , .func = ng_send_pu    },  // ぷ
  {.key = B_M|B_Z       , .func = ng_send_po    },  // ぽ
  {.key = B_M|B_X       , .func = ng_send_pi    },  // ぴ
  {.key = B_M|B_C       , .func = ng_send_pa    },  // ぱ

  // 小書き
  {.key = B_Q|B_I       , .func = ng_send_xyo   },  // ょ
  {.key = B_Q|B_O       , .func = ng_send_xe    },  // ぇ
  {.key = B_Q|B_P       , .func = ng_send_xyu   },  // ゅ
  {.key = B_Q|B_H       , .func = ng_send_xya   },  // ゃ
  {.key = B_Q|B_J       , .func = ng_send_xa    },  // ぁ
  {.key = B_Q|B_K       , .func = ng_send_xi    },  // ぃ
  {.key = B_Q|B_L       , .func = ng_send_xu    },  // ぅ
  {.key = B_Q|B_N       , .func = ng_send_xo    },  // ぉ
  {.key = B_Q|B_DOT     , .func = ng_send_xwa   },  // ゎ
  // {.key = B_Q|B_S       , .func = ng_send_xke   }, // ヶ
  // {.key = B_Q|B_F       , .func = ng_send_xka   }, // ヵ

  // 拗音、外来音
  {.key = B_W|B_H       , .func = ng_send_kya   },  // きゃ
  {.key = B_E|B_H       , .func = ng_send_rya   },  // りゃ
  {.key = B_R|B_H       , .func = ng_send_sya   },  // しゃ
  {.key = B_D|B_H       , .func = ng_send_nya   },  // にゃ
  {.key = B_G|B_H       , .func = ng_send_tya   },  // ちゃ
  {.key = B_X|B_H       , .func = ng_send_hya   },  // ひゃ
  {.key = B_B|B_H       , .func = ng_send_mya   },  // みゃ
  {.key = B_W|B_P       , .func = ng_send_kyu   },  // きゅ
  {.key = B_E|B_P       , .func = ng_send_ryu   },  // りゅ
  {.key = B_R|B_P       , .func = ng_send_syu   },  // しゅ
  {.key = B_D|B_P       , .func = ng_send_nyu   },  // にゅ
  {.key = B_G|B_P       , .func = ng_send_tyu   },  // ちゅ
  {.key = B_X|B_P       , .func = ng_send_hyu   },  // ひゅ
  {.key = B_B|B_P       , .func = ng_send_myu   },  // みゅ
  {.key = B_W|B_I       , .func = ng_send_kyo   },  // きょ
  {.key = B_E|B_I       , .func = ng_send_ryo   },  // りょ
  {.key = B_R|B_I       , .func = ng_send_syo   },  // しょ
  {.key = B_D|B_I       , .func = ng_send_nyo   },  // にょ
  {.key = B_G|B_I       , .func = ng_send_tyo   },  // ちょ
  {.key = B_X|B_I       , .func = ng_send_hyo   },  // ひょ
  {.key = B_B|B_I       , .func = ng_send_myo   },  // みょ

  // 機能部分
  {.key = B_H|B_J       , .func = naginata_on},   //　かなオン
  {.key = B_F|B_G       , .func = naginata_off},  //　かなオフ
  {.key = B_V|B_M       , .func = ng_enter},

  // センターシフト
  {.key = B_SHFT|B_Q    , .func = ng_send_vu    },  // ヴ
  {.key = B_SHFT|B_W    , .func = ng_send_nu    },  // ぬ
  {.key = B_SHFT|B_E    , .func = ng_send_ri    },  // り
  {.key = B_SHFT|B_R    , .func = ng_send_ne    },  // ね
  {.key = B_SHFT|B_T    , .func = ng_edit_s1_left}, // +{←}
  {.key = B_SHFT|B_Y    , .func = ng_edit_s1_right}, // +{→}
  {.key = B_SHFT|B_U    , .func = ng_send_sa    },  // さ
  {.key = B_SHFT|B_I    , .func = ng_send_yo    },  // よ
  {.key = B_SHFT|B_O    , .func = ng_send_e     },  // え
  {.key = B_SHFT|B_P    , .func = ng_send_yu    },  // ゆ
  {.key = B_SHFT|B_A    , .func = ng_send_se    },  // せ
  {.key = B_SHFT|B_S    , .func = ng_send_me    },  // め
  {.key = B_SHFT|B_D    , .func = ng_send_ni    },  // に
  {.key = B_SHFT|B_F    , .func = ng_send_ma    },  // ま
  {.key = B_SHFT|B_G    , .func = ng_send_ti    },  // ち
  {.key = B_SHFT|B_H    , .func = ng_send_ya    },  // や
  {.key = B_SHFT|B_J    , .func = ng_send_no    },  // の
  {.key = B_SHFT|B_K    , .func = ng_send_mo    },  // も
  {.key = B_SHFT|B_L    , .func = ng_send_tu    },  // つ
  {.key = B_SHFT|B_SCLN , .func = ng_send_hu    },  // ふ
  {.key = B_SHFT|B_Z    , .func = ng_send_ho    },  // ほ
  {.key = B_SHFT|B_X    , .func = ng_send_hi    },  // ひ
  {.key = B_SHFT|B_C    , .func = ng_send_wo    },  // を
  {.key = B_V|B_SHFT    , .func = ng_edit_touten},  // 、
  {.key = B_SHFT|B_B    , .func = ng_send_mi    },  // み
  {.key = B_SHFT|B_N    , .func = ng_send_o     },  // お
  {.key = B_M|B_SHFT    , .func = ng_edit_kuten },  // 。
  {.key = B_SHFT|B_COMM , .func = ng_send_mu    },  // む
  {.key = B_SHFT|B_DOT  , .func = ng_send_wa    },  // わ
  {.key = B_SHFT|B_SLSH , .func = ng_send_re    },  // れ

  // ********** 単打 **********
  {.key = B_SHFT        , .func = ng_space_or_enter},
  {.key = B_Q           , .func = ng_send_vu    },  // ヴ
  {.key = B_W           , .func = ng_send_ki    },  // き
  {.key = B_E           , .func = ng_send_te    },  // て
  {.key = B_R           , .func = ng_send_si    },  // し
  {.key = B_T           , .func = ng_edit_1_left},  // {←}
  {.key = B_Y           , .func = ng_edit_1_right}, // {→}
  {.key = B_U           , .func = ng_backspace  },  // {BS}
  {.key = B_I           , .func = ng_send_ru    },  // る
  {.key = B_O           , .func = ng_send_su    },  // す
  {.key = B_P           , .func = ng_send_he    },  // へ
  {.key = B_A           , .func = ng_send_ro    },  // ろ
  {.key = B_S           , .func = ng_send_ke    },  // け
  {.key = B_D           , .func = ng_send_to    },  // と
  {.key = B_F           , .func = ng_send_ka    },  // か
  {.key = B_G           , .func = ng_send_xtu   },  // っ
  {.key = B_H           , .func = ng_send_ku    },  // く
  {.key = B_J           , .func = ng_send_a     },  // あ
  {.key = B_K           , .func = ng_send_i     },  // い
  {.key = B_L           , .func = ng_send_u     },  // う
  {.key = B_SCLN        , .func = ng_send_minus },  // ー
  {.key = B_Z           , .func = ng_send_ho    },  // ほ
  {.key = B_X           , .func = ng_send_hi    },  // ひ
  {.key = B_C           , .func = ng_send_ha    },  // は
  {.key = B_V           , .func = ng_send_ko    },  // こ
  {.key = B_B           , .func = ng_send_so    },  // そ
  {.key = B_N           , .func = ng_send_ta    },  // た
  {.key = B_M           , .func = ng_send_na    },  // な
  {.key = B_COMM        , .func = ng_send_nn    },  // ん
  {.key = B_DOT         , .func = ng_send_ra    },  // ら
  {.key = B_SLSH        , .func = ng_send_re    },  // れ
};

// 薙刀式のレイヤー、オンオフするキー
void set_naginata(uint8_t layer, uint16_t *onk, uint16_t *offk) {
  naginata_layer = layer;
  ngon_keys[0] = *onk;
  ngon_keys[1] = *(onk+1);
  ngoff_keys[0] = *offk;
  ngoff_keys[1] = *(offk+1);

  naginata_config.raw = eeconfig_read_user();
  #ifdef ENABLE_NG_IOS
  if (naginata_config.os != NG_WIN && naginata_config.os != NG_MAC && naginata_config.os != NG_LINUX && naginata_config.os != NG_IOS) {
  #else
  if (naginata_config.os != NG_WIN && naginata_config.os != NG_MAC && naginata_config.os != NG_LINUX) {
  #endif
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
  // pushed_key = 0;
  // naginata_clear();
  layer_on(naginata_layer);

  switch (naginata_config.os) {
    case NG_WIN:
    case NG_LINUX:
      tap_code(KC_INTERNATIONAL_2); // ひらがな
      tap_code(KC_INTERNATIONAL_2);
      break;
    case NG_MAC:
    #ifdef ENABLE_NG_IOS
    case NG_IOS:
    #endif
      tap_code(KC_LANGUAGE_1);      // (Mac)かな
      break;
  }
}

// 薙刀式をオフ
void naginata_off(void) {
  is_naginata = false;
  pushed_key = 0;
  naginata_clear();
  layer_off(naginata_layer);

  switch (naginata_config.os) {
    case NG_WIN:
      // Shift+Ctrl+変換 にIMEの確定を設定しておくこと
      // 確定→ひらがな→半角/全角
      tap_code16(LSFT(LCTL(KC_INTERNATIONAL_4))); // Shift+Ctrl+変換
      tap_code(KC_INTERNATIONAL_2); // ひらがな
      tap_code(KC_GRV); // 半角/全角
      break;
    case NG_MAC:
    #ifdef ENABLE_NG_IOS
    case NG_IOS:
    #endif
      tap_code(KC_LANGUAGE_2);  // (Mac)英数
      break;
    case NG_LINUX:
      tap_code(KC_GRV); // 半角/全角
      break;
  }
}

// 薙刀式のon/off状態を返す
bool naginata_state(void) {
  return is_naginata;
}

void switchOS(uint8_t os) {
  naginata_config.os = os;
  eeconfig_update_user(naginata_config.raw);
  ng_set_unicode_mode(naginata_config.os);
}

void ng_set_unicode_mode(uint8_t os) {
  switch (os) {
    case NG_WIN:
      set_unicode_input_mode(UNICODE_MODE_WINCOMPOSE);
      break;
    case NG_MAC:
    #ifdef ENABLE_NG_IOS
    case NG_IOS:
    #endif
      set_unicode_input_mode(UNICODE_MODE_MACOS);
      break;
    case NG_LINUX:
      set_unicode_input_mode(UNICODE_MODE_LINUX);
      break;
  }
}

void mac_live_conversion_toggle() {
  naginata_config.live_conv ^= 1;
  eeconfig_update_user(naginata_config.raw);
}

void tategaki_toggle() {
  naginata_config.tategaki ^= 1;
  eeconfig_update_user(naginata_config.raw);
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
        send_string("/"SS_TAP(X_KP_PLUS)"lc");
      } else {
        send_string("/-lc");
      }
      break;
    case NG_LINUX:
      send_string("linux");
      break;
    #ifdef ENABLE_NG_IOS
    case NG_IOS:
      send_string("ios");
      break;
    #endif
  }
  if (naginata_config.tategaki) {
    send_string("/tate");
  } else {
    send_string("/yoko");
  }
  if (naginata_config.kouchi_shift) {
    send_string("/"SS_TAP(X_KP_PLUS)"kouchi");
  } else {
    send_string("/-kouchi");
  }
}

#define NG_SEND_UNICODE_STRING(string) ng_send_unicode_string_P(PSTR(string))

void ng_send_unicode_string_P(const char *str) {
#ifndef MAC_USE_KAWASEMI
  static uint16_t last_send = 0;
#endif

  switch (naginata_config.os) {
    case NG_LINUX:
      tap_code(KC_GRV);
      send_unicode_string_P(str);
      tap_code(KC_INTERNATIONAL_2);
      break;
    case NG_WIN:
      send_unicode_string_P(str);
      tap_code(KC_ENT);
      break;
    case NG_MAC:
    #ifdef ENABLE_NG_IOS
    case NG_IOS:
    #endif
#ifndef MAC_USE_KAWASEMI
      // Karabiner-Elementsが必要
      {
        uint16_t delay = timer_elapsed(last_send);
        if (delay < 103) {
          wait_ms(delay + 1);
        }
      }
      tap_code(KC_LANGUAGE_2);  // 未確定文字を確定する
      tap_code16(LCTL(KC_F20)); // Unicode HEX Inputへ切り替え
      wait_ms(32);
      send_unicode_string_P(str);
      tap_code(KC_LANGUAGE_1);
      tap_code(KC_NUM_LOCK);  // IME Cancel
#else
      // かわせみ専用
      tap_code16(LOPT(LSFT(KC_B)));
      send_unicode_string_P(str);
      tap_code(KC_LANGUAGE_1);  // (Mac)かな
#endif
      break;
  }

#ifndef MAC_USE_KAWASEMI
  last_send = timer_read();
#endif
}

// modifierが押されたら薙刀式レイヤーをオフしてベースレイヤーに戻す
// get_mods()がうまく動かない
static int8_t n_modifier = 0;

bool process_modifier(uint16_t keycode, keyrecord_t *record) {
  if (IS_MODIFIER_KEYCODE(keycode) || IS_QK_MOD_TAP(keycode)) {
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
static uint8_t nkeypress = 0; // 同時にキーを押している数

// 薙刀式の起動処理(容量が大きいCOMBOを使わない)
bool enable_naginata(uint16_t keycode, keyrecord_t *record) {
  // キープレス
  if (record->event.pressed) {
    nkeypress++;
    // 1キー目、JKの前に他のキーを押していないこと
    if (fghj_buf == 0 && nkeypress == 1) {
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
        nkeypress = 0;
        return false;
      } else if ((keycode == ngoff_keys[0] && fghj_buf == ngoff_keys[1]) ||
          (keycode == ngoff_keys[1] && fghj_buf == ngoff_keys[0])) {
        naginata_off();
        fghj_buf = 0;
        nkeypress = 0;
        return false;
      // ２キー目はかなオンキーではない
      } else {
        tap_code(fghj_buf); // 1キー目を出力
        fghj_buf = 0;
        nkeypress = 0;
        return true; // 2キー目はQMKにまかせる
      }
    }
  } else {
    nkeypress = 0;
    // J/K単押しだった
    if (fghj_buf > 0) {
      tap_code(fghj_buf);
      fghj_buf = 0;

      // Shift + Jで、先にShiftを外した場合にShiftがリリースされない不具合対策
      if (IS_MODIFIER_KEYCODE(keycode)) {
        unregister_code(keycode);
      } else if (IS_QK_MOD_TAP(keycode)) {
        if (keycode & (MOD_LCTL << 8))
          unregister_code(KC_LEFT_CTRL);
        if (keycode & (MOD_LSFT << 8))
          unregister_code(KC_LEFT_SHIFT);
        if (keycode & (MOD_LALT << 8))
          unregister_code(KC_LEFT_ALT);
        if (keycode & (MOD_LGUI << 8))
          unregister_code(KC_LEFT_GUI);
        if (keycode & (MOD_RCTL << 8))
          unregister_code(KC_RIGHT_CTRL);
        if (keycode & (MOD_RSFT << 8))
          unregister_code(KC_RIGHT_SHIFT);
        if (keycode & (MOD_RALT << 8))
          unregister_code(KC_RIGHT_ALT);
        if (keycode & (MOD_RGUI << 8))
          unregister_code(KC_RIGHT_GUI);
      }
      return false;
    }
  }

  fghj_buf = 0;
  return true;
}

// バッファをクリアする
void naginata_clear(void) {
  n_modifier = 0;
  nkeypress = 0;
  fghj_buf = 0;
}

// #define LOG_PROCESS_NAGINATA
// 薙刀式の入力処理
bool process_naginata(uint16_t keycode, keyrecord_t *record) {
  // まれに薙刀モードオンのまま、レイヤーがオフになることがあるので、対策
  if (n_modifier == 0 && is_naginata && !layer_state_is(naginata_layer))
    layer_on(naginata_layer);
  if (n_modifier == 0 && !is_naginata && layer_state_is(naginata_layer))
    layer_off(naginata_layer);
  if (n_modifier > 0 && layer_state_is(naginata_layer))
    layer_off(naginata_layer);

  // OS切り替え(UNICODE出力)
  if (record->event.pressed) {
    switch (keycode) {
      case NG_ON:
        naginata_on();
        return false;
        break;
      case NG_OFF:
        naginata_off();
        return false;
        break;
      case NG_CLR:
        naginata_clear();
        return false;
        break;
      case NGSW_WIN:
        switchOS(NG_WIN);
        return false;
        break;
      case NGSW_MAC:
        switchOS(NG_MAC);
        return false;
        break;
      case NGSW_LNX:
        switchOS(NG_LINUX);
        return false;
        break;
      #ifdef ENABLE_NG_IOS
      case NGSW_IOS:
        switchOS(NG_IOS);
        return false;
        break;
      #endif
      case NG_MLV:
        mac_live_conversion_toggle();
        return false;
        break;
      case NG_SHOS:
        ng_show_os();
        return false;
        break;
      case NG_TAYO:
        tategaki_toggle();
        return false;
        break;
      case NG_KOTI:
        kouchi_shift_toggle();
        return false;
        break;
    }
  }
  if (!is_naginata)
    return enable_naginata(keycode, record);

  if (process_modifier(keycode, record))
    return true;

  // キー入力を文字に変換して出力する
  return naginata_type(keycode, record->event.pressed);
}

void ng_cut() {
  switch (naginata_config.os) {
    case NG_WIN:
    case NG_LINUX:
      tap_code16(LCTL(KC_X));
      break;
    case NG_MAC:
    #ifdef ENABLE_NG_IOS
    case NG_IOS:
    #endif
      tap_code16(LCMD(KC_X));
      break;
  }
}

void ng_copy() {
  switch (naginata_config.os) {
    case NG_WIN:
    case NG_LINUX:
      tap_code16(LCTL(KC_C));
      break;
    case NG_MAC:
    #ifdef ENABLE_NG_IOS
    case NG_IOS:
    #endif
      tap_code16(LCMD(KC_C));
      break;
  }
}

void ng_paste() {
  switch (naginata_config.os) {
    case NG_WIN:
    case NG_LINUX:
      tap_code16(LCTL(KC_V));
      break;
    case NG_MAC:
    #ifdef ENABLE_NG_IOS
    case NG_IOS:
    #endif
      register_code(KC_LCMD);
      wait_ms(100);
      tap_code(KC_V);
      wait_ms(100);
      unregister_code(KC_LCMD);
      wait_ms(100);
      break;
  }
}

void ng_up(uint8_t c) {
  for (uint8_t i = 0; i < c; i++) { // サイズ削減
    if (naginata_config.tategaki) {
      tap_code(KC_UP);
    } else {
      tap_code(KC_LEFT);
    }
  }
}

void ng_down(uint8_t c) {
  for (uint8_t i = 0; i < c; i++) {
    if (naginata_config.tategaki) {
      tap_code(KC_DOWN);
    } else {
      tap_code(KC_RIGHT);
    }
  }
}

void ng_left(uint8_t c) {
  for (uint8_t i = 0; i < c; i++) {
    if (naginata_config.tategaki) {
      tap_code(KC_LEFT);
    } else {
      tap_code(KC_DOWN);
    }
  }
}

void ng_right(uint8_t c) {
  for (uint8_t i = 0; i < c; i++) {
    if (naginata_config.tategaki) {
      tap_code(KC_RIGHT);
    } else {
      tap_code(KC_UP);
    }
  }
}

void ng_home() {
  switch (naginata_config.os) {
    case NG_WIN:
    case NG_LINUX:
      tap_code(KC_HOME);
      break;
    case NG_MAC:
    #ifdef ENABLE_NG_IOS
    case NG_IOS:
    #endif
      tap_code16(LCTL(KC_A));
      break;
  }
}

void ng_end() {
  switch (naginata_config.os) {
    case NG_WIN:
    case NG_LINUX:
      tap_code(KC_END);
      break;
    case NG_MAC:
    #ifdef ENABLE_NG_IOS
    case NG_IOS:
    #endif
      tap_code16(LCTL(KC_E));
      break;
  }
}

void ng_katakana() {
  tap_code(KC_F7);
}

void ng_save() {
  switch (naginata_config.os) {
    case NG_WIN:
    case NG_LINUX:
      tap_code16(LCTL(KC_S));
      break;
    case NG_MAC:
    #ifdef ENABLE_NG_IOS
    case NG_IOS:
    #endif
      tap_code16(LCMD(KC_S));
      break;
  }
}

void ng_hiragana() {
  tap_code(KC_F6);
}

void ng_redo() {
  switch (naginata_config.os) {
    case NG_WIN:
    case NG_LINUX:
      tap_code16(LCTL(KC_Y));
      break;
    case NG_MAC:
    #ifdef ENABLE_NG_IOS
    case NG_IOS:
    #endif
      tap_code16(LCMD(LSFT(KC_Z)));
      break;
  }
}

void ng_undo() {
  switch (naginata_config.os) {
    case NG_WIN:
    case NG_LINUX:
      tap_code16(LCTL(KC_Z));
      break;
    case NG_MAC:
    #ifdef ENABLE_NG_IOS
    case NG_IOS:
    #endif
      tap_code16(LCMD(KC_Z));
      break;
  }
}

void ng_saihenkan() {
  switch (naginata_config.os) {
    case NG_WIN:
      tap_code16(LWIN(KC_SLSH));
      break;
    case NG_LINUX:
      tap_code(KC_INT4);
      break;
    case NG_MAC:
    #ifdef ENABLE_NG_IOS
    case NG_IOS:
    #endif
      tap_code(KC_LANGUAGE_1);
      tap_code(KC_LANGUAGE_1);
      break;
  }
}

void ng_eof() {
  ng_ime_complete();
  switch (naginata_config.os) {
    case NG_WIN:
      tap_code16(LCTL(KC_END));
      break;
    case NG_LINUX:
      tap_code16(LCTL(KC_END));
      break;
    case NG_MAC:
    #ifdef ENABLE_NG_IOS
    case NG_IOS:
    #endif
      if (naginata_config.tategaki)
        tap_code16(LCMD(KC_LEFT));
      else
        tap_code16(LCMD(KC_DOWN));
      break;
  }
}

void ng_ime_cancel() {
  switch (naginata_config.os) {
    case NG_WIN:
    case NG_LINUX:
      tap_code16(LSFT(LCTL(KC_INTERNATIONAL_5)));  // Shift+Ctrl+無変換x2
      tap_code16(LSFT(LCTL(KC_INTERNATIONAL_5)));
      break;
    case NG_MAC:
      tap_code(KC_NUM_LOCK);
      break;
    #ifdef ENABLE_NG_IOS
    case NG_IOS:
      tap_code(KC_ESC);
      break;
    #endif
  }
}

void ng_ime_complete() {
  switch (naginata_config.os) {
    case NG_WIN:
      tap_code16(LSFT(LCTL(KC_INTERNATIONAL_4))); // Shift+Ctrl+変換
      tap_code16(LSFT(LCTL(KC_INTERNATIONAL_4))); // Shift+Ctrl+変換
      break;
    case NG_LINUX:
      tap_code(KC_GRV);             // 半角/全角
      tap_code(KC_INTERNATIONAL_2); // ひらがな
      break;
    case NG_MAC:
#ifndef MAC_USE_KAWASEMI
      tap_code(KC_LANGUAGE_2);  // (Mac)英数
      tap_code16(LSFT(KC_LANGUAGE_1));  // Shift+(Mac)かな
      tap_code(KC_LANGUAGE_1);  // (Mac)かな
#else
      tap_code16(LOPT(LSFT(KC_B)));
      tap_code(KC_LANGUAGE_1);  // (Mac)かな
#endif
      break;
    #ifdef ENABLE_NG_IOS
    case NG_IOS:
      tap_code(KC_LANGUAGE_2);  // (Mac)英数
      tap_code(KC_LANGUAGE_1);  // (Mac)かな
      tap_code(KC_LCTRL); // ディレイの代わり
      break;
    #endif
  }
}

#ifdef ENABLE_NG_IOS
void ios_send_string(const char *str) {
    send_string(str);
    tap_code(KC_LCTRL); tap_code(KC_LSFT); tap_code(KC_LCTRL); // ディレイの代わり
    tap_code(KC_SPC);
    tap_code(KC_ENT);
    tap_code(KC_LCTRL); // ディレイの代わり
}

void ios_send_string_with_cut_paste(const char *str) {
  ng_cut();
  ios_send_string(str);
  ng_up(1);     // 1文字戻る
  tap_code(KC_LCTRL); tap_code(KC_LSFT); // ディレイの代わり
  ng_paste();
  ng_down(1);   // 1文字進む
}
#endif

bool ng_pushed_spc = false, ng_pushed_ent = false;
uint8_t ng_center_keycode = KC_NO;
enum RestShiftState { Off, Next, On };

// キー入力を文字に変換して出力する
// 薙刀式のキー入力だったなら false を返す
// そうでなければ未出力のキーを全て出力し、QMKにまかせるため true を返す
bool naginata_type(uint16_t keycode, bool pressed) {
  static uint32_t waiting_keys[NKEYS];  // 各ビットがキーに対応する
  static uint8_t waiting_count = 0; // 文字キー入力のカウンタ
  static enum RestShiftState rest_shift_state = Off;

  uint32_t recent_key;  // 各ビットがキーに対応する
  bool add_key_later = false;

  switch (keycode) {
    case NG_Q ... NG_SLSH:
      recent_key = ng_key[keycode - NG_Q];
      break;
    case NG_SHFT: // スペースキー
      recent_key = B_SHFT;
      ng_pushed_spc = pressed;
      // センターキーの渡り対策
      ng_center_keycode = pressed || ng_center_keycode == KC_SPACE ? KC_SPACE : KC_NO;
      break;
    case NG_SHFT2:  // エンターキー
      recent_key = B_SHFT;
      ng_pushed_ent = pressed;
      // センターキーの渡り対策
      ng_center_keycode = pressed || ng_center_keycode == KC_ENTER ? KC_ENTER : KC_NO;
      break;
    default:
      recent_key = 0;
      break;
  }

  // センターシフトは連続する
  uint32_t center_shift = pushed_key & B_SHFT;

  // 薙刀式のキーを押した時
  if (pressed && recent_key) {
    pushed_key |= recent_key;  // キーを加える

    // センターシフト(後置シフトなし)の時
    if (recent_key == B_SHFT && !naginata_config.kouchi_shift) {
      add_key_later = true;
    } else {
      // 配列に押したキーを保存
      waiting_keys[waiting_count++] = recent_key;
    }
  }

  // 出力
  {
    uint8_t searching_count = waiting_count;
    while (searching_count) {
      // バッファ内のキーを組み合わせる
      uint32_t searching_key = center_shift;
      for (uint8_t i = 0; i < searching_count; i++) {
        searching_key |= waiting_keys[i];
      }
      // シフト残り処理
      if (rest_shift_state == On) {
        Ngmap_num num = ng_search_with_rest_key(searching_key, pushed_key);
        if (num < NGMAP_COUNT) {
#if defined(__AVR__)
          uint32_t key;
          memcpy_P(&key, &ngmap[num].key, sizeof(key));
          searching_key |= key;
#else
          searching_key |= ngmap[num].key;
#endif
        }
      }
      // バッファ内の全てのキーを組み合わせた時は
      // (センターシフト(後置シフトなし)の時は全て出力する)
      if (searching_count == waiting_count && !add_key_later) {
        if (pressed && recent_key) {
          // 今押したキー以外が出力済みの時にシフト残り処理開始
          if (waiting_count == 1 && rest_shift_state == Next) {
            rest_shift_state = On;
            continue;
          }
          // 変換候補を数える
          uint8_t nc = number_of_candidates(searching_key);
          // 組み合わせがない = 0: 変換を開始する
          if (nc == 0) {
            searching_count--;  // 最後のキーを減らして検索
            continue;
          // 組み合わせをしぼれない = 2: 変換しない
          // (薙刀式以外のキーを押した時は全て出力する)
          } else if (nc != 1) {
            break;
          }
        // キーを離した時は、そのキーが関わるところまで出力する
        // (薙刀式以外のキーを離した時は出力しない)
        } else if (!pressed && !(searching_key & recent_key)) {
          break;
        }
      }

      // かな定義を探して出力する
      if (ng_search_and_send(searching_key)) {
        // 1回出力したらシフト残り処理は終わり
        if (rest_shift_state == On) {
          rest_shift_state = Off;
        }
        // 見つかった分のキーを配列から取り除く
        waiting_count -= searching_count;
        for (uint8_t i = 0; i < waiting_count; i++) {
          waiting_keys[i] = waiting_keys[i + searching_count];
        }
        searching_count = waiting_count;
      // 見つからなかったら最後のキーを減らして再検索
      } else {
        searching_count--;
      }
    }
    // 何も定義がないキーへの応急対策
    if (!searching_count) {
      waiting_count = 0;
    }
  }

  // センターシフト(後置シフトなし)の時
  if (add_key_later) {
    // 配列に押したキーを保存
    waiting_keys[waiting_count++] = recent_key;
  // キーを離した時
  } else if (!pressed) {
#ifdef USE_SHIFT_WHEN_SPACE_UP
    pushed_key &= ~recent_key; // キーを取り除く
#endif
    // スペースを押していないなら次回、シフト残りを含めて探す
    if (pushed_key & B_SHFT || !pushed_key) {
      rest_shift_state = Off;
    } else if (rest_shift_state != On) {
      rest_shift_state = Next;
    }
#ifndef USE_SHIFT_WHEN_SPACE_UP
    pushed_key &= ~recent_key; // キーを取り除く
#endif
  }

  return (recent_key == 0);
}

// かな定義を探し出力する
// 成功すれば true を返す
bool ng_search_and_send(uint32_t searching_key) {
  // if (!searching_key)  return false;
  for (Ngmap_num num = NGMAP_COUNT; num-- > 0; ) {  // 逆順で検索
#if defined(__AVR__)
    uint32_t key;
    memcpy_P(&key, &ngmap[num].key, sizeof(key));
    if (searching_key == key) {
      void (*func)(void);
      memcpy_P(&func, &ngmap[num].func, sizeof(func));
      func();
      return true;
    }
#else
    if (searching_key == ngmap[num].key) {
      ngmap[num].func();
      return true;
    }
#endif
  }
  return false;
}

// すでに押されているキーをシフトとし、いま押したキーを含むかな定義を探し、配列の添え字を返す
// 見つからなければ、かな定義の要素数 NGMAP_COUNT を返す
Ngmap_num ng_search_with_rest_key(uint32_t searching_key, uint32_t pushed_key) {
  // if (!(searching_key && pushed_key))  return NGMAP_COUNT;
  Ngmap_num num = 0;
  for ( ; num < NGMAP_COUNT; num++) {
    uint32_t key;
#if defined(__AVR__)
    memcpy_P(&key, &ngmap[num].key, sizeof(key));
#else
    key = ngmap[num].key;
#endif
    // 押しているキーに全て含まれ、今回のキーを含み、スペースを押さない定義を探す
    if ((pushed_key & key) == key && (key & searching_key) == searching_key && !(key & B_SHFT)) {
      break;
    }
  }
  return num;
}

// 組み合わせをしぼれない = 2: 変換しない
// 組み合わせが一つしかない = 1: 変換を開始する
// 組み合わせがない = 0: 変換を開始する
int8_t number_of_candidates(uint32_t search) {
  int8_t c = 0;
  for (Ngmap_num i = 0; i < NGMAP_COUNT; i++) {
    uint32_t key;
#if defined(__AVR__)
    memcpy_P(&key, &ngmap[i].key, sizeof(key));
#else
    key = ngmap[i].key;
#endif
    if ((search & key) == search) {
      c++;
      if (search != key || c > 1) {
        return 2;
      }
    }
  }
  return c;
}
