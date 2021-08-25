/*
* ���� 14499�� �ֻ��� ������
* https://www.acmicpc.net/problem/14499
* ���� / �ùķ��̼�
*/
#include <iostream>
using namespace std;

int height, width, diceXPoint, diceYPoint, cmdCnt;
int board[20][20];
int dice[7];
int curIdx = 1;
int moveDirection;
/*
* ���ʸ��
*    [1]
* [2][3][4]
*    [5]
*    [6]
*/

pair<int, int> movePoint[5]{
    {0, 0},
    {0, 1},
    {0, -1},
    {-1, 0},
    {+1, 0}
};

// �ٴ� ����. ���⿡ ���� Ʋ������. ������ �� �Ͱ� ��� ������ ���ؼ� �Դ����� ���� �޶���.
int curAround[5] = { 0, 4, 2, 6, 3 };

int getAcross(int idx) {
    switch (idx) {
    case 1: return 5;
    case 2: return 4;
    case 3: return 6;
    case 4: return 2;
    case 5: return 1;
    case 6: return 3;
    }
}

void move(int direction) {
    if (diceYPoint + movePoint[direction].first < 0 || diceYPoint + movePoint[direction].first >= height
        || diceXPoint + movePoint[direction].second < 0 || diceXPoint + movePoint[direction].second >= width) {
        return;
    }

    int prevIdx = curIdx;
    curIdx = curAround[direction];
    diceYPoint += movePoint[direction].first;
    diceXPoint += movePoint[direction].second;

    if (board[diceYPoint][diceXPoint] == 0) {
        board[diceYPoint][diceXPoint] = dice[curIdx];
    }
    else {
        dice[curIdx] = board[diceYPoint][diceXPoint];
        board[diceYPoint][diceXPoint] = 0;
    }

    switch (curIdx) {
    case 1: cout << dice[5] << "\n"; break;
    case 2: cout << dice[4] << "\n"; break;
    case 3: cout << dice[6] << "\n"; break;
    case 4: cout << dice[2] << "\n"; break;
    case 5: cout << dice[1] << "\n"; break;
    case 6: cout << dice[3] << "\n"; break;
    }

    // ������ 1, ������ 2, ������ 3, ������ 4 �׸��� �������� ���� ����, �������� ��������
    if (direction == 1) {
        curAround[2] = prevIdx;
        curAround[1] = getAcross(prevIdx);
    }
    else if (direction == 2) {
        curAround[1] = prevIdx;
        curAround[2] = getAcross(prevIdx);
    }
    else if (direction == 3) {
        curAround[4] = prevIdx;
        curAround[3] = getAcross(prevIdx);
    }
    else {
        curAround[3] = prevIdx;
        curAround[4] = getAcross(prevIdx);
    }

}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    cin >> height >> width >> diceYPoint >> diceXPoint >> cmdCnt;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cin >> board[i][j];
        }
    }

    while (cmdCnt--) {
        cin >> moveDirection;
        move(moveDirection);
    }

    return 0;
}