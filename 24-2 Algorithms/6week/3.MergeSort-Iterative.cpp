#include <iostream>
using namespace std;

int tcase[100] = { 0, };
int cnt = 0;

void merge(int start, int mid, int end)
{
	int i, j, k;
	int tmp[100];
	for (i = start; i <= end; i++)
		tmp[i] = tcase[i];
	i = k = start;
	j = mid + 1;
	while (i <= mid && j <= end)
		if (tmp[i] <= tmp[j]) {
			tcase[k++] = tmp[i++];
			cnt++;
		}
		else {
			tcase[k++] = tmp[j++];
			cnt++;
		}
	while (i <= mid)
		tcase[k++] = tmp[i++];
	while (j <= end)
		tcase[k++] = tmp[j++];
}

void merge_sort(int n) {
	int size = 1;
	while (size < n) {
		for (int i = 0; i < n; i += 2 * size) {
			int start = i;
			int mid = start+ size - 1;
			int end = min(i + 2 * size - 1, n - 1);

			if (mid >= n - 1) break;
			merge(start, mid, end);
		}
		size *= 2;
	}
}

int main() {
	int a = 0;
	int n = 0;
	
	cin >> a;
	for (int i = 0; i < a; i++) {
		cin >> n;
		cnt = 0;
		for (int j = 0; j < n; j++)
			cin >> tcase[j];
		merge_sort(n);
		cout << cnt << endl;
	}

	return 0;
}