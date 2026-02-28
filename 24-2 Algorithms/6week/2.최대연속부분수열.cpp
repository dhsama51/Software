#include <iostream>
using namespace std;

int tcase[100] = { 0, };

int partial_sum(int start, int end) {
	if (start == end) return tcase[start];

	int mid = (start + end) / 2;
	int left_sum = 0, right_sum = 0;

	int temp = 0;
	for (int i = mid - 1; i >= start; i--) {
		temp += tcase[i];
		if (temp > left_sum) left_sum = temp;
	}

	temp = 0;
	for (int i = mid+1; i <= end; i++) {
		temp += tcase[i];
		if (temp > right_sum) right_sum = temp;
	}

	return max(max(partial_sum(start, mid), partial_sum(mid + 1, end)), tcase[mid] + left_sum + right_sum);
}

int main() {
	int a = 0;
	int n = 0;

	cin >> a;
	for (int i = 0; i < a; i++) {
		cin >> n;
		for (int j = 0; j < n; j++)
			cin >> tcase[j];
		cout << partial_sum(0, n-1) << endl;
	}

	return 0;
}