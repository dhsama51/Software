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

void merge_sort(int start, int end) {
	if (start == end) return;

	int mid = (start + end) / 2;
	merge_sort(start, mid);
	merge_sort(mid + 1, end);
	merge(start, mid, end);
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
		merge_sort(0,n-1);
		cout << cnt << endl;
	}

	return 0;
}