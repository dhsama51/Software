#include <iostream>
using namespace std;

int given[101] = { 0, };

int chain_mat_mult(int start, int end) {
	if (start == end) return 0;

	int temp;
	int least = 100000;
	for (int i = start; i < end; i++) {
		temp = chain_mat_mult(start, i) + chain_mat_mult(i + 1, end) + given[start - 1] * given[i] * given[end];
		least = least < temp ? least : temp;
	}
	return least;
}

int main() {
	int a = 0;
	cin >> a;

	for (int i = 0; i < a; i++) {
		int n = 0;
		cin >> n;
		for (int j = 0; j < n + 1 ; j++) {
			cin >> given[j];
		}

		cout << chain_mat_mult(1, n) << endl;
	}

	return 0;
}