# Modint

自動でmodを取る構造体です。AC Libraryはmodintを使わなくとも全アルゴリズムが使えるように整備しているので、必ずしもこのファイルの内容を把握する必要はありません。

多くの問題では `modint998244353`, `modint1000000007`, `modint` のどれかを使えば十分で、以下のように使えます。

```cpp
#include <atcoder/modint>
#include <iostream>

using namespace std;
using namespace atcoder;

using mint = modint998244353;
// or: typedef modint998244353 mint;

int main() {
    // print sum of array (mod 998244353)
    int n;
    cin >> n;
    mint sum = 0;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        sum += x;
    }
    cout << sum.val() << endl;
}
```

modがfixedでない場合は、`modint` を使用し以下のように書けます。

```cpp
#include <atcoder/modint>
#include <iostream>

using namespace std;

using mint = modint;
// or: typedef modint mint;

int main() {
    // print sum of array (input mod)
    int n, mod;
    cin >> n >> mod;
    mint::set_mod(mod);
    mint sum = 0;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        sum += x;
    }
    cout << sum.val() << endl;
}
```

以下の関数らは、`set_mod` を除き $3$ つともに対して動きます。

## コンストラクタ

```cpp
(1) modint x()
(2) modint x<T>(T y)
```

- (1) modintのデフォルトコンストラクタです。$0$ が格納されます。
- (2) 整数型 `T`(`int, char, ull, bool, ...`) に対するコンストラクタです。`y` のmodを取ってmodintに格納します。

## set_mod

```cpp
void modint::set_mod(int m)
```

modを設定します。最初に呼んでください。

**制約**

- $1 \leq m \leq 2 \times 10^9 + 1000$

**計算量**

- $O(1)$

## mod

```cpp
int modint::mod()
```

modを返します。

## val

```cpp
int x.val();
```

`x`に格納されている値を返します。

## 各種演算

```cpp
-modint;

modint++;
modint--;
++modint;
--modint;

modint + modint;
modint - modint;
modint * modint;
modint / modint;

modint += modint;
modint -= modint;
modint *= modint;
modint /= modint;

modint == modint;
modint != modint;
```

が動きます。

```cpp
modint x = 10;
1 + x;
```

も(`modint(1) + x`と自動で解釈されるので)動きます。

```cpp
modint::set_mod(11);
modint y = 10;
int z = 1234;
y * z;
```

も`y * modint(z)`と解釈され、動きます。

**制約**

- `a / b`(or `a /= b`)を行なう時、`gcd(b.val(), mod) == 1`

**計算量**

- $O(1)$ (割り算以外)
- $O(\log \mathrm{mod})$ (割り算)

## pow

```cpp
modint x.pow(ll n)
```

$x^n$ を返します。

**制約**

- $0 \le n$

**計算量**

- $O(\log n)$

## inv

```cpp
modint x.inv()
```

$xy \equiv 1$ なる $y$ を返します。

**制約**

- `gcd(x.val(), mod) = 1`

**計算量**

- $O(\log \mathrm{mod})$

## raw

```cpp
modint modint::raw(int x)
```

`x`に対してmodを取らずに、`modint(x)`を返す。

定数倍高速化のための関数です。

上で述べたように

```cpp
modint a;
int i;
a += i;
```

は、`i`がmod以上でも動きます。勝手に`i`に対してmodを取るためです。

ですが、たとえば以下のようなコードでは、`i`がmodを超えないことを保証できます。

```cpp
int main() {
    modint::set_mod(1000000007);
    modint a = 1;
    for (int i = 1; i < 100000; i++) {
        a += i;
    }
}
```

このような時に、

```cpp
int main() {
    modint::set_mod(1000000007);
    modint a = 1;
    for (int i = 1; i < 100000; i++) {
        a += modint::raw(i);
    }
}
```

と書くと、modの回数を減らすことが出来ます。

当然ながら`modint::raw(x)`にmod以上の値を入れたときの挙動は未定義です。

**制約**

- $0 \leq x \lt \mathrm{mod}$

## Tips(other mod)

問題文で他のmod (例: `1000000009`) が与えられる場合、以下のように書けます

```cpp
using mint = static_modint<1000000009>;
```

`modint998244353`, `modint1000000007` は、`static_modint<998244353>`, `static_modint<1000000007>`のエイリアスになっています。

```cpp
using modint998244353 = static_modint<998244353>;
using modint1000000007 = static_modint<1000000007>;
```

## Tips(複数mod)

複数種類modを使用したい場合以下のようにできます

```cpp
using mint0 = dynamic_modint<0>;
using mint1 = dynamic_modint<1>;
```

`modint`は、`dynamic_modint<-1>`のエイリアスになっています。

```cpp
using modint = dynamic_modint<-1>;
```

## 使用例

### How to Use

```cpp
#include <atcoder/modint>
#include <cstdio>

using namespace std;
using namespace atcoder;

using mint = static_modint<11>;
int main() {
    mint a = 10;
    mint b(3);

    // equal
    assert(a == 21);
    assert(a == -1);
    assert(-1 == a);

    // negative
    assert(-b == 8);

    // plus
    assert(a + b == 2);  // (10 + 3) mod 11
    assert(1 + a == 0);

    // minus
    assert(a - b == 7);  // (10 - 3) mod 11
    assert(b - a == 4);

    // mul
    assert(a * b == 8);  // (10 * 3) mod 11

    // inv
    assert(b.inv() == 4);  // (3 * 4) mod 11 == 1

    // div
    assert(a / b == 7);  // (10 * 4) mod 11

    // +=, -=, *=, /=
    a += b;
    assert(a == 2 && b == 3);
    a -= b;
    assert(a == 10 && b == 3);
    a *= b;
    assert(a == 8 && b == 3);
    a /= b;
    assert(a == 10 && b == 3);

    // pow
    assert(mint(2).pow(4) == 5);  // 16 mod 11

    // print value
    printf("%d\n", a.val());  // 10

    // get mod
    assert(mint::mod() == 11 && a.mod() == 11);

    // mint(x) と書くとmodを取る操作が発生します((x % mod + mod) % modをmodintに代入します)
    // mint::raw(x) はxをmodを取らずに代入するので高速です(もちろんxが[0, mod)であることを利用者が保証しないといけません)
    assert(mint::raw(3) == 3);
}
```
