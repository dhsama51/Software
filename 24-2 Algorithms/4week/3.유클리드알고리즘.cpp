//-

#include <iostream>
#include <string>
using namespace std;

int gcd(int m, int n) {
	if (n == 0) return m;
	else return gcd(n, m % n);
}

int main() {
	int a = 0;
	int m = 0, n = 0;

	cin >> a;
	for (int i = 0; i < a; i++) {
		cin >> m >> n;
		cout << gcd(m, n) << endl;
	}
}