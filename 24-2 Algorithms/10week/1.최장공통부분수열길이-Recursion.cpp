#include <iostream>
#include <cstring>
using namespace std;

string stra = "";
string strb = "";

int LCS(int aend, int bend) {
	if (aend == 0 || bend == 0) return 0;
	if (stra[aend - 1] == strb[bend - 1]) {
		return LCS(aend - 1, bend - 1) + 1;
	}

	else {
		int a = LCS(aend, bend - 1);
		int b = LCS(aend - 1, bend);
		return a > b ? a : b;
	}
}

int main() {
	int a = 0;

	cin >> a;
	for (int i = 0; i < a; i++) {
		cin >> stra;
		cin >> strb;

		int aend = stra.length();
		int bend = strb.length();
		cout << LCS(aend, bend) << endl;
	}

	return 0;
}