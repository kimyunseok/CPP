/*
* ���� 21608�� ��� �ʵ��б�
* https://www.acmicpc.net/problem/21608
* ����
*/
#include <iostream>
#include <vector>
using namespace std;

// |r1 - r2| + |c1 - c2| = 1�� ������ ��. ->���� 1���̳��ų� ���� 1���̳��� ��� ��, ��, ��, ��, ��

int studentCnt;
int classRoom[21][21];
int nearLeft[21][21];
vector<int> studentVec;
vector<int> studentLove[401]; // Ʋ������ : �л� ���� �ִ� 21���� �ƴ϶� 401���̴�.
pair<int, int> direction[4] = {
	{-1, 0},
	{1, 0},
	{0, -1},
	{0, 1}
};
int addLove[5] = { 0, 1, 10, 100, 1000 };
int ans;

int checkNearLover(int studentNum, int r, int c) {
	int ret = 0;
	for (int i = 0; i < 4; i++) {
		int nxtR = r + direction[i].first;
		int nxtC = c + direction[i].second;
		if (nxtR < 1 || nxtR > studentCnt || nxtC < 1 || nxtC > studentCnt ||
			classRoom[nxtR][nxtC] == 0) {
			continue;
		}

		for (int j = 0; j < 4; j++) {
			if (classRoom[nxtR][nxtC] == studentLove[studentNum][j]) {
				ret++;
				break;
			}
		}
	}
	return ret;
}

void go(int studentNum) {
	int inputR = 0;
	int inputC = 0;
	int curNearLoveCnt = -1;
	for (int i = 1; i <= studentCnt; i++) {
		for (int j = 1; j <= studentCnt; j++) {
			if (classRoom[i][j] != 0) { continue; }
			else {
				int nearLoveCnt = checkNearLover(studentNum, i, j);
				if ((nearLoveCnt > curNearLoveCnt) || (curNearLoveCnt == nearLoveCnt &&
					nearLeft[inputR][inputC] < nearLeft[i][j])) {
					inputR = i;
					inputC = j;
					curNearLoveCnt = nearLoveCnt;
				}

			}
		}
	}

	for (int i = 0; i < 4; i++) {
		int nxtR = inputR + direction[i].first;
		int nxtC = inputC + direction[i].second;
		if (nxtR < 1 || nxtR > studentCnt || nxtC < 1 || nxtC > studentCnt) {
			continue;
		}

		nearLeft[nxtR][nxtC]--;
	}

	classRoom[inputR][inputC] = studentNum;
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	cin >> studentCnt;

	for (int i = 1; i <= studentCnt; i++) {
		for (int j = 1; j <= studentCnt; j++) {
			for (int k = 0; k < 4; k++) {
				int nxtR = i + direction[k].first;
				int nxtC = j + direction[k].second;
				if (nxtR < 1 || nxtR > studentCnt || nxtC < 1 || nxtC > studentCnt) {
					continue;
				}

				nearLeft[i][j]++;
			}
		}
	}

	int studentNum;
	int loveNum;
	for (int i = 1; i <= studentCnt * studentCnt; i++) {
		cin >> studentNum;
		studentVec.push_back(studentNum);
		for (int i = 1; i <= 4; i++) {
			cin >> loveNum;
			studentLove[studentNum].push_back(loveNum);
		}
	}

	for (int i = 0; i < studentCnt * studentCnt; i++) {
		int num = studentVec[i];
		go(num);
	}

	for (int i = 1; i <= studentCnt; i++) {
		for (int j = 1; j <= studentCnt; j++) {
			int student = classRoom[i][j];
			int curLove = 0;
			for (int k = 0; k < 4; k++) {
				int nxtR = i + direction[k].first;
				int nxtC = j + direction[k].second;
				if (nxtR < 1 || nxtR > studentCnt || nxtC < 1 || nxtC > studentCnt) {
					continue;
				}

				for (int l = 0; l < 4; l++) {
					if (classRoom[nxtR][nxtC] == studentLove[student][l]) {
						curLove++;
						break;
					}
				}
			}
			ans += addLove[curLove];
		}
	}

	cout << ans;

	return 0;
}