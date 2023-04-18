tanda = <<ETANDA
ヴ|き|て|し|{←}|{→}|{BS}|る|す|へ|@|[  |
ろ|け|と|か|っ  |く  |あ  |い|う|ー|:|]  |
ほ|ひ|は|こ|そ  |た  |な  |ん|ら|れ|\\|
ETANDA

shifted = <<ESHIFTED
 ヴ|ぬ|り |ね       |+{←}|+{→}|さ       |よ|え|ゆ|`|{{}|
 せ|め|に |ま       |ち   |や   |の       |も|つ|ふ|*|{}}|
 ほ|ひ|を |、{Enter}|み   |お   |。{Enter}|む|わ|れ|_|
ESHIFTED

mode1l = <<MEND
^{End}    |《》{改行}{↑}|/*ディ*/|^s            |・            ||||||||
……{改行}|(){改行}{↑}  |？{改行}|「」{改行}{↑}|『』{改行}{↑}||||||||
││{改行}|【】{改行}{↑}|！{改行}|{改行}{↓}    |／{改行}      |||||||
MEND

mode1r = <<MEND
|||||{Home}      |+{End}{BS}|{vk1Csc079}|{Del}  |{Esc 3}|  |  |
|||||{Enter}{End}|{↑}      |+{↑}      |+{↑ 7}|^i     |  |  |
|||||{End}       |{↓}      |+{↓}      |+{↓ 7}|^u     |  |
MEND

mode2l = <<MEND
{Home}{Del 3}{BS}{←}           |^x｜{改行}^v《》{改行}{↑}  |{Home}{改行}{Space 3}{←}|{Space 3}                      |〇{改行}      ||||||||
{Home}{Del 1}{BS}{←}           |^x(^v){改行}{Space}+{↑}^x  |{Home}{改行}{Space 1}{←}|^x「^v」{改行}{Space}+{↑}^x   |^x『^v』{改行}{Space}+{↑}^x||||||||
　　　×　　　×　　　×{改行 2}|^x【^v】{改行}{Space}+{↑}^x|{改行}{End}{改行}}       |{改行}{End}{改行}「」{改行}{↑}|{End}{改行}   |||||||
MEND

mode2r = <<MEND
|||||+{Home}|^x    |^z   |^y      |^v      |  |  |
|||||^c     |{→ 5}|+{→}|+{→ 5} |+{→ 20}|  |  |
|||||+{End} |{← 5}|+{←}|+{← 5} |+{← 20}|  |
MEND

eiji    = %w(Q W E R T  Y U I O P  A S D F G  H J K L SCLN  Z X C V B  N M COMM DOT SLSH)
eiji_r  = %w(Y U I O P  H J K L SCLN N M COMM DOT SLSH)
eiji_l  = %w(Q W E R T  A S D F G  Z X C V B)

tanda = tanda.split("|").map{|c| c.strip}
tanda.delete_at(35)
tanda.delete_at(34)
tanda.delete_at(23)
tanda.delete_at(22)
tanda.delete_at(11)
tanda.delete_at(10)

shifted = shifted.split("|").map{|c| c.strip}
shifted.delete_at(35)
shifted.delete_at(34)
shifted.delete_at(23)
shifted.delete_at(22)
shifted.delete_at(11)
shifted.delete_at(10)

kana      = %w(あ い う え お か き く け こ さ し す せ そ た ち つ て と な に ぬ ね の は ひ ふ へ ほ ま み む め も や ゆ よ ら り る れ ろ わ を ん ー ヴ)
r_kana    = %w(a i u e o ka ki ku ke ko sa si su se so ta ti tu te to na ni nu ne no ha hi hu he ho ma mi mu me mo ya yu yo ra ri ru re ro wa wo nn - vu)

daku      = %w(が ぎ ぐ げ ご ざ じ ず ぜ ぞ だ ぢ づ で ど ば び ぶ べ ぼ)
r_daku    = %w(ga gi gu ge go za zi zu ze zo da di du de do ba bi bu be bo)
t_daku    = %w(か き く け こ さ し す せ そ た ち つ て と は ひ ふ へ ほ)

handaku   = %w(ぱ ぴ ぷ ぺ ぽ)
t_handaku = %w(は ひ ふ へ ほ)
r_handaku = %w(pa pi pu pe po)

kogaki    = %w(ゃ ゅ ょ ぁ ぃ ぅ ぇ ぉ ゎ っ)
t_kogaki  = %w(や ゆ よ あ い う え お わ っ)
r_kogaki  = %w(xya xyu xyo xa xi xu xe xo xwa xtu)

kumiawase = []
r_kumiawase = []
kumiawase << %w(しゃ しゅ しょ じゃ じゅ じょ)
r_kumiawase << %w(sya syu syo ja ju jo)
kumiawase << %w(きゃ きゅ きょ ぎゃ ぎゅ ぎょ)
r_kumiawase << %w(kya kyu kyo gya gyu gyo)
kumiawase << %w(ちゃ ちゅ ちょ ぢゃ ぢゅ ぢょ)
r_kumiawase << %w(tya tyu tyo dya dyu dyo)
kumiawase << %w(にゃ にゅ にょ)
r_kumiawase << %w(nya nyu nyo)
kumiawase << %w(ひゃ ひゅ ひょ びゃ びゅ びょ ぴゃ ぴゅ ぴょ)
r_kumiawase << %w(hya hyu hyo bya byu byo pya pyu pyo)
kumiawase << %w(みゃ みゅ みょ)
r_kumiawase << %w(mya myu myo)
kumiawase << %w(りゃ りゅ りょ)
r_kumiawase << %w(rya ryu ryo)

gairai = []
r_gairai = []
gairai << %w(てぃ てゅ でぃ でゅ)
r_gairai << %w(thi thu dhi dhu)
gairai << %w(とぅ どぅ)
r_gairai << %w(twu dwu)
gairai << %w(しぇ ちぇ じぇ ぢぇ)
r_gairai << %w(sye tye zye dye)

gairai << %w(ふぁ ふぃ ふぇ ふぉ ふゅ)
r_gairai << %w(fa fi fe fo fyu)
gairai << %w(いぇ)
r_gairai << %w(ye)
gairai << %w(うぃ うぇ うぉ ヴぁ ヴぃ ヴぇ ヴぉ ヴゅ)
r_gairai << %w(wi we who va vi ve vo vyu)
gairai << %w(くぁ くぃ くぇ くぉ くゎ ぐぁ ぐぃ ぐぇ ぐぉ ぐゎ)
r_gairai << %w(qa qi qe qo kuxwa gwa gwi gwe gwo guxwa)
gairai << %w(つぁ) # つぃ つぇ つぉ)
r_gairai << %w(tsa) # tsi tse tso)

kumiawase.flatten!
r_kumiawase.flatten!
gairai.flatten!
r_gairai.flatten!

def teigi(a, b, c, prefix="", suffix="")
  _a = [a].flatten.map{|k| "B_" + k}.join("|")
  sprintf("  {%-32s", ".key = #{prefix}#{_a}#{suffix}") + sprintf("%-20s", ", .kana = \"#{b}\"") + "}, // #{c}"
end

puts "  // 清音"
kana.each_with_index do |k, i|
  j = tanda.index(k)
  if j && j >= 0
    puts teigi(eiji[j], r_kana[i], k)
  end
  j = shifted.index(k)
  if j && j >= 0
    puts teigi(eiji[j], r_kana[i], k, "B_SHFT|")
  end
end

puts
puts "  // 濁音"
daku.each_with_index do |k, i|
  j = tanda.index(t_daku[i]) || shifted.index(t_daku[i])
  if j && j >= 0
    if eiji_r.index(eiji[j])
      puts teigi(eiji[j], r_daku[i], k, "B_F|")
      # puts teigi(eiji[j], r_daku[i], k + "(冗長)", "B_F|", "|B_SHFT")
    else
      puts teigi(eiji[j], r_daku[i], k, "B_J|")
      # puts teigi(eiji[j], r_daku[i], k + "(冗長)", "B_J|", "|B_SHFT")
    end
  end
end

puts
puts "  // 半濁音"
handaku.each_with_index do |k, i|
  j = tanda.index(t_handaku[i]) || shifted.index(t_handaku[i])
  if j && j >= 0
    if eiji_r.index(eiji[j])
      puts teigi(eiji[j], r_handaku[i], k, "B_V|")
      # puts teigi(eiji[j], r_handaku[i], k + "(冗長)", "B_V|", "|B_SHFT")
    else
      puts teigi(eiji[j], r_handaku[i], k, "B_M|")
      # puts teigi(eiji[j], r_handaku[i], k + "(冗長)", "B_M|", "|B_SHFT")
    end
  end
end

puts
puts "  // 小書き"
kogaki.each_with_index do |k, i|
  j = tanda.index(k)
  if j && j >= 0
    puts teigi(eiji[j], r_kogaki[i], k)
    next
  end
  j = shifted.index(k)
  if j && j >= 0
    puts teigi(eiji[j], r_kogaki[i], k, "B_SHFT|")
    next
  end

  j = tanda.index(t_kogaki[i]) || shifted.index(t_kogaki[i])
  if j && j >= 0
    puts teigi(eiji[j], r_kogaki[i], k, "B_Q|")
    # puts teigi(eiji[j], r_kogaki[i], k, "B_Q|B_SHFT|")
    # puts teigi(eiji[j], r_kogaki[i], k, "B_V|B_M|")
    # puts teigi(eiji[j], r_kogaki[i], k + "(冗長)", "B_V|B_M|", "|B_SHFT")
  end
end

puts
puts "  // 清音拗音 濁音拗音 半濁拗音"
kumiawase.each_with_index do |k, i|
  j = tanda.index(k[0])
  if j && j >= 0
    e0 = eiji[j]
  end
  unless e0
    j = shifted.index(k[0])
    if j && j >= 0
      e0 = eiji[j]
    end
  end
  unless e0
    l = daku.index(k[0])
    if l && l >= 0
      j = tanda.index(t_daku[l]) || shifted.index(t_daku[l])
      if j && j >= 0
        if eiji_r.index(eiji[j])
          e0 = ["F", eiji[j]]
        else
          e0 = ["J", eiji[j]]
        end
      end
    end
  end
  unless e0
    l = handaku.index(k[0])
    if l && l >= 0
      j = tanda.index(t_handaku[l]) || shifted.index(t_handaku[l])
      if j && j >= 0
        if eiji_r.index(eiji[j])
          e0 = ["V", eiji[j]]
        else
          e0 = ["M", eiji[j]]
        end
      end
    end
  end

  l = kogaki.index(k[1])
  j = tanda.index(t_kogaki[l]) || shifted.index(t_kogaki[l])
  if j && j >= 0
    e1 = eiji[j]
    puts teigi([e0, e1], r_kumiawase[i], k)
    # puts teigi([e0, e1], r_kumiawase[i], k + "(冗長)", "", "|B_SHFT")
  end
end

puts
puts "  // 清音外来音 濁音外来音"
gairai.each_with_index do |k, i|
  j = tanda.index(k[0]) || shifted.index(k[0])
  if j && j >= 0
    if eiji_r.index(eiji[j])
      e0 = ["V", eiji[j]]
    else
      e0 = ["M", eiji[j]]
    end
  end
  unless e0
    l = daku.index(k[0])
    if l && l >= 0
      j = tanda.index(t_daku[l]) || shifted.index(t_daku[l])
      if j && j >= 0
        if eiji_r.index(eiji[j])
          e0 = ["F", eiji[j]]
        else
          e0 = ["J", eiji[j]]
        end
      end
    end
  end

  l = kogaki.index(k[1])
  j = tanda.index(t_kogaki[l]) || shifted.index(t_kogaki[l])
  if j && j >= 0
    e1 = eiji[j]
    puts teigi([e0, e1], r_gairai[i], k)
    # puts teigi([e0, e1], r_gairai[i], k + "(冗長)", "", "|B_SHFT")
  end
end


# 編集モード

macro1 =<<END
// 『』{改行}{↑}
      if (naginata_config.os != NG_MAC) {
        ng_send_unicode_string("『』");
      } else {
        mac_send_string("naginika");
      }
      send_back();
      break;
END

macro2 =<<END
// (){改行}{↑}
      send_nagimaka();
      break;
END

macro3 =<<END
// 「」{改行}{↑}
      send_nagikagi();
      break;
END

macro4 =<<END
// {改行}{End}{改行}「」{改行}{↑}
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
END

macro5 =<<END
// 【】{改行}{↑}
      if (naginata_config.os != NG_MAC) {
        ng_send_unicode_string("【】");
      } else {
        mac_send_string("nagisuka");
      }
      send_back();
      break;
END

macro6 =<<END
// ^x『^v』{改行}{Space}+{↑}^x
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
END

macro7 =<<END
// 《》{改行}{↑}
      if (naginata_config.os != NG_MAC) {
        ng_send_unicode_string("《》");
      } else {
        mac_send_string("naginiya");
      }
      send_back();
      break;
END

macro8 =<<END
// ^x(^v){改行}{Space}+{↑}^x
      if (naginata_config.os != NG_MAC) {
        send_string(SS_LCTL("x"));
      } else {
        send_string(SS_LCMD("x"));
      }
      send_nagimaka();
      paste_and_clear_clipboard();
      break;
END

macro9 =<<END
// ^x「^v」{改行}{Space}+{↑}^x
      if (naginata_config.os != NG_MAC) {
        send_string(SS_LCTL("x"));
      } else {
        send_string(SS_LCMD("x"));
      }
      send_nagikagi();
      paste_and_clear_clipboard();
      break;
END

macro10 =<<END
// ^x｜{改行}^v《》{改行}{↑}{Space}+{↑}^x
      if (naginata_config.os != NG_MAC) {
        send_string(SS_LCTL("x"));
        ng_send_unicode_string("｜");
        send_string(SS_LCTL("v"));
        ng_send_unicode_string("《》");
        send_back();
        tap_code(KC_SPC);
        send_back();
        send_string(SS_LCTL("x"));
      } else {
        send_string(SS_LCMD("x"));
        mac_send_string("nagitabo");
        send_string(SS_LCMD("v"));
        mac_send_string("naginiya");
        send_back();
        tap_code(KC_SPC);
        send_back();
        send_string(SS_LCMD("x"));
      }
      break;
END

macro11 =<<END
// ^x【^v】{改行}{Space}+{↑}^x
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
END

macro12 =<<END
//　　　×　　　×　　　×{改行 2}
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
END

macro13 =<<END
// +{→ 20}
      cursor_move(true, KC_RIGHT, 20);
      break;
END

macro14 =<<END
// +{← 20}
      cursor_move(true, KC_LEFT, 20);
      break;
END

macro15 =<<END
// ^x｜{改行}^v《》{改行}{↑}
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
END

macro16 =<<END
// ││{改行}
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
END

macro101 =<<END
// {↑}
      cursor_move(false, KC_UP, 1);
      break;
END
macro102 =<<END
// {↓}
      cursor_move(false, KC_DOWN, 1);
      break;
END
macro103 =<<END
// +{↑}
      cursor_move(true, KC_UP, 1);
      break;
END
macro104 =<<END
// +{↓}
      cursor_move(true, KC_DOWN, 1);
      break;
END
macro105 =<<END
// {↑ 5}
      cursor_move(false, KC_UP, 5);
      break;
END
macro106 =<<END
// {↓ 5}
      cursor_move(false, KC_DOWN, 5);
      break;
END
macro107 =<<END
// +{↑ 7}
      cursor_move(true, KC_UP, 7);
      break;
END
macro108 =<<END
// +{↓ 7}
      cursor_move(true, KC_DOWN, 7);
      break;
END
macro109 =<<END
// +{→ 5}
      cursor_move(true, KC_RIGHT, 5);
      break;
END
macro110 =<<END
// +{← 5}
      cursor_move(true, KC_LEFT, 5);
      break;
END
macro111 =<<END
// {→ 5}
      cursor_move(false, KC_RIGHT, 5);
      break;
END
macro112 =<<END
// {← 5}
      cursor_move(false, KC_LEFT, 5);
      break;
END
macro113 =<<END
// +{↑ 5}
      cursor_move(true, KC_UP, 5);
      break;
END
macro114 =<<END
// +{↓ 5}
      cursor_move(true, KC_DOWN, 5);
      break;
END
macro115 =<<END
// {→}
      cursor_move(false, KC_RIGHT, 1);
      break;
END
macro116 =<<END
// +{→}
      cursor_move(true, KC_RIGHT, 1);
      break;
END
macro117 =<<END
// {←}
      cursor_move(false, KC_LEFT, 1);
      break;
END
macro118 =<<END
// +{←}
      cursor_move(true, KC_LEFT, 1);
      break;
END


$henshu = {
#"/*ディ*/"         => ["kana", "\"dhi\""],
"・"            => ["kana"  , "\"/\""],
"？{改行}"      => ["kana"  , "\"\?\"SS_TAP(KAKT)"],
"！{改行}"      => ["kana"  , "\"!\"SS_TAP(KAKT)"],
"{Home}"        => ["kana", "SS_TAP(X_HOME)"],
"{End}"         => ["kana", "SS_TAP(X_END)"],
"+{Home}"       => ["kana", "SS_LSFT(SS_TAP(X_HOME))"],
"^{Home}"       => ["kana", "SS_LCTL(SS_TAP(X_HOME))", "SS_TAP(KAKT)SS_LCMD(SS_TAP(X_RIGHT)SS_TAP(X_UP))"],
"+{End}"        => ["kana", "SS_LSFT(SS_TAP(X_END))"],
"^{End}"        => ["kana", "SS_LCTL(SS_TAP(X_END))", "SS_TAP(KAKT)SS_LCMD(SS_TAP(X_LEFT)SS_TAP(X_DOWN))"],
"+{End}{BS}"    => ["kana", "SS_LSFT(SS_TAP(X_END))\"\\b\""], # 末消
"{vk1Csc079}"   => ["kana", "SS_LGUI(SS_TAP(X_SLSH))", "SS_TAP(X_LANG1)SS_TAP(X_LANG1)"], # 再変換
"{Del}"         => ["kana", "\"\\x7F\""],
"{Esc 3}"       => ["kana", "\"\\x1B\\x1B\\x1B\"", "\"\\x1B\"SS_TAP(X_NUMLOCK)"],
"{↑}"          => ["macro", macro101],
"{↓}"          => ["macro", macro102],
"+{↑}"         => ["macro", macro103],
"+{↓}"         => ["macro", macro104],
"{↑ 5}"        => ["macro", macro105],
"{↓ 5}"        => ["macro", macro106],
"+{↑ 7}"        => ["macro", macro107],
"+{↓ 7}"        => ["macro", macro108],
"+{→ 5}"       => ["macro", macro109],
"+{← 5}"       => ["macro", macro110],
"{→ 5}"        => ["macro", macro111],
"{← 5}"        => ["macro", macro112],
"^{PgUp}"       => ["kana", "SS_LCTL(SS_TAP(X_PGUP))", "SS_TAP(X_PGUP)"],
"^{PgDn}"       => ["kana", "SS_LCTL(SS_TAP(X_PGDOWN))", "SS_TAP(X_PGDOWN)"],
"^{PgUp 5}"       => ["kana", "SS_LCTL(SS_TAP(X_PGUP)SS_TAP(X_PGUP)SS_TAP(X_PGUP)SS_TAP(X_PGUP)SS_TAP(X_PGUP))", "SS_TAP(X_PGUP)SS_TAP(X_PGUP)SS_TAP(X_PGUP)SS_TAP(X_PGUP)SS_TAP(X_PGUP)"],
"^{PgDn 5}"       => ["kana", "SS_LCTL(SS_TAP(X_PGDOWN)SS_TAP(X_PGDOWN)SS_TAP(X_PGDOWN)SS_TAP(X_PGDOWN)SS_TAP(X_PGDOWN))", "SS_TAP(X_PGDOWN)SS_TAP(X_PGDOWN)SS_TAP(X_PGDOWN)SS_TAP(X_PGDOWN)SS_TAP(X_PGDOWN)"],
"{Enter}{End}"  => ["kana", "SS_TAP(KAKT)SS_TAP(X_END)"],
"{Home}{改行}{Space 3}{End}"=> ["kana", "SS_TAP(X_HOME)\"\\n   \"SS_TAP(X_END)"], # 台マクロ
"{Home}{改行}{Space 1}{End}"=> ["kana", "SS_TAP(X_HOME)\"\\n \"SS_TAP(X_END)"], # ト マクロ

"+{↑ 5}" => ["macro", macro113],
"+{↓ 5}" => ["macro", macro114],
"+{PgUp}" => ["kana", "SS_LSFT(SS_TAP(X_PGUP))"],
"+{PgDn}" => ["kana", "SS_LSFT(SS_TAP(X_PGDOWN))"],
"^x{BS}{Del}^v" => ["kana", "SS_LCTL(\"x\")\"\\b\\x7F\"SS_LCTL(\"v\")", "SS_LCMD(\"x\")\"\\b\\x7F\"SS_LCMD(\"v\")"],
"{Home}{BS}{Del 3}{End}" => ["kana", "SS_TAP(X_HOME)\"\\b\\x7F\\x7F\\x7F\"SS_TAP(X_END)"],
"{Home}{BS}{Del 1}{End}" => ["kana", "SS_TAP(X_HOME)\"\\b\\x7F\"SS_TAP(X_END)"],
"{→}" => ["macro", macro115],
"+{→}" => ["macro", macro116],
"{←}" => ["macro", macro117],
"+{←}" => ["macro", macro118],
"{改行}{↓}" => ["kana", "SS_TAP(KAKT)SS_TAP(NGDN)"],
"{改行}{End}{改行}{Space}" => ["kana", "SS_TAP(KAKT)SS_TAP(X_END)\"\\n \""],

"^x(^v){改行}{Space}+{↑}^x" => ["macro", macro8],
"^x「^v」{改行}{Space}+{↑}^x" => ["macro", macro9],
"『』{改行}{↑}" => ["macro", macro1],
"(){改行}{↑}" => ["macro", macro2],
"「」{改行}{↑}" => ["macro", macro3],
"{改行}{End}{改行}「」{改行}{↑}" => ["macro", macro4],
"【】{改行}{↑}" => ["macro", macro5],
"《》{改行}{↑}" => ["macro", macro7],
"^x『^v』{改行}{Space}+{↑}^x" => ["macro", macro6],

"{Home}{Del 3}{BS}{←}" => ["kana", "SS_TAP(X_HOME)\"\\x7F\\x7F\\x7F\\b\"SS_TAP(NGLT)"],
"^x｜{改行}^v《》{改行}{↑}" => ["macro", macro15],
"{Home}{改行}{Space 3}{←}" => ["kana", "SS_TAP(X_HOME)\"\\n   \"SS_TAP(NGLT)"],
"{Home}{Del 1}{BS}{←}" => ["kana", "SS_TAP(X_HOME)\"\\x7F\\b\"SS_TAP(NGLT)"],
"{Home}{改行}{Space 1}{←}" => ["kana", "SS_TAP(X_HOME)\"\\n \"SS_TAP(NGLT)"],
"{改行}{End}{改行}}" => ["kana", "SS_TAP(KAKT)SS_TAP(X_END)\"\\n\""],
"{End}{改行}" => ["kana", "SS_TAP(X_END)\"\\n\""],

"｜{改行}"      => ["uc"  , "｜", "nagitabo"],
"……{改行}"    => ["uc"  , "……", "nagitete"],  # Mac: SS_LALT(";;")"\n"
"／{改行}"      => ["uc"  , "／", "naginame"],  # Mac: SS_LALT("/")"\n"
"││{改行}"    => ["macro", macro16], #["uc"  , "││", "nagitase"],
"──{改行}"    => ["macro", macro16], #["uc"  , "──", "nagiyose"],
"〇{改行}"      => ["uc"  , "〇", "nagimaru"],

"^x｜{改行}^v《》{改行}{↑}{Space}+{↑}^x"=> ["macro", macro10], # ルビ
# "」{改行 2}「{改行}"=> ["macro", ""],
# "」{改行 2}{Space}"=> ["macro", ""],
"　　　×　　　×　　　×{改行 2}"=> ["macro", macro12],
"^x【^v】{改行}{Space}+{↑}^x"=> ["macro", macro11],
"+{→ 20}"=> ["macro", macro13],
"+{← 20}"=> ["macro", macro14],

"{Space 3}"     => ["kana", "\"   \""],
"^i"            => ["kana", "SS_TAP(X_F7)"],  # カタカナ
"^u"            => ["kana", "SS_TAP(X_F6)"],  # ひらがな
"^s"            => ["kana", "SS_LCTL(\"s\")", "SS_LCMD(\"s\")"],
"^x"            => ["kana", "SS_LCTL(\"x\")", "SS_LCMD(\"x\")"],
"^v"            => ["kana", "SS_LCTL(\"v\")", "SS_LCMD(\"v\")"],
"^y"            => ["kana", "SS_LCTL(\"y\")", "SS_LCMD(SS_LSFT(\"z\"))"],
"^z"            => ["kana", "SS_LCTL(\"z\")", "SS_LCMD(\"z\")"],
"^c"            => ["kana", "SS_LCTL(\"c\")", "SS_LCMD(\"c\")"],
}

qwerty    = %w(Q W E R T  Y U I O P NO NO A S D F G  H J K L SCLN NO NO Z X C V B  N M COMM DOT SLSH NO)

mode1l = mode1l.split("|").map{|x| x.strip}
mode1r = mode1r.split("|").map{|x| x.strip}
mode2l = mode2l.split("|").map{|x| x.strip}
mode2r = mode2r.split("|").map{|x| x.strip}


$hwin = []
$hmac = []
$uwin = []
$umac = []
$macrokey = []
$macros = []

def outputHenshu(pk, m, k)
  if $henshu[m][0] == "kana"
    if $henshu[m][2]
      $hwin << "  {.key = #{pk}|B_#{k}\t\t, .kana = #{$henshu[m][1]}}, // #{m}"
      $hmac << "  {.key = #{pk}|B_#{k}\t\t, .kana = #{$henshu[m][2]}}, // #{m}"
    else
      $hwin << "  {.key = #{pk}|B_#{k}\t\t, .kana = #{$henshu[m][1]}}, // #{m}"
    end
  elsif $henshu[m][0] == "uc"
    $uwin << "  {.key = #{pk}|B_#{k}\t\t, .win   = \"#{$henshu[m][1]}\",\t\t.mac = \"#{$henshu[m][2] || $henshu[m][1]}\"}, // #{m}"
    $hwin << "//{.key = #{pk}|B_#{k}\t\t, .win   = \"#{$henshu[m][1]}\",\t\t.mac = \"#{$henshu[m][2] || $henshu[m][1]}\"}, // #{m}"
  elsif $henshu[m][0] == "macro"
    $hwin << "//{.key = #{pk}|B_#{k}\t\t, .macro , // #{m}"
    $macrokey << "  {.key = #{pk}|B_#{k}}, // #{m}"
    $macros << "    case #{pk}|B_#{k}: #{$henshu[m][1]}"
  end
end

qwerty.each_with_index do |k, i|
  unless $henshu.key? mode1l[i]
    $stderr.puts "missing #{mode1l[i]}"
    next
  end
  m =  mode1l[i]
  pk = "B_J|B_K"
  outputHenshu(pk, m, k)
end

qwerty.each_with_index do |k, i|
  unless $henshu.key? mode1r[i]
    $stderr.puts "missing #{mode1r[i]}"
    next
  end
  m =  mode1r[i]
  pk = "B_D|B_F"
  outputHenshu(pk, m, k)
end

qwerty.each_with_index do |k, i|
  unless $henshu.key? mode2l[i]
    $stderr.puts "missing #{mode2l[i]}"
    next
  end
  m =  mode2l[i]
  pk = "B_M|B_COMM"
  outputHenshu(pk, m, k)
end

qwerty.each_with_index do |k, i|
  unless $henshu.key? mode2r[i]
    $stderr.puts "missing #{mode2r[i]}"
    next
  end
  m =  mode2r[i]
  pk = "B_C|B_V"
  outputHenshu(pk, m, k)
end

puts "// 編集モード Win/Linux"
puts $hwin

puts "// 編集モード Mac"
puts $hmac

puts "// 編集モード unicode"
puts $uwin

puts "// 編集モード macro の使用キー"
puts $macrokey

puts "// 編集モード macro"
puts $macros
