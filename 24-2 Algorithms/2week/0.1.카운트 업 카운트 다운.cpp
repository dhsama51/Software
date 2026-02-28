#include <iostream>
using namespace std;

void countup(int n) {
	if (n == 0) return;
	countup(n - 1);
	cout << n << endl;
}

void countdown(int n) {
	if (n == 0) return;
	cout << n << endl;
	countdown(n - 1);
}

int main() {
	int a = 0;
	int updown = 0;
	int n = 0;

	cin >> a;
	for (int i = 0; i < a; i++) {
		cin >> updown >> n;
		if (updown == 1) countup(n);
		else countdown(n);
	}

	return 0;
}