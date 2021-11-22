/*
* ���� 20057�� ������ ���� ����̵�
* https://www.acmicpc.net/problem/20057
* ���� / �ùķ��̼�
*/
#include <iostream>
using namespace std;

class Direction {
public:
    int r;
    int c;
    int percentage;
    Direction(int r, int c, int percentage) {
        this->r = r;
        this->c = c;
        this->percentage = percentage;
    }
};

int widthHeight;
pair<int, int> curPoint;
int maxMoveCnt = 1; // �̵��ϴ� Ƚ��. ���� ��, �� 1���ϰ� �� �� 2���� �ϰ� �ٽ� ��, �� 3����... �̷��� ������� �뵵.
int curMoveCnt = 0;
int board[500][500];
int ans;
int curDirection = 0; // 0 : ��, 1 : ��, 2: ��, 3: ��
Direction moveDirection[4][10] = { // �̵��� ĭ ����.
    //9 idx�� �ִ� ���� ������ �̵��� ��
    {
        Direction(-1, 1, 1), Direction(1, 1, 1), Direction(-1, 0, 7),  Direction(-2, 0, 2), Direction(1, 0, 7),
     Direction(2, 0, 2),  Direction(-1, -1, 10),  Direction(1, -1, 10),  Direction(0, -2, 5),  Direction(0, -1, 100)
    },
    {
        Direction(-1, -1, 1), Direction(-1, 1, 1), Direction(0, -1, 7), Direction(0, -2, 2), Direction(0, 1, 7),
    Direction(0, 2, 2), Direction(1, -1, 10), Direction(1, 1, 10), Direction(2, 0, 5), Direction(1, 0, 100)
    },
    {
        Direction(-1, -1, 1), Direction(1, -1, 1), Direction(-1, 0, 7),  Direction(-2, 0, 2), Direction(1, 0, 7),
     Direction(2, 0, 2),  Direction(-1, 1, 10),  Direction(1, 1, 10),  Direction(0, 2, 5),  Direction(0, 1, 100)
    },
    {
        Direction(1, -1, 1), Direction(1, 1, 1), Direction(0, -1, 7), Direction(0, -2, 2), Direction(0, 1, 7),
    Direction(0, 2, 2), Direction(-1, -1, 10), Direction(-1, 1, 10), Direction(-2, 0, 5), Direction(-1, 0, 100)
    },
};

void moveTornado() {
    //1. ����̵� �̵���Ŵ.
    int nxtR = curPoint.first + moveDirection[curDirection][9].r;
    int nxtC = curPoint.second + moveDirection[curDirection][9].c;
    curPoint = { nxtR, nxtC };
    int initialSand = board[nxtR][nxtC];
    int totalMoveSand = 0;

    //2. �� ��ǥ�� % ���
    for (int i = 0; i <= 8; i++) {
        int checkR = curPoint.first + moveDirection[curDirection][i].r;
        int checkC = curPoint.second + moveDirection[curDirection][i].c;
        int moveSand = initialSand * ((double)(moveDirection[curDirection][i].percentage) / 100);
        totalMoveSand += moveSand;
        if (checkR <= 0 || checkR > widthHeight || checkC <= 0 || checkC > widthHeight) {
            ans += moveSand;
        }
        else {
            board[checkR][checkC] += moveSand;
        }
    }
    board[nxtR][nxtC] -= totalMoveSand;

    //3. ���� �� ������ ��ǥ�� �̵�.
    int remainSand = board[nxtR][nxtC];
    board[nxtR][nxtC] = 0;
    int sandMoveR = nxtR + moveDirection[curDirection][9].r;
    int sandMoveC = nxtC + moveDirection[curDirection][9].c;
    if (sandMoveR <= 0 || sandMoveR > widthHeight || sandMoveC <= 0 || sandMoveC > widthHeight) {
        ans += remainSand;
    }
    else {
        board[sandMoveR][sandMoveC] += remainSand;
    }

    //4. �̵�Ƚ�� Check
    curMoveCnt++;
    if (curMoveCnt == maxMoveCnt) {
        curMoveCnt = 0;

        if (curDirection == 1 || curDirection == 3) {
            maxMoveCnt++;
        }

        curDirection = curDirection + 1;
        if (curDirection > 3) { curDirection = 0; }
    }
}

void go() {
    if (curPoint.first == 1 && curPoint.second == 1) { return; }
    else {
        moveTornado();
        go();
    }

}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    cin >> widthHeight;

    curPoint.first = (widthHeight / 2) + 1;
    curPoint.second = (widthHeight / 2) + 1;

    for (int i = 1; i <= widthHeight; i++) {
        for (int j = 1; j <= widthHeight; j++) {
            cin >> board[i][j];
        }
    }

    go();

    cout << ans;

    return 0;
}