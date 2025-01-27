このソースは eswaiさんの [naginata_v15](https://github.com/eswai/qmk_firmware/tree/master/users/naginata_v15) を元に作成しています。

# 薙刀式カナ配列キーマップ

薙刀式カナ配列による入力をQMKで実現します。薙刀式v15に準拠しています。
編集モードも実装していますが、
編集モードでの記号入力方式がOSによって異なるので、
使用するOS(Windows、MacOS、Linux)によって切り替える必要があります。
切り替えは再コンパイル不要で、動的に切り替えられます。

## 薙刀式とは

【薙刀式】v15fix版、発表
http://oookaworks.seesaa.net/article/500180437.html#gsc.tab=0
## OSの設定 (BLE Micro Pro以外)
### Windowsの場合
キーボード設定を日本語106キーボードにする。
[WinCompose](http://wincompose.info/)をインストールする。

IMEのキー設定
|* キー|入力/変換済み文字なし|他|
|---|:---:|:---:|
|Ctrl+Shift+無変換| - |全消去|
|Ctrl+Shift+変換| - |全確定|
### Macの場合
キーボードが日本語/英語どちらの設定でも動きます。

IM にかわせみを使用する場合は、コード入力に Option+Shift+B が設定されているか確認してください。  
（config.h の中に ``#define NG_USE_KAWASEMI`` を書き加えてコンパイルします）

また、「キーボード設定を開く...」から「入力ソース」に英語「U.S.」を加えます。  
「英数」キーでIMをオフにしたとき「U.S.」になるようにしてください。

かわせみを使わない場合は下の設定も必要です。

[Karabiner-Elements](https://karabiner-elements.pqrs.org/)をインストールします。  
ファイル unicode_hex_input_switcher.json をフォルダ ~/.config/karabiner/assets/complex_modification にコピーし、  
Karabiner-Elements に Unicode Hex Input Switcher を登録してください。

念のため、Karabiner-Elements の設定 Device で、本ファームウェアが入ったキーボードが有効になっているか確認してください。
### Linuxの場合
キーボード設定を日本語106キーボードにする。

IMEのキー設定
|モード|入力キー|コマンド|
|---|:---:|:---:|
|変換前入力中|Ctrl Shift Henkan|__確定__|
|変換中|〃|__〃__|
|変換前入力中|Ctrl Shift Muhenkan|__キャンセル__|
|変換中|〃|__〃__|
## OSの設定とIMへの単語の登録 (BLE Micro Pro)
iOSとLinux以外では下表のものを __辞書登録__ してください。
|単語|読み|参考|
|---|---|---|
|……|なぎてて|__なぎ__ なたしき __て__ ん __て__ ん|
|――|なぎよせ|__なぎ__ なたしき __よ__ こ __せ__ ん|
|《》|なぎにや|__なぎ__ なたしき __に__ じゅう __や__ まかっこ|
|（）|なぎまか|__なぎ__ なたしき __ま__ る __か__ っこ|
|【】|なぎすか|__なぎ__ なたしき __す__ みつき __か__ っこ|
|「」|なぎかぎ|__なぎ__ なたしき __かぎ__ かっこ|
|『』|なぎにか|__なぎ__ なたしき __に__ じゅう __か__ ぎかっこ|
|×　　　×　　　×|なぎばつ|__なぎ__ なたしき __ばつ__|
|｜《》|なぎる|__なぎ__ なたしき __るび__|
|○|なぎまる|__なぎ__ なたしき __まる__|
### Windows辞書式(BMP専用)の場合
キーボード設定を日本語106キーボードにする。

IMEのキー設定
|* キー|入力/変換済み文字なし|他|
|---|:---:|:---:|
|Ctrl+Shift+無変換| - |全消去|
|Ctrl+Shift+変換| - |全確定|
### Mac辞書式(BMP専用)の場合
キーボードが日本語/英語どちらの設定でも動きます。

また、「キーボード設定を開く...」から「入力ソース」に英語「U.S.」を加えます。  
「英数」キーでIMをオフにしたとき「U.S.」になるようにしてください。
### Linux(BMP専用)の場合
キーボード設定を日本語106キーボードにする。
単語登録した記号を入力することが不可能なので、__編集モードの多くは押しても何も起きません__。
### iOS(BMP専用)の場合
キーボードが日本語/英語どちらの設定でも動きます。  
単語登録した記号を入力することが不可能なので、__編集モードの多くは押しても何も起きません__。
## QMKファームウェアの設定

独自拡張として、標準のシフト&スペースに加えて、シフト&エンターキーを追加しました。
NG_SHFT2をキーマップに配置すると、単押しならエンター、
同時押しで薙刀式のシフトキーとして機能します。

OLEDが有効な場合には左側のOLEDには、
日本語入力モードに応じて「かな」または「ABC」と表紙されます。
右側のOLEDには薙刀式のロゴが表示されます。薙刀式のロゴは大岡俊彦氏に帰属します。

3キー同時押しのカーソル移動と、Delキーにだけキーリピートが効きます。

F+G を押さなくても 左右シフト＋英字 で IMEオフになるので、アルファベットの入力がすぐにできます。  
再びかな入力にするときは H+J を押して IMEオン にします。

## キーボードの切り替え操作

以下の機能を動的に切り替えることができます。
設定内容をEEPROMに記録することができます。

| 設定項目 | 設定 | キーコード | 関数呼び出し | 
|---|---|---|---|
| OS切り替え            | Windows  | NGSW_WIN  | switchOS(NG_WIN)  | 
|                      | MacOS    | NGSW_MAC  | switchOS(NG_MAC)  | 
|                      | Linux    | NGSW_LNX  | switchOS(NG_LNX)  | 
|                      | Windows(BMP専用) | NGSW_WIN  | switchOS(NG_WIN_BMP)  | 
|                      | Mac(BMP専用)     | NGSW_MAC  | switchOS(NG_MAC_BMP)  | 
|                      | Linux(BMP専用)   | NGSW_LNX  | switchOS(NG_LNX_BMP)  | 
|                      | iOS(BMP専用) | NGSW_IOS  | switchOS(NG_IOS_BMP)  | 
| MacOSのライブ変換対応(BMPにはなし) | ON/OFFトグル   | NG_MLV   | mac_live_conversion_toggle()  | 
| 縦書き、横書き        | ON/OFFトグル   | NG_TAYO    | tategaki_toggle()  | 
| 後置シフト            | ON/OFFトグル   | NG_KOTI  | kouchi_shift_toggle()  | 
| 現在設定の出力        |   | NG_SHOS   | ng_show_os()  | 

本家のDvorakJ版薙刀式は前置シフトですが、
時間制限付き後置シフトも有効にできます。
naginata.h 内の `#define NG_KOUCHI_SHIFT_MS 60` にミリ秒単位で設定し、コンパイルします。

MacOSでライブ変換が有効な場合、
記号入力で変換操作が不要なので、ライブ変換対応をオンにしてください。

OLEDをオンにしているときは、設定の状態がOLEDに表示されます。
 * 1行目 OS設定。Windows、Mac、Linuxの頭文字です。
 * 2行目 縦書き(T)、横書き(Y)
 * 3行目 後置シフト(Kでオン)
 * 4行目 ライブ変換(Lでオン)
## 通常の QMK で使う場合
config.h の中に ``#define NG_BMP`` がなければ、そのままコンパイルできます。
## BLE Micro Pro で使う場合
config.h の中に ``#define NG_BMP`` を書き加えてコンパイルします。

USB接続時には、編集モードの多くが正常に動きません。

Bluetooth接続でご利用ください。  
なお、「選択範囲を括弧でくくる」編集モードを使ったあとにクリップボードは消去されません。
* Mac辞書式(BMP専用)  
日本語IMのライブ変換を使用中に M+Comma+Z を押すと、「　　　×　　　×　　　×」が入力できなくなります。
ライブ変換をやめ、変換学習をリセットすると入力できるようになります。
* iOS辞書式(BMP専用)  
iPhoneの仕様で、ひらがな変換、カタカナ変換、再変換などは使えません。
## 不具合
* 定義が設定されていないキーを押しても、何の代わりも出力しない  
この場合、キーマップから該当するキーを NG_** でないものに変えてください。
* Windows の秀丸エディタでは、入力が速すぎると表示が乱れたりクラッシュすることがあります。
## DvorakJ版、Hachikuとの違い
### 文字キーを押し、未出力のままスペースを押し離す
|DvorakJ|Hachiku|naginata_v15m|
|---|---|---|
|何も文字が出力されないままスペース押しが起きる|残った文字を出力しスペース押しが起きる|同左|
### かな2文字→どちらか離す
キー|DvorakJ|Hachiku|naginata_v15m|
|---|---|---|---|
|J押す||||
|I押す||||
|J離す|ある|あ|あ|
|I離す|ある|ある|ある|

キー|DvorakJ|Hachiku|naginata_v15m|
|---|---|---|---|
|J押す||||
|I押す||||
|I離す|ある|ある|ある|
|J離す|ある|ある|ある|

DvorakJ版はキーを離してすべて入力されることがあります。
Hachikuと本バージョンは、まだ同時押しになるキーがあったら待つようになっています。
### 2キー同時押し→キー1個離す→3キー同時押し
キー|DvorakJ|Hachiku|naginata_v15m|注|
|---|---|---|---|---|
|J押す|||||
|I押す|||||
|I離す|ある|ある|ある||
|I押す|ある|ある|ある|(1)|
|R押す|ある|ある|あるじょ|(2)|
|L押す|ある|あるしょ|あるじょ||
|J離す|ある|あるしょ|あるじょ|(3)|
|I離す|あるしょう|あるしょ|あるじょ||
|L離す|あるしょう|あるしょう|あるじょう||
|R離す|あるしょう|あるしょう|あるじょう||
|Z押す|あるしょう|あるしょう|あるじょう||
|L押す|あるしょう|あるしょうほ|あるじょうほ||
|Z離す|あるしょうほう|あるしょうほ|あるじょうほ||
|L離す|あるしょうほう|あるしょうほう|あるじょうほう||

DvorakJで 3かな以上のロールオーバー押しをしたときは、__最初に離すキーを選びます__。  
(3)で J以外のキーを最初に離せば、「あるじょ」と入力されます。
Hachikuでは __キーを離した後は、左にあるキーを先に押さないと__ 3キー同時押しになりません。  
(1)で Iより先に Rを押せば、(2)では「あるじょ」と入力されます。
### 4キー以上のキー押し→キー1個離す→キー押し
キー|DvorakJ|Hachiku|naginata_v15m|
|---|---|---|---|
|RJI押す||じょ|じょ|
|L押す||じょ|じょ|
|R離す|じょう|じょ|じょ|
|S押す|じょう|じょう|じょう|
|D押す|じょう|じょうげ|じょうげ|
|全部離す|じょうけと|じょうげと|じょうげと|
||(離す順序によって不定)|||

キー|DvorakJ|Hachiku|naginata_v15m|
|---|---|---|---|
|D押す||||
|S押す||と|と|
|K押す||と|と|
|F押す||とけ|とけい|
|K離す|とけいか|とけい|とけい|
|K押す|とけいか|とけい|とけい|
|全部離す|とけいかい|とけいかい|とけいかい|
||(離す順序によって不定)|||
## 変換ロジック
eswaiさん作成の関数number_of_candidates()を活用し、早期出力をします。

その仕組みです。  
戻り値が1の時はそのまま検索、0の時は後ろを縮めて検索、2の時は候補を絞れないので次の入力を待ちます。  
未変換のキーが3つあれば即変換してよいので関数は使用しません。

キー|キー残り|出力|関数戻り値|補足|
|---|---|---|:---:|---|
|あ押す|(あ)||2||
|い押す|(あい)||2||
|ら押す|(あいら)||-|3キーなので変換。定義を探すがないので縮める|
||(あい)ら||-|定義を探すがないので縮める|
||(あ)いら|あ|-||
||(いら)||0|__残りを評価し__、0なので後ろを縮める|
||(い)ら|い|-||
||(ら)||2|__残りを評価し__、終わり|

1つかなを出力したら残りをまた関数にかけるのがポイントです。

キーを離した後の入力で、濁点や拗音を再利用するのはDvorakJを真似ていますが、今押したキー以外が出力済みになる時だけにしています。

上の方「__じょうげと__」の例では、「じょう」を押したあとにRを離しSを押しています。この時「じょう」まで出力されているので「け(S)」は「濁点(J)」を再利用します。

「__とけいかい__」の例では、「とけいか」を押して「い(K)」を離し、再び押しています。この時の出力は「とけい」までで、「か」がまだなのでDF+Kの編集モードにはなりません。
