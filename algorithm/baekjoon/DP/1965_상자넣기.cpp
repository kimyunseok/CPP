/*
* ���� 1965�� ���ڳֱ�
* https://www.acmicpc.net/problem/1965
* ���̳��� ���α׷���
*/
#include <iostream>
#include <algorithm>
using namespace std;

int boxCnt;
int boxSize[1001];
int dp[1001];
int ans = 1;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	cin >> boxCnt;

	for (int i = 1; i <= boxCnt; i++) {
		cin >> boxSize[i];
	}

	for (int i = 1; i <= boxCnt; i++) {
		dp[i] = 1; // Ʋ������, �ϳ��� �ִ� �� ���������.
		for (int j = 1; j < i; j++) {
			if (boxSize[j] >= boxSize[i]) { continue; }
			dp[i] = max(dp[i], dp[j] + 1);
		}
		ans = max(ans, dp[i]);
	}

	cout << ans;

	return 0;
}
