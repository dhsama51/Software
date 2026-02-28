#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

vector<vector<int>> node;
vector<int> visit_order; //몇 번째로 방문했는지 기록. 이걸로 방문 여부도 확인
vector<int> parent;
vector<int> parentest; // 최대조상 기록
vector<int> child_count; //1이 반드시 세지는 것을 방지하기 위함
vector<bool> articulation;
int bicon_comp_count = 0;

//교재 스도코드는 biconnected component를 구성하는 edge 출력 -> 수정 필요
void cal_biconnected_component(int cur_node, int order) {
	parentest[cur_node] = visit_order[cur_node] = ++order;
	
	for (int near : node[cur_node]) {
		//교재엔 이 사이에 edge 출력 관련 내용 존재. 필요 X
		if (visit_order[near] == 0) { //첫 방문인 경우
			parent[near] = cur_node;
			child_count[cur_node]++;
			cal_biconnected_component(near, order);

			int a = parentest[cur_node];
			int b = parentest[near];
			parentest[cur_node] = a < b ? a : b; //비교해서 최대조상 업데이트

			//조상이 없고 자식이 여럿인 경우(조상 부분 없으면 오류)
			if (parent[cur_node] == 0 && child_count[cur_node] > 1)
				articulation[cur_node] = 1;

			//조상이 있고, near의 최대조상이 현재 노드의 방문 순서보다 크거나 같은 경우
			//->현재 노드 이상 못 올라가므로 articulation
			if (parent[cur_node] != 0 && parentest[near] >= visit_order[cur_node])
				articulation[cur_node] = 1;

			//near의 최대조상이 현재 노드의 방문 순서보다 크거나 같은 경우
			if (parentest[near] >= visit_order[cur_node])
				bicon_comp_count++;
		}

		else { //이미 방문한 경우 -> 비교해서 최대조상 업데이트
			int a = parentest[cur_node];
			int b = visit_order[near];
			parentest[cur_node] = a < b ? a : b;
		}
	}
}

int main() {
	int testcase = 0, node_num = 0, edge_num_per_node=0;

	cin >> testcase;
	for (int i = 0; i < testcase; i++) {
		cin >> node_num;

		node.assign(node_num + 1, vector<int>(0));
		visit_order.assign(node_num + 1, 0);
		child_count.assign(node_num + 1, 0);
		parent.assign(node_num + 1, 0);
;		parentest.assign(node_num + 1, 0);
		articulation.assign(node_num + 1, 0);
		bicon_comp_count = 0;

		for (int j = 1; j <= node_num; j++) {
			cin >> edge_num_per_node >> edge_num_per_node;
			node[j].assign(edge_num_per_node, 0);
			for (int k = 0; k < edge_num_per_node; k++) {
				cin >> node[j][k];
			}
		}


		cal_biconnected_component(1, 0);
		vector<int> result;
		for (int j = 1; j <= node_num; j++) {
			if (articulation[j]) result.push_back(j);
		}

		cout << bicon_comp_count << endl;
		cout << result.size() << " ";
		for (int arti : result) {
			cout << arti << " ";
		}
		cout << endl;
	}
	return 0;
}