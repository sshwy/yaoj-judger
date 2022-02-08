// https://loj.ac/s/1305222
#include <bits/stdc++.h>
using namespace std;
#define pb push_back
#define mp make_pair
typedef pair<int,int> pii;
typedef long long ll;
typedef double ld;
typedef vector<int> vi;
#define fi first
#define se second
#define fe first
#define FO(x) {freopen(#x".in","r",stdin);freopen(#x".out","w",stdout);}
#define Edg int M=0,fst[SZ],vb[SZ],nxt[SZ];void ad_de(int a,int b){++M;nxt[M]=fst[a];fst[a]=M;vb[M]=b;}void adde(int a,int b){ad_de(a,b);ad_de(b,a);}
#define Edgc int M=0,fst[SZ],vb[SZ],nxt[SZ],vc[SZ];void ad_de(int a,int b,int c){++M;nxt[M]=fst[a];fst[a]=M;vb[M]=b;vc[M]=c;}void adde(int a,int b,int c){ad_de(a,b,c);ad_de(b,a,c);}
#define es(x,e) (int e=fst[x];e;e=nxt[e])
#define esb(x,e,b) (int e=fst[x],b=vb[e];e;e=nxt[e],b=vb[e])
const int md = 998244353;

inline void add(int &x, int y) {
  x += y;
  if (x >= md) {
    x -= md;
  }
}

inline void sub(int &x, int y) {
  x -= y;
  if (x < 0) {
    x += md;
  }
}

inline int mul(int x, int y) {
  return (int) ((long long) x * y % md);
}

inline int power(int x, int y) {
  int res = 1;
  while (y) {
    if (y & 1) {
      res = mul(res, x);
    }
    x = mul(x, x);
    y >>= 1;
  }
  return res;
}

inline int inv(int a) {
  if (a < 0) {
    a += md;
  }
  int b = md, u = 0, v = 1;
  while (a) {
    int t = b / a;
    b -= t * a;
    swap(a, b);
    u -= t * v;
    swap(u, v);
  }
  if (u < 0) {
    u += md;
  }
  return u;
}

namespace ntt {
int base = 1, root = -1, max_base = -1;
vector<int> rev = {0, 1}, roots = {0, 1};

void init() {
  int temp = md - 1;
  max_base = 0;
  while (temp % 2 == 0) {
    temp >>= 1;
    ++max_base;
  }
  root = 2;
  while (true) {
    if (power(root, 1 << max_base) == 1 && power(root, 1 << (max_base - 1)) != 1) {
      break;
    }
    ++root;
  }
}

void ensure_base(int nbase) {
  if (max_base == -1) {
    init();
  }
  if (nbase <= base) {
    return;
  }
  assert(nbase <= max_base);
  rev.resize(1 << nbase);
  for (int i = 0; i < 1 << nbase; ++i) {
    rev[i] = rev[i >> 1] >> 1 | (i & 1) << (nbase - 1);
  }
  roots.resize(1 << nbase);
  while (base < nbase) {
    int z = power(root, 1 << (max_base - 1 - base));
    for (int i = 1 << (base - 1); i < 1 << base; ++i) {
      roots[i << 1] = roots[i];
      roots[i << 1 | 1] = mul(roots[i], z);
    }
    ++base;
  }
}

void dft(vector<int> &a) {
  int n = a.size(), zeros = __builtin_ctz(n);
  ensure_base(zeros);
  int shift = base - zeros;
  for (int i = 0; i < n; ++i) {
    if (i < rev[i] >> shift) {
      swap(a[i], a[rev[i] >> shift]);
    }
  }
  for (int i = 1; i < n; i <<= 1) {
    for (int j = 0; j < n; j += i << 1) {
      for (int k = 0; k < i; ++k) {
        int x = a[j + k], y = mul(a[j + k + i], roots[i + k]);
        a[j + k] = (x + y) % md;
        a[j + k + i] = (x + md - y) % md;
      }
    }
  }
}

vector<int> multiply(vector<int> a, vector<int> b) {
  int need = a.size() + b.size() - 1, nbase = 0;
  while (1 << nbase < need) {
    ++nbase;
  }
  ensure_base(nbase);
  int sz = 1 << nbase;
  a.resize(sz);
  b.resize(sz);
  bool equal = a == b;
  dft(a);
  if (equal) {
    b = a;
  } else {
    dft(b);
  }
  int inv_sz = inv(sz);
  for (int i = 0; i < sz; ++i) {
    a[i] = mul(mul(a[i], b[i]), inv_sz);
  }
  reverse(a.begin() + 1, a.end());
  dft(a);
  a.resize(need);
  return a;
}

vector<int> inverse(vector<int> a) {
  int n = a.size(), m = (n + 1) >> 1;
  if (n == 1) {
    return vector<int>(1, inv(a[0]));
  } else {
    vector<int> b = inverse(vector<int>(a.begin(), a.begin() + m));
    int need = n << 1, nbase = 0;
    while (1 << nbase < need) {
      ++nbase;
    }
    ensure_base(nbase);
    int sz = 1 << nbase;
    a.resize(sz);
    b.resize(sz);
    dft(a);
    dft(b);
    int inv_sz = inv(sz);
    for (int i = 0; i < sz; ++i) {
      a[i] = mul(mul(md + 2 - mul(a[i], b[i]), b[i]), inv_sz);
    }
    reverse(a.begin() + 1, a.end());
    dft(a);
    a.resize(n);
    return a;
  }
}
}

