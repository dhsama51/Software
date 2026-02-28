/*직접 만든 문제
구슬이 n개 줄세워져 있고, 더 가벼운 1개를 제외하고는 구슬의 무게가 같다.
이 구슬의 인덱스(0 based)를 log3(N)의 복잡도로 계산하여 반환하시오.
최대 구슬 개수는 10개임.
*/

#include <iostream>
using namespace std;

int ball[100] = { 0, };

void light(int start, int end) {
	if (start == end) {
		cout << start << endl;
		return;
	}
	if (start + 1 == end) {
		int answer = ball[start] < ball[end] ? start : end;
		cout << answer;
		return;
	}
	int size = end - start + 1;
	int left = 0;
	int right = 0;

	for (int i = 0; i < size / 3; i++) {
		left += ball[i];
		right += ball[end - i];
	}

	if (left < right) light(start, start + size / 3 - 1);
	else if (left > right) light(end - size / 3 + 1, end);
	else light(start + size / 3, end - size / 3);
}

int main() {
	int a = 0;
	int n = 0;
	cin >> a;
	for (int i = 0; i < a; i++) {
		cin >> n;
		for (int j = 0; j < n; j++) {
			cin >> ball[j];
		}
		light(0, n-1);
	}

	return 0;
}