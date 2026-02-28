//기본이어서 basic_fibo 사용해도 됨. 계산값 저장까지만 구현해봤음.

#include <iostream>
#include <string>
using namespace std;

int y[31] = { 0,1, };

int basic_fibo(int n) {
	if (n == 0) return 0;
	else if (n == 1) return 1;
	else {
		y[n] = basic_fibo(n - 1) + basic_fibo(n - 2);
		return y[n];
	}


}

int main() {
	int a = 0;
	int input = -1;
	basic_fibo(30);

	cin >> a;
	for (int i = 0; i < a; i++) {
		cin >> input;
		cout << y[input] << endl;
	}

	return 0;
}