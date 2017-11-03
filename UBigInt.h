#ifndef _UBIGINT_H_
#define _UBIGINT_H_

#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

typedef long long DType;
const DType DTYPE_X = 1e9;
const int DTYPE_X_LEN = 9;

class UBigInt{
public:
	UBigInt();
	UBigInt(DType);
	explicit UBigInt(const char *);
	explicit UBigInt(string);

	// 一元运算符
	UBigInt& operator++();//++a
	UBigInt operator++(int dummy);//a++
	UBigInt& operator--();//--a
	UBigInt operator--(int dummy);//a--

	friend UBigInt& operator+=(UBigInt&,const UBigInt&);
	friend UBigInt& operator-=(UBigInt&, const UBigInt&);
	friend UBigInt& operator*=(UBigInt&, const UBigInt&);
	friend UBigInt& operator/=(UBigInt&, const UBigInt&);
	friend UBigInt& operator%=(UBigInt&, const UBigInt&);

	friend UBigInt& UBigIntDivide(UBigInt &a, const UBigInt &b, bool mod);//除法和取模用

	//二元运算
	friend UBigInt operator+(const UBigInt&, const UBigInt &);
	friend UBigInt operator-(const UBigInt&, const UBigInt &);
	friend UBigInt operator*(const UBigInt&, const UBigInt &);
	friend UBigInt operator/(const UBigInt&, const UBigInt &);
	friend UBigInt operator%(const UBigInt&, const UBigInt &);

	//比较,使用友元能实现int < UBigInt
	friend int UBigIntCmp(const UBigInt&,const UBigInt&);	//比较用，返回负数时a<b;	0时a=b;	正数时a>b
	friend bool operator==(const UBigInt&,const UBigInt&);
	friend bool operator!=(const UBigInt&,const UBigInt&);
	friend bool operator<(const UBigInt&, const UBigInt&);
	friend bool operator<=(const UBigInt&, const UBigInt&);
	friend bool operator>(const UBigInt&, const UBigInt&);
	friend bool operator>=(const UBigInt&, const UBigInt&);
	
	// 位运算
	friend DType operator&(const UBigInt&, const DType);
	friend DType operator|(const UBigInt&, const DType);
	friend DType operator^(const UBigInt&, const DType);

public:
	friend ostream& operator<<(ostream &, const UBigInt &);
	friend ostream& operator<<(ostream &, const UBigInt &&);
	friend istream& operator>>(istream &, UBigInt &);

public:
	void random(const UBigInt &n);

private:
	void del_pre_zero();

protected:
	vector<DType> datas; // 使用DTYPE_X进制记录大数
};

void get_ubigint_random(UBigInt &x, const UBigInt &n);

#endif
