/*
* ���� 2230�� �� ����
* https://www.acmicpc.net/problem/2230
* �� ������
*/
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int numCnt, minDiff;
vector<int> numVec;
int ans = 2100000000; // Ʋ������ 3 : �ִ� ���̰� 20����.

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	cin >> numCnt >> minDiff;

	int input;
	for (int i = 1; i <= numCnt; i++) {
		cin >> input;
		numVec.push_back(input);
	}

	sort(numVec.begin(), numVec.end());

	int front = 0;
	int tail = 1;

	while (front <= tail && tail < numCnt) { // Ʋ������ 1 : front < tail�� �ϸ� Ʋ�� ex.) 2 0 1 2
		int frontNum = numVec[front];
		int tailNum = numVec[tail];
		int diff = tailNum - frontNum;

		if (diff == minDiff) {
			ans = diff;
			break;
		}
		else if (diff < minDiff) {
			tail++;
		}
		else if (diff > minDiff) {
			ans = min(ans, diff); // Ʋ�� ���� 2 : ans�� �׻� �� ���� ������ ������ ����Ѵ�.
			front++;
		}
	}

	cout << ans;

	return 0;
}
