#include "UBigInt.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <stdexcept>
#include <algorithm>

using namespace std;

UBigInt::UBigInt():datas(1, 0){
}

UBigInt::UBigInt(DType u){
	if (u == 0){
		datas.push_back(0);
		return;
	}
	while (u > 0){
		datas.push_back(u % DTYPE_X);
		u /= DTYPE_X;
	}
}

bool IsSpaceChar(char c){
	return (c == ' ') || (c == '\n') || (c == '\t') || (c == '\r') || (c == '\b');
}

bool IsNumChar(char c){
	return (c >= '0' && c <= '9');
}

enum class UBIGINT_READ_STATE{
	START, READING, END
}; // 分别指还未读取(包括读取间隔符号)，正在读取，读取结束

UBigInt::UBigInt(const char *s){
	int slen = strlen(s);
	DType base = 1; // 基数
	DType buf = 0; // 缓存 
	UBIGINT_READ_STATE state = UBIGINT_READ_STATE::START;
	// 可以接受字符串:"      123      "
	for (int i = slen - 1;i >= 0;--i){
		char c = s[i];
		if (IsSpaceChar(c)){
			// 当state为START或END的时候，跳过间隔符号
			if (state == UBIGINT_READ_STATE::READING){
				state = UBIGINT_READ_STATE::END;
				break;
			}
		}else{
			// 非间隔字符
			if (IsNumChar(c)){
				if (state == UBIGINT_READ_STATE::START){
					state = UBIGINT_READ_STATE::READING;
				}else if (state == UBIGINT_READ_STATE::END){
					throw runtime_error("非法输入，传入了多段输入:-(");
				}
				// READING
				DType k = c - '0';
				buf += k * base; 
				if (base == DTYPE_X){
					// 缓存已满
					base = 1;
					datas.push_back(buf);
					buf = 0;
				}else{
					base *= 10;
				}
			}else{
				throw runtime_error("您的输入包含了非数字字符。");
			}
		}
	}

	if (buf > 0){
		datas.push_back(buf);
	}

	if (datas.empty())datas.push_back(0);
}

//比较类
int UBigIntCmp(const UBigInt &a,const UBigInt &b){
	if (a.datas.size() != b.datas.size())return a.datas.size() - b.datas.size();
	for (int i = a.datas.size() - 1; i >= 0; --i){
		if (a.datas[i] != b.datas[i])return a.datas[i] - b.datas[i];
	}
	return 0;
}

bool operator==(const UBigInt &a, const UBigInt &b){
	return UBigIntCmp(a, b) == 0;
}

bool operator!=(const UBigInt &a, const UBigInt &b){
	return UBigIntCmp(a, b) != 0;
}

bool operator<(const UBigInt &a, const UBigInt &b){
	return UBigIntCmp(a, b) < 0;
}

bool operator<=(const UBigInt &a, const UBigInt &b){
	return UBigIntCmp(a, b) <= 0;
}

bool operator>(const UBigInt &a, const UBigInt &b){
	return UBigIntCmp(a, b) > 0;
}

bool operator>=(const UBigInt &a, const UBigInt &b){
	return UBigIntCmp(a, b) >= 0;
}

UBigInt& UBigInt::operator++(){
	//++a
	return ((*this) += 1);
}
UBigInt UBigInt::operator++(int dummy){
	//a++
	UBigInt old = (*this);
	(*this) += 1;
	return old;
}
UBigInt& UBigInt::operator--(){
	//--a
	return ((*this) -= 1);
}
UBigInt UBigInt::operator--(int dummy){
	//a--
	UBigInt old = (*this);
	(*this) -= 1;
	return old;
}

UBigInt& operator+=(UBigInt &a, const UBigInt &b){
	DType carry = 0; // 进位
	int maxLen = max(a.datas.size(), b.datas.size()); 
	a.datas.resize(maxLen, 0);
	for (int i = 0; i < maxLen; ++i){
		DType anum = i < a.datas.size() ? a.datas[i] : 0;
		DType bnum = i < b.datas.size() ? b.datas[i] : 0;
		a.datas[i] = anum + bnum + carry;
		carry = a.datas[i] / DTYPE_X;
		a.datas[i] %= DTYPE_X;
	}
	if (carry > 0){
		a.datas.push_back(carry);
	}
	return a;
}

