/*팰린드롬(회문) 검사
문자열이 들어오면 회문인지 검사하는 것.
최대 15글자. 보니까 한글일 때 확인하려면 3바이트를 차지하기에 나오면 다 틀릴 듯.
*/

#include <iostream>
#include <cstring>
using namespace std;

string object = "";

void is_palindrome(int start, int end) {
	if (start >= end) {
		cout << "this is palindrome" << endl;
		return;
	}
	if (object[start] != object[end]) cout << "this is not palindrome" << endl;
	else is_palindrome(start + 1, end - 1);
}

int main() {
	int a = 0;
	
	cin >> a;
	for (int i = 0; i < a; i++) {
		object = "";
		cin >> object;
		is_palindrome(0, object.length() - 1);
	}

	return 0;
}