//교수님이 의도한 방향과 다르게 했을 때 6 8 6 7 등 연산이 매우 많이 필요한 경우 결과가 제대로 출력되지 않음. 따라서,
//시윤이의 경우 의도한 방향과 같았기에 시간이 상대적으로 적게 걸림.
//정확히는 시도는 못 해봤지만 정윤이가 시간초과 났던 걸 시간 바꾸니 줄었다고 했으므로 가능성 큼
#include <iostream>
using namespace std;

int chess[9][9] = { {0,}, };
int direct[8][2] = { {1, -2}, {2, -1}, {2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2} };
int stack[512][3] = { {0,}, };
int pointer = 0;

void initiate(int m, int n) {
	for (int i = 1; i <= m; i++)
		for (int j = 1; j <= n; j++)
			chess[i][j] = 0;
	for (int i = 0; i < 512; i++)
		for (int j = 0; j < 3; j++)
			stack[i][j] = 0;
	pointer = 0;
}

void print_tour(int m, int n) {
	for (int i = 1; i <= m; i++) {
		for (int j = 1; j <= n; j++)
			printf("%02d ", chess[i][j]);
		//cout << chess[i][j] << " ";
		cout << endl;
	}
}

int knight_tour(int m, int n, int x, int y) {
	int count = 0;
	int temp[3] = { 0, };

	pointer++;
	stack[pointer][0] = x;
	stack[pointer][1] = y;
	stack[pointer][2] = count + 1;
	while (pointer != 0) {
		x = stack[pointer][0];
		y = stack[pointer][1];
		count = stack[pointer][2];
		chess[y][x] = count;

		if (count == m * n) return 1;

		int end = 1;
		for (int i = 0; i < 8; i++) {
			int next[3] = { x + direct[i][0], y + direct[i][1], count + 1 };
			if (next[0] > 0 && next[0] <= n && next[1] > 0 && next[1] <= m
				&& chess[next[1]][next[0]] == 0) {
				pointer++;
				stack[pointer][0] = next[0];
				stack[pointer][1] = next[1];
				stack[pointer][2] = next[2];
				end = 0;
			}
		}

		if (end) {
			chess[y][x] = 0;
			temp[0] = stack[pointer][0];
			temp[1] = stack[pointer][1];
			temp[2] = stack[pointer][2];
			pointer--;

			while (pointer != 0 && temp[2] != stack[pointer][2]) {
				temp[0] = stack[pointer][0];
				temp[1] = stack[pointer][1];
				temp[2] = stack[pointer][2];
				pointer--;
				chess[temp[1]][temp[0]] = 0;
			}
		}
	}
	return 0;
}

int main() {
	int a = 0;
	int m, n, x, y;

	cin >> a;
	for (int i = 0; i < a; i++) {
		cin >> m >> n >> y >> x;
		initiate(m, n);
		if (knight_tour(m, n, x, y)) {
			cout << 1 << endl;
			print_tour(m, n);
		}
		else cout << 0 << endl;
	}

	return 0;
}