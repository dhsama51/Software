#include <iostream>
using namespace std;

int tcase[101] = { 0, };
int best_sum, best_start, best_end;
int can_sum, can_start, can_end;

void kadane(int n) {
	can_sum = 0, can_start = 0, can_end = 0;
	best_sum = 0, best_start = -1, best_end = -1;
	for (int i = 0; i < n; i++) {
		can_sum += tcase[i];
		if (can_sum <= 0) {
			can_sum = 0;
			can_start = i + 1;
			can_end = -1;
		}
		else if (can_sum > best_sum) {
			can_end = i;
			best_sum = can_sum;
			best_start = can_start;
			best_end = can_end;
		}
		else {
			can_end = i;
		}
	}
	cout << best_sum << " " << best_start << " " << best_end << endl;
}

int main() {
	int a = 0;
	int n = 0;

	cin >> a;
	for (int i = 0; i < a; i++) {
		cin >> n;
		for (int j = 0; j < n; j++)
			cin >> tcase[j];

		kadane(n);
	}
	return 0;
}