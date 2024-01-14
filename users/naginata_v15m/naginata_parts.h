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

#pragma once

void ng_null(void);

// アルファベット
void ng_send_a(void); // あ
void ng_send_b(void);
void ng_send_c(void);
void ng_send_d(void);
void ng_send_e(void); // え
void ng_send_f(void);
void ng_send_g(void);
void ng_send_h(void);
void ng_send_i(void); // い
void ng_send_j(void);
void ng_send_k(void);
void ng_send_l(void);
void ng_send_m(void);
void ng_send_n(void);
void ng_send_o(void); // お
void ng_send_p(void);
void ng_send_q(void);
void ng_send_r(void);
void ng_send_s(void);
void ng_send_t(void);
void ng_send_u(void); // う
void ng_send_v(void);
void ng_send_w(void);
void ng_send_x(void);
void ng_send_y(void);
void ng_send_z(void);

// 清音
void ng_send_ka(void);      // か
void ng_send_ki(void);      // き
void ng_send_ku(void);      // く
void ng_send_ke(void);      // け
void ng_send_ko(void);      // こ
void ng_send_sa(void);      // さ
void ng_send_si(void);      // し
void ng_send_su(void);      // す
void ng_send_se(void);      // せ
void ng_send_so(void);      // そ
void ng_send_ta(void);      // た
void ng_send_ti(void);      // ち
void ng_send_tu(void);      // つ
void ng_send_te(void);      // て
void ng_send_to(void);      // と
void ng_send_na(void);      // な
void ng_send_ni(void);      // に
void ng_send_nu(void);      // ぬ
void ng_send_ne(void);      // ね
void ng_send_no(void);      // の
void ng_send_ha(void);      // は
void ng_send_hi(void);      // ひ
void ng_send_hu(void);      // ふ
void ng_send_he(void);      // へ
void ng_send_ho(void);      // ほ
void ng_send_ma(void);      // ま
void ng_send_mi(void);      // み
void ng_send_mu(void);      // む
void ng_send_me(void);      // め
void ng_send_mo(void);      // も
void ng_send_ya(void);      // や
void ng_send_yu(void);      // ゆ
void ng_send_yo(void);      // よ
void ng_send_ra(void);      // ら
void ng_send_ri(void);      // り
void ng_send_ru(void);      // る
void ng_send_re(void);      // れ
void ng_send_ro(void);      // ろ
void ng_send_wa(void);      // わ
void ng_send_wo(void);      // を
void ng_send_nn(void);      // ん
void ng_send_minus(void);   // ー

// 濁音
void ng_send_ga(void);      // が
void ng_send_gi(void);      // ぎ
void ng_send_gu(void);      // ぐ
void ng_send_ge(void);      // げ
void ng_send_go(void);      // ご
void ng_send_za(void);      // ざ
void ng_send_zi(void);      // じ
void ng_send_zu(void);      // ず
void ng_send_ze(void);      // ぜ
void ng_send_zo(void);      // ぞ
void ng_send_da(void);      // だ
void ng_send_di(void);      // ぢ
void ng_send_du(void);      // づ
void ng_send_de(void);      // で
void ng_send_do(void);      // ど
void ng_send_ba(void);      // ば
void ng_send_bi(void);      // び
void ng_send_bu(void);      // ぶ
void ng_send_be(void);      // べ
void ng_send_bo(void);      // ぼ
void ng_send_vu(void);      // ヴ
void ng_send_vu(void);      // ヴ

// 半濁音
void ng_send_pa(void);      // ぱ
void ng_send_pi(void);      // ぴ
void ng_send_pu(void);      // ぷ
void ng_send_pe(void);      // ぺ
void ng_send_po(void);      // ぽ

// 小書き
void ng_send_xya(void);     // ゃ
void ng_send_xyu(void);     // ゅ
void ng_send_xyo(void);     // ょ
void ng_send_xa(void);      // ぁ
void ng_send_xi(void);      // ぃ
void ng_send_xu(void);      // ぅ
void ng_send_xe(void);      // ぇ
void ng_send_xo(void);      // ぉ
void ng_send_xwa(void);     // ゎ
void ng_send_xtu(void);     // っ
void ng_send_xke(void);     // ヶ
void ng_send_xka(void);     // ヵ

// 清音拗音 濁音拗音 半濁拗音
void ng_send_sya(void);     // しゃ
void ng_send_syu(void);     // しゅ
void ng_send_syo(void);     // しょ
void ng_send_ja(void);      // じゃ
void ng_send_ju(void);      // じゅ
void ng_send_jo(void);      // じょ
void ng_send_kya(void);     // きゃ
void ng_send_kyu(void);     // きゅ
void ng_send_kyo(void);     // きょ
void ng_send_gya(void);     // ぎゃ
void ng_send_gyu(void);     // ぎゅ
void ng_send_gyo(void);     // ぎょ
void ng_send_tya(void);     // ちゃ
void ng_send_tyu(void);     // ちゅ
void ng_send_tyo(void);     // ちょ
void ng_send_dya(void);     // ぢゃ
void ng_send_dyu(void);     // ぢゅ
void ng_send_dyo(void);     // ぢょ
void ng_send_nya(void);     // にゃ
void ng_send_nyu(void);     // にゅ
void ng_send_nyo(void);     // にょ
void ng_send_hya(void);     // ひゃ
void ng_send_hyu(void);     // ひゅ
void ng_send_hyo(void);     // ひょ
void ng_send_bya(void);     // びゃ
void ng_send_byu(void);     // びゅ
void ng_send_byo(void);     // びょ
void ng_send_pya(void);     // ぴゃ
void ng_send_pyu(void);     // ぴゅ
void ng_send_pyo(void);     // ぴょ
void ng_send_mya(void);     // みゃ
void ng_send_myu(void);     // みゅ
void ng_send_myo(void);     // みょ
void ng_send_rya(void);     // りゃ
void ng_send_ryu(void);     // りゅ
void ng_send_ryo(void);     // りょ

