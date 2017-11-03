#ifndef _UBIGINT_H_
#define _UBIGINT_H_

#include <vector>
#include <iostream>
#include <string>
using namespace std;

typedef int DType;
const unsigned int DTYPE_X = 10000;
const unsigned int DTYPE_X_LEN = 4;

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

public:
	friend ostream& operator<<(ostream &,UBigInt &);
	friend ostream& operator<<(ostream &,UBigInt &&);
	friend istream& operator>>(istream &, UBigInt &);

protected:
	vector<DType> datas; // 使用DTYPE_X进制记录大数
};

#endif