UBigInt& operator-=(UBigInt &a, const UBigInt &b){
	//按竖式减法，从低位开始减
	//考虑到效率，可以到最后再比较a,b大小
	//简单的长度比较,a<b时
	if (a.datas.size() < b.datas.size()){
		throw runtime_error("无符号整数减法a-b中,a不能比b小");
	}
	for (int i = 0; i < b.datas.size(); ++i){
		a.datas[i] -= b.datas[i];
		if (a.datas[i] < 0){
			//向高位借位
			if (i + 1 >= a.datas.size()){
				throw runtime_error("无符号整数a-b中，a不能比b小");
			}
			--a.datas[i + 1]; // 借位
			a.datas[i] += DTYPE_X;
		}
	}

	// 更新长度, 去掉前面的0
	int len = a.datas.size(); 
	while (a.datas[len - 1] == 0 && len > 1)--len;
	a.datas.resize(len);

	return a;
}

UBigInt& operator*=(UBigInt &a, const UBigInt &b){
	vector<DType> temp(a.datas.size() + b.datas.size(), 0); // 临时累加器
	for (int i = 0;i < b.datas.size();++i){
		for (int j = 0;j < a.datas.size();++j){
			temp[i + j] += b.datas[i] * a.datas[j];
			temp[i + j + 1] += temp[i + j] / DTYPE_X;
			temp[i + j] %= DTYPE_X;
			//这里简单地做了第一次进位处理，防止溢出
		}
	}

	//w为从左到右数第一个非零位，最小值为0
	int w;
	for (w = temp.size() - 1; w > 0; --w){
		if (temp[w] != 0)break;
	}

	a.datas.resize(w + 1);
	int carry = 0;
	for (int i = 0; i <= w; ++i){
		temp[i] += carry;
		a.datas[i] = temp[i] % DTYPE_X;
		carry = temp[i] / DTYPE_X;
	}
	if (carry > 0){
		a.datas.push_back(carry);
	}
	return a;
}

