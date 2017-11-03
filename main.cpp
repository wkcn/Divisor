#include <iostream>
#include <cmath>
#include <cassert>
#include <vector>
#include <cstring>
#include <map>
#include <set>
#include <ctime>
#include <algorithm>
using namespace std;

#include "UBigInt.h"
#include "mathlib.h"

int test_op(){
	UBigInt a; 
	UBigInt b;
	while(1){
		cin >> a;
		cin >> b;
		cout << "a + b = " << a + b << endl;
		cout << "a - b = " << a - b << endl;
		cout << "a * b = " << a * b << endl;
		cout << "a / b = " << a / b << endl;
		cout << "a % b = " << a % b << endl;
	}
	return 0;
}

const int RABIN_MILLER_TEST_TIMES = 50;
bool RabinMillerTest(const UBigInt &n){
	UBigInt t = log2(n);
	t *= t;
	t *= 2;
	if (t > n)t = n;
	UBigInt mo = n - 1;
	if (t > RABIN_MILLER_TEST_TIMES + 2){
		for (int i = 0;i < RABIN_MILLER_TEST_TIMES;++i){
			UBigInt a;
			a.random(t);
			a += 2;
			if (powermod(a, mo, n) != 1)return false;
		}
	}else{
		for (UBigInt a = 2;a < t;++a){
			if (powermod(a, mo, n) != 1)return false;
		}
	}
	return true;
	/*
	UBigInt t = log2(x);
	t *= t;
	t *= 2;
	if (t > x)t = x;
	UBigInt mo = x - 1;
	for (UBigInt a = 2;a < t;++a){
		if (powermod(a, mo, x) != 1)return false;
	}
	return true;
	*/
	/*
	if (n == 2)return true;
	if (n < 2 || !(n & 1))return false;
	UBigInt minus_two = n - 2;
	UBigInt minus_one = n - 1;
	for (int i = 0;i < RABIN_MILLER_TEST_TIMES;++i){
		UBigInt a;
		a.random(minus_two);
		++a;
		UBigInt mo = minus_one;
		int j = factorn(mo, UBigInt(2));
		UBigInt x = powermod(a, mo, n); 
		// ?
		cout << x << ", " << n << endl;
		if (x == 1 || x == minus_one)return true;
		while ((j--) != 0){
			x *= x; 
			x %= n;
			if (x == minus_one)return true;
		}
	}
	return false;
	*/
}

bool LucasLehmerTest(const UBigInt &x){
	// x = 3为特例?
	int t = log2(x + 1) - 2;
	UBigInt L = 4;
	for (int i = 0;i < t;++i){
		L *= L;
		L -= 2;
		L %= x;
		if (L == 0)return true;
	}
	return false;
}

inline void FuncG(UBigInt &x, const UBigInt &n){
	x *= x;
	x += 2;
	x %= n;
}

bool PollardBrent(const UBigInt &n, UBigInt &d){
	UBigInt x, y;
	d = 1;
	while (d == 1){
		x.random(n);	
		y = x;	
		do{
			FuncG(x, n);
			FuncG(y, n);FuncG(y, n);
			if (x == y)break;
			UBigInt w = (x > y ? x - y : y - x);
			d = gcd(w, n);
			if (d > 1)return true;
		}while(x != y);
	}
	return false;
}

void init_small_primes_table(vector<UBigInt> &table){
	const int SMALL_PRIMES_UPPER = 101;
	bool isprimes[SMALL_PRIMES_UPPER];
	memset(isprimes, 1, sizeof(isprimes));
	isprimes[0] = 0;
	for (int i = 2;i <= sqrt(SMALL_PRIMES_UPPER);++i){
		if (isprimes[i]){
			for (int j = i + i;j < SMALL_PRIMES_UPPER; j += i){
				isprimes[j] = false;
			}
		}
	}
	for (int i = 2;i < SMALL_PRIMES_UPPER;++i){
		if (isprimes[i])
			table.push_back(i);
	}
}

void Factor(UBigInt &x, map<UBigInt, int> &primes){
	if (!RabinMillerTest(x)){
		UBigInt r1;
		PollardBrent(x, r1);
		UBigInt r2 = x / r1; 
		Factor(r1, primes);
		Factor(r2, primes);
	}else{
		++primes[x];
	}
}

int main(){
	clock_t start_time = clock();
	vector<UBigInt> small_primes_table;
	init_small_primes_table(small_primes_table);
	UBigInt n;
	cin >> n;
	map<UBigInt, int> primes;

	for (UBigInt &p : small_primes_table){
		// 可以限制大小, 及时break
		int ti = factorn(n, p);
		if (ti)primes[p] += ti;
	}
	Factor(n, primes);

#if 0
	cout << "===============" << endl;
	for (auto it = primes.begin();it != primes.end();++it){
		cout << it->first << "^" << it->second << endl;
	}
	cout << "===============" << endl;
#endif

	// remove 1
	primes.erase(UBigInt(1));

	int s = 1; 
	for (auto it = primes.begin();it != primes.end();++it){
		s += s * it->second;
	}
	vector<UBigInt> all(s);
	all[0] = 1;
	int ns = 1;
	for (auto it = primes.begin();it != primes.end();++it){
		const UBigInt &x = it->first;
		int time = it->second;
		int b = ns;
		UBigInt base = 1;
		for (int t = 0;t < time;++t){
			base *= x;
			for (int i = 0;i < ns;++i){
				all[b++] = all[i] * base; 
			}
		}
		ns = b;
	}
	sort(all.begin(), all.end());

	clock_t end_time = clock();
	printf("%d ms\n", (int(end_time) - int(start_time)) * 1000 / CLOCKS_PER_SEC);
	for (UBigInt &u : all){
		u.print();
		putchar('\n');
	}
	return 0;
}
