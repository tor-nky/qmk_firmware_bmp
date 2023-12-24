/* Copyright Satoru NAKAYA <@tor-nky>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY { without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "naginata.h"
#include "naginata_parts.h"

void ng_send_a(void) {	// あ
	tap_code(KC_A);
}
void ng_send_b(void) {
	tap_code(KC_B);
}
void ng_send_c(void) {
	tap_code(KC_C);
}
void ng_send_d(void) {
	tap_code(KC_D);
}
void ng_send_e(void) {	// え
	tap_code(KC_E);
}
void ng_send_f(void) {
	tap_code(KC_F);
}
void ng_send_g(void) {
	tap_code(KC_G);
}
void ng_send_h(void) {
	tap_code(KC_H);
}
void ng_send_i(void) {	// い
	tap_code(KC_I);
}
void ng_send_j(void) {
	tap_code(KC_J);
}
void ng_send_k(void) {
	tap_code(KC_K);
}
void ng_send_l(void) {
	tap_code(KC_L);
}
void ng_send_m(void) {
	tap_code(KC_M);
}
void ng_send_n(void) {
	tap_code(KC_N);
}
void ng_send_o(void) {	// お
	tap_code(KC_O);
}
void ng_send_p(void) {
	tap_code(KC_P);
}
void ng_send_q(void) {
	tap_code(KC_Q);
}
void ng_send_r(void) {
	tap_code(KC_R);
}
void ng_send_s(void) {
	tap_code(KC_S);
}
void ng_send_t(void) {
	tap_code(KC_T);
}
void ng_send_u(void) {	// う
	tap_code(KC_U);
}
void ng_send_v(void) {
	tap_code(KC_V);
}
void ng_send_w(void) {
	tap_code(KC_W);
}
void ng_send_x(void) {
	tap_code(KC_X);
}
void ng_send_y(void) {
	tap_code(KC_Y);
}
void ng_send_z(void) {
	tap_code(KC_Z);
}

// 清音
void ng_send_ka(void) {	// か
	ng_send_k();
	ng_send_a();
}
void ng_send_ki(void) {	// き
	ng_send_k();
	ng_send_i();
}
void ng_send_ku(void) {	// く
	ng_send_k();
	ng_send_u();
}
void ng_send_ke(void) {	// け
	ng_send_k();
	ng_send_e();
}
void ng_send_ko(void) {	// こ
	ng_send_k();
	ng_send_o();
}
void ng_send_sa(void) {	// さ
	ng_send_s();
	ng_send_a();
}
void ng_send_si(void) {	// し
	ng_send_s();
	ng_send_i();
}
void ng_send_su(void) {	// す
	ng_send_s();
	ng_send_u();
}
void ng_send_se(void) {	// せ
	ng_send_s();
	ng_send_e();
}
void ng_send_so(void) {	// そ
	ng_send_s();
	ng_send_o();
}
void ng_send_ta(void) {	// た
	ng_send_t();
	ng_send_a();
}
void ng_send_ti(void) {	// ち
	ng_send_t();
	ng_send_i();
}
void ng_send_tu(void) {	// つ
	ng_send_t();
	ng_send_u();
}
void ng_send_te(void) {	// て
	ng_send_t();
	ng_send_e();
}
void ng_send_to(void) {	// と
	ng_send_t();
	ng_send_o();
}
void ng_send_na(void) {	// な
	ng_send_n();
	ng_send_a();
}
void ng_send_ni(void) {	// に
	ng_send_n();
	ng_send_i();
}
void ng_send_nu(void) {	// ぬ
	ng_send_n();
	ng_send_u();
}
void ng_send_ne(void) {	// ね
	ng_send_n();
	ng_send_e();
}
void ng_send_no(void) {	// の
	ng_send_n();
	ng_send_o();
}
void ng_send_ha(void) {	// は
	ng_send_h();
	ng_send_a();
}
void ng_send_hi(void) {	// ひ
	ng_send_h();
	ng_send_i();
}
void ng_send_hu(void) {	// ふ
	ng_send_h();
	ng_send_u();
}
void ng_send_he(void) {	// へ
	ng_send_h();
	ng_send_e();
}
void ng_send_ho(void) {	// ほ
	ng_send_h();
	ng_send_o();
}
void ng_send_ma(void) {	// ま
	ng_send_m();
	ng_send_a();
}
void ng_send_mi(void) {	// み
	ng_send_m();
	ng_send_i();
}
void ng_send_mu(void) {	// む
	ng_send_m();
	ng_send_u();
}
void ng_send_me(void) {	// め
	ng_send_m();
	ng_send_e();
}
void ng_send_mo(void) {	// も
	ng_send_m();
	ng_send_o();
}
void ng_send_ya(void) {	// や
	ng_send_y();
	ng_send_a();
}
void ng_send_yu(void) {	// ゆ
	ng_send_y();
	ng_send_u();
}
void ng_send_yo(void) {	// よ
	ng_send_y();
	ng_send_o();
}
void ng_send_ra(void) {	// ら
	ng_send_r();
	ng_send_a();
}
void ng_send_ri(void) {	// り
	ng_send_r();
	ng_send_i();
}
void ng_send_ru(void) {	// る
	ng_send_r();
	ng_send_u();
}
void ng_send_re(void) {	// れ
	ng_send_r();
	ng_send_e();
}
void ng_send_ro(void) {	// ろ
	ng_send_r();
	ng_send_o();
}
void ng_send_wa(void) {	// わ
	ng_send_w();
	ng_send_a();
}
void ng_send_wo(void) {	// を
	ng_send_w();
	ng_send_o();
}
void ng_send_nn(void) {	// ん
	ng_send_n();
	ng_send_n();
}
void ng_send_minus(void) {	// ー
	tap_code(KC_MINUS);
}

// 濁音
void ng_send_ga(void) {	// が
	ng_send_g();
	ng_send_a();
}
void ng_send_gi(void) {	// ぎ
	ng_send_g();
	ng_send_i();
}
void ng_send_gu(void) {	// ぐ
	ng_send_g();
	ng_send_u();
}
void ng_send_ge(void) {	// げ
	ng_send_g();
	ng_send_e();
}
void ng_send_go(void) {	// ご
	ng_send_g();
	ng_send_o();
}
void ng_send_za(void) {	// ざ
	ng_send_z();
	ng_send_a();
}
void ng_send_zi(void) {	// じ
	ng_send_z();
	ng_send_i();
}
void ng_send_zu(void) {	// ず
	ng_send_z();
	ng_send_u();
}
void ng_send_ze(void) {	// ぜ
	ng_send_z();
	ng_send_e();
}
void ng_send_zo(void) {	// ぞ
	ng_send_z();
	ng_send_o();
}
void ng_send_da(void) {	// だ
	ng_send_d();
	ng_send_a();
}
void ng_send_di(void) {	// ぢ
	ng_send_d();
	ng_send_i();
}
void ng_send_du(void) {	// づ
	ng_send_d();
	ng_send_u();
}
void ng_send_de(void) {	// で
	ng_send_d();
	ng_send_e();
}
void ng_send_do(void) {	// ど
	ng_send_d();
	ng_send_o();
}
void ng_send_ba(void) {	// ば
	ng_send_b();
	ng_send_a();
}
void ng_send_bi(void) {	// び
	ng_send_b();
	ng_send_i();
}
void ng_send_bu(void) {	// ぶ
	ng_send_b();
	ng_send_u();
}
void ng_send_be(void) {	// べ
	ng_send_b();
	ng_send_e();
}
void ng_send_bo(void) {	// ぼ
	ng_send_b();
	ng_send_o();
}
void ng_send_vu(void) {	// ヴ
	ng_send_v();
	ng_send_u();
}

// 半濁音
void ng_send_pa(void) {	// ぱ
	ng_send_p();
	ng_send_a();
}
void ng_send_pi(void) {	// ぴ
	ng_send_p();
	ng_send_i();
}
void ng_send_pu(void) {	// ぷ
	ng_send_p();
	ng_send_u();
}
void ng_send_pe(void) {	// ぺ
	ng_send_p();
	ng_send_e();
}
void ng_send_po(void) {	// ぽ
	ng_send_p();
	ng_send_o();
}

// 小書き
void ng_send_xya(void) {	// ゃ
	ng_send_x();
	ng_send_ya();
}
void ng_send_xyu(void) {	// ゅ
	ng_send_x();
	ng_send_yu();
}
void ng_send_xyo(void) {	// ょ
	ng_send_x();
	ng_send_yo();
}
void ng_send_xa(void) {		// ぁ
	ng_send_x();
	ng_send_a();
}
void ng_send_xi(void) {		// ぃ
	ng_send_x();
	ng_send_i();
}
void ng_send_xu(void) {		// ぅ
	ng_send_x();
	ng_send_u();
}
void ng_send_xe(void) {		// ぇ
	ng_send_x();
	ng_send_e();
}
void ng_send_xo(void) {		// ぉ
	ng_send_x();
	ng_send_o();
}
void ng_send_xwa(void) {	// ゎ
	ng_send_x();
	ng_send_wa();
}
void ng_send_xtu(void) {	// っ
	ng_send_x();
	ng_send_tu();
}
void ng_send_xke(void) {	// ヶ
	ng_send_x();
	ng_send_ke();
}
void ng_send_xka(void) {	// ヵ
	ng_send_x();
	ng_send_ka();
}

// 清音拗音 濁音拗音 半濁拗音
void ng_send_sya(void) {	// しゃ
	ng_send_s();
	ng_send_ya();
}
void ng_send_syu(void) {	// しゅ
	ng_send_s();
	ng_send_yu();
}
void ng_send_syo(void) {	// しょ
	ng_send_s();
	ng_send_yo();
}
void ng_send_ja(void) {		// じゃ
	ng_send_j();
	ng_send_a();
}
void ng_send_ju(void) {		// じゅ
	ng_send_j();
	ng_send_u();
}
void ng_send_jo(void) {		// じょ
	ng_send_j();
	ng_send_o();
}
void ng_send_kya(void) {	// きゃ
	ng_send_k();
	ng_send_ya();
}
void ng_send_kyu(void) {	// きゅ
	ng_send_k();
	ng_send_yu();
}
void ng_send_kyo(void) {	// きょ
	ng_send_k();
	ng_send_yo();
}
void ng_send_gya(void) {	// ぎゃ
	ng_send_g();
	ng_send_ya();
}
void ng_send_gyu(void) {	// ぎゅ
	ng_send_g();
	ng_send_yu();
}
void ng_send_gyo(void) {	// ぎょ
	ng_send_g();
	ng_send_yo();
}
void ng_send_tya(void) {	// ちゃ
	ng_send_t();
	ng_send_ya();
}
void ng_send_tyu(void) {	// ちゅ
	ng_send_t();
	ng_send_yu();
}
void ng_send_tyo(void) {	// ちょ
	ng_send_t();
	ng_send_yo();
}
void ng_send_dya(void) {	// ぢゃ
	ng_send_d();
	ng_send_ya();
}
void ng_send_dyu(void) {	// ぢゅ
	ng_send_d();
	ng_send_yu();
}
void ng_send_dyo(void) {	// ぢょ
	ng_send_d();
	ng_send_yo();
}
void ng_send_nya(void) {	// にゃ
	ng_send_n();
	ng_send_ya();
}
void ng_send_nyu(void) {	// にゅ
	ng_send_n();
	ng_send_yu();
}
void ng_send_nyo(void) {	// にょ
	ng_send_n();
	ng_send_yo();
}
void ng_send_hya(void) {	// ひゃ
	ng_send_h();
	ng_send_ya();
}
void ng_send_hyu(void) {	// ひゅ
	ng_send_h();
	ng_send_yu();
}
void ng_send_hyo(void) {	// ひょ
	ng_send_h();
	ng_send_yo();
}
void ng_send_bya(void) {	// びゃ
	ng_send_b();
	ng_send_ya();
}
void ng_send_byu(void) {	// びゅ
	ng_send_b();
	ng_send_yu();
}
void ng_send_byo(void) {	// びょ
	ng_send_b();
	ng_send_yo();
}
void ng_send_pya(void) {	// ぴゃ
	ng_send_p();
	ng_send_ya();
}
void ng_send_pyu(void) {	// ぴゅ
	ng_send_p();
	ng_send_yu();
}
void ng_send_pyo(void) {	// ぴょ
	ng_send_p();
	ng_send_yo();
}
void ng_send_mya(void) {	// みゃ
	ng_send_m();
	ng_send_ya();
}
void ng_send_myu(void) {	// みゅ
	ng_send_m();
	ng_send_yu();
}
void ng_send_myo(void) {	// みょ
	ng_send_m();
	ng_send_yo();
}
void ng_send_rya(void) {	// りゃ
	ng_send_r();
	ng_send_ya();
}
void ng_send_ryu(void) {	// りゅ
	ng_send_r();
	ng_send_yu();
}
void ng_send_ryo(void) {	// りょ
	ng_send_r();
	ng_send_yo();
}

// 清音外来音 濁音外来音
void ng_send_thi(void) {	// てぃ
	ng_send_t();
	ng_send_hi();
}
void ng_send_thu(void) {	// てゅ
	ng_send_t();
	ng_send_hu();
}
void ng_send_dhi(void) {	// でぃ
	ng_send_d();
	ng_send_hi();
}
void ng_send_dhu(void) {	// でゅ
	ng_send_d();
	ng_send_hu();
}
void ng_send_twu(void) {	// とぅ
	ng_send_t();
	ng_send_w();
	ng_send_u();
}
void ng_send_dwu(void) {	// どぅ
	ng_send_d();
	ng_send_w();
	ng_send_u();
}
void ng_send_sye(void) {	// しぇ
	ng_send_s();
	ng_send_ye();
}
void ng_send_tye(void) {	// ちぇ
	ng_send_t();
	ng_send_ye();
}
void ng_send_je(void) {		// じぇ
	ng_send_j();
	ng_send_e();
}
void ng_send_dye(void) {	// ぢぇ
	ng_send_d();
	ng_send_ye();
}
void ng_send_fa(void) {		// ふぁ
	ng_send_f();
	ng_send_a();
}
void ng_send_fi(void) {		// ふぃ
	ng_send_f();
	ng_send_i();
}
void ng_send_fe(void) {		// ふぇ
	ng_send_f();
	ng_send_e();
}
void ng_send_fo(void) {		// ふぉ
	ng_send_f();
	ng_send_o();
}
void ng_send_fyu(void) {	// ふゅ
	ng_send_f();
	ng_send_yu();
}
void ng_send_ye(void) {		// いぇ
	ng_send_y();
	ng_send_e();
}
void ng_send_wi(void) {		// うぃ
	ng_send_w();
	ng_send_i();
}
void ng_send_we(void) {		// うぇ
	ng_send_w();
	ng_send_e();
}
void ng_send_who(void) {	// うぉ
	ng_send_w();
	ng_send_ho();
}
void ng_send_va(void) {		// ヴぁ
	ng_send_v();
	ng_send_a();
}
void ng_send_vi(void) {		// ヴぃ
	ng_send_v();
	ng_send_i();
}
void ng_send_ve(void) {		// ヴぇ
	ng_send_v();
	ng_send_e();
}
void ng_send_vo(void) {		// ヴぉ
	ng_send_v();
	ng_send_o();
}
void ng_send_vyu(void) {	// ヴゅ
	ng_send_v();
	ng_send_yu();
}
void ng_send_qa(void) {		// くぁ
	ng_send_q();
	ng_send_a();
}
void ng_send_qi(void) {		// くぃ
	ng_send_q();
	ng_send_i();
}
void ng_send_qe(void) {		// くぇ
	ng_send_q();
	ng_send_e();
}
void ng_send_qo(void) {		// くぉ
	ng_send_q();
	ng_send_o();
}
void ng_send_kuxwa(void) {	// くゎ
	ng_send_ku();
	ng_send_xwa();
}
void ng_send_gwa(void) {	// ぐぁ
	ng_send_g();
	ng_send_wa();
}
void ng_send_gwi(void) {	// ぐぃ
	ng_send_g();
	ng_send_wi();
}
void ng_send_gwe(void) {	// ぐぇ
	ng_send_g();
	ng_send_we();
}
void ng_send_gwo(void) {	// ぐぉ
	ng_send_g();
	ng_send_wo();
}
void ng_send_guxwa(void) {	// ぐゎ
	ng_send_gu();
	ng_send_xwa();
}
void ng_send_tsa(void) {	// つぁ
	ng_send_t();
	ng_send_sa();
}

// 追加
void ng_edit_touten(void) { // 、
	tap_code(KC_COMMA);
	#ifdef NG_BMP
	switch (naginata_config.os) {
    case NG_MAC_DIC:
    case NG_IOS:
        ng_ime_complete();
        break;
    default:
	#endif
        tap_code(KC_ENT);
	#ifdef NG_BMP
		break;
	}
	#endif
}
void ng_edit_kuten(void) { // 。
	tap_code(KC_DOT);
	#ifdef NG_BMP
	switch (naginata_config.os) {
    case NG_MAC_DIC:
    case NG_IOS:
        ng_ime_complete();
        break;
    default:
	#endif
        tap_code(KC_ENT);
	#ifdef NG_BMP
        break;
	}
	#endif
}
void ng_enter(void) { // {Enter}
	tap_code(KC_ENTER);
}

// 編集モード
void ng_edit_tenten(void) { // ……{改行}
	#ifdef NG_BMP
	switch (naginata_config.os) {
    case NG_MAC_DIC:
    case NG_IOS:
        ng_ime_complete();
        ios_send_string("nagitete"); // "……"
        break;
    default:
	#endif
        ng_send_unicode_string_P(PSTR("……"));
	#ifdef NG_BMP
        break;
	}
	#endif
}
void ng_symbol_yokobou(void) { // ――{改行}
	#ifdef NG_BMP
	switch (naginata_config.os) {
    case NG_MAC_DIC:
    case NG_IOS:
        ng_ime_complete();
        ios_send_string("nagiyose"); // "──"
        break;
    default:
	#endif
        ng_send_unicode_string_P(PSTR("──"));
	#ifdef NG_BMP
        break;
	}
	#endif
}
void ng_symbol_question(void) { // ？{改行}
	tap_code16(LSFT(KC_SLSH));
	ng_ime_complete();
}
void ng_symbol_exclaim(void) { // ！{改行}
	tap_code16(LSFT(KC_1));
	ng_ime_complete();
}
void ng_symbol_chuuten(void) { // ・
	tap_code(KC_SLASH);
}
void ng_symbol_slash(void) { // ／{改行}
	#ifdef NG_BMP
	switch (naginata_config.os) {
    case NG_MAC_DIC:
    case NG_IOS:
        ng_ime_complete();
        ios_send_string("naginame"); // "／"
        break;
    default:
	#endif
        ng_send_unicode_string_P(PSTR("／"));
	#ifdef NG_BMP
        break;
	}
	#endif
}
void ng_symbol_maru(void) { // 〇{改行}
	#ifdef NG_BMP
	switch (naginata_config.os) {
    case NG_MAC_DIC:
    case NG_IOS:
        ng_ime_complete();
        ios_send_string("nagimaru"); // "〇"
        break;
    default:
	#endif
        ng_send_unicode_string_P(PSTR("〇"));
	#ifdef NG_BMP
        break;
	}
	#endif
}
void ng_edit_1_up(void) { // {↑}
	ng_up(1);
}
void ng_edit_s1_up(void) { // +{↑}
	register_code(KC_LSFT);
	ng_up(1);
}
void ng_edit_s7_up(void) { // +{↑ 7}
	register_code(KC_LSFT);
	ng_up(7);
}
void ng_edit_1_down(void) { // {↓}
	ng_down(1);
}
void ng_edit_s1_down(void) { // +{↓}
	register_code(KC_LSFT);
	ng_down(1);
}
void ng_edit_s7_down(void) { // +{↓ 7}
	register_code(KC_LSFT);
	ng_down(7);
}
void ng_edit_1_right(void) { // {→}
	ng_right(1);
}
void ng_edit_s1_right(void) { // +{→}
	register_code(KC_LSFT);
	ng_right(1);
}
void ng_edit_s5_right(void) { // +{→ 5}
	register_code(KC_LSFT);
	ng_right(5);
}
void ng_edit_s20_right(void) { // +{→ 20}
	register_code(KC_LSFT);
	ng_right(20);
}
void ng_edit_1_left(void) { // {←}
	ng_left(1);
}
void ng_edit_s1_left(void) { // +{←}
	register_code(KC_LSFT);
	ng_left(1);
}
void ng_edit_s5_left(void) { // +{← 5}
	register_code(KC_LSFT);
	ng_left(5);
}
void ng_edit_s20_left(void) { // +{← 20}
    register_code(KC_LSFT);
    ng_left(20);
}
void ng_edit_s_home(void) { // +{Home}
	register_code(KC_LSFT);
	ng_home();
	unregister_code(KC_LSFT);
}
void ng_edit_s_end(void) { // +{End}
	register_code(KC_LSFT);
	ng_end();
    unregister_code(KC_LSFT);
}
void ng_edit_delete_to_end(void) { // +{End}{BS}
	register_code(KC_LSFT);
	ng_end();
    unregister_code(KC_LSFT);
	tap_code(KC_BSPC);
}
void ng_edit_nijuu_yama_gakko(void) { // 『』{改行}{↑}
	#ifdef NG_BMP
	switch (naginata_config.os) {
    case NG_MAC_DIC:
	case NG_IOS:
		ng_ime_complete();
		ios_send_string("naginika"); // "『』"
		break;
	default:
	#endif
		ng_send_unicode_string_P(PSTR("『』"));
	#ifdef NG_BMP
		break;
	}
	#endif
	ng_up(1);
}
void ng_edit_maru_kakko(void) { // (){改行}{↑}
	#ifdef NG_BMP
	switch (naginata_config.os) {
    case NG_MAC_DIC:
	case NG_IOS:
		ng_ime_complete();
		ios_send_string("nagimaka"); // "（）"
		break;
	default:
	#endif
		ng_send_unicode_string_P(PSTR("（）"));
	#ifdef NG_BMP
		break;
	}
	#endif
	ng_up(1);
}
void ng_edit_sumituki_kakko(void) { // 【】{改行}{↑}
	#ifdef NG_BMP
	switch (naginata_config.os) {
    case NG_MAC_DIC:
	case NG_IOS:
		ng_ime_complete();
		ios_send_string("nagisuka"); // "【】"
		break;
	default:
	#endif
		ng_send_unicode_string_P(PSTR("【】"));
	#ifdef NG_BMP
		break;
	}
	#endif
	ng_up(1);
}
void ng_edit_kagi_kakko(void) { // 「」{改行}{↑}
	#ifdef NG_BMP
	switch (naginata_config.os) {
    case NG_MAC_DIC:
	case NG_IOS:
		ng_ime_complete();
		ios_send_string("nagikagi"); // "「」"
		break;
	default:
	#endif
		ng_send_unicode_string_P(PSTR("「」"));
	#ifdef NG_BMP
		break;
	}
	#endif
	ng_up(1);
}
void ng_edit_nijuu_yama_kakko(void) { // 《》{改行}{↑}
	#ifdef NG_BMP
	switch (naginata_config.os) {
    case NG_MAC_DIC:
	case NG_IOS:
		ng_ime_complete();
		ios_send_string("naginiya"); // "《》"
		break;
	default:
	#endif
		ng_send_unicode_string_P(PSTR("《》"));
	#ifdef NG_BMP
		break;
	}
	#endif
	ng_up(1);
}
void ng_edit_surround_nijuu_yama_gakko(void) { // ^x『^v』{改行}{Space}+{↑}^x
	#ifdef NG_BMP
	switch (naginata_config.os) {
    case NG_MAC_DIC:
	case NG_IOS:
		ios_send_string_with_cut_paste("naginika"); // "『』"
		break;
	default:
	#endif
		ng_cut();
		ng_send_unicode_string_P(PSTR("『"));
		ng_paste();
		ng_send_unicode_string_P(PSTR("』"));
		tap_code(KC_SPC);
		register_code(KC_LSFT);
		ng_up(1);
		unregister_code(KC_LSFT);
		ng_cut();
	#ifdef NG_BMP
		break;
	}
	#endif
}
void ng_edit_surround_maru_kakko(void) { // ^x(^v){改行}{Space}+{↑}^x
	#ifdef NG_BMP
	switch (naginata_config.os) {
    case NG_MAC_DIC:
	case NG_IOS:
		ios_send_string_with_cut_paste("nagimaka"); // "（）"
		break;
	default:
	#endif
		ng_cut();
		ng_send_unicode_string_P(PSTR("（"));
		ng_paste();
		ng_send_unicode_string_P(PSTR("）"));
		tap_code(KC_SPC);
		register_code(KC_LSFT);
		ng_up(1);
		unregister_code(KC_LSFT);
		ng_cut();
	#ifdef NG_BMP
		break;
	}
	#endif
}
void ng_edit_surround_sumituki_kakko(void) { // ^x【^v】{改行}{Space}+{↑}^x
	#ifdef NG_BMP
	switch (naginata_config.os) {
    case NG_MAC_DIC:
	case NG_IOS:
		ios_send_string_with_cut_paste("nagisuka"); // "【】"
		break;
	default:
	#endif
		ng_cut();
		ng_send_unicode_string_P(PSTR("【"));
		ng_paste();
		ng_send_unicode_string_P(PSTR("】"));
		tap_code(KC_SPC);
		register_code(KC_LSFT);
		ng_up(1);
		unregister_code(KC_LSFT);
		ng_cut();
	#ifdef NG_BMP
		break;
	}
	#endif
}
void ng_edit_surround_kagi_kakko(void) { // ^x「^v」{改行}{Space}+{↑}^x
	#ifdef NG_BMP
	switch (naginata_config.os) {
    case NG_MAC_DIC:
	case NG_IOS:
		ios_send_string_with_cut_paste("nagikagi"); // "「」"
		break;
	default:
	#endif
		ng_cut();
		ng_send_unicode_string_P(PSTR("「"));
		ng_paste();
		ng_send_unicode_string_P(PSTR("」"));
		tap_code(KC_SPC);
		register_code(KC_LSFT);
		ng_up(1);
		unregister_code(KC_LSFT);
		ng_cut();
	#ifdef NG_BMP
		break;
	}
	#endif
}
void ng_edit_surround_ruby(void) { // ^x｜{改行}^v《》{改行}{↑}{Space}+{↑}^x
	#ifdef NG_BMP
	switch (naginata_config.os) {
    case NG_MAC_DIC:
	case NG_IOS:
		ng_cut();
		ios_send_string("nagiru"); // "｜《》"
		ng_up(2);	// 2文字戻る
		tap_code(KC_LCTRL); tap_code(KC_LSFT); // ディレイの代わり
		ng_paste();
		tap_code(KC_LCTRL); // ディレイの代わり
		ng_down(1);	// 1文字進む
		break;
	default:
	#endif
		ng_cut();
		ng_send_unicode_string_P(PSTR("｜"));
		ng_paste();
		tap_code(KC_SPC);
		register_code(KC_LSFT);
		ng_up(1);
		unregister_code(KC_LSFT);
		ng_cut();
		ng_send_unicode_string_P(PSTR("《》"));
		ng_up(1);
	#ifdef NG_BMP
		break;
	}
	#endif
}
void ng_edit_kakutei_down(void) { // {改行}{↓}
	ng_ime_complete();
	ng_down(1);
}
void ng_edit_kakutei_left(void) { // {改行}{←}
	ng_ime_complete();
	ng_left(1);
}
void ng_edit_kakutei_end(void) { // {Enter}{End}
	ng_ime_complete();
	ng_end();
}
void ng_edit_next_line_kagi_kakko(void) { // {改行}{End}{改行}「」{改行}{↑}
	ng_ime_complete();
	ng_end();
	tap_code(KC_ENT);
	#ifdef NG_BMP
	switch (naginata_config.os) {
    case NG_MAC_DIC:
	case NG_IOS:
		ios_send_string("nagikagi"); // "「」"
		break;
	default:
	#endif
		ng_send_unicode_string_P(PSTR("「」"));
	#ifdef NG_BMP
		break;
	}
	#endif
	ng_up(1);
}
void ng_edit_next_line_space(void) { // {改行}{End}{改行}{Space}
	ng_ime_complete();
	ng_end();
	tap_code(KC_ENT);
	tap_code(KC_SPC);
}
void ng_edit_separate_line(void) { // 　　　×　　　×　　　×{改行 2}
	#ifdef NG_BMP
	switch (naginata_config.os) {
    case NG_MAC_DIC:
	case NG_IOS:
		tap_code(KC_SPACE);
		tap_code(KC_SPACE);
		tap_code(KC_SPACE);
		ios_send_string("nagibatu"); // "　　　×　　　×　　　×"
		break;
	default:
	#endif
		ng_send_unicode_string_P(PSTR("　　　×　　　×　　　×"));
	#ifdef NG_BMP
		break;
	}
	#endif
	tap_code(KC_ENT);
}
void ng_edit_3_space(void) { // {Space 3}
	tap_code(KC_SPC);
	tap_code(KC_SPC);
	tap_code(KC_SPC);
}
void ng_edit_togaki(void) { // {Home}{改行}{Space 3}{←}
	ng_home();
	tap_code(KC_ENT);
	tap_code(KC_SPC);
	tap_code(KC_SPC);
	tap_code(KC_SPC);
	ng_left(1);
}
void ng_edit_serifu(void) { // {Home}{改行}{Space 1}{←}
	ng_home();
	tap_code(KC_ENT);
	tap_code(KC_SPC);
	ng_left(1);
}
void ng_edit_togaki_zengyo(void) { // {Home}{→}{End}{Del 4}{←}
	ng_home();
	ng_right(1);
	ng_end();
	tap_code(KC_DEL);
	tap_code(KC_DEL);
	tap_code(KC_DEL);
	tap_code(KC_DEL);
	ng_left(1);
}
void ng_edit_serifu_zengyo(void) { // {Home}{→}{End}{Del 2}{←}
	ng_home();
	ng_right(1);
	ng_end();
	tap_code(KC_DEL);
	tap_code(KC_DEL);
	ng_left(1);
}
