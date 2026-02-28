#include <iostream>
using namespace std;

int given[100];
int ifexist = 0;
int ifexistpos = -1;

int binary_search(int low, int high) {
    if (ifexistpos != -1) return 1;
    if (low == high) {
        ifexistpos = ((given[low] == ifexist) ? low : -1);
        return 0;
    }

    int middle = (low + high) / 2;
    binary_search(low, middle);
    binary_search(middle + 1, high);
    return ifexistpos;
}

int main() {
    int a = 0;
    int n = 0;

    cin >> a;
    for (int i = 0; i < a; i++) {
        cin >> n;
        for (int j = 0; j < n; j++) {
            cin >> given[j];
        }
        cin >> ifexist;
        ifexistpos = -1;
        binary_search(0, n - 1);
        cout << ifexistpos << endl;
    }

    return 0;
}