/*
* ���� 1068�� Ʈ��
* https://www.acmicpc.net/problem/1068
* �׷��� Ž�� - ���� �켱 Ž�� / Ʈ��
*/
#include <iostream>
#include <vector>
using namespace std;

int rootNodeNum;
int nodeCnt;
int deleteNodeNum;
vector<int> childNode[50];
int parNode[50];
int ans;

void order(int nodeNum) {
	if (childNode[nodeNum].size() == 0) {
		ans++;
		return;
	}
	else {
		for (int child : childNode[nodeNum]) {
			order(child);
		}
	}


}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	cin >> nodeCnt;

	int parNodeNum;
	for (int i = 0; i < nodeCnt; i++) {
		cin >> parNodeNum;
		if (parNodeNum == -1) {
			rootNodeNum = i;
			continue;
		}
		childNode[parNodeNum].push_back(i);
		parNode[i] = parNodeNum;
	}

	cin >> deleteNodeNum;

	if (deleteNodeNum != rootNodeNum) {
		int deleteNodeParent = parNode[deleteNodeNum];

		for (int i = 0; i < childNode[deleteNodeParent].size(); i++) {
			if (deleteNodeNum == childNode[deleteNodeParent][i]) {
				childNode[deleteNodeParent].erase(childNode[deleteNodeParent].begin() + i);
				break;
			}
		}

		order(rootNodeNum);
	}


	cout << ans;

	return 0;
}
