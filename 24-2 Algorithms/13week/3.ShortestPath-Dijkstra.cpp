#include <iostream>
#include <vector>
using namespace std;;

vector<int> node(1001);
vector<int> touch(1001);
vector<int> length(1001);
vector<vector<int>> edge(1001, vector<int>(1001));

int prim(int node_num) {
	int mst_sum = 0;
	int near_to_Y = 0;

	for (int i = 2; i <= node_num; i++) {
		touch[i] = 1;
		length[i] = edge[1][i];
	}

	for (int i = 2; i <= node_num; i++) {
		int min = 100000;
		for (int j = 2; j <= node_num; j++) {
			if (length[j] >= 0 && length[j] < min) {
				min = length[j];
				near_to_Y = j;
			}
		}
		mst_sum += edge[touch[near_to_Y]][near_to_Y];
		for (int j = 2; j <= node_num; j++) {
			if (length[near_to_Y] + edge[near_to_Y][j] < length[j]) {
				touch[j] = near_to_Y;
				length[j] = length[near_to_Y] + edge[near_to_Y][j];
			}
		}
		length[near_to_Y] = -1;
	}

	return mst_sum;
}

int main() {
	int testcase = 0;
	int node_num = 0, edge_num_per_node = 0;
	int end = 0, weight = 0;

	cin >> testcase;
	for (int i = 0; i < testcase; i++) {
		cin >> node_num;

		for (int j = 1; j <= node_num; j++)
			for (int k = 1; k <= node_num; k++)
				edge[j][k] = 100000;

		for (int j = 1; j <= node_num; j++) {
			cin >> edge_num_per_node >> edge_num_per_node;
			for (int k = 0; k < edge_num_per_node; k++) {
				cin >> end >> weight;
				edge[j][end] = weight;
			}
		}

		cout << prim(node_num) << endl;
	}

	return 0;
}