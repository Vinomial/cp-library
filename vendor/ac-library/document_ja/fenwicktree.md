# Fenwick Tree

長さ $N$ の配列に対し、

- 要素の $1$ 点変更
- 区間の要素の総和

を $O(\log N)$ で求めることが出来るデータ構造です。

## コンストラクタ

```cpp
fenwick_tree<T> fw(int n)
```

- 長さ $n$ の配列 $a_0, a_1, \cdots, a_{n-1}$ を作ります。初期値はすべて $0$ です。

**制約**

- $T$ は `int / uint / ll / ull / modint`
- $0 \leq n \leq 10^8$

**計算量**

- $O(n)$

## add

```cpp
void fw.add(int p, T x)
```

`a[p] += x` を行う

**制約**

- $0 \leq p < n$

**計算量**

- $O(\log n)$

## sum

```cpp
T fw.sum(int l, int r)
```

`a[l] + a[l - 1] + ... + a[r - 1]` を返す。
`T` が整数型(`int / uint / ll / ull`)の場合、答えがオーバーフローしたならば $\bmod 2^{\mathrm{bit}}$ で等しい値が返る。

**制約**

- $0 \leq l \leq r \leq n$

**計算量**

- $O(\log n)$

## 使用例

### AC code of [https://atcoder.jp/contests/practice2/tasks/practice2_b](https://atcoder.jp/contests/practice2/tasks/practice2_b)

```cpp
#include <atcoder/fenwicktree>
#include <cstdio>

using namespace std;
using namespace atcoder;

int main() {
    int n, q;
    scanf("%d %d", &n, &q);

    fenwick_tree<long long> fw(n);
    for (int i = 0; i < n; i++) {
        int a;
        scanf("%d", &a);
        fw.add(i, a);
    }

    for (int i = 0; i < q; i++) {
        int t;
        scanf("%d", &t);
        if (t == 0) {
            int p, x;
            scanf("%d %d", &p, &x);
            fw.add(p, x);
        } else {
            int l, r;
            scanf("%d %d", &l, &r);
            printf("%lld\n", fw.sum(l, r));
        }
    }
}
```
