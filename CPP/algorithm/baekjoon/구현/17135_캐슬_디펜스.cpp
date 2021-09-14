/*
* ���� 17135�� ĳ�� ���潺
* https://www.acmicpc.net/problem/17135
* ���� / �ùķ��̼� / ���Ʈ���� �˰���
*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;

class EnemyForArcher {
public:
    int r;
    int c;
    int distance;
    EnemyForArcher(int r, int c, int distance) {
        this->r = r;
        this->c = c;
        this->distance = distance;
    }
};

int height, width, maxAttackDistance;
vector<pair<int, int>> archerVec; // i - 1���� �����Ѵ�.
vector<pair<int, int>> enemyPointVec;
int board[17][16]; // ���� + 1�� �ü��� ��ġ, ���� 16�� ���̰� �� �� �ִ�.
int boardForGame[17][16];
vector<EnemyForArcher> enemyInfoVec[4]; //�ü��� ���� ��ǥ�� ���� �Ÿ��� ��ǥ�� ��Ƶδ� ����.
int ans;

bool comp(EnemyForArcher e1, EnemyForArcher e2) {
    if (e1.distance < e2.distance) {
        return true;
    }
    else if (e1.distance == e2.distance) {
        if (e1.c < e2.c) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

void saveEnemyInfo(int archerIdx, int r, int c) {
    for (pair<int, int> enemyPoint : enemyPointVec) {
        int enemyR = enemyPoint.first;
        int enemyC = enemyPoint.second;
        int distance = abs(r - enemyR) + abs(c - enemyC);

        enemyInfoVec[archerIdx].push_back(EnemyForArcher(enemyR, enemyC, distance));
    }
    sort(enemyInfoVec[archerIdx].begin(), enemyInfoVec[archerIdx].end(), comp);
}

void startGame() {
    int enemyCnt = enemyPointVec.size();
    int killCnt = 0;
    for (int i = 1; i <= height; i++) {
        for (int j = 1; j <= width; j++) {
            boardForGame[i][j] = board[i][j];
        }
    }

    for (int i = 1; i <= 3; i++) {
        enemyInfoVec[i].clear();
        int archerC = archerVec[i - 1].second;
        saveEnemyInfo(i, height + 1, archerC);
    }

    while (enemyCnt > 0) {
        // 1. �ü��� �Ÿ� D ���� & ���� ���� & ���� ����� �� ����
        for (int i = 1; i <= 3; i++) {
            for (int j = 0; j < enemyInfoVec[i].size(); j++) {
                int enemyDistance = enemyInfoVec[i][j].distance;
                int enemyR = enemyInfoVec[i][j].r;
                int enemyC = enemyInfoVec[i][j].c;
                int curState = boardForGame[enemyR][enemyC];

                if (enemyDistance > maxAttackDistance) { break; }
                else {
                    if (curState == 1) {
                        killCnt++;
                        enemyCnt--;
                        boardForGame[enemyR][enemyC] = 0;
                    }
                    break; // ���ð����̹Ƿ�,
                }
            }
        }

        // 2 - 1. �� �Ʒ��� �� ĭ �̵�(����)
        for (int i = height; i >= 0; i--) { // ������ 0���� �ؾ� 1���̿� �ִ� ���� ������ ����� �� �ִ�.
            for (int j = 1; j <= width; j++) {
                if (i == height) {
                    if (boardForGame[i][j] == 1) {
                        enemyCnt--; // �� N + 1�� �ͼ� ���ӿ��� ����.
                    }
                }
                else {
                    boardForGame[i + 1][j] = boardForGame[i][j]; // �Ʒ� ĭ���� �̵���Ŵ.
                }
            }
        }

        // 2 - 2. �� �Ʒ��� �� ĭ �̵�(��ǥ ���� ����)
        for (int i = 1; i <= 3; i++) {
            for (int j = 0; j < enemyInfoVec[i].size(); j++) {
                int enemyDistance = enemyInfoVec[i][j].distance;
                int enemyR = enemyInfoVec[i][j].r;
                int enemyC = enemyInfoVec[i][j].c;
                int nxtR = enemyR + 1;
                int curState = boardForGame[nxtR][enemyC];

                if (enemyR + 1 > height || curState == 0) {
                    // �Ʒ��� �� ĭ �̵����� ��, height���� ũ�ٸ� ���Ϳ��� ����.
                    enemyInfoVec[i].erase(enemyInfoVec[i].begin() + j);
                    j--; // ���� ����� 1�پ������Ƿ�, j�� ������Ű�� �ʴ´�.
                    continue;
                }

                enemyInfoVec[i][j].r = nxtR;

                //�Ÿ� ���� �������.
                int archerR = archerVec[i - 1].first;
                int archerC = archerVec[i - 1].second;
                enemyInfoVec[i][j].distance = abs(archerR - (enemyR + 1)) + abs(archerC - enemyC);
            }
            sort(enemyInfoVec[i].begin(), enemyInfoVec[i].end(), comp);
        }

    }
    ans = max(ans, killCnt);
}

void setArcher(int startArcherPoint) {
    if (archerVec.size() >= 3) {
        startGame();
        return;
    }
    else {
        int archerIdx = archerVec.size() + 1;
        for (int i = startArcherPoint; i <= width; i++) {
            archerVec.push_back({ height + 1, i });
            setArcher(i + 1);
            archerVec.pop_back();
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    cin >> height >> width >> maxAttackDistance;

    for (int i = 1; i <= height; i++) {
        for (int j = 1; j <= width; j++) {
            cin >> board[i][j];
            if (board[i][j] == 1) {
                enemyPointVec.push_back({ i, j });
            }
        }
    }

    setArcher(1);

    cout << ans;

    return 0;
}