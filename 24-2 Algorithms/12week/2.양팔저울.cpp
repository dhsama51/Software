#include <iostream>
using namespace std;

int given[10000] = { 0, };
int mass[7] = { 1,2,5,10,20,50,100 };
int scale_left = 0;
int scale_right = 0;
int result = 0;

void equalize_scale(int n) {
	for (int i = 0; i < n; i++) {
		if (scale_left <= scale_right) scale_left += given[i];
		else scale_right += given[i];
	}

	if (scale_left == scale_right) return;

	else if (scale_left < scale_right) {
		for (int j = 6; j >= 0; j--) {
			while (scale_left + mass[j] <= scale_right) {
				scale_left += mass[j];
				result++;
				if (scale_left == scale_right) return;
			}
		}
	}

	else {
		for (int j = 6; j >= 0; j--) {
			while (scale_right + mass[j] <= scale_left) {
				scale_right += mass[j];
				result++;
				if (scale_left == scale_right) return;
			}
		}
	}
}

int main() {
	int a = 0;
	int n = 0;

	cin >> a;
	for (int i = 0; i < a; i++) {
		cin >> n;
		for (int j = 0; j < n; j++) {
			cin >> given[j];
		}
		scale_left = 0;
		scale_right = 0;
		result = 0;
		equalize_scale(n);
		cout << result << endl;
	}

	return 0;
}