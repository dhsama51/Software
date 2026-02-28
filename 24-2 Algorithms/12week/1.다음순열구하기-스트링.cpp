#include <iostream>
#include <cstring>
using namespace std;

string given = "";

void flip_string(int low, int high) {
    for (int i = 0; i < (high - low + 1) / 2; i++) {
        swap(given[low + i], given[high - i]);
    }
}

void narayana(int n) {
    int endpos = 0;
    for (int i = n - 2; i >= 0; i--) {
        if (given[i] < given[i + 1]) {
            for (int j = n - 1; j >= i + 1; j--) {
                if (given[i] < given[j]) {
                    swap(given[i], given[j]);
                    break;
                }
            }
            flip_string(i + 1, n - 1);
            return;
        }
    }
    flip_string(0, n - 1);
    return;
}

int main() {
    int a = 0;
    int n = 0;

    cin >> a;
    for (int i = 0; i < a; i++) {
        cin >> n;
        cin >> given;

        narayana(n);
        for (int j = 0; j < n; j++) {
            cout << given[j];
        }
        cout << endl;
    }

    return 0;
}