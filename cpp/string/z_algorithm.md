---
title: ZAlgorithm
documentation_of: ./z_algorithm.hpp
---

# Z Algorithm

文字列 `s` に対して、「`s` 自身と、`s` を `i` 文字だけずらしたもの」の最長共通接頭辞の長さを、全ての `i` について `O(N)` でまとめて求めるライブラリ。

```cpp
#include "library/cpp/string/z_algorithm.hpp"
```

## 何を持つか

文字列 `s` の長さを `n` とする。

```text
z[i]
```

は、次の値を表す。

```text
s と s[i..] の最長共通接頭辞(LCP)の長さ
```

例えば、

```text
s = "abcabcabx"
```

なら、

```text
z[0] = 9   // 番兵。s 自身との LCP は全長
z[1] = 0   // "bcabcabx" は "a" から始まらない
z[2] = 0
z[3] = 5   // "abcabx" と "abcabcabx" の共通接頭辞は "abcab" で5文字
z[4] = 0
z[5] = 0
z[6] = 2   // "abx" と "abcabcabx" の共通接頭辞は "ab" で2文字
z[7] = 0
z[8] = 0
```

`z[0]` は自分自身との比較なので、慣習として全長 `n` を入れる（`next_occurrence` の番兵と同様、境界処理を楽にするための約束事）。

## API

```cpp
auto z = z_algorithm_lib::calc_z(s);
```

`s` から Z配列を作る。`s` は `std::string` のほか、`std::vector<T>`（`T` は `==` 比較できる任意の型）も受け付ける。

返り値の型は次の通り。

```cpp
std::vector<int>
```

サイズは `s.size()`。

## 計算量

- 計算: `O(N)`（`N = s.size()`）
- メモリ: `O(N)`

`N = 10^6` 程度でも十分軽い。

## 実装の考え方

「今わかっている中で最も右まで LCP が伸びている区間 `[j, j + z[j])`」を1つだけ管理しながら左から `i` を進める。

- `i` がその区間の中にあれば、鏡写しの位置 `i - j` の情報 `z[i-j]` を初期値として使い回せる（区間の右端を超えない範囲で）。
- 区間の外に出た分だけは、1文字ずつ愚直に比較して伸ばす。
- 進めた結果、区間の右端が更新されたら `j` を今の `i` に更新する。

この「使い回し」があるため、全体で愚直な文字比較の回数がならし `O(N)` に収まる。

## 例: パターン検索（`text` の中から `pattern` を全部探す）

区切り文字（`pattern`・`text` に出てこない文字、例えば `'\0'` や `'#'`）を挟んで連結し、Z配列を計算する。

```cpp
vector<int> find_occurrences(const string &pattern, const string &text)
{
    string s = pattern + '\1' + text; // '\1' は pattern/text に含まれない文字
    auto z = z_algorithm_lib::calc_z(s);

    vector<int> positions;
    int m = (int)pattern.size();
    int offset = m + 1;
    for (int i = 0; i + offset < (int)s.size(); i++)
    {
        if (z[i + offset] >= m)
        {
            positions.push_back(i); // text の i 文字目から pattern が出現
        }
    }
    return positions;
}
```

閾値判定（`z[...] >= m`）だけを見るなら、区切り文字を挟まなくても正しく動く場合が多いが（`pattern` 自身の中に一致が"はみ出す"影響を受けない限り）、区切り文字を挟んでおくのが安全で分かりやすい。

## 例: 巡回シフトでの一致判定（ABC430 E）

`A` を何回か「先頭の文字を末尾に回す」操作をして `B` に一致させたい、という問題。

`k` 回操作した後の `A` は、`A + A` の `k` 文字目から長さ `|A|` を取り出した部分文字列に等しい。つまり「`B` が `A+A` のどこから出現するか（最小の `k < |A|`）」という完全一致検索に帰着する。

```cpp
string s = B + A + A;
auto z = z_algorithm_lib::calc_z(s);

int ans = -1;
for (int k = 0; k < (int)A.size(); k++)
{
    if (z[k + (int)B.size()] >= (int)B.size())
    {
        ans = k;
        break;
    }
}
```

このケースは `B` と `A` がどちらも `0/1` の2文字種のみなので、区切り文字を挟まなくても閾値判定だけなら正しく動く。

## よく使う場面

次のような言葉が問題文や考察に出たら候補になる。

- 文字列 `s` の中で、パターンが出現する位置を全部（または最初の1つを）`O(N)` で探したい
- 巡回シフト・回転で一致させられるか（`A+A` に落とし込む定番テクとセット）
- 文字列の周期性（`n - z[i]` が `i` の倍数なら周期 `i` を持つ、等）
- 累積和やセグ木では届かない「文字列そのものの一致判定」の高速化

KMP法・ロリハ（文字列ハッシュ）でも同じ種類の問題が解けるので、状況に応じて使い分ける（Z algorithm は実装がシンプルで済むことが多い）。

## 注意点

- `z[0]` は「全長 `n`」として返る。パターン検索で「区切り文字より前の `pattern` 自身」の部分を走査対象に含めないよう、`offset`（`pattern` の長さ+区切り文字の分）から先だけを見ること。
- 区切り文字は `pattern`・`text` の両方に出てこない文字を選ぶこと（アルファベットが小文字26種のみなら `'#'` や `'\1'` などで十分。文字種を使い切っている場合は要注意）。
- `O(N)` 陣営のアルゴリズムなので、`s` の長さの合計に対して線形。複数テストケースがある場合は合計長で計算量を見積もる。

## 関連する典型

- ABC430 E: Cyclic Shift（`A+A` への埋め込み + Z algorithm）
- KMP法・ロリハ（同じ問題を解ける別の道具）
- 文字列の最小周期の判定
- Suffix Array / LCP Array（より重い前処理が要る場面で使う関連データ構造、今回は未実装）
