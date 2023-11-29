# 薙刀式カナ配列キーマップ

薙刀式かな入力の新しい変換ロジックの開発です。

[【薙刀式】v15fix版、発表](http://oookaworks.seesaa.net/article/500180437.html#gsc.tab=0)の実装です。

DvorakJ風になるよう、書き直しました。

通常のQMKでも、フォルダ users/naginata_v15m の全てのファイルをコピーし、naginata.h を少し書き換えれば使えます。

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
