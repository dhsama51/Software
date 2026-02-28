#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
using namespace std;

vector<int> node, parent, size_;
vector<tuple<int, int, int>> edges;

int find(int i) {
	while (parent[i] != i) {
		parent[i] = parent[parent[i]];
		i = parent[i];
	}
	return i;
}

void union_(int p, int q) {
	int i = find(p), j = find(q);
	if (i != j) {
		if (size_[i] < size_[j]) {
			parent[i]= j;
			size_[j] += size_[i];
		}
		else {
			parent[j] = i;
			size_[i] += size_[j];
		}
	}
}

int kruskal(int node_num, int edge_num) {
	sort(edges.begin(), edges.end(), [](auto& a, auto& b) {
		return get<0>(a) < get<0>(b);
		});

	int mst_sum = 0;
	for (auto& edge : edges) {
		int start = get<1>(edge);
		int end = get<2>(edge);

		if (find(start) != find(end)) {
			union_(start, end);
			int weight = get<0>(edge);
			mst_sum += weight;
		}
	}

	return mst_sum;
}

int main() {
	int testcase = 0, node_num = 0, edge_num_per_node = 0;
	int dep = 0, weight = 0;
	int result;
	int temp;

	cin >> testcase;
	for (int i = 0; i < testcase; i++) {
		int edge_num = 0;
		cin >> node_num;
		node.clear();
		parent.clear();
		size_.clear();
		edges.clear();
		
		for (int j = 0; j <= node_num; j++) {
			node.push_back(j);
			parent.push_back(j);
			size_.push_back(1);
		}

		for (int j = 1; j <= node_num; j++) {
			cin >> edge_num_per_node >> edge_num_per_node; //입력 하나 패스
			for (int k = 0; k < edge_num_per_node; k++) {
				cin >> dep >> weight;
				if (dep < j) continue; // 중복 edge 제거
				edges.push_back(make_tuple(weight, j, dep));
				edge_num++;
			}
		}

		result = kruskal(node_num, edge_num);
		cout << result << endl;
	}

	return 0;
}