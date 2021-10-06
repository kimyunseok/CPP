/*
* ���� 2110�� ������ ��ġ
* https://www.acmicpc.net/problem/2110
* �̺� Ž��
*/
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int houseCnt, shareCnt;
vector<int> houseVec;
int ans;

bool checkAllocate(int size) {
	int lastAllocate = houseVec[0];
	int curAllocateCnt = 1;

	for (int i = 1; i < houseVec.size(); i++) {
		int curHousePoint = houseVec[i];
		int diff = curHousePoint - lastAllocate;
		if (diff >= size) {
			lastAllocate = curHousePoint;
			curAllocateCnt++;
		}

		if (curAllocateCnt == shareCnt) {
			return true;
		}
	}

	return false;
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	cin >> houseCnt >> shareCnt;

	int input;
	for (int i = 1; i <= houseCnt; i++) {
		cin >> input;
		houseVec.push_back(input);
	}

	sort(houseVec.begin(), houseVec.end());

	int front = 1; // Ʋ�� ����. ���� ���� �� ���� ���� ���� ��ȣ�� �����ؼ� Ʋ��. ������ 1���� Ž���ؾ� ��.
	int tail = houseVec[houseVec.size() - 1];
	int middle = 0;

	while (front < tail) {
		middle = (front + tail) / 2;

		bool check = checkAllocate(middle);

		if (check) {
			front = middle + 1;
			ans = middle;
		}
		else {
			tail = middle;
		}
	}

	cout << ans;

	return 0;
}
