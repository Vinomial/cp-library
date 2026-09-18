---
title: MaxFlow
documentation_of: ./max_flow.hpp
---

## MaxFlow

```C++
graph_lib::mf_graph<Cap = long long>
```

最大流(Dinic法)を計算するライブラリ。
AtCoder Library の `mf_graph` をベースに、`library/cpp` で単体利用しやすい形に整理してある。
API・挙動は基本的にAtCoder Library互換。

### 使い方

```C++
#include "library/cpp/graph/max_flow.hpp"

graph_lib::mf_graph<int> g(4); // 頂点数4, 容量の型int (既定はlong long)
g.add_edge(0, 1, 10);
g.add_edge(0, 2, 2);
g.add_edge(1, 2, 6);
g.add_edge(1, 3, 6);
g.add_edge(2, 3, 8);

int max_flow = g.flow(0, 3); // 頂点0(源)から頂点3(シンク)への最大流
```

辺を1本追加するたびに `add_edge` が返す辺番号を使って、あとから流量を確認できる。

```C++
auto e = g.get_edge(0);       // 0番目に追加した辺(0->1)の現在の状態
// e.from, e.to, e.cap (容量), e.flow (現在の流量)

auto edges = g.edges();       // 全ての辺をadd_edgeした順に
auto reachable = g.min_cut(0); // 直前のflow()の残余グラフで頂点0から到達可能な頂点集合
```

### 関数一覧

**コンストラクタ**

```C++
graph_lib::mf_graph<Cap> g(int n)
```

`n` 頂点 `0` 辺のグラフを作る。`Cap` は容量の型（`int`, `long long` など）。

**add_edge**

```C++
int g.add_edge(int from, int to, Cap cap)
```

`from` から `to` へ、容量 `cap` ・流量 `0` の有向辺を追加し、何番目に追加された辺かを返す（`0`-indexed）。
自己ループ・多重辺も追加できる。

**flow**

```C++
(1) Cap g.flow(int s, int t)
(2) Cap g.flow(int s, int t, Cap flow_limit)
```

- (1) 頂点 `s` から `t` へ流せるだけ流し、流せた量を返す。
- (2) 流量が `flow_limit` に達するまで流せるだけ流し、流せた量を返す。

同じ `g` に対して複数回呼ぶこともでき、その場合は前回までの残余グラフから続きを流す
（呼ぶたびに独立した最大流が求まるわけではないので注意）。

**min_cut**

```C++
std::vector<bool> g.min_cut(int s)
```

直前に呼んだ `flow` の残余グラフにおいて、頂点 `s` から到達可能な頂点集合を返す。
`flow(s, t)` をちょうど1回呼んだ直後に呼べば、`s`-`t` 最小カットの `s` 側に対応する。

**get_edge / edges**

```C++
struct mf_graph<Cap>::edge
{
    int from, to;
    Cap cap, flow;
};

(1) mf_graph<Cap>::edge g.get_edge(int i)
(2) std::vector<mf_graph<Cap>::edge> g.edges()
```

- (1) `i` 番目に追加した辺の現在の状態（容量・流量）を返す。
- (2) 全ての辺の現在の状態を、`add_edge` で追加した順に返す。

**change_edge**

```C++
void g.change_edge(int i, Cap new_cap, Cap new_flow)
```

`i` 番目の辺の容量・流量を `new_cap` / `new_flow` に書き換える。他の辺には影響しない。
`flow` を呼ぶ前後どちらでも呼べるが、内部状態を直接いじるので使い所には注意（詳細はAtCoder Libraryのドキュメント参照）。

### 計算量

- `add_edge`: ならし `O(1)`
- `flow`: 流量の上限を $F$、頂点数を $n$、辺数を $m$ として $O(\min(n^{2/3}m, m^{3/2}))$（容量が全て1の場合）、一般には $O(n^2 m)$
- `min_cut`: `O(頂点数 + 辺数)`
- `get_edge`: `O(1)`、`edges`: `O(辺数)`
- `change_edge`: `O(1)`

### 注意

- 頂点番号・辺番号はどちらも `0` から始まる。
- `Cap` は符号付き整数型を想定（`int` だとオーバーフローに注意。迷ったら既定の `long long` のままでよい）。
- `flow` を複数回呼んだときの正しさは、呼ぶ間に他の辺を触っていないことが前提（`add_edge`/`change_edge`を挟むと挙動が変わりうる）。
- AtCoder Library本家の`internal_queue`（簡易キュー）の代わりに`std::queue`を使っている。計算量オーダーは変わらない。

### 検証

AtCoder Libraryの`atcoder::mf_graph`本体（`vendor/ac-library`）と、ランダムなグラフ・自己ループ・多重辺・`flow_limit`指定を含むケースで出力を突き合わせて一致することを確認済み。
