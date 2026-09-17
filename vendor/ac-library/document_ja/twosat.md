# 2-SAT

2-SATを解きます。
変数 $x_0, x_1, \cdots, x_{N - 1}$ に関して、

- $(x_i = f) \lor (x_j = g)$

というクローズを足し、これをすべて満たす変数の割当があるかを解きます。

## コンストラクタ

```cpp
two_sat ts(int n)
```

$n$ 変数の2-SATを作ります。

**制約**

- $0 \leq n \leq 10^8$

**計算量**

- $O(n)$

## add_clause

```cpp
void ts.add_clause(int i, bool f, int j, bool g)
```

$(x_i = f) \lor (x_j = g)$ というクローズを足します。

**制約**

- $0 \leq i \lt n$
- $0 \leq j \lt n$

**計算量**

- ならし $O(1)$

## satisfiable

```cpp
bool ts.satisfiable()
```

条件を足す割当が存在するかどうかを判定する。割当が存在するならば`true`、そうでないなら`false`を返す。

**制約**

- 複数回呼ぶことも可能。

**計算量**

足した制約の個数を $m$ として

- $O(n + m)$

## answer

```cpp
vector<bool> ts.answer()
```

最後に呼んだ `satisfiable` の、クローズを満たす割当を返す。`satisfiable` を呼ぶ前や、`satisfiable` で割当が存在しなかったときにこの関数を呼ぶと、中身が未定義の長さ $n$ の vectorを返す。

**計算量**

- $O(n)$

## 使用例

### AC code of [https://atcoder.jp/contests/practice2/tasks/practice2_h](https://atcoder.jp/contests/practice2/tasks/practice2_h)

```cpp
#include <atcoder/twosat>
#include <iostream>
#include <vector>

using namespace std;
using namespace atcoder;

int main() {
    int n, d;
    cin >> n >> d;
    vector<int> x(n), y(n);
    for (int i = 0; i < n; i++) {
        cin >> x[i] >> y[i];
    }

    // ts[i] = (i-th flag is located on x[i])
    two_sat ts(n);

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (abs(x[i] - x[j]) < d) {
                // cannot use both of x[i] and x[j]
                ts.add_clause(i, false, j, false);
            }
            if (abs(x[i] - y[j]) < d) {
                ts.add_clause(i, false, j, true);
            }
            if (abs(y[i] - x[j]) < d) {
                ts.add_clause(i, true, j, false);
            }
            if (abs(y[i] - y[j]) < d) {
                ts.add_clause(i, true, j, true);
            }
        }
    }

    if (!ts.satisfiable()) {
        cout << "No" << endl;
        return 0;
    }

    cout << "Yes" << endl;
    auto answer = ts.answer();
    for (int i = 0; i < n; i++) {
        if (answer[i])
            cout << x[i] << endl;
        else
            cout << y[i] << endl;
    }

    return 0;
}
```