using ntt::multiply;
using ntt::inverse;

vector<int>& operator += (vector<int> &a, const vector<int> &b) {
  if (a.size() < b.size()) {
    a.resize(b.size());
  }
  for (int i = 0; i < (int) b.size(); ++i) {
    add(a[i], b[i]);
  }
  return a;
}

vector<int> operator + (const vector<int> &a, const vector<int> &b) {
  vector<int> c = a;
  return c += b;
}

vector<int>& operator -= (vector<int> &a, const vector<int> &b) {
  if (a.size() < b.size()) {
    a.resize(b.size());
  }
  for (int i = 0; i < (int) b.size(); ++i) {
    sub(a[i], b[i]);
  }
  return a;
}

vector<int> operator - (const vector<int> &a, const vector<int> &b) {
  vector<int> c = a;
  return c -= b;
}

vector<int>& operator *= (vector<int> &a, const vector<int> &b) {
  if ((int) min(a.size(), b.size()) < 128) {
    vector<int> c = a;
    a.assign(a.size() + b.size() - 1, 0);
    for (int i = 0; i < (int) c.size(); ++i) {
      for (int j = 0; j < (int) b.size(); ++j) {
        add(a[i + j], mul(c[i], b[j]));
      }
    }
  } else {
    a = multiply(a, b);
  }
  return a;
}

vector<int> operator * (const vector<int> &a, const vector<int> &b) {
  vector<int> c = a;
  return c *= b;
}

vector<int>& operator /= (vector<int> &a, const vector<int> &b) {
  int n = a.size(), m = b.size();
  if (n < m) {
    a.clear();
  } else {
    vector<int> c = b;
    reverse(a.begin(), a.end());
    reverse(c.begin(), c.end());
    c.resize(n - m + 1);
    a *= inverse(c);
    a.erase(a.begin() + n - m + 1, a.end());
    reverse(a.begin(), a.end());
  }
  return a;
}

vector<int> operator / (const vector<int> &a, const vector<int> &b) {
  vector<int> c = a;
  return c /= b;
}

vector<int>& operator %= (vector<int> &a, const vector<int> &b) {
  int n = a.size(), m = b.size();
  if (n >= m) {
    vector<int> c = (a / b) * b;
    a.resize(m - 1);
    for (int i = 0; i < m - 1; ++i) {
      sub(a[i], c[i]);
    }
  }
  return a;
}

vector<int> operator % (const vector<int> &a, const vector<int> &b) {
  vector<int> c = a;
  return c %= b;
}

