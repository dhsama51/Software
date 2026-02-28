//팩토리얼 계산 시 3자리만 출력하면 되는데 왜 1000이 안 되고(반례는 25!에서 936*25와 5936*25의 결과가 다름) 1000000이 아니라 10000만 해도 되는지는 아직 모름.

#include <iostream>
#include <string>
using namespace std;

int y[1001] = { 1, };

int fact(int n) {
	if (n == 0) return 1;
	else {
		int temp = n * fact(n - 1);
		y[n] = temp;
		while (y[n] % 10 == 0) y[n] /= 10;
		y[n] %= 1000000;
		return y[n];

	}
}

int main() {
	int a = 0;
	int input = -1;
	fact(1000);

	cin >> a;
	for (int i = 0; i < a; i++) {
		cin >> input;
		y[input] %= 1000;
		cout << y[input] << endl;
	}
	return 0;
}