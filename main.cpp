#include "UBigInt.h"
#include <iostream>
using namespace std;

int main(){
	UBigInt a; 
	UBigInt b;
	while(1){
		cin >> a;
		cin >> b;
		cout << "a + b = " << a + b << endl;
		cout << "a - b = " << a - b << endl;
		cout << "a * b = " << a * b << endl;
		cout << "a / b = " << a / b << endl;
	}
	return 0;
}