vector<int> derivative(const vector<int> &a) {
  int n = a.size();
  vector<int> b(n - 1);
  for (int i = 1; i < n; ++i) {
    b[i - 1] = mul(a[i], i);
  }
  return b;
}

vector<int> primitive(const vector<int> &a) {
  int n = a.size();
  vector<int> b(n + 1), invs(n + 1);
  for (int i = 1; i <= n; ++i) {
    invs[i] = i == 1 ? 1 : mul(md - md / i, invs[md % i]);
    b[i] = mul(a[i - 1], invs[i]);
  }
  return b;
}

vector<int> logarithm(const vector<int> &a) {
  vector<int> b = primitive(derivative(a) * inverse(a));
  b.resize(a.size());
  return b;
}

vector<int> exponent(const vector<int> &a) {
  vector<int> b(1, 1);
  while (b.size() < a.size()) {
    vector<int> c(a.begin(), a.begin() + min(a.size(), b.size() << 1));
    add(c[0], 1);
    vector<int> old_b = b;
    b.resize(b.size() << 1);
    c -= logarithm(b);
    c *= old_b;
    for (int i = b.size() >> 1; i < (int) b.size(); ++i) {
      b[i] = c[i];
    }
  }
  b.resize(a.size());
  return b;
}

vector<int> power(const vector<int> &a, int m) {
  int n = a.size(), p = -1;
  vector<int> b(n);
  for (int i = 0; i < n; ++i) {
    if (a[i]) {
      p = i;
      break;
    }
  }
  if (p == -1) {
    b[0] = !m;
    return b;
  }
  if ((long long) m * p >= n) {
    return b;
  }
  int mu = power(a[p], m), di = inv(a[p]);
  vector<int> c(n - m * p);
  for (int i = 0; i < n - m * p; ++i) {
    c[i] = mul(a[i + p], di);
  }
  c = logarithm(c);
  for (int i = 0; i < n - m * p; ++i) {
    c[i] = mul(c[i], m);
  }
  c = exponent(c);
  for (int i = 0; i < n - m * p; ++i) {
    b[i + m * p] = mul(c[i], mu);
  }
  return b;
}

vector<int> sqrt(const vector<int> &a) {
  vector<int> b(1, 1);
  while (b.size() < a.size()) {
    vector<int> c(a.begin(), a.begin() + min(a.size(), b.size() << 1));
    vector<int> old_b = b;
    b.resize(b.size() << 1);
    c *= inverse(b);
    for (int i = b.size() >> 1; i < (int) b.size(); ++i) {
      b[i] = mul(c[i], (md + 1) >> 1);
    }
  }
  b.resize(a.size());
  return b;
}

vector<int> multiply_all(int l, int r, vector<vector<int>> &all) {
  if (l > r) {
    return vector<int>(1, 1);
  } else if (l == r) {
    return all[l];
  } else {
    int y = (l + r) >> 1;
    return multiply_all(l, y, all) * multiply_all(y + 1, r, all);
  }
}

vector<int> evaluate(const vector<int> &f, const vector<int> &x) {
  int n = x.size();
  if (!n) {
    return vector<int>();
  }
  vector<vector<int>> up(n * 2);
  for (int i = 0; i < n; ++i) {
    up[i + n] = vector<int>{(md - x[i]) % md, 1};
  }
  for (int i = n - 1; i; --i) {
    up[i] = up[i << 1] * up[i << 1 | 1];
  }
  vector<vector<int>> down(n * 2);
  down[1] = f % up[1];
  for (int i = 2; i < n * 2; ++i) {
    down[i] = down[i >> 1] % up[i];
  }
  vector<int> y(n);
  for (int i = 0; i < n; ++i) {
    y[i] = down[i + n][0];
  }
  return y;
}