//为了更方便地写div和mod,将其封装为一个函数
UBigInt& UBigIntDivide(UBigInt &a, const UBigInt &b, bool mod){
	// mod 是否去模 %
	// 使用二分法判断除数某位
	// a的位数小于b的位数的情况（必有a<b）
	if (a.datas.size() < b.datas.size()){
		// a确实小于b的情况
		if (mod){
			return a;
		}else{
			a.datas.resize(1, 1);
			return a;
		}
	}
	//特殊情况优化，不是很确定命中频率
	if (b.datas.size() == 1){
		if (b.datas[0] == 0){
			throw runtime_error("除数不能为0");
		}
		if (b.datas[0] == 1){
			//除以1
			if (!mod){
				// a / b
				return a;
			}else{
				// a % b, b == 1
				// so, a = 0
				a.datas.resize(1, 0);
				return a;
			}
		}
	}
	// a的位数大于等于b的位数
	vector<int> res;	// 结果缓存
	int notZero = a.datas.size() - 1;// a被减后的非零位
	int qlen = a.datas.size() - b.datas.size();// for中的继续条件中的变量是时刻更新的
	for (int q = 0; q <= qlen; ++q){
		//q代表将除数靠左后向右移动的格数
		int delta = a.datas.size() - b.datas.size() - q;//相对位置偏差

		//用二分取商的某位的最大值(由于是万进制(DTYPE_X = 10000)，所以hi取DTYPE_X)
		int j = b.datas.size() - 1 + delta;	//当前a被对齐的最高位

		int big = 0;
		int base = 1;
		for (int u = j; u <= notZero; ++u){
			big += a.datas[u] * base;
			base *= DTYPE_X;
		}

		int lo = big / (b.datas[b.datas.size() -1] + 1), hi = big / (b.datas[b.datas.size() - 1]) + 1;// [lo,hi)
		//int lo = 0, hi = DTYPE_X;
		UBigInt temp;	// 将要减去的偏移数
		// 这里的二分法原理是，首先估计出temp的上下界，再用二分法求出temp
		while (lo < hi){
			int mi = lo + ((hi - lo) >> 1);
			temp = b * mi;
			bool isLarge = false; //过大
			if (temp.datas.size() - 1 + delta < notZero){
				isLarge = false; // 对齐后a还是比temp多一位
			}else{

				for (int i = temp.datas.size() - 1; i >= 0; --i){
					if (temp.datas[i] > a.datas[i + delta]){
						isLarge = true;
						break;
					}else{
						if (temp.datas[i] < a.datas[i + delta])break;
					}
				}

			}
		}
		//--lo为所求
		res[delta] = --lo;
		temp = b * res[delta];	//上面二分过程中的temp不一定是最终结果 = = 因此最后还要算一次
		//对齐位减去temp
		for (int i = 0; i < temp.datas.size(); ++i){
			a.datas[i + delta] -= temp.datas[i];
			//检查是否需要向前借位
			for (int w = i + delta; w <= notZero; ++w){
				if (a.datas[w] < 0){
					a.datas[w] += DTYPE_X;
					--a.datas[w + 1];
				}else{
					break;
				}
			}
		}
		//更新a的最高非零位
		for (; notZero > 0; --notZero){
			if (a.datas[notZero])break;
		}
	}
	if (mod){
		//取模
		int w;
		//去除a多余的前缀0,由模的性质，a%b的位数 <= b的位数
		for (w = b.datas.size() - 1; w > 0; --w){
			if (a.datas[w] != 0)break;
		}
		a.datas.resize(w + 1);
	}
	else{
		//w为从左到右数第一个非零位，最小值为0
		int w;
		for (w = res.size() - 1; w > 0; --w){
			if (res[w] != 0)break;
		}
		a.datas.resize(w + 1);
		for (int i = 0; i <= w; ++i){
			a.datas[i] = res[i];
		}
	}
	return a;
}

UBigInt& operator/=(UBigInt &a, const UBigInt &b){
	return UBigIntDivide(a, b, false);
}
UBigInt& operator%=(UBigInt &a, const UBigInt &b){
	return UBigIntDivide(a, b, true);
}

UBigInt operator+(const UBigInt &a, const UBigInt &b){
	UBigInt temp = a;
	temp += b;
	return temp;
}
UBigInt operator-(const UBigInt &a, const UBigInt &b){
	UBigInt temp = a;
	temp -= b;
	return temp;
}
UBigInt operator*(const UBigInt &a, const UBigInt &b){
	UBigInt temp = a;
	temp *= b;
	return temp;
}
UBigInt operator/(const UBigInt &a, const UBigInt &b){
	UBigInt temp = a;
	temp /= b;
	return temp;
}
UBigInt operator%(const UBigInt &a, const UBigInt &b){
	UBigInt temp = a;
	temp %= b;
	return temp;
}

//IO流
ostream& operator<<(ostream &os, UBigInt &&u){
	os.fill('0');
	os << u.datas[u.datas.size() - 1];
	for (int i = u.datas.size() - 2; i >= 0; i--){
		os << setw(DTYPE_X_LEN) << u.datas[i];
	}
	os.fill();
	return os;
}

ostream& operator<<(ostream &os, UBigInt &u){
	//我记得C++ IO流还有一种很简单的补零方法，现在找不到了
	os.fill('0');
	os << u.datas[u.datas.size() - 1];
	for (int i = u.datas.size() - 2; i >= 0; i--){
		os << setw(DTYPE_X_LEN) << u.datas[i];
	}
	os.fill();
	return os;
}

istream& operator>>(istream &is, UBigInt &u){
	string s;
	is >> s;
	u = UBigInt(s.c_str());
	return is;
}
