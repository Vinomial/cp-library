# Math

数論的アルゴリズム詰め合わせです。

## pow_mod

```cpp
ll pow_mod(ll x, ll n, int m)
```

$x^n \bmod m$ を返します。

**制約**

- $0 \le n$
- $1 \le m$

**計算量**

- $O(\log n)$

## inv_mod

```cpp
ll inv_mod(ll x, ll m)
```

$xy \equiv 1 \pmod m$ なる $y$ のうち、$0 \le y < m$ を満たすものを返します。

**制約**

- $\gcd(x, m) = 1$
- $1 \leq m$

**計算量**

- $O(\log m)$

## crt

```cpp
pair<ll, ll> crt(vector<ll> r, vector<ll> m)
```

同じ長さの配列 $r, m$ を渡します。この配列の長さを $n$ とした時、

$$x \equiv r[i] \pmod{m[i]}, \forall i \in \lbrace 0,1,\cdots, n - 1 \rbrace$$

を解きます。答えは(存在するならば) $y, z (0 \leq y < z = \mathrm{lcm}(m[i]))$ を用いて $x \equiv y \pmod z$ の形で書けることが知られており、この $(y, z)$ をpairとして返します。答えがない場合は $(0, 0)$ を返します。$n=0$ の時は $(0, 1)$ を返します。

**制約**

- $|r| = |m|$
- $1 \le m[i]$
- $\mathrm{lcm}(m[i])$ が `ll` に収まる。

**計算量**

- $O(n \log{\mathrm{lcm}(m[i])})$

## floor_sum

```cpp
ll floor_sum(ll n, ll m, ll a, ll b)
```

$\sum_{i = 0}^{n - 1} \mathrm{floor}(\frac{a \times i + b}{m})$

を返します。

**制約**

- $0 \leq n \leq 10^9$
- $1 \leq m \leq 10^9$
- $0 \leq a, b \lt m$

**計算量**

- $O(\log{(n+m+a+b)})$

## 使用例

### AC code of [https://atcoder.jp/contests/practice2/tasks/practice2_c](https://atcoder.jp/contests/practice2/tasks/practice2_c)

```cpp
#include <atcoder/math>
#include <cstdio>

using namespace std;
using namespace atcoder;

int main() {
    int t;
    scanf("%d", &t);
    for (int i = 0; i < t; i++) {
        long long n, m, a, b;
        scanf("%lld %lld %lld %lld", &n, &m, &a, &b);
        printf("%lld\n", floor_sum(n, m, a, b));
    }
    return 0;
}
```
