//비교 연산자 총 횟수와 교환 횟수를 계산하기만 하면 되므로 처음부터 정렬을 구현하지는 않음.

#include <iostream>
#include <cstring>
using namespace std;

void selection_sort(int input[], int num) {
	int count_compare = 0, count_swap = 0;
	int tmp = 0 , min = 0;

	for (int i = 0; i < num - 1; i++) {
		min = i;
		for (int j = i + 1; j < num; j++) {
			if (input[j] < input[min]) min = j;
			count_compare += 1;
		}
		if (i != min) {
			tmp = input[i];
			input[i] = input[min];
			input[min] = tmp;
			count_swap += 1;
		}
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
		selection_sort(input, num);

	}

	return 0;
}