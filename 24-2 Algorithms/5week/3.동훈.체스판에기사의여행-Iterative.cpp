//2 2 1 1를 처리하지 못하는 경우를 제외하고 멀쩡했으나,(지금은 수정 완료)
//교수님이 의도한 방향과 다르게 했을 때 6 8 6 7 등 연산이 매우 많이 필요한 경우 결과가 제대로 출력되지 않음. 따라서,
//stack에 반시계방향으로 넣음=시계방향으로 실행=내 첫 설계 를 포기하고,
//stack에 시계방향으로 넣음=반시계방향으로 실행=내 현 설계 를 이용함.->성공.

#include <iostream>
using namespace std;

int chess[9][9] = { {0,}, };
int direct[8][2] = { {-2, 1}, {-1, 2}, {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1} };
int stack[512][3] = { {0,}, };
int pointer = 0;

void initiate(int m, int n) {
    for (int i = 1; i < m + 1; i++)
        for (int j = 1; j < n + 1; j++)
            chess[i][j] = 0;
    for (int i = 0; i < 512; i++)
        for (int j = 0; j < 3; j++)
            stack[i][j] = 0;
    pointer = 0;
}

void print_tour(int m, int n) {
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++)
            cout << chess[i][j] << " ";
        cout << endl;
    }
}

int knight_tour(int m, int n, int x, int y) {
    int count = 0;

    chess[x][y] = ++count;
    while (1) {
        for (int i = 0; i < 8; i++) {
            int next[2] = { x + direct[i][0], y + direct[i][1] };
            if (next[0] > 0 && next[0] <= m && next[1] > 0 && next[1] <= n) {
                stack[pointer][0] = next[0];
                stack[pointer][1] = next[1];
                stack[pointer][2] = count;
                pointer++;
            }
        }

        while (pointer > 0) {
            pointer--;

            if (chess[stack[pointer][0]][stack[pointer][1]] == 0) {
                chess[stack[pointer][0]][stack[pointer][1]] = ++count;
                x = stack[pointer][0];
                y = stack[pointer][1];
                break;
            }
            else {
                if (stack[pointer][2] != stack[pointer - 1][2]) {
                    count = stack[pointer - 1][2];
                    for (int j = 1; j < m + 1; j++)
                        for (int k = 1; k < n + 1; k++)
                            if (chess[j][k] > count)
                                chess[j][k] = 0;
                }
            }
        }
        if (count == m * n) return 1;
        if (pointer <= 0) return 0;
    }
}

int main() {
    int a = 0;
    int m, n, x, y;

    cin >> a;
    for (int i = 0; i < a; i++) {
        cin >> m >> n >> x >> y;
        initiate(m, n);
        if (knight_tour(m, n, x, y)) {
            cout << 1 << endl;
            print_tour(m, n);
        }
        else cout << 0 << endl;
    }

    return 0;
}