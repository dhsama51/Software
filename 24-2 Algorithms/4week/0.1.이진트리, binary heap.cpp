#include <iostream>
using namespace std;

int given[100];

void fix_heap(int root) {
    if (given[root] < given[root * 2]) {
        swap(given[root], given[root * 2]);
    }
    if (given[root] < given[root * 2 + 1]) {
        swap(given[root], given[root * 2 + 1]);
    }
}

void fix_left(int root) {
    if (given[root] < given[root * 2]) {
        swap(given[root], given[root * 2]);
    }
}

void create_min_heap(int root, int end) {
    int left_node = root * 2;
    int right_node = root * 2 + 1;
    if (right_node <= end) {
        create_min_heap(left_node, end);
        create_min_heap(right_node, end);
    }

    if (right_node <= end) {
        fix_heap(root);
    }
    else if (left_node = end) {
        fix_left(root);
    }
}

int main() {
    int a = 0;
    int n = 0;

    cin >> a;
    for (int i = 0; i < a; i++) {
        cin >> n;
        for (int j = 0; j < n; j++) {
            cin >> given[j+1];
        }
        create_min_heap(1, n);

        for (int j = 0; j < n; j++) {
            cout << given[j+1];
        }
    }

    return 0;
}