/*
* ���� 14503�� �κ� û�ұ�
* https://www.acmicpc.net/problem/14503
* ���� / �ùķ��̼�
*/
#include <iostream>
using namespace std;

int height, width;
int room[51][51]; // 0 : ��ĭ, 1 : �� �׸��� 2�� û���� ��
int robotY, robotX, robotDirection; // 0: ��, 1: ��, 2: ��, 3: ��
int result;
/*
* ���ʹ��⿡ ���� ���
* ����(0)�� ��� ���ʹ����� ����(3)�̰� ���� index�� x�� -1, ������ y�� +1
* ����(1)�� ��� ���ʹ����� ����(0)�̰� ���� index�� y�� -1, ������ x�� -1
* ����(2)�� ��� ���ʹ����� ����(1)�̰� ���� index�� x�� +1, ������ y�� -1
* ����(3)�� ��� ���ʹ����� ����(2)�̰� ���� index�� y�� +1, ������ x�� +1
*/
pair<int, pair<int, int>> directionGoSystem[4] = {
    { 3, {0, -1} },
    { 0, {-1, 0} },
    { 1, {0, 1} },
    { 2, {1, 0} }
};
pair<int, int> directionBackSystem[4] = {
    {1, 0},
    {0, -1},
    {-1, 0},
    {0, 1}
};

void cleanSystem(int y, int x, int direction) {
    if (y < 1 || y > height || x < 1 || x > width) { return; }
    if (room[y][x] == 0) {
        room[y][x] = 2; // 1. ���� ��ġ�� û���Ѵ�.
        result++;
    }
    //2. ���� ��ġ, ���� ������ �������� ���ʹ������ Ž��
    int leftDirection = directionGoSystem[direction].first;
    int leftY = y + directionGoSystem[direction].second.first;
    int leftX = x + directionGoSystem[direction].second.second;

    int backY = y + directionBackSystem[direction].first;
    int backX = x + directionBackSystem[direction].second;

    if (room[leftY][leftX] == 0) {
        cleanSystem(leftY, leftX, leftDirection);
        return;
    }
    else if (room[leftY][leftX] != 0) {
        if (room[y - 1][x] != 0 && room[y + 1][x] != 0 && room[y][x - 1] != 0 && room[y][x + 1] != 0) {
            if (room[backY][backX] == 1) { return; }
            else {
                cleanSystem(backY, backX, direction);
            }
        }
        else {
            while (room[leftY][leftX] != 0) {
                leftY = y + directionGoSystem[leftDirection].second.first;
                leftX = x + directionGoSystem[leftDirection].second.second;
                leftDirection = directionGoSystem[leftDirection].first;
            }
            cleanSystem(leftY, leftX, leftDirection);
        }
        return;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    cin >> height >> width;

    cin >> robotY >> robotX >> robotDirection;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cin >> room[i][j];
        }
    }

    cleanSystem(robotY, robotX, robotDirection);

    cout << result;

    return 0;
}