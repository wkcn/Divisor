#include <vector>
#include <stack>
using namespace std;

// (a^s) mod n
template <typename T>
T powermod(T a, T s, T n){
	T p = 1;
	T b = a;
	while (s > 0){
		// 如果s是奇数
		if (s & 1){
			p = (p * b) % n;
		}
		b = (b * b) % n;
		s /= 2;
	}
	return p;
}

// (a*s) mod n
template <typename T>
T mulmod(T a, T s, T n){
	return (a * s) % n;
}

// a^n
template <typename T>
T pow(T a, T n){
	T p = 1;
	T b = a;
	while (n > 0){
		if (n & 1){
			p *= b;
		}
		b *= b;
		n /= 2;
	}
	return p;
}

// int(log2(x))
template <typename T>
int log2(T x){
	stack<T> st;
	st.push(2);
	int r = 0;
	int k = 1;
	while (1){
		T &b = st.top();
		if (b > x)break;
		x /= b;
		r += k;
		st.push(b * b);
		k <<= 1;
	}
	st.pop();
	k >>= 1;
	while (!st.empty()){
		T &b = st.top();
		while (b <= x){
			x /= b;
			r += k;
		}
		st.pop();
		k >>= 1;
	}
	return r;
}

template <typename T>
int factorn(T &x, T n){
	stack<T> st;
	st.push(n);
	int r = 0;
	int k = 1;
	while (1){
		T &b = st.top();
		if (b > x)break;
		if (x % b != 0)break;
		x /= b;
		r += k;
		st.push(b * b);
		k <<= 1;
	}
	st.pop();
	k >>= 1;
	while (!st.empty()){
		T &b = st.top();
		while (b <= x){
			if (x % b != 0)break;
			x /= b;
			r += k;
		}
		st.pop();
		k >>= 1;
	}
	return r;
}

#include <iostream>
using namespace std;
template <typename T>
T gcd(T a, T b){
	if (a > b)return gcd(b, a);
	while (1){
		b %= a;
		if (b == 0)return a;
		a %= b;
		if (a == 0)return b;
	}
	return 0;
}
