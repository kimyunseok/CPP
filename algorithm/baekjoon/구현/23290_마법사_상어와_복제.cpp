/*
* ���� 23290�� ������ ���� ����
* https://www.acmicpc.net/problem/23290
* ���� / �ùķ��̼�
*/
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

class Fish {
public:
	int r;
	int c;
	int dir;
	bool isDead;
	Fish(int r, int c, int dir, bool isDead) {
		this->r = r;
		this->c = c;
		this->dir = dir;
		this->isDead = isDead;
	}
};

pair<int, int> fishDir[9] = {
	{-1, -1},
	{0, -1},
	{-1, -1},
	{-1, 0},
	{-1, 1},
	{0, 1},
	{1, 1},
	{1, 0},
	{1, -1}
};

pair<int, int> sharkDir[5] = {
	{-1, -1},
	{-1, 0},
	{0, -1},
	{1, 0},
	{0, 1}
};

vector<vector<vector<Fish>>> fishVec(5, vector<vector<Fish>>(5, vector<Fish>()));
vector<Fish> copyVec;
vector<Fish> moveVec;
pair<int, int> curSharkPoint;
int maxRemovedsharkMove;
int maxRemovedFishCnt;
int smellMap[5][5];
int fishCntTmpMap[5][5];
int fishCnt;
int spellCnt;
int ans;

void findSharkMove(int idx, int prevMove, int removedFishCnt, int prevR, int prevC) {
	if (idx == 4) {
		if (maxRemovedFishCnt < removedFishCnt) {
			maxRemovedFishCnt = removedFishCnt;
			maxRemovedsharkMove = prevMove;
		}
		else if (maxRemovedFishCnt == removedFishCnt) {
			maxRemovedsharkMove = min(maxRemovedsharkMove, prevMove);
		}
		return;
	}
	else {
		prevMove *= 10;
		for (int i = 1; i <= 4; i++) {
			int nxtR = prevR + sharkDir[i].first;
			int nxtC = prevC + sharkDir[i].second;
			if (nxtR > 4 || nxtR < 1 || nxtC > 4 || nxtC < 1) { continue; }
			int originalFishCnt = fishCntTmpMap[nxtR][nxtC];
			fishCntTmpMap[nxtR][nxtC] = 0;
			findSharkMove(idx + 1, prevMove + i, removedFishCnt + originalFishCnt, nxtR, nxtC);
			fishCntTmpMap[nxtR][nxtC] = originalFishCnt;
		}
	}
}

