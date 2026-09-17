# MinCostFlow

[Minimum-cost flow problem](https://en.wikipedia.org/wiki/Minimum-cost_flow_problem)を扱うライブラリです。

## コンストラクタ

```cpp
mcf_graph<Cap, Cost> graph(int n);
```

$n$ 頂点 $0$ 辺のグラフを作る。`Cap`は容量の型、`Cost`はコストの型

**制約**

- $0 \leq n \leq 10^8$
- `Cap, Cost` は `int, ll`

**計算量**

- $O(n)$

## add_edge

```cpp
int graph.add_edge(int from, int to, Cap cap, Cost cost);
```

`from`から`to`へ最大容量`cap`, コスト`cost`の辺を追加する。何番目に追加された辺かを返す。

**制約**

- $0 \leq \mathrm{from}, \mathrm{to} \lt n$
- $0 \leq \mathrm{cap}, \mathrm{cost}$

**計算量**

- ならし $O(1)$

## min_cost_max_flow

```cpp
(1) pair<Cap, Cost> graph.flow(int s, int t);
(2) pair<Cap, Cost> graph.flow(int s, int t, Cap flow_limit);
```

$s$ から $t$ へ流せるだけ流し、その流量とコストを返す。

- (1) sからtへ流せるだけ流す
- (2) sからtへ流量`flow_limit`まで流せるだけ流す

**制約**

- `min_cost_slope`と同じ

**計算量**

- `min_cost_slope`と同じ

## min_cost_slope

```cpp
vector<pair<Cap, Cost>> graph.slope(int s, int t);
vector<pair<Cap, Cost>> graph.slope(int s, int t, Cap flow_limit);
```

帰り値に流量とコストの関係の折れ線が入る。全ての $x$ について、流量 $x$ の時の最小コストを $g(x)$ とすると、$(x, g(x))$ は帰り値を折れ線として見たものに含まれる。

- 帰り値の最初の要素は $(0, 0)$
- 帰り値の`.first`、`.second`は共に狭義単調増加
- 3点が同一線上にあることはない
- (1) 帰り値の最後の要素は最大流量 $x$ として $(x, g(x))$
- (2) 帰り値の最後の要素は $y = \min(x, \mathrm{flow\_limit})$ として $(y, g(y))$

**制約**

辺のコストの最大を $x$ として

- $s \neq t$
- `min_cost_slope`や`min_cost_max_flow`を合わせて複数回呼んだときの挙動は未定義
- `s`から`t`へ流したフローの流量が `Cap` に収まる。
- 流したフローのコストの総和が `Cost` に収まる。
- (Cost : `int`): $0 \leq nx \leq 2 \times 10^9 + 1000$
- (Cost : `ll`): $0 \leq nx \leq 8 \times 10^{18} + 1000$

**計算量**

$F$を流量、$m$を追加した辺の本数として

- $O(F (n + m) \log n)$

## edges

```cpp
struct edge<Cap, Cost> {
    int from, to;
    Cap cap, flow;
    Cost cost;
};

(1) mcf_graph<Cap, Cost>::edge graph.get_edge(int i);
(2) vector<mcf_graph<Cap, Cost>::edge> graph.edges();
```

- 今の内部の辺の状態を返す
- 辺の順番はadd_edgeで追加された順番と同一

$m$ を追加された辺数として

**制約**

- (1): $0 \leq i \lt m$

**計算量**

- (1): $O(1)$
- (2): $O(m)$

## 使用例

### AC code of [https://atcoder.jp/contests/practice2/tasks/practice2_e](https://atcoder.jp/contests/practice2/tasks/practice2_e)

```cpp
#include <atcoder/mincostflow>
#include <iostream>

using namespace std;
using namespace atcoder;

const long long BIG = 1'000'000'000;

int main() {
    int n, k;
    cin >> n >> k;

    /**
     * generate (s -> row -> column -> t) graph
     * i-th row correspond to vertex i
     * i-th col correspond to vertex n + i
     **/
    mcf_graph<int, long long> g(2 * n + 2);
    int s = 2 * n, t = 2 * n + 1;

    // we can "waste" the flow
    g.add_edge(s, t, n * k, BIG);

    for (int i = 0; i < n; i++) {
        g.add_edge(s, i, k, 0);
        g.add_edge(n + i, t, k, 0);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            long long a;
            cin >> a;
            g.add_edge(i, n + j, 1, BIG - a);
        }
    }

    auto result = g.flow(s, t, n * k);
    cout << 1LL * n * k * BIG - result.second << endl;

    vector<string> grid(n, string(n, '.'));
    auto edges = g.edges();
    for (auto e : edges) {
        if (e.from == s || e.to == t || e.flow == 0) continue;

        grid[e.from][e.to - n] = 'X';
    }

    for (int i = 0; i < n; i++) {
        cout << grid[i] << endl;
    }
    return 0;
}
```
