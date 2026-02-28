#include <iostream>
using namespace std;

int square[256][256] = { {0,}, };
int cnt = 1;

void initiate(int n, int x, int y) {
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			square[i][j] = -1;
	square[y][x] = 0;
	cnt = 1;
}

void print_square(int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			cout << square[i][j] << " ";
		cout << endl;
	}
}

void tromino(int start_x, int start_y, int n, int x, int y) {
	if (n == 2) {
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				if (square[start_y + i][start_x + j] == -1)
					square[start_y + i][start_x + j] = cnt;
		cnt++;
		return;
	}
	
	int middle_x = start_x + n / 2;
	int middle_y = start_y + n / 2;
	if (x < start_x + n / 2 && y < start_y + n / 2) {
		square[middle_y][middle_x] = cnt;
		square[middle_y - 1][middle_x] = cnt;
		square[middle_y][middle_x - 1] = cnt;
		cnt++;
		tromino(start_x, start_y, n / 2, x, y);
		tromino(middle_x, start_y, n / 2, middle_x, middle_y - 1);
		tromino(middle_x, middle_y, n / 2, middle_x, middle_y);
		tromino(start_x, middle_y, n / 2, middle_x - 1, middle_y);
	}
	else if (x >= start_x + n / 2 && y < start_y + n / 2) {
		square[middle_y][middle_x] = cnt;
		square[middle_y][middle_x - 1] = cnt;
		square[middle_y - 1][middle_x - 1] = cnt;
		cnt++;
		tromino(start_x, start_y, n / 2, middle_x - 1, middle_y - 1);
		tromino(middle_x, start_y, n / 2, x, y);
		tromino(middle_x, middle_y, n / 2, middle_x, middle_y);
		tromino(start_x, middle_y, n / 2, middle_x - 1, middle_y);
	}
	else if (x >= start_x + n / 2 && y >= start_y + n / 2) {
		square[middle_y - 1][middle_x] = cnt;
		square[middle_y][middle_x - 1] = cnt;
		square[middle_y - 1][middle_x - 1] = cnt;
		cnt++;
		tromino(start_x, start_y, n / 2, middle_x - 1, middle_y - 1);
		tromino(middle_x, start_y, n / 2, middle_x, middle_y - 1);
		tromino(middle_x, middle_y, n / 2, x, y);
		tromino(start_x, middle_y, n / 2, middle_x - 1, middle_y);
	}
	else {
		square[middle_y][middle_x] = cnt;
		square[middle_y - 1][middle_x] = cnt;
		square[middle_y - 1][middle_x - 1] = cnt;
		cnt++;
		tromino(start_x, start_y, n / 2, middle_x - 1, middle_y - 1);
		tromino(middle_x, start_y, n / 2, middle_x, middle_y - 1);
		tromino(middle_x, middle_y, n / 2, middle_x, middle_y);
		tromino(start_x, middle_y, n / 2, x, y);
	}

}

int main() {
	int a = 0;
	int n, x, y;

	cin >> a;
	for (int i = 0; i < a; i++) {
		cin >> n >> x >> y;
		initiate(n, y, x);
		tromino(0, 0, n, y, x);
		print_square(n);
	}


	return 0;
}