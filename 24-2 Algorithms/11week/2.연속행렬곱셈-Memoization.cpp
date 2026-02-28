#include <iostream>
using namespace std;

int given[101] = { 0, };
int result[101][101] = { {0,}, };

int chain_mat_mult(int low, int high) {
	if (low == high) return 0;

	int temp;
	for (int i = low; i < high; i++) {
		int a = result[low][i] != 100000 ? result[low][i] : chain_mat_mult(low, i);
		int b = result[i + 1][high] != 100000 ? result[i + 1][high] : chain_mat_mult(i + 1, high);
		temp = a + b + given[low - 1] * given[i] * given[high];
		result[low][high] = result[low][high] < temp ? result[low][high] : temp;
	}
	return result[low][high];
}

int main() {
	int a = 0;
	cin >> a;

	for (int i = 0; i < a; i++) {
		int n = 0;
		cin >> n;

		for (int j = 0; j < n + 1; j++) {
			cin >> given[j];
		}
		for (int j = 0; j < 100; j++)
			for (int k = j + 1; k < 101; k++)
				result[j][k] = 100000;
		chain_mat_mult(1, n);
		cout << result[1][n] << endl;
	}

	return 0;
}