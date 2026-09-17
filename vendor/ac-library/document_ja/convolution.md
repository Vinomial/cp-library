# Convolution

畳み込みを行います。数列 $a_0, a_1, \cdots, a_{N - 1}$ と数列 $b_0, b_1, \cdots, b_{M - 1}$ から、長さ $N + M - 1$ の数列

$$c_i = \sum_{j = 0}^i a_j b_{i - j}$$

を計算します。

## convolution

```cpp
(1) vector<T> convolution<int m = 998244353>(vector<T> a, vector<T> b)
💻(2) vector<static_modint<m>> convolution<int m>(vector<static_modint<m>> a, vector<static_modint<m>> b)
```

畳み込みを $\bmod m$ で計算します。$a, b$ の少なくとも一方が空配列の場合は空配列を返します。

**制約**

- $2 \leq m \leq 2 \times 10^9$
- $\mathrm{m}$ は素数
- $2^c | (\mathrm{m} - 1)$ かつ $|a| + |b| - 1 \leq 2^c$ なる $c$ が存在する
- (1) `T`は`int, uint, ll, ull`

**計算量**

$n = |a| + |b|$ として

- $O(n\log{n} + \log{\mathrm{mod}})$

## convolution_ll

```cpp
vector<ll> convolution_ll(vector<ll> a, vector<ll> b)
```

畳み込みを計算します。$a, b$ の少なくとも一方が空配列の場合は空配列を返します。

**制約**

- $|a| + |b| - 1 \leq 2^{24}$
- 畳み込んだ後の配列の要素が全て`ll`に収まる

**計算量**

$n = |a| + |b|$ として

- $O(n\log{n})$

## 使用例

### AC code of [https://atcoder.jp/contests/practice2/tasks/practice2_f](https://atcoder.jp/contests/practice2/tasks/practice2_f)

```cpp
#include <atcoder/convolution>
#include <atcoder/modint>
#include <cstdio>

using namespace std;
using namespace atcoder;

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    vector<long long> a(n), b(m);
    for (int i = 0; i < n; i++) {
        scanf("%lld", &(a[i]));
    }
    for (int i = 0; i < m; i++) {
        scanf("%lld", &(b[i]));
    }

    vector<long long> c = convolution(a, b);
    // or: vector<long long> c = convolution<998244353>(a, b);

    for (int i = 0; i < n + m - 1; i++) {
        printf("%lld ", c[i]);
    }
    printf("\n");

    return 0;
}
```

### AC code of [https://atcoder.jp/contests/practice2/tasks/practice2_f](https://atcoder.jp/contests/practice2/tasks/practice2_f)

```cpp
#include <atcoder/convolution>
#include <atcoder/modint>
#include <cstdio>

using namespace std;
using namespace atcoder;

using mint = modint998244353;

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    vector<mint> a(n), b(m);
    for (int i = 0; i < n; i++) {
        int x;
        scanf("%d", &x);
        a[i] = x;
    }
    for (int i = 0; i < m; i++) {
        int x;
        scanf("%d", &x);
        b[i] = x;
    }

    auto c = convolution(a, b);

    for (int i = 0; i < n + m - 1; i++) {
        printf("%d ", c[i].val());
    }
    printf("\n");

    return 0;
}
```
