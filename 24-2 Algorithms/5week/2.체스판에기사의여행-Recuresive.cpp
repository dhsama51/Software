#include <iostream>
#include <vector>
using namespace std;

int chess[9][9] = { {0,}, };
int direct[8][2] = { {1, -2}, {2, -1}, {2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2} };

void initiate(int m, int n, int x, int y) {
	for (int i = 1; i < m+1; i++)
		for (int j = 1; j < n+1; j++)
			chess[i][j] = 0;
	chess[x][y] = 1;
}

void print_tour(int m, int n) {
	for (int i = 1; i < m+1; i++) {
		for (int j = 1; j < n+1; j++)
			cout << chess[i][j] << " ";
		cout << endl;
	}
}

int knight_tour(int m, int n, int x, int y, int count) {
	if (count == m * n) return 1;

	for (int i = 0; i < 8; i++) {
		int next[2] = { x + direct[i][1], y + direct[i][0] };
		if (next[0] > 0 && next[0] <= m && next[1] > 0 && next[1] <= n && chess[next[0]][next[1]] == 0) {
			chess[next[0]][next[1]] = count + 1;
			if (knight_tour(m, n, next[0], next[1], count + 1)) return 1;
			chess[next[0]][next[1]] = 0;
		}
	}

	return 0;
}

int main() {
	int a = 0;
	int m = 0, n = 0;
	int x = 0, y = 0;
	
	cin >> a;
	for (int i = 0; i < a; i++) {
		cin >> m >> n >> x >> y;

		initiate(m, n, x, y);
		if (knight_tour(m, n, x, y, 1)) {
			cout << 1 << endl;
			print_tour(m, n);
		}
		else cout << 0 << endl;
	}

	return 0;
}