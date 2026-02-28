#include <iostream>
#include <cstring>
using namespace std;

int lcs[101][101] = { {0,}, };
int lcsstring[101][101] = { {0,}, };
string stra = "";
string strb = "";

void LCS(int aend, int bend) {
	for (int i = 1; i < aend + 1; i++) {
		for (int j = 1; j < bend + 1; j++)
		{
			if (stra[i - 1] == strb[j - 1]) {
				lcs[i][j] = lcs[i - 1][j - 1] + 1;
				lcsstring[i][j] = 0;
			}

			else {
				int a = lcs[i][j - 1];
				int b = lcs[i - 1][j];
				lcs[i][j] = a > b ? a : b;
				if (lcs[i][j] == a) lcsstring[i][j] = 1;
				else if(lcs[i][j] == b) lcsstring[i][j] = 2;
			}
		}
	}
}

int printLCS(int aend, int bend) {
	if (aend == 0 || bend == 0) return 0;
	if (lcsstring[aend][bend] == 0) {
		printLCS(aend - 1, bend - 1);
		cout << stra[aend - 1];
	}
	else if (lcsstring[aend][bend] == 1) printLCS(aend, bend - 1);
	else if (lcsstring[aend][bend] == 2) printLCS(aend - 1, bend);
}
int main() {
	int a = 0;

	cin >> a;
	for (int i = 0; i < a; i++) {
		cin >> stra;
		cin >> strb;

		int aend = stra.length();
		int bend = strb.length();
		LCS(aend, bend);
		cout << lcs[aend][bend] << " ";
		printLCS(aend, bend);
		cout << endl << endl;
	}

	return 0;
}