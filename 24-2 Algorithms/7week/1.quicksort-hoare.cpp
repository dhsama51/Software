#include <iostream>
using namespace std;

int hoarecase[1000] = { 0, };
int lomutocase[1000] = { 0, };

int hoare_comp_count = 0;
int hoare_swap_count = 0;
int lomuto_comp_count = 0;
int lomuto_swap_count = 0;

int partition_hoare(int start, int end) {
	int pivot = hoarecase[start];
	int i = start-1;
	int j = end+1;
	
	while (1) {
		do {
			i++;
			hoare_comp_count++;
		}
		while (hoarecase[i] < pivot);

		do {
			j--;
			hoare_comp_count++;
		}
		while (hoarecase[j] > pivot);
		
		if (i < j) {
			hoare_swap_count++;
			swap(hoarecase[i], hoarecase[j]);
		}
		else return j;
	}
}

void quick_sort_hoare(int start, int end) {
	if (start >= end) return;

	int pivotpos = partition_hoare(start, end);
	quick_sort_hoare(start, pivotpos);
	quick_sort_hoare(pivotpos + 1, end);
}

int partition_lomuto(int start, int end) {
	int pivot = lomutocase[start];

	int j = start;
	for (int i = start + 1; i <= end; i++) {
		lomuto_comp_count++;
		if (lomutocase[i] < pivot) {
			j++;
			lomuto_swap_count++;
			swap(lomutocase[i], lomutocase[j]);
		}
	}
	int pivotpos = j;
	lomuto_swap_count++;
	swap(lomutocase[start], lomutocase[pivotpos]);
	return pivotpos;
}

void quick_sort_lomuto(int start, int end) {
	if (start >= end) return;

	int pivotpos = partition_lomuto(start, end);
	quick_sort_lomuto(start, pivotpos-1);
	quick_sort_lomuto(pivotpos + 1, end);
}

int main() {
	int a = 0;
	int n = 0;
	
	cin >> a;
	for (int i = 0; i < a; i++) {
		cin >> n;
		for (int j = 0; j < n; j++)
			cin >> hoarecase[j];
		for (int j = 0; j < n; j++)
			lomutocase[j] = hoarecase[j];

		hoare_swap_count = 0;
		lomuto_swap_count = 0;
		hoare_comp_count = 0;
		lomuto_comp_count = 0;

		quick_sort_hoare(0, n - 1);
		quick_sort_lomuto(0, n - 1);
		
		cout << hoare_swap_count << " ";
		cout << lomuto_swap_count << " ";
		cout << hoare_comp_count << " ";
		cout << lomuto_comp_count << endl;
	}

	return 0;
}