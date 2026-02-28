//비교 연산자 총 횟수와 교환 횟수를 계산하기만 하면 되므로 처음부터 정렬을 구현하지는 않음.

#include <iostream>
#include <cstring>
using namespace std;

void insertion_sort(int input[], int num) {
	int count_compare = 0, count_swap = 0;
	int i = 0, j = 0, tmp = 0;

	for (i = 1; i < num; i++) {
		tmp = input[i];
		for (j = i - 1; j >= 0; j--) {
			if (input[j] > tmp) {
				input[j + 1] = input[j];
				count_compare += 1;
				count_swap += 1;
			}
			else {
				count_compare += 1;
				break;
			}
		}
		input[j + 1] = tmp;
	}

	cout << count_compare << " " << count_swap << endl;
}

int main() {
	int a = 0;

	cin >> a;
	for (int i = 0; i < a; i++) {
		int num = 0, input[1000] = { 0, };
		cin >> num;
		for (int j = 0; j < num; j++) cin >> input[j];
			insertion_sort(input, num);

	}

	return 0;
}