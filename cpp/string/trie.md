---
title: Trie
documentation_of: ./trie.hpp
---

## Trie

```C++
namespace trie_lib
template <int ALPHABET = 26, char BASE = 'a'>
class Trie
```

文字列集合を管理するトライ木の最小限の骨組み。挿入・削除・検索・接頭辞ごとの通過数カウントのみを提供し、
問題ごとの探索ロジック（LCP・XOR最大化など）は `node()` を使って呼び出し側で書く前提。

### 使い方

```C++
trie_lib::Trie<> trie; // 既定: 英小文字26種 ('a'-'z')
trie.insert("abc");
trie.insert("abb");

trie.contains("abc");   // true  (ちょうど挿入されている)
trie.contains("ab");    // false (途中までしか一致しない)
trie.has_prefix("ab");  // true  (abで始まる文字列が存在する)

trie.erase("abc");      // "abc" を1本削除
trie.contains("abc");   // false
trie.has_prefix("ab");  // true  (abb がまだ残っている)

trie.count("abc");      // 0     (現在の本数)
trie.empty();           // false (abb が残っている)
trie.node(trie.root()).prefix_count; // 1 (現在挿入されている総本数)

// 自前の探索ロジックの例（ABC287E: 他の文字列との最長共通接頭辞）
int cur = trie.root();
int ans = 0;
for (int i = 0; i < (int)s.size(); i++)
{
    cur = trie.node(cur).children[s[i] - 'a'];
    if (trie.node(cur).prefix_count >= 2) ans = std::max(ans, i + 1);
}
```

01-trie（XOR最大化などに使う）にしたい場合はテンプレート引数を変える。

```C++
trie_lib::Trie<2, '0'> bit_trie; // 分岐数2、'0'/'1'の2文字を扱う
```

### 関数一覧

**コンストラクタ**
```C++
Trie()
```
根 (`nodes_[0]`) のみを持つ空のトライ木を作る。

**clear**
```C++
void trie.clear()
```
根だけの空の状態に戻す。

**reserve**
```C++
void trie.reserve(int n)
```
ノード数の目安（= 挿入する文字列の長さの総和 + 1）が事前に分かっている場合、
`vector` の再割り当てを避けるために呼ぶ。省略可。

**insert**
```C++
int trie.insert(const std::string &s)
```
文字列 `s` を挿入し、終端ノードの番号を返す。根から終端ノードまでの経路上の全ノード（根も含む）の
`prefix_count` を+1し、終端ノードの `end_count` を+1する。同じ文字列を複数回insertしてよい。

**erase**
```C++
void trie.erase(const std::string &s)
```
文字列 `s` を1本削除する。経路上の全ノード（根も含む）の `prefix_count` を-1し、
終端ノードの `end_count` を-1する。

前提: `s` が現在挿入されている（`std::multiset::erase(value)` と同様、挿入されていない `s` を渡すのは
未定義動作）。違反していれば途中の `assert` で落ちる。

**find_node**
```C++
int trie.find_node(const std::string &s) const
```
`s` を辿った先のノード番号を返す。途中で辿れなくなったら `-1`。

**contains**
```C++
bool trie.contains(const std::string &s) const
```
`s` がちょうど挿入されている文字列と一致するか（末端まで一致し、`end_count > 0` か）。

**count**
```C++
int trie.count(const std::string &s) const
```
`s` が現在何本挿入されているか（`std::multiset::count` 相当）。挿入されていなければ `0`。

**has_prefix**
```C++
bool trie.has_prefix(const std::string &s) const
```
`s` を接頭辞に持つ文字列が1つ以上挿入されているか（末端まで一致し、`prefix_count > 0` か）。
`erase` で通過数が0になったノードは構造上は残るが、辿れても「存在する」とは数えない。

**empty**
```C++
bool trie.empty() const
```
挿入されている文字列が1本もないか。

**node / root / size**
```C++
const Node &trie.node(int i) const
int trie.root() const
int trie.size() const
```
ノード番号からノードの中身（`children` / `end_count` / `prefix_count`）を参照する。
問題ごとの探索ロジックはこれらを使って呼び出し側で組み立てる。`size()` はトライの**ノード数**であり、
挿入されている文字列の本数ではない（本数は `node(trie.root()).prefix_count` で得られる）。

### 計算量

- `insert` / `erase` / `find_node` / `contains` / `count` / `has_prefix`: $O(|s|)$
- `empty`: $O(1)$。`clear` はそれまでのノード数に比例するが、`Node` が trivial なため実質ノーコスト
- 全体のノード数・メモリは $O(\Sigma |S_i| \times \mathrm{ALPHABET})$
- `erase` はノード自体を回収しない（`prefix_count` / `end_count` を減らすのみ）ので、
  何本削除してもメモリ使用量は減らない。`clear` は文字列集合を空にするだけで、
  確保済みのノード用メモリ自体は解放されない（`std::vector::clear` と同様）。

### よくあるミス

- `Node` の `children` を `-1` で初期化し忘れると、未初期化値が根(0)などの実在ノード番号と
  誤認識されて誤った経路を辿ってしまう（このライブラリではコンストラクタで `fill(-1)` 済み）。
- `insert` で終端の文字も含めて `prefix_count` をインクリメントしているので、
  ある文字列が別の文字列の接頭辞になっているケース（例: `"abra"` と `"abracadabra"`）も
  特別扱いなく正しく処理できる。逆に、終端だけ数えたい／通過数を数えたくない場合は
  カウント対象を調整すること。
- `erase(s)` は `s` が現在挿入されていることが前提（`std::multiset::erase(value)` と同様）。
  挿入されていない文字列や、すでに削除済みの文字列をもう一度 `erase` すると未定義動作で、
  途中の `assert` で落ちる（同じ文字列を2回 `insert` して1回しか `erase` していないのに
  「もう無い」と思い込んで2回目の `erase` を呼んでしまう、といったミスが典型）。

### Trie とは？

文字列の集合を、共通の接頭辞を共有する木構造として管理するデータ構造。
根から葉に向かって1文字ずつ辺をたどることで、ある文字列が集合に含まれるか、
ある接頭辞を持つ文字列が何個あるか、などを $O(|s|)$ で判定できる。

典型的な用途は、接頭辞に関するクエリ（今回のLCP最大値など）、01-trieによるXOR最大化、
複数パターン文字列検索（Aho-Corasickの土台）など。