// 清音外来音 濁音外来音
void ng_send_thi(void);     // てぃ
void ng_send_thu(void);     // てゅ
void ng_send_dhi(void);     // でぃ
void ng_send_dhu(void);     // でゅ
void ng_send_twu(void);     // とぅ
void ng_send_dwu(void);     // どぅ
void ng_send_sye(void);     // しぇ
void ng_send_tye(void);     // ちぇ
void ng_send_je(void);      // じぇ
void ng_send_dye(void);     // ぢぇ
void ng_send_fa(void);      // ふぁ
void ng_send_fi(void);      // ふぃ
void ng_send_fe(void);      // ふぇ
void ng_send_fo(void);      // ふぉ
void ng_send_fyu(void);     // ふゅ
void ng_send_ye(void);      // いぇ
void ng_send_wi(void);      // うぃ
void ng_send_we(void);      // うぇ
void ng_send_who(void);     // うぉ
void ng_send_va(void);      // ヴぁ
void ng_send_vi(void);      // ヴぃ
void ng_send_ve(void);      // ヴぇ
void ng_send_vo(void);      // ヴぉ
void ng_send_vyu(void);     // ヴゅ
void ng_send_qa(void);      // くぁ
void ng_send_qi(void);      // くぃ
void ng_send_qe(void);      // くぇ
void ng_send_qo(void);      // くぉ
void ng_send_kuxwa(void);   // くゎ
void ng_send_gwa(void);     // ぐぁ
void ng_send_gwi(void);     // ぐぃ
void ng_send_gwe(void);     // ぐぇ
void ng_send_gwo(void);     // ぐぉ
void ng_send_guxwa(void);   // ぐゎ
void ng_send_tsa(void);     // つぁ

// 追加
void ng_edit_touten(void);  // 、
void ng_edit_kuten(void);   // 。
void ng_enter(void);        // {Enter}

// 編集モード
void ng_edit_tenten(void); // ……{改行}
void ng_symbol_yokobou(void); // ――{改行}
void ng_symbol_question(void); // ？{改行}
void ng_symbol_exclaim(void); // ！{改行}
void ng_symbol_chuuten(void); // ・
void ng_symbol_slash(void); // ／{改行}
void ng_symbol_maru(void); // 〇{改行}

void ng_edit_1_up(void); // {↑}
void ng_edit_s1_up(void); // +{↑}
void ng_edit_s7_up(void); // +{↑ 7}
void ng_edit_1_down(void); // {↓}
void ng_edit_s1_down(void); // +{↓}
void ng_edit_s7_down(void); // +{↓ 7}
void ng_edit_1_right(void); // {→}
void ng_edit_s1_right(void); // +{→}
void ng_edit_s5_right(void); // +{→ 5}
void ng_edit_s20_right(void); // +{→ 20}
void ng_edit_1_left(void); // {←}
void ng_edit_s1_left(void); // +{←}
void ng_edit_s5_left(void); // +{← 5}
void ng_edit_s20_left(void); // +{← 20}
void ng_edit_s_home(void); // +{Home}
void ng_edit_s_end(void); // +{End}

void ng_edit_delete_to_end(void); // +{End}{BS}

void ng_edit_nijuu_yama_gakko(void); // 『』{改行}{↑}
void ng_edit_maru_kakko(void); // (void){改行}{↑}
void ng_edit_sumituki_kakko(void); // 【】{改行}{↑}
void ng_edit_kagi_kakko(void); // 「」{改行}{↑}
void ng_edit_nijuu_yama_kakko(void); // 《》{改行}{↑}
void ng_edit_surround_nijuu_yama_gakko(void); // ^x『^v』{改行}{Space}+{↑}^x
void ng_edit_surround_maru_kakko(void); // ^x(^v){改行}{Space}+{↑}^x
void ng_edit_surround_sumituki_kakko(void); // ^x【^v】{改行}{Space}+{↑}^x
void ng_edit_surround_kagi_kakko(void); // ^x「^v」{改行}{Space}+{↑}^x
void ng_edit_surround_ruby(void); // ^x｜{改行}^v《》{改行}{↑}{Space}+{↑}^x

void ng_edit_kakutei_down(void); // {改行}{↓}
void ng_edit_kakutei_left(void); // {改行}{←}
void ng_edit_kakutei_end(void); // {Enter}{End}
void ng_edit_next_line_kagi_kakko(void); // {改行}{End}{改行}「」{改行}{↑}
void ng_edit_next_line_space(void); // {改行}{End}{改行}{Space}

void ng_edit_separate_line(void); // 　　　×　　　×　　　×{改行 2}
void ng_edit_3_space(void); // {Space 3}
void ng_edit_togaki(void); // {Home}{改行}{Space 3}{←}
void ng_edit_serifu(void); // {Home}{改行}{Space 1}{←}
void ng_edit_togaki_zengyo(void); // {Home}{→}{End}{Del 4}{←}
void ng_edit_serifu_zengyo(void); // {Home}{→}{End}{Del 2}{←}
