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
static Ngkey pushed_key = 0; // 同時押しの状態を示す。各ビットがキーに対応する。

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
const Ngkey ng_key[] = {
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
  Ngkey key;
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
  {.key = B_D|B_F|B_O   , .func = ng_delete_with_repeat}, // {Del}
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
  {.key = B_U           , .func = ng_backspace_with_repeat  },  // {BS}
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
#ifndef NG_BMP
  switch (naginata_config.os) {
    case NG_WIN ...  NG_LINUX:
      break;
    default:
      naginata_config.os = NG_WIN;
      naginata_config.live_conv = 1;
      naginata_config.tategaki = 1;
      naginata_config.kouchi_shift = 0;
      break;
  }
#else
  switch (naginata_config.os) {
    case NG_WIN_DIC ... NG_IOS:
      break;
    default:
      naginata_config.os = NG_IOS;
      naginata_config.live_conv = 0;
      naginata_config.tategaki = 0;
      naginata_config.kouchi_shift = 0;
      break;
  }
#endif
  eeconfig_update_user(naginata_config.raw);
  ng_set_unicode_mode(naginata_config.os);
}

// 薙刀式をオン
void naginata_on(void) {
  is_naginata = true;
  // pushed_key = 0;
  // naginata_clear();
  layer_on(naginata_layer);

  switch (naginata_config.os) {
#ifndef NG_BMP
    case NG_WIN:
    case NG_LINUX:
      tap_code(KC_INTERNATIONAL_2); // ひらがな
      tap_code(KC_INTERNATIONAL_2);
      break;
    case NG_MAC:
      tap_code(KC_LANGUAGE_1);      // (Mac)かな
      break;
#else
    case NG_WIN_DIC:
    case NG_LINUX_DIC:
      tap_code(KC_INTERNATIONAL_2); // ひらがな
      tap_code(KC_INTERNATIONAL_2);
      break;
    case NG_MAC_DIC:
    case NG_IOS:
      tap_code(KC_LANGUAGE_1);      // (Mac)かな
      break;
#endif
  }
}

