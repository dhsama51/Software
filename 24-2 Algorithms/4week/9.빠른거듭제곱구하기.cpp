//메모리도 절반으로 쓰고 나보다 빠른 사람이 있긴 함. 다만 나도 상위권.

#include <iostream>
using namespace std;

int power(int m, int n) {
	m %= 1000;
	if (n == 0) return 1;
	else if(n == 1) return m % 1000;
	else if (n % 2 == 0) return power(m * m, n / 2) % 1000;
	else return m * power(m * m, n / 2) % 1000;
}

int main() {
	int a = 0;
	int m = 0, n = 0;


	cin >> a;
	for (int i = 0; i < a; i++) {
		cin >> m >> n;
		cout << power(m, n) << endl;
	}
}