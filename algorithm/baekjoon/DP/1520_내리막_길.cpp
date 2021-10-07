/*
* ���� 1520�� ������ ��
* https://www.acmicpc.net/problem/1520
* �׷��� Ž�� �̷�, ���̳��� ���α׷���
*/
#include <iostream>
using namespace std;

int height, width;
int board[501][501];
int dp[501][501];
bool visit[501][501]; // Ʋ������. �ش� ĭ�� �ƿ� 0�� �� �ִ�. ���� ĭ�� ���� 0�̶�� �湮���� ���� ������ �����ؼ� Ʋ��.
pair<int, int> direction[4] = {
	{-1, 0},
	{1, 0},
	{0, -1},
	{0, 1}
};

int go(int r, int c, int prev) {
	if (r < 1 || r > height || c < 1 || c > width) { return 0; }

	if (board[r][c] >= prev) { return 0; }

	if (r == height && c == width) { return 1; }

	if (visit[r][c]) { return dp[r][c]; }

	visit[r][c] = true;

	for (pair<int, int> dir : direction) {
		int nxtR = r + dir.first;
		int nxtC = c + dir.second;
		dp[r][c] += go(nxtR, nxtC, board[r][c]);
	}

	return dp[r][c];

}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	cin >> height >> width;

	for (int i = 1; i <= height; i++) {
		for (int j = 1; j <= width; j++) {
			cin >> board[i][j];
		}
	}

	go(1, 1, 10001);

	cout << dp[1][1];

	return 0;
}