// 薙刀式をオフ
void naginata_off(void) {
  is_naginata = false;
  pushed_key = 0;
  naginata_clear();
  layer_off(naginata_layer);

  switch (naginata_config.os) {
#ifndef NG_BMP
    case NG_WIN:
      // Shift+Ctrl+変換 にIMEの確定を設定しておくこと
      // 確定→ひらがな→半角/全角
      tap_code16(LSFT(LCTL(KC_INTERNATIONAL_4))); // Shift+Ctrl+変換
# ifdef USB_POLLING_INTERVAL_MS
      wait_ms(USB_POLLING_INTERVAL_MS + 7); // 確実にIMEオフにするため 8ms 空ける
# else
      wait_ms(8); // 確実にIMEオフにするため
# endif
      tap_code(KC_INTERNATIONAL_2); // ひらがな
      tap_code(KC_GRV); // 半角/全角
      break;
    case NG_MAC:
      tap_code(KC_LANGUAGE_2);  // (Mac)英数
      break;
    case NG_LINUX:
      tap_code(KC_GRV); // 半角/全角
      break;
#else
    case NG_WIN_DIC:
      // Shift+Ctrl+変換 にIMEの確定を設定しておくこと
      // 確定→ひらがな→半角/全角
      tap_code16(LSFT(LCTL(KC_INTERNATIONAL_4))); // Shift+Ctrl+変換
      tap_code(KC_INTERNATIONAL_2); // ひらがな
      tap_code(KC_GRV); // 半角/全角
      break;
    case NG_MAC_DIC:
    case NG_IOS:
      tap_code(KC_LANGUAGE_2);  // (Mac)英数
      break;
    case NG_LINUX_DIC:
      tap_code(KC_GRV); // 半角/全角
      break;
#endif
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
#ifndef NG_BMP
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
#else
    case NG_WIN_DIC:
      send_string("win-dic");
      break;
    case NG_MAC_DIC:
      send_string("mac-dic");
      if (naginata_config.live_conv) {
        send_string("/"SS_TAP(X_KP_PLUS)"lc");
      } else {
        send_string("/-lc");
      }
      break;
    case NG_LINUX_DIC:
      send_string("linux-dic");
      break;
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
#ifndef NG_USE_KAWASEMI
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
#ifndef NG_USE_KAWASEMI
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

#ifndef NG_USE_KAWASEMI
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

static uint16_t fghj_buf = KC_NO; // 押しているJかKのキーコード

// 薙刀式の起動処理(容量が大きいCOMBOを使わない)
bool enable_naginata(uint16_t keycode, keyrecord_t *record) {
  // キープレス
  if (record->event.pressed) {
    // １キー目がかなオン・オフキーだった
    if (fghj_buf != KC_NO) {
      // ２キー目、１キー目、両方ともかなオンキー
      if ((keycode == ngon_keys[0] && fghj_buf == ngon_keys[1]) ||
          (keycode == ngon_keys[1] && fghj_buf == ngon_keys[0])) {
        naginata_on();
        fghj_buf = KC_NO;
        return false;
      // ２キー目、１キー目、両方ともかなオフキー
      } else if ((keycode == ngoff_keys[0] && fghj_buf == ngoff_keys[1]) ||
          (keycode == ngoff_keys[1] && fghj_buf == ngoff_keys[0])) {
        naginata_off();
        fghj_buf = KC_NO;
        return false;
      }
      // どちらでもなければ、1キー目を出力
      tap_code(fghj_buf);
      fghj_buf = KC_NO;
    }
    // 今度のキーがかなオン・オフキーの場合
    if (keycode == ngon_keys[0] || keycode == ngon_keys[1] || keycode == ngoff_keys[0] || keycode == ngoff_keys[1]) {
      fghj_buf = keycode;
      return false;
    }
  // かなオン・オフキーが離された
  } else if (fghj_buf != KC_NO) {
    tap_code(fghj_buf);
    fghj_buf = KC_NO;
    // Shift + Jで、先にShiftを外した場合にShiftがリリースされない不具合対策
    // unregister_code16(keycode);
    // return false;
  }
  return true;  // QMKにまかせる
}

// バッファをクリアする
void naginata_clear(void) {
  end_repeating_key();  // キーリピート解除
  n_modifier = 0;
  fghj_buf = 0;
}

// #define LOG_PROCESS_NAGINATA
// 薙刀式の入力処理
bool process_naginata(uint16_t keycode, keyrecord_t *record) {
  // // まれに薙刀モードオンのまま、レイヤーがオフになることがあるので、対策
  // if (n_modifier == 0 && is_naginata && !layer_state_is(naginata_layer))
  //   layer_on(naginata_layer);
  // if (n_modifier == 0 && !is_naginata && layer_state_is(naginata_layer))
  //   layer_off(naginata_layer);
  // if (n_modifier > 0 && layer_state_is(naginata_layer))
  //   layer_off(naginata_layer);

  // OS切り替え(UNICODE出力)
  if (record->event.pressed) {
    switch (keycode) {
      case NG_ON:
        naginata_on();
        return false;
      case NG_OFF:
        naginata_off();
        return false;
#ifndef NG_BMP
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
#else
      case NGSW_WIN:
        switchOS(NG_WIN_DIC);
        return false;
      case NGSW_MAC:
        switchOS(NG_MAC_DIC);
        return false;
      case NGSW_LNX:
        switchOS(NG_LINUX_DIC);
        return false;
      case NGSW_IOS:
        switchOS(NG_IOS);
        return false;
#endif
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
    return enable_naginata(keycode, record);

  // Shift+英字 で IMEオフ
  {
    uint8_t basic_keycode = QK_MODS_GET_BASIC_KEYCODE(keycode);
    if (record->event.pressed && basic_keycode >= KC_A && basic_keycode <= KC_Z) {
      uint8_t mods = get_mods();
      if ((mods & MOD_BIT(KC_LSFT)) == MOD_BIT(KC_LSFT) ||
          (mods & MOD_BIT(KC_RSFT)) == MOD_BIT(KC_RSFT)) {
        del_mods(mods);
        naginata_off();
        add_mods(mods);
        return true;
      }
    }
  }

  if (process_modifier(keycode, record))
    return true;

  // キー入力を文字に変換して出力する
  return naginata_type(keycode, record);
}

// かな定義を探し出力する
// 成功すれば true を返す
bool ng_search_and_send(Ngkey searching_key) {
  // if (!searching_key)  return false;
  for (Ngmap_num num = NGMAP_COUNT; num-- > 0; ) {  // 逆順で検索
#if defined(__AVR__)
    Ngkey key;
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

// 押されているキーのどれかをシフトとするかな定義を探し、配列の添え字を返す
// 見つからないと NGMAP_COUNT を返す
Ngmap_num ng_search_with_rest_key(Ngkey searching_key, Ngkey pushed_key) {
  // if (!(searching_key && pushed_key))  return NGMAP_COUNT;
  Ngmap_num num = 0;
  for ( ; num < NGMAP_COUNT; num++) {
    Ngkey key;
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
int_fast8_t number_of_candidates(Ngkey search) {
  int_fast8_t c = 0;
  for (Ngmap_num i = 0; i < NGMAP_COUNT; i++) {
    Ngkey key;
#if defined(__AVR__)
    memcpy_P(&key, &ngmap[i].key, sizeof(key));
#else
    key = ngmap[i].key;
#endif
    // search を含む。前置シフト限定ならセンターシフトも一致している
    if ((search & key) == search && (naginata_config.kouchi_shift || (key & B_SHFT) == (search & B_SHFT))) {
      c++;
      if (search != key || c > 1) {
        return 2;
      }
    }
  }
  return c;
}

// リピート中を示す変数
static struct {
  uint8_t code; // リピート中の文字コード
  uint8_t mod;  // リピート中の装飾キーの文字コード
} repeating = { KC_NO, KC_NO };

// キーリピート解除
void end_repeating_key(void) {
  if (repeating.code != KC_NO) {
    unregister_code(repeating.code);
    unregister_code(repeating.mod);
    repeating.code = repeating.mod = KC_NO;
  }
}

static bool ng_pushed_spc = false, ng_pushed_ent = false;
static uint8_t ng_center_keycode = KC_NO;
enum RestShiftState { Stop, Checking, Once };

// キー入力を文字に変換して出力する
// 薙刀式のキー入力だったなら false を返す
// そうでない時は未出力を全て出力、true を返してQMKにまかせる
bool naginata_type(uint16_t keycode, keyrecord_t *record) {
  static Ngkey waiting_keys[NKEYS];  // 各ビットがキーに対応する
  static Ngkey repeating_key = 0;
#ifdef NG_KOUCHI_SHIFT_MS
  static uint16_t previous_pushed_ms = 0;
#endif
  static uint_fast8_t waiting_count = 0; // 文字キーを数える
  static enum RestShiftState rest_shift_state = Stop;

  Ngkey recent_key;  // 各ビットがキーに対応する
  const bool pressed = record->event.pressed;
  bool store_key_later = false;

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

  // センターシフトの連続用
  Ngkey contains_center_shift = pushed_key;

  // 薙刀式のキーを押した
  if (pressed) {
    pushed_key |= recent_key;  // キーを加える

#ifdef NG_KOUCHI_SHIFT_MS
    // センターシフト(時間制限内のため後置シフトとして処理するものを除く)
    if (recent_key == B_SHFT && (!naginata_config.kouchi_shift
        || (uint16_t)(record->event.time - previous_pushed_ms) > (NG_KOUCHI_SHIFT_MS))) {
#else
    // センターシフト(前置シフト限定)
    if (recent_key == B_SHFT && !naginata_config.kouchi_shift) {
#endif
      store_key_later = true;
    } else if (recent_key) {
      // 配列に押したキーを保存
      waiting_keys[waiting_count++] = recent_key;
    }
#ifdef NG_KOUCHI_SHIFT_MS
    previous_pushed_ms = record->event.time;
#endif
  }
  // 何かキーを押したか、リピート中のキーを離した時
  if (pressed || (repeating_key & recent_key)) {
    end_repeating_key();  // キーリピート解除
  }

  // 出力
  {
    uint_fast8_t searching_count = waiting_count;
    while (searching_count) {
      // バッファ内のキーを組み合わせる
      Ngkey searching_key = contains_center_shift & B_SHFT; // センターキー
      for (uint_fast8_t i = 0; i < searching_count; i++) {
        searching_key |= waiting_keys[i];
      }

      // シフト復活処理
      if (rest_shift_state == Once) {
        Ngmap_num num = ng_search_with_rest_key(searching_key, pushed_key);
        if (num < NGMAP_COUNT) {
#if defined(__AVR__)
          Ngkey key;
          memcpy_P(&key, &ngmap[num].key, sizeof(key));
          searching_key |= key;
#else
          searching_key |= ngmap[num].key;
#endif
        }
      }

      // バッファ内の全てのキーを組み合わせている
      // (前置シフト限定でセンターシフトの時は全て出力する)
      if (searching_count == waiting_count && !store_key_later) {
        if (pressed && recent_key) {
          // 今押したキー以外の出力が済んでいるとシフト復活へ
          if (waiting_count == 1 && rest_shift_state == Checking) {
            rest_shift_state = Once;
            continue;
          }
          // 変換候補を数える
          int_fast8_t nc = number_of_candidates(searching_key);
          // 組み合わせがない = 0: 変換を開始する
          if (nc == 0) {
            searching_count--;  // 最後のキーを減らして検索
            continue;
          // 組み合わせをしぼれない = 2: 変換しない
          } else if (nc != 1) {
            break;
          }
        // キーを離した時は、そのキーが関わるところまで出力する
        // (薙刀式以外のキーを離したのなら出力しない)
        } else if (!pressed && !(searching_key & recent_key)) {
          break;
        }
      }

      // かな定義を探して出力する
      if (ng_search_and_send(searching_key)) {
        // センターシフトの連続用
        contains_center_shift = searching_key; // 薙刀式v15では不要
        // 1回出力したらシフト復活は終わり
        if (rest_shift_state == Once) {
          rest_shift_state = Stop;
        }
        // 見つかった分のキーを配列から取り除く
        waiting_count -= searching_count;
        for (uint_fast8_t i = 0; i < waiting_count; i++) {
          waiting_keys[i] = waiting_keys[i + searching_count];
        }
        searching_count = waiting_count;
        // キーを離したり、まだ探すキーが残ってたらキーリピートしない
        if (!pressed || searching_count) {
          end_repeating_key();  // キーリピート解除
        // リピートするキーを保存
        } else {
          repeating_key = searching_key;
        }
      // 見つからなかったら最後のキーを減らして再検索
      } else {
        searching_count--;
      }
    }

    // 何も定義がないキー
    if (!searching_count) {
      waiting_count = 0;
    }
    // シフト復活は不発
    if (rest_shift_state == Checking) {
      rest_shift_state = Stop;
    }
  }

  // センターシフト(前置シフト限定)
  if (store_key_later) {
    // 配列に押したキーを保存
    waiting_keys[waiting_count++] = recent_key;
  // キーを離した時
  } else if (!pressed) {
#ifdef NG_USE_SHIFT_WHEN_SPACE_UP
    pushed_key &= ~recent_key; // キーを取り除く
#endif
    // スペースを押していないなら次回、シフト復活可能
    if (pushed_key & B_SHFT || !pushed_key) {
      rest_shift_state = Stop;
    } else if (rest_shift_state != Once) {
      rest_shift_state = Checking;
    }
#ifndef NG_USE_SHIFT_WHEN_SPACE_UP
    pushed_key &= ~recent_key; // キーを取り除く
#endif
  }

  return (recent_key == 0);
}

void ng_space_or_enter(void) {
  if (ng_center_keycode == KC_NO) return;
  if (ng_pushed_spc | ng_pushed_ent) {
    add_mods(MOD_BIT(KC_LSFT));
    tap_code(ng_center_keycode);
    del_mods(MOD_BIT(KC_LSFT));
  } else {
    tap_code(ng_center_keycode);
  }
}

void ng_backspace_with_repeat(void) { // {BS}
  repeating.code = KC_BACKSPACE;
  register_code(repeating.code);
}

void ng_delete_with_repeat(void) { // {Del}
  repeating.code = KC_DEL;
  register_code(repeating.code);
}

void ng_cut() {
  switch (naginata_config.os) {
#ifndef NG_BMP
    case NG_WIN:
    case NG_LINUX:
      tap_code16(LCTL(KC_X));
      break;
    case NG_MAC:
      tap_code16(LCMD(KC_X));
      break;
#else
    case NG_WIN_DIC:
    case NG_LINUX_DIC:
      tap_code16(LCTL(KC_X));
      break;
    case NG_MAC_DIC:
    case NG_IOS:
      tap_code16(LCMD(KC_X));
      break;
#endif
  }
}

void ng_copy() {
  switch (naginata_config.os) {
#ifndef NG_BMP
    case NG_WIN:
    case NG_LINUX:
      tap_code16(LCTL(KC_C));
      break;
    case NG_MAC:
      tap_code16(LCMD(KC_C));
      break;
#else
    case NG_WIN_DIC:
    case NG_LINUX_DIC:
      tap_code16(LCTL(KC_C));
      break;
    case NG_MAC_DIC:
    case NG_IOS:
      tap_code16(LCMD(KC_C));
      break;
#endif
  }
}

void ng_paste() {
  switch (naginata_config.os) {
#ifndef NG_BMP
    case NG_WIN:
    case NG_LINUX:
      tap_code16(LCTL(KC_V));
      break;
    case NG_MAC:
      register_code(KC_LCMD);
      wait_ms(100);
      tap_code(KC_V);
      wait_ms(100);
      unregister_code(KC_LCMD);
      wait_ms(100);
      break;
#else
    case NG_WIN_DIC:
    case NG_LINUX_DIC:
      tap_code16(LCTL(KC_V));
      break;
    case NG_MAC_DIC:
    case NG_IOS:
      tap_code16(LGUI(KC_V));
      break;
#endif
  }
}

// リピート対応の方向キー移動
// リピート中を示す変数を更新
void ng_move_cursor_with_repeat(bool shift, uint8_t code, uint8_t count) {
  if (shift) {
    repeating.mod = KC_LSFT;
    register_code(repeating.mod);
  }
  repeating.code = code;
  if (!naginata_config.tategaki) {
    switch (code) {
      case KC_LEFT:   repeating.code = KC_DOWN;   break;
      case KC_DOWN:   repeating.code = KC_RIGHT;  break;
      case KC_RIGHT:  repeating.code = KC_UP;     break;
      case KC_UP:     repeating.code = KC_LEFT;   break;
      default:    break;
    }
  }
  for (uint8_t i = count; i > 1; i--) {
    tap_code(repeating.code);
  }
  register_code(repeating.code);
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
#ifndef NG_BMP
    case NG_WIN:
    case NG_LINUX:
      tap_code(KC_HOME);
      break;
    case NG_MAC:
      tap_code16(LCTL(KC_A));
      break;
#else
    case NG_WIN_DIC:
    case NG_LINUX_DIC:
      tap_code(KC_HOME);
      break;
    case NG_MAC_DIC:
    case NG_IOS:
      tap_code16(LCTL(KC_A));
      break;
#endif
  }
}

void ng_end() {
  switch (naginata_config.os) {
#ifndef NG_BMP
    case NG_WIN:
    case NG_LINUX:
      tap_code(KC_END);
      break;
    case NG_MAC:
      tap_code16(LCTL(KC_E));
      break;
#else
    case NG_WIN_DIC:
    case NG_LINUX_DIC:
      tap_code(KC_END);
      break;
    case NG_MAC_DIC:
    case NG_IOS:
      tap_code16(LCTL(KC_E));
      break;
#endif
  }
}

void ng_katakana() {
  tap_code(KC_F7);
}

void ng_save() {
  switch (naginata_config.os) {
#ifndef NG_BMP
    case NG_WIN:
    case NG_LINUX:
      tap_code16(LCTL(KC_S));
      break;
    case NG_MAC:
      tap_code16(LCMD(KC_S));
      break;
#else
    case NG_WIN_DIC:
    case NG_LINUX_DIC:
      tap_code16(LCTL(KC_S));
      break;
    case NG_MAC_DIC:
    case NG_IOS:
      tap_code16(LCMD(KC_S));
      break;
#endif
  }
}

void ng_hiragana() {
  tap_code(KC_F6);
}

void ng_redo() {
  switch (naginata_config.os) {
#ifndef NG_BMP
    case NG_WIN:
    case NG_LINUX:
      tap_code16(LCTL(KC_Y));
      break;
    case NG_MAC:
      tap_code16(LCMD(LSFT(KC_Z)));
      break;
#else
    case NG_WIN_DIC:
    case NG_LINUX_DIC:
      tap_code16(LCTL(KC_Y));
      break;
    case NG_MAC_DIC:
    case NG_IOS:
      tap_code16(LCMD(LSFT(KC_Z)));
      break;
#endif
  }
}

void ng_undo() {
  switch (naginata_config.os) {
#ifndef NG_BMP
    case NG_WIN:
    case NG_LINUX:
      tap_code16(LCTL(KC_Z));
      break;
    case NG_MAC:
      tap_code16(LCMD(KC_Z));
      break;
#else
    case NG_WIN_DIC:
    case NG_LINUX_DIC:
      tap_code16(LCTL(KC_Z));
      break;
    case NG_MAC_DIC:
    case NG_IOS:
      tap_code16(LCMD(KC_Z));
      break;
#endif
  }
}

void ng_saihenkan() {
  switch (naginata_config.os) {
#ifndef NG_BMP
    case NG_WIN:
      tap_code16(LWIN(KC_SLSH));
      break;
    case NG_LINUX:
      tap_code(KC_INT4);
      break;
    case NG_MAC:
      tap_code(KC_LANGUAGE_1);
      tap_code(KC_LANGUAGE_1);
      break;
#else
    case NG_WIN_DIC:
      tap_code16(LWIN(KC_SLSH));
      break;
    case NG_LINUX_DIC:
      tap_code(KC_INT4);
      break;
    case NG_MAC_DIC:
    case NG_IOS:
      tap_code(KC_LANGUAGE_1);
      tap_code(KC_LANGUAGE_1);
      break;
#endif
  }
}

void ng_eof() {
  ng_ime_complete();
  switch (naginata_config.os) {
#ifndef NG_BMP
    case NG_WIN:
      tap_code16(LCTL(KC_END));
      break;
    case NG_LINUX:
      tap_code16(LCTL(KC_END));
      break;
    case NG_MAC:
      if (naginata_config.tategaki)
        tap_code16(LCMD(KC_LEFT));
      else
        tap_code16(LCMD(KC_DOWN));
      break;
#else
    case NG_WIN_DIC:
      tap_code16(LCTL(KC_END));
      break;
    case NG_LINUX_DIC:
      tap_code16(LCTL(KC_END));
      break;
    case NG_MAC_DIC:
    case NG_IOS:
      if (naginata_config.tategaki)
        tap_code16(LCMD(KC_LEFT));
      else
        tap_code16(LCMD(KC_DOWN));
      break;
#endif
  }
}

void ng_ime_cancel() {
  switch (naginata_config.os) {
#ifndef NG_BMP
    case NG_WIN:
    case NG_LINUX:
      tap_code16(LSFT(LCTL(KC_INTERNATIONAL_5)));  // Shift+Ctrl+無変換x2
      tap_code16(LSFT(LCTL(KC_INTERNATIONAL_5)));
      break;
    case NG_MAC:
      tap_code(KC_NUM_LOCK);
      break;
#else
    case NG_WIN_DIC:
    case NG_LINUX_DIC:
      tap_code16(LSFT(LCTL(KC_INTERNATIONAL_5)));  // Shift+Ctrl+無変換x2
      tap_code16(LSFT(LCTL(KC_INTERNATIONAL_5)));
      break;
    case NG_MAC_DIC:
      tap_code(KC_NUM_LOCK);
      break;
    case NG_IOS:
      tap_code(KC_ESC);
      break;
#endif
  }
}

void ng_ime_complete() {
#ifndef NG_BMP
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
# ifndef NG_USE_KAWASEMI
      tap_code(KC_LANGUAGE_2);  // (Mac)英数
      tap_code16(LSFT(KC_LANGUAGE_1));  // Shift+(Mac)かな
      tap_code(KC_LANGUAGE_1);  // (Mac)かな
# else
      tap_code16(LOPT(LSFT(KC_B)));
      tap_code(KC_LANGUAGE_1);  // (Mac)かな
# endif
      break;
  }
#else
  switch (naginata_config.os) {
    case NG_WIN_DIC:
      tap_code16(LSFT(LCTL(KC_INTERNATIONAL_4))); // Shift+Ctrl+変換
      tap_code16(LSFT(LCTL(KC_INTERNATIONAL_4))); // Shift+Ctrl+変換
      break;
    case NG_LINUX_DIC:
      tap_code(KC_GRV);             // 半角/全角
      tap_code(KC_INTERNATIONAL_2); // ひらがな
      break;
    case NG_MAC_DIC:
      tap_code(KC_LANGUAGE_2);  // (Mac)英数
      tap_code(KC_LANGUAGE_1);  // (Mac)かな
      break;
    case NG_IOS:
      tap_code(KC_LANGUAGE_2);  // (Mac)英数
      tap_code(KC_LANGUAGE_1);  // (Mac)かな
      tap_code(KC_LCTL); // ディレイの代わり
      break;
  }
#endif
}

// 辞書式用
void dic_send_string(const char *str) {
  switch (naginata_config.os) {
    case NG_IOS:
      ng_send_kana(str);
      tap_code(KC_LCTL); tap_code(KC_LSFT); tap_code(KC_LCTL); // ディレイの代わり
      tap_code(KC_SPC);
      tap_code(KC_ENT);
      tap_code(KC_LCTL); // ディレイの代わり
      break;
    default:
      ng_send_kana(str);
      tap_code(KC_SPC);
      tap_code(KC_ENT);
      break;
  }
}

void dic_send_string_with_cut_paste(const char *str) {
  switch (naginata_config.os) {
    case NG_IOS:
      ng_cut();
      dic_send_string(str);
      ng_up(1);     // 1文字戻る
      tap_code(KC_LCTL); tap_code(KC_LSFT); // ディレイの代わり
      ng_paste();
      tap_code(KC_LCTL); tap_code(KC_LSFT); // ディレイの代わり
      ng_down(1);   // 1文字進む
      break;
    default:
      ng_cut();
      dic_send_string(str);
      ng_up(1);     // 1文字戻る
      ng_paste();
      ng_down(1);   // 1文字進む
      break;
  }
}

#define MAX_REGISTER_KEY 4 // 6ロールオーバーのうち、予備用に2を減ずる

// キーをまとめて一気に押してから離す
// 例: xtu → x押すt押すu押すx離すt離すu離す
void ng_send_kana(const char *str) {
  uint_fast8_t registered_count = 0;
  uint8_t registered[MAX_REGISTER_KEY];
  char ascii_code;

  while ((ascii_code = pgm_read_byte(str++)) != '\0') {
    // アスキーコードからキーコードに変換
    uint8_t keycode = pgm_read_byte(&ascii_to_keycode_lut[(uint8_t)ascii_code]);
    // バッファに同じキーがあったらそのキーを離す
    for (uint_fast8_t i = 0; i < registered_count; i++) {
      if (registered[i] == keycode) {
        unregister_code(keycode);
        // registered[] を詰める
        registered_count--;
        for ( ; i < registered_count; i++) {
          registered[i] = registered[i + 1];
        }
        break;
      }
    }
    // バッファがいっぱいだったらバッファの先頭のキーを離す
    if (registered_count == MAX_REGISTER_KEY) {
      unregister_code(registered[0]);
      registered_count--;
      for (uint_fast8_t i = 0; i < registered_count; i++) {
        registered[i] = registered[i + 1];
      }
    }
    // キーを押す
    register_code(keycode);
    registered[registered_count++] = keycode;
  }

  // 最後にすべてのキーを離す
  for (uint_fast8_t i = 0; i < registered_count; i++) {
    unregister_code(registered[i]);
  }
}
