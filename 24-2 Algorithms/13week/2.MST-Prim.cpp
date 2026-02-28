#include <iostream>
#include <vector>
using namespace std;;

vector<int> node(1001);
vector<int> nearest(1001);
vector<int> distance_(1001);
vector<vector<int>> edge(1001, vector<int>(1001));

int prim(int node_num) {
	int mst_sum = 0;
	int near_to_Y = 0;

	for (int i = 2; i <= node_num; i++) {
		nearest[i] = 1;
		distance_[i] = edge[1][i];
	}
	
	for (int i = 2; i <= node_num; i++) {
		int min = 101;
		for (int j = 2; j <= node_num; j++) {
			if (distance_[j] >= 0 && distance_[j] < min) {
				min = distance_[j];
				near_to_Y = j;
			}
		}
		mst_sum += min;
		distance_[near_to_Y] = -1;
		for (int j = 2; j <= node_num; j++) {
			if (edge[j][near_to_Y] < distance_[j]) {
				nearest[j] = near_to_Y;
				distance_[j] = edge[j][near_to_Y];
			}
		}
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
				edge[j][k] = 101;

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