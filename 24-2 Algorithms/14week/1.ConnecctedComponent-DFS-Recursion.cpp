#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

vector<vector<int>> node;
vector<bool> visited;
vector<int> conn_com_size;
int conn_com_count = 0;

void cal_connected_component(int node_name) {
	visited[node_name] = true;
	conn_com_size[conn_com_count]++;

	for (int neighbor : node[node_name]) {
		if (!visited[neighbor])
			cal_connected_component(neighbor);
	}
	
}

int main() {
	int testcase, node_num, edge_num_per_node;
	int tmp;
	
	cin >> testcase;
	for (int i = 0; i < testcase; i++) {
		cin >> node_num;

		node.clear();
		visited.clear();
		conn_com_size.resize(0);
		conn_com_count = 0;
		node.resize(node_num+1);
		visited.resize(node_num+1, 0);

		for (int j = 1; j <= node_num; j++) {
			cin >> edge_num_per_node >> edge_num_per_node;
			for (int k = 0; k < edge_num_per_node; k++) {
				cin >> tmp;
				node[j].push_back(tmp);
			}
		}

		for (int j = 1; j <= node_num; j++) {
			if (!visited[j]) {
				conn_com_size.push_back(0);
				cal_connected_component(j);
				conn_com_count++;
			}
		}
		sort(conn_com_size.begin(), conn_com_size.end());
		cout << conn_com_count << " ";
		for (int size_ : conn_com_size) {
			cout << size_ << " ";
		}
		cout << endl;
	}

	return 0;
}