vector<int> interpolate(const vector<int> &x, const vector<int> &y) {
  int n = x.size();
  vector<vector<int>> up(n * 2);
  for (int i = 0; i < n; ++i) {
    up[i + n] = vector<int>{(md - x[i]) % md, 1};
  }
  for (int i = n - 1; i; --i) {
    up[i] = up[i << 1] * up[i << 1 | 1];
  }
  vector<int> a = evaluate(derivative(up[1]), x);
  for (int i = 0; i < n; ++i) {
    a[i] = mul(y[i], inv(a[i]));
  }
  vector<vector<int>> down(n * 2);
  for (int i = 0; i < n; ++i) {
    down[i + n] = vector<int>(1, a[i]);
  }
  for (int i = n - 1; i; --i) {
    down[i] = down[i << 1] * up[i << 1 | 1] + down[i << 1 | 1] * up[i << 1];
  }
  return down[1];
}

int linear_recurrence(vector<int> a, vector<int> f, long long n) {
  int m = a.size();
  if (n < m) {
    return f[n];
  }
  int zero = 0;
  while (m && !a.back()) {
    a.pop_back();
    ++zero;
    --n;
    --m;
  }
  if (!m) {
    return 0;
  }
  f.erase(f.begin(), f.begin() + zero);
  vector<int> inv_a = a;
  reverse(inv_a.begin(), inv_a.end());
  inv_a = inverse(inv_a);
  function<vector<int>(long long)> solve = [&](long long n) {
    if (n == 1) {
      return a;
    } else if (n & 1) {
      vector<int> b = solve(n - 1);
      int coef = b[0];
      for (int i = 0; i < m; ++i) {
        b[i] = mul(a[i], coef);
        if (i < m - 1) {
          add(b[i], b[i + 1]);
        }
      }
      return b;
    } else {
      vector<int> b = solve(n >> 1);
      b = b * b;
      vector<int> c = b;
      reverse(c.begin(), c.end());
      c *= inv_a;
      c.resize(m);
      reverse(c.begin(), c.end());
      c[0] = 0;
      vector<int> d = c * a;
      b.resize(m);
      for (int i = 0; i < m; ++i) {
        sub(b[i], d[i]);
        if (i < m - 1) {
          add(b[i], c[i + 1]);
        }
      }
      return b;
    }
  };
  vector<int> ans = solve(n - m + 1);
  int res = 0;
  for (int i = 0; i < m; ++i) {
    add(res, mul(f[i], ans[m - i - 1]));
  }
  return res;
}
#define MOD md
vector<int> et(vector<int> s)
{
    vector<int> w(s.size());
    for(int j=1;j<s.size();++j)
    {
        ll ww=inv(j);
        for(int k=j,c=1;k<s.size();k+=j,++c)
            w[k]=(w[k]+s[c]*(ll)ww)%MOD;
    }
    w=exponent(w);
    w.resize(s.size());
    --w[0]; return w;
}

vector<int> solve(int n) {
  vector<int> f(1, 0);
  f.push_back(1);
  while (f.size() < n) {
    vector<int> old_f = f;
    
    vector<int> c = vector<int>(1,md-2) * f;
    c[0]+=2;
    c[0]%=md;
    vector<int> invc = inverse(c);
    c+=f;
    vector<int> d=c*f*invc;
    
    vector<int> e=vector<int>(1,2)*d*invc;
    e[0]++;
    e[0]%=md;
    vector<int> h(2);h[1]=1;
    vector<int> g=exponent(d)*h;
    e=e*g;
    e[0]--;
    e[0]=(e[0]%md+md)%md;
    e=inverse(e);
    
    vector<int> new_f=old_f-(g-old_f)*e;
    
    f=new_f;
    f.resize(old_f.size() << 1);
    
//    for(auto x:f) printf("%d ",x);printf("\n");
    
  }
  f.resize(n);
  return f;
}

int fac[300005];
int main()
{
	fac[0]=1;
	for(int i=1;i<=262143;i++) fac[i]=1ll*fac[i-1]*i%md;
	int n=262143;
	vector<int> f=solve(n);
	int T,x;
	scanf("%d",&T);
	while(T--){
		scanf("%d",&x);
		printf("%d\n",1ll*f[x]*fac[x-1]%md);
	}
}
