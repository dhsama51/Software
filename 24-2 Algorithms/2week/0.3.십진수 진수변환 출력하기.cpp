/*16진수를 10진수로 변환만 구현함
*/

#include <iostream>
using namespace std;

int hexnumber = 0;
int answer = 0;

void hex_to_dec(int number) {
	if (number / 10 >= 1) {
		hex_to_dec(number/10);
	}
	cout << number % 10;
	return;
}

int main() {
	int a = 0;
	
	cin >> a;
	for (int i = 0; i < a; i++) {
		cin >> hex >> hexnumber;
		answer = 0;
		hex_to_dec(hexnumber);
		cout << endl;
	}

	return 0;
}