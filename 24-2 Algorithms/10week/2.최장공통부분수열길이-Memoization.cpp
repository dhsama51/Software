#include <iostream>
#include <cstring>
using namespace std;

int lcs[101][101];
string stra = "";
string strb = "";

int LCS(int aend, int bend) {
	if (aend == 0 || bend == 0) return 0;

	if (stra[aend - 1] == strb[bend - 1]) {
		if (lcs[aend - 1][bend - 1] != -1)
			lcs[aend][bend] = lcs[aend - 1][bend - 1] + 1;
		else
			lcs[aend][bend] = LCS(aend - 1, bend - 1) + 1;
		return lcs[aend][bend];
	}

	else {
		if (lcs[aend][bend - 1] != -1 && lcs[aend - 1][bend] != -1) {
			int a = lcs[aend][bend - 1];
			int b = lcs[aend - 1][bend];
			lcs[aend][bend] = a > b ? a : b;
		}
		else if (lcs[aend][bend - 1] != -1 && lcs[aend - 1][bend] == -1){
			int a = lcs[aend][bend - 1];
			int b = LCS(aend - 1, bend);
			lcs[aend][bend] = a > b ? a : b;
		}
		else if (lcs[aend][bend - 1] == -1 && lcs[aend - 1][bend] != -1) {
			int a = LCS(aend, bend - 1);
			int b = lcs[aend - 1][bend];
			lcs[aend][bend] = a > b ? a : b;
		}
		else {
			int a = LCS(aend, bend - 1);
			int b = LCS(aend - 1, bend);
			lcs[aend][bend] = a > b ? a : b;
		}
		return lcs[aend][bend];
	}
}

int main() {
	int a = 0;

	cin >> a;
	for (int i = 0; i < a; i++) {
		cin >> stra >> strb;

		for (int j = 0; j < 101; j++) {
			lcs[j][0] = 0;
			lcs[0][j] = 0;
		}
		for (int j = 1; j < 101; j++)
			for (int k = 1; k < 101; k++)
				lcs[j][k] = -1;
		int aend = stra.length();
		int bend = strb.length();
		cout << LCS(aend, bend) << endl;
	}

	return 0;
}