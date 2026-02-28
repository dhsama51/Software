#include <iostream>
using namespace std;

int given[101] = { 0, };
int result[102][102] = { {0,}, };
int order[102][102] = { {0,}, };

void chain_mat_mult(int n) {
	for (int j = 1; j < n; j++) {
		for (int i = 1; i < n - j + 1; i++) {
			int temp_result;
			int temp_order = i;
			int least = 100000;
			for (int k = i; k < i + j; k++) {
				temp_result = result[i][k] + result[k + 1][i + j] + given[i - 1] * given[k] * given[i + j];
				least = least < temp_result ? least : temp_result;
				temp_order = least < temp_result ? temp_order : k;
			}
			result[i][i + j] = least;
			order[i][i + j] = temp_order;
		}
	}
}

void print_order(int low, int high) {
	if (low == high)
		cout << "M" << low;
	else {
		int k = order[low][high];
		cout << "(";
		print_order(low, k);
		print_order(k + 1, high);
		cout << ")";
	}
	
}

int main() {
	int a = 0;
	int n = 0;

	cin >> a;
	for (int i = 0; i < a; i++) {
		cin >> n;
		for (int j = 0; j < n + 1; j++) {
			cin >> given[j];
		}
		for (int j = 1; j < 101; j++)
			for (int k = j + 1; k < 101; k++)
				result[j][k] = 0;
		chain_mat_mult(n);
		print_order(1, n);
		cout << endl;
		cout << result[1][n] << endl;
	}

	return 0;
}