void go(int idx) {

	if (idx > spellCnt) { return; }

	// 1. ����� ���� - 5���� ����������
	copyVec.clear();
	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			for (int k = 0; k < fishVec[i][j].size(); k++) {
				if (!fishVec[i][j][k].isDead) {
					copyVec.push_back(fishVec[i][j][k]);
				}
			}
		}
	}

	// 2. ����� �� ĭ �̵�
	/*
	* - 1. ��� �ִ� ĭ �̵� �Ұ�
	* - 2. ����� ���� �ִ� ĭ �̵� �Ұ� (�ش� ĭ ����� ���� idx���� 2 ���ϸ� �̵� �Ұ�)
	* - 3. ���� ����� ĭ �̵� �Ұ�
	* �̵� ������ �� ���� 45�� �ݽð� ȸ��(�̵����⿡�� -1, 0�̸� 8 9�� 1)
	*/
	moveVec.clear();
	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			for (int k = 0; k < fishVec[i][j].size(); k++) {
				if (!fishVec[i][j][k].isDead) {

					int curDir = fishVec[i][j][k].dir;

					int turnIdx = 8;

					while (turnIdx--) {
						int nxtR = i + fishDir[curDir].first;
						int nxtC = j + fishDir[curDir].second;
						if ((nxtR > 4 || nxtC > 4 || nxtR < 1 || nxtC < 1) ||
							(nxtR == curSharkPoint.first && nxtC == curSharkPoint.second) ||
							(smellMap[nxtR][nxtC] != 0 && idx - smellMap[nxtR][nxtC] <= 2)) {
							curDir -= 1;
							if (curDir == 0) { curDir = 8; }
							continue;
						}
						else {
							moveVec.push_back(Fish(nxtR, nxtC, curDir, false));
							fishVec[i][j][k].isDead = true;
							break;
						}
					}
				}

			}
		}
	}
	for (Fish fish : moveVec) {
		fishVec[fish.r][fish.c].push_back(fish);
	}

	// 3. ��� 3ĭ �̵� ��, ��, ��, �� 
	/*
	* ����� ���� �˾Ƴ��� ���� ��Ű�� ���� �����.
	*/
	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			fishCntTmpMap[i][j] = 0;
			for (int k = 0; k < fishVec[i][j].size(); k++) {
				if (!fishVec[i][j][k].isDead) { fishCntTmpMap[i][j]++; }
			}
		}
	}
	maxRemovedFishCnt = -1;
	findSharkMove(1, 0, 0, curSharkPoint.first, curSharkPoint.second);

	int firstMove = maxRemovedsharkMove / 100;
	int secondMove = (maxRemovedsharkMove - (firstMove * 100)) / 10;
	int thirdMove = maxRemovedsharkMove - (firstMove * 100) - (secondMove * 10);

	//cout << firstMove << " " << secondMove << " " << thirdMove << "\n";

	//first
	curSharkPoint.first += sharkDir[firstMove].first;
	curSharkPoint.second += sharkDir[firstMove].second;
	int liveFishCnt = 0;
	for (int i = 0; i < fishVec[curSharkPoint.first][curSharkPoint.second].size(); i++) {
		if (!fishVec[curSharkPoint.first][curSharkPoint.second][i].isDead) {
			liveFishCnt++;
		}
	}
	if (liveFishCnt > 0) {
		smellMap[curSharkPoint.first][curSharkPoint.second] = idx;
		fishVec[curSharkPoint.first][curSharkPoint.second].clear();
	}
	
	//second
	curSharkPoint.first += sharkDir[secondMove].first;
	curSharkPoint.second += sharkDir[secondMove].second;
	liveFishCnt = 0;
	for (int i = 0; i < fishVec[curSharkPoint.first][curSharkPoint.second].size(); i++) {
		if (!fishVec[curSharkPoint.first][curSharkPoint.second][i].isDead) {
			liveFishCnt++;
		}
	}
	if (liveFishCnt > 0) {
		smellMap[curSharkPoint.first][curSharkPoint.second] = idx;
		fishVec[curSharkPoint.first][curSharkPoint.second].clear();
	}

	//third
	curSharkPoint.first += sharkDir[thirdMove].first;
	curSharkPoint.second += sharkDir[thirdMove].second;
	liveFishCnt = 0;
	for (int i = 0; i < fishVec[curSharkPoint.first][curSharkPoint.second].size(); i++) {
		if (!fishVec[curSharkPoint.first][curSharkPoint.second][i].isDead) {
			liveFishCnt++;
		}
	}
	if (liveFishCnt > 0) {
		smellMap[curSharkPoint.first][curSharkPoint.second] = idx;
		fishVec[curSharkPoint.first][curSharkPoint.second].clear();
	}
	
	// 4. ���� ����. - ������ 2 �������� �˻��ϹǷ� �� �ʿ� X

	// 5. ����� ����
	for (Fish fish : copyVec) {
		fishVec[fish.r][fish.c].push_back(fish);
	}

	go(idx + 1);
}

int main() {
	cin >> fishCnt >> spellCnt;

	int r, c, dir;
	for (int i = 1; i <= fishCnt; i++) {
		cin >> r >> c >> dir;
		fishVec[r][c].push_back(Fish(r, c, dir, false));
	}

	cin >> curSharkPoint.first >> curSharkPoint.second;

	go(1);

	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			for (int k = 0; k < fishVec[i][j].size(); k++) {
				if (!fishVec[i][j][k].isDead) {
					ans++;
				}
			}
		}
	}

	cout << ans;

	return 0;
}