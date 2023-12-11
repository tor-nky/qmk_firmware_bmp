# 薙刀式カナ配列キーマップ

薙刀式カナ配列による入力をQMKで実現します。薙刀式v15に準拠しています。
編集モードも実装していますが、
編集モードでの記号入力方式がOSによって異なるので、
使用するOS(Windows、MacOS、Linux)によって切り替え る必要があります。
切り替えは再コンパイル不要で、動的に切り替えられます。

## 薙刀式とは

【薙刀式】v15fix版、発表
http://oookaworks.seesaa.net/article/500180437.html#gsc.tab=0
## OSの設定

1. OSのキーボード設定を日本語106キーボードにする。
1. Windowsの場合は[WinCompose](http://wincompose.info/)をインストールする。
1. Macの場合は[Karabiner-Elements](https://karabiner-elements.pqrs.org/)をインストールします。  
ファイル unicode_hex_input_switcher.json をフォルダ ~/.config/karabiner/assets/complex_modification にコピーし、  
Karabiner-Elements に Unicode Hex Input Switcher を登録してください。  
また、「キーボード設定を開く...」から「入力ソース」に英語「U.S.」を加えます。
「英数」キーでIMをオフにしたとき「U.S.」になるようにしてください。  
1. iOSの場合は、ユーザー辞書にnaginata_dictionary.plistの中身を登録する。  
config.h の中に ``#define ENABLE_NG_IOS`` を書き加えてコンパイルします。

## QMKファームウェアの設定

独自拡張として、標準のシフト&スペースに加えて、シフト&エンターキーを追加しました。
NG_SHFT2をキーマップに配置すると、単押しならエンター、
同時押しで薙刀式のシフトキーとして機能します。

OLEDが有効な場合には左側のOLEDには、
日本語入力モードに応じて「かな」または「ABC」と表紙されます。
右側のOLEDには薙刀式のロゴが表示されます。薙刀式のロゴは大岡俊彦氏に帰属します。

## キーボードの切り替え操作

以下の機能を動的に切り替えることができます。
設定内容をEEPROMに記録することができます。

| 設定項目 | 設定 | キーコード | 関数呼び出し | 
|---|---|---|---|
| OS切り替え            | Windows  | NGSW_WIN  | switchOS(NG_WIN)  | 
|                       | MacOS    | NGSW_MAC  | switchOS(NG_MAC)  | 
|                       | Linux    | NGSW_LNX  | switchOS(NG_LNX)  | 
| MacOSのライブ変換対応 | ON/OFFトグル   | NG_MLV   | mac_live_conversion_toggle()  | 
| 縦書き、横書き        | ON/OFFトグル   | NG_TAYO    | tategaki_toggle()  | 
| 後置シフト            | ON/OFFトグル   | NG_KOTI  | kouchi_shift_toggle()  | 
| 現在設定の出力        |   | NG_SHOS   | ng_show_os()  | 

本家のDvorakJ版薙刀式は前置シフトですが、
後置シフトも有効にできます。

MacOSでライブ変換が有効な場合、
記号入力で変換操作が不要なので、ライブ変換対応をオンにしてください。

OLEDをオンにしているときは、設定の状態がOLEDに表示されます。
 * 1行目 OS設定。Windows、Mac、Linuxの頭文字です。
 * 2行目 縦書き(T)、横書き(Y)
 * 3行目 後置シフト(Kでオン)
 * 4行目 ライブ変換(Lでオン)

## BLE Micro Pro で使う場合
iPhone の __Bluetooth接続専用__ です。  
「選択範囲を括弧でくくる」編集モードを使ったあとにクリップボードは消去されません。

__辞書登録が必要__ です。_naginata_dictionary.plist_ 内に12個あります。

キーボードの種類は ANSI、JIS(日本語)のどちらに設定していても使えます。

* iPhone  
キーボードの電源を入れた直後、あるいはキーボードをリセットした直後には、```『』【】〇``` などの辞書登録によった記号が _1〜2回_ 入力できません。  
iPhoneの仕様で、ひらがな変換、カタカナ変換、再変換などは使えません。

## 通常のQMKへの逆移植
フォルダ users/naginata_v15m の全てのファイルをコピーし、naginata.h を少し書き換えれば使えます。

```
#include "bmp_custom_keycode.h" // BMP用
```
を削除。

```
  NG_Q = BMP_SAFE_RANGE + 2, // 薙刀式シフトキー    // BMP用
```
を次のように戻す。
```
  NG_Q = SAFE_RANGE, // 薙刀式シフトキー
```

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

DvorakJ版は、5キーくらい押すまではどのキーを離しても全部一気に入力されるようです。
Hachikuと本バージョンは、押したままのキーが重ね押しされ得るなら待つようになっています。
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
