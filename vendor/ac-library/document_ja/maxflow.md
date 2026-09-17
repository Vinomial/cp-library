# MaxFlow

[最大フロー問題](https://ja.wikipedia.org/wiki/%E6%9C%80%E5%A4%A7%E3%83%95%E3%83%AD%E3%83%BC%E5%95%8F%E9%A1%8C) を解くライブラリです。

## コンストラクタ

```cpp
mf_graph<Cap> graph(int n)
```

`n` 頂点 $0$ 辺のグラフを作る。`Cap`は容量の型。

**制約**

- $0 \leq n \leq 10^8$
- `Cap` は `int`, `ll`

**計算量**

- $O(n)$

## add_edge

```cpp
int graph.add_edge(int from, int to, Cap cap);
```

`from`から`to`へ最大容量`cap`、流量 $0$ の辺を追加し、何番目に追加された辺かを返す。

**制約**

- $0 \leq \mathrm{from}, \mathrm{to} \lt n$
- $0 \leq \mathrm{cap}$

**計算量**

- ならし $O(1)$

## flow

```cpp
(1) Cap graph.flow(int s, int t);
(2) Cap graph.flow(int s, int t, Cap flow_limit);
```

- (1) 頂点 $s$ から $t$ へ流せる限り流し、流せた量を返す。
- (2) 頂点 $s$ から $t$ へ流量 $flow_limit$ に達するまで流せる限り流し、流せた量を返す。
- 複数回呼ぶことも可能で、その時の挙動は [Appendix](./appendix.md) を参照してください。

**制約**

- $s \neq t$
- 帰り値が `Cap` に収まる

**計算量**

$m$ を追加された辺数として

- $O(\min(n^{\frac{2}{3}}m, m^{\frac{3}{2}}))$ (辺の容量がすべて $1$ の時)
- $O(n^2 m)$

## min_cut

```cpp
vector<bool> graph.min_cut(int s)
```

長さ $n$ のvectorを返す。$i$ 番目の要素には、頂点 $s$ から $i$ へ残余グラフで到達可能なとき、またその時のみ `true` を返す。`flow(s, t)`をflow_limitなしでちょうど一回呼んだ後に呼ぶと、返り値は $s$, $t$ 間のmincutに対応します。詳細な挙動は [Appendix](./appendix.md) を参照してください。

**計算量**

$m$ を追加された辺数として

- $O(n + m)$

## get_edge / edges

```cpp
struct mf_graph<Cap>::edge {
    int from, to;
    Cap cap, flow;
};

(1) mf_graph<Cap>::edge graph.get_edge(int i);
(2) vector<mf_graph<Cap>::edge> graph.edges();
```

- 今の内部の辺の状態を返す
- 辺の順番はadd_edgeで追加された順番と同一

**制約**

**計算量**

$m$ を追加された辺数として

- (1): $O(1)$
- (2): $O(m)$

## change_edge

```cpp
void graph.change_edge(int i, Cap new_cap, Cap new_flow);
```

$i$ 番目に変更された辺の容量、流量を`new_cap`, `new_flow`に変更する。他の辺の容量、流量は変更しない。詳細は [Appendix](./appendix.md) を参照してください

**制約**

- $0 \leq \mathrm{newflow} \leq \mathrm{newcap}$

**計算量**

- $O(1)$

## 使用例

### AC code of [https://atcoder.jp/contests/practice2/tasks/practice2_d](https://atcoder.jp/contests/practice2/tasks/practice2_d)

```cpp
#include <atcoder/maxflow>
#include <iostream>

using namespace std;
using namespace atcoder;

int main() {
    int n, m;
    cin >> n >> m;

    vector<string> grid(n);
    for (int i = 0; i < n; i++) {
        cin >> grid[i];
    }

    /**
     * generate (s -> even grid -> odd grid -> t) graph
     * grid(i, j) correspond to vertex (i * m + j)
     **/
    mf_graph<int> g(n * m + 2);
    int s = n * m, t = n * m + 1;

    // s -> even / odd -> t
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (grid[i][j] == '#') continue;
            int v = i * m + j;
            if ((i + j) % 2 == 0) {
                g.add_edge(s, v, 1);
            } else {
                g.add_edge(v, t, 1);
            }
        }
    }

    // even -> odd
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if ((i + j) % 2 || grid[i][j] == '#') continue;
            int v0 = i * m + j;
            if (i && grid[i - 1][j] == '.') {
                int v1 = (i - 1) * m + j;
                g.add_edge(v0, v1, 1);
            }
            if (j && grid[i][j - 1] == '.') {
                int v1 = i * m + (j - 1);
                g.add_edge(v0, v1, 1);
            }
            if (i + 1 < n && grid[i + 1][j] == '.') {
                int v1 = (i + 1) * m + j;
                g.add_edge(v0, v1, 1);
            }
            if (j + 1 < m && grid[i][j + 1] == '.') {
                int v1 = i * m + (j + 1);
                g.add_edge(v0, v1, 1);
            }
        }
    }

    cout << g.flow(s, t) << endl;

    auto edges = g.edges();
    for (auto e : edges) {
        if (e.from == s || e.to == t || e.flow == 0) continue;
        int i0 = e.from / m, j0 = e.from % m;
        int i1 = e.to / m, j1 = e.to % m;

        if (i0 == i1 + 1) {
            grid[i1][j1] = 'v';
            grid[i0][j0] = '^';
        } else if (j0 == j1 + 1) {
            grid[i1][j1] = '>'; grid[i0][j0] = '<';
        } else if (i0 == i1 - 1) {
            grid[i0][j0] = 'v';
            grid[i1][j1] = '^';
        } else {
            grid[i0][j0] = '>'; grid[i1][j1] = '<';
        }
    }

    for (int i = 0; i < n; i++) {
        cout << grid[i] << endl;
    }

    return 0;
}
```
