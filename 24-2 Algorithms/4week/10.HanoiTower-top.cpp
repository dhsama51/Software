// -

#include <iostream>
using namespace std;

int stack[11] = { 0, };
int pointer = 0;

void move(int n, int from, int to) {
	if (to == 3) {
		stack[pointer + 1] = n;
		pointer += 1;
		cout << stack[pointer] << " ";
	}

	else if (from == 3) {
		stack[pointer] = 0;
		pointer -= 1;
		cout << stack[pointer] << " ";
	}

}

void hanoi(int n, int from, int sub, int to) {
	if (n > 0) {
		hanoi(n - 1, from, to, sub);
		move(n, from, to);
		hanoi(n - 1, sub, from, to);
	}
}

int main() {
	int a = 0;
	int n = 0;

	cin >> a;
	for (int i = 0; i < a; i++) {
		cin >> n;
		for (int j = 0; j < 11; j++) stack[j] = 0;
		pointer = 0;
		hanoi(n, 1, 2, 3);
		cout << endl;
	}
	return 0;
}