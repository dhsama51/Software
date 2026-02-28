//통과는 됐으나, 시간 제한 1000 중 906을 써서 시간이 빡빡함.
// 시간 짧게 쓴 사람도 많아서 개선 여지 有. 아마 kadein 알고리즘처럼 중간에 더해가면 되지 않을까..

#include <iostream>
#include <cstring>
using namespace std;

int answer = 0;

int computing(string s, int size) {
	if (size == 1) {
		answer += 1;
		return 0;
	}

	int sum = 0;

	for (int i = 0; i < size; i += 2) sum += s[i] - 'a';
	for (int i = 1; i < size; i += 2) sum -= s[i] - 'a';

	if (sum > 0) answer += 1;

	return 0;
}

void string_perm(string s, int start, int end) {
	int range = end - start;

	if (range == 1) {
		computing(s, end);
	}
	for (int i = 0; i < range; i++) {
		swap(s[start], s[start + i]);

		string_perm(s, start + 1, end);
		swap(s[start], s[start + i]);
	}
}

int main() {
	int a = 0;
	string s = "";

	cin >> a;
	for (int i = 0; i < a; i++) {
		cin >> s;
		answer = 0;
		string_perm(s, 0, s.length());
		cout << answer << endl;
	}
}