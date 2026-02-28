//메모리 사용이 내 절반밖에 안 되는 사람도 있음.

#include <iostream>
#include <cstring>
using namespace std;


int flip_string(string& s, int start, int end) {
	if (start >= end) return 0;
	else {
		swap(s[start], s[end]);
		flip_string(s, start + 1, end - 1);
	}
}

int main() {
	int a = 0;
	string input = "";

	cin >> a;
	for (int i = 0; i < a; i++) {
		cin >> input;
		flip_string(input, 0, input.length()-1);
		cout << input << endl;
	}
}