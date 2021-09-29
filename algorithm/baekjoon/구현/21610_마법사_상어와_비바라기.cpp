/*
* ���� 21610�� ������ ���� ��ٶ��
* https://www.acmicpc.net/problem/21610
* ���� / �ùķ��̼�
*/
#include <iostream>
#include <vector>
using namespace std;

int widthHeight, testCaseCnt;
int board[51][51];
bool check[51][51][101];
int curStage, curDirection, curMovement;
int ans;
vector<pair<int, int>> cloudPoint;

pair<int, int> direction[9] = {
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

int copyWater(int _y, int _x) {
    int result = 0;
    int checkY = _y + direction[2].first;
    int checkX = _x + direction[2].second;
    if (1 <= checkY && checkY <= widthHeight && 1 <= checkX && checkX <= widthHeight
        && board[checkY][checkX] > 0) {
        result++;
    }
    checkY = _y + direction[4].first;
    checkX = _x + direction[4].second;
    if (1 <= checkY && checkY <= widthHeight && 1 <= checkX && checkX <= widthHeight
        && board[checkY][checkX] > 0) {
        result++;
    }
    checkY = _y + direction[6].first;
    checkX = _x + direction[6].second;
    if (1 <= checkY && checkY <= widthHeight && 1 <= checkX && checkX <= widthHeight
        && board[checkY][checkX] > 0) {
        result++;
    }
    checkY = _y + direction[8].first;
    checkX = _x + direction[8].second;
    if (1 <= checkY && checkY <= widthHeight && 1 <= checkX && checkX <= widthHeight
        && board[checkY][checkX] > 0) {
        result++;
    }

    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    cin >> widthHeight >> testCaseCnt;

    for (int i = 1; i <= widthHeight; i++) {
        for (int j = 1; j <= widthHeight; j++) {
            cin >> board[i][j];
        }
    }

    //���� ��ٶ�� ����
    cloudPoint.push_back({ widthHeight - 1, 1 });
    cloudPoint.push_back({ widthHeight - 1, 2 });
    cloudPoint.push_back({ widthHeight, 1 });
    cloudPoint.push_back({ widthHeight, 2 });

    while (curStage++ < testCaseCnt) {
        cin >> curDirection >> curMovement;

        //1. ���� ��ǥ�̵�
        for (int i = 0; i < cloudPoint.size(); i++) {
            int curY = cloudPoint[i].first;
            int curX = cloudPoint[i].second;

            for (int i = 1; i <= curMovement; i++) {
                curY += direction[curDirection].first;
                curX += direction[curDirection].second;
                if (curY < 1) { curY = widthHeight; }
                if (curY > widthHeight) { curY = 1; }
                if (curX < 1) { curX = widthHeight; }
                if (curX > widthHeight) { curX = 1; }
            }
            check[curY][curX][curStage] = true;
            cloudPoint[i].first = curY;
            cloudPoint[i].second = curX;
            //2. �ش� ���� ĭ�� �� �� ����
            board[curY][curX]++;
        }

        //3. �� ���� ����
        for (pair<int, int> point : cloudPoint) {
            int curY = point.first;
            int curX = point.second;
            board[curY][curX] += copyWater(curY, curX);
        }

        // ���� ��ǥ�� ����
        cloudPoint.clear();

        for (int i = 1; i <= widthHeight; i++) {
            for (int j = 1; j <= widthHeight; j++) {
                if (board[i][j] >= 2 && !check[i][j][curStage]) {
                    board[i][j] -= 2;
                    cloudPoint.push_back({ i, j });
                }
            }
        }
    }

    for (int i = 1; i <= widthHeight; i++) {
        for (int j = 1; j <= widthHeight; j++) {
            ans += board[i][j];
        }
    }

    cout << ans;

    return 0;
}