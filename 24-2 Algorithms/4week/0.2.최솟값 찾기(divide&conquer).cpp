#include <iostream>
using namespace std;

int given[100] = { 0, };
int given_min = 0;

int cal_min(int low, int high) {
    if (low == high) return given[low];
    int middle = (low + high) / 2;

    int left_min = cal_min(low, middle);
    int right_min = cal_min(middle + 1, high);
    given_min = left_min < right_min ? left_min : right_min;
    return given_min;

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
        cal_min(0, n - 1);
        cout << given_min << endl;
    }

    return 0;
}