//드디어 해결!!

#include <iostream>
using namespace std;

void mat_mul(int x[][2], int y[][2], int(&z)[2][2]) {
	z[0][0] = (x[0][0] * y[0][0] + x[0][1] * y[1][0]) % 1000;
	z[0][1] = (x[0][0] * y[0][1] + x[0][1] * y[1][1]) % 1000;
	z[1][0] = (x[1][0] * y[0][0] + x[1][1] * y[1][0]) % 1000;
	z[1][1] = (x[1][0] * y[0][1] + x[1][1] * y[1][1]) % 1000;
}

void advanced_fibo(int n, int y[][2], int (&result)[2][2]) {
	int identity_matrix[2][2] = { {1,0},{0,1} };
	int base[2][2] = { {1,1},{1,0} };
	int temp1[2][2] = { {0,0}, {0,0} };
	int temp2[2][2] = { {0,0}, {0,0} };

	if (n == 0)
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				result[i][j] = identity_matrix[i][j];
	else if (n == 1)
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				result[i][j] = y[i][j];
	else if (n % 2 == 0) {
		mat_mul(y, y, temp1);
		advanced_fibo(n / 2, temp1, result);
	}
	else {
		mat_mul(y, y, temp1);
		advanced_fibo(n / 2, temp1, temp2);
		mat_mul(temp2, y, result);
	}
}

int main() {
	int a = 0;
	int n = 0;
	int base[2][2] = { {1,1},{1,0} };
	int result[2][2] = { {1,1},{1,0} };
	cin >> a;
	for (int i = 0; i < a; i++) {
		cin >> n;
		advanced_fibo(n, base , result);
		cout << result[0][1] << endl;
	}
}