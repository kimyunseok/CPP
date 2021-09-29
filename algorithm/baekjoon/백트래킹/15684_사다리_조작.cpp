/*
* ���� 15684�� ��ٸ� ����
* https://www.acmicpc.net/problem/15684
* ���� / ���Ʈ���� / ��Ʈ��ŷ
*/
#include <iostream>
using namespace std;

int verticalLines, horizontalLines, maxHorizontal;
bool isConnected[31][11][2]; // [����][��][0] : ��(-1), [����][��][1] : ��(+1)
int ans = 4;

bool checkAlreadyConnect(int height, int point) {
    return (isConnected[height][point][0] || isConnected[height][point][1]);
}

bool findCanAns() {
    for (int i = 1; i <= verticalLines; i++) {
        int curPoint = i;
        for (int j = 1; j <= maxHorizontal; j++) {
            int frontPoint = curPoint - 1;
            int backPoint = curPoint + 1;
            if (isConnected[j][curPoint][0]) {
                curPoint = frontPoint;
            }
            else if (isConnected[j][curPoint][1]) {
                curPoint = backPoint;
            }
        }
        if (curPoint != i) { return false; }
    }
    return true;
}

// �ߺ��� �����ϱ� ���� �����ϴ� ���μ��� �����ϴ� ���μ��� ������ �Է¹޴´�.
void go(int idx, int startVerticalNum, int startHorizontalHeight) {
    if (idx > ans) { return; } //����ġ��, ���캸�� idx�� ans���� ũ�ٸ� return. �ʼ���Ҵ� �ƴ�.

    if (findCanAns()) {
        //�ش� idx��°���� ���μ��� �־��� ��, ��ٸ��� i�� ��� i�� ���ٸ� ans�� ����.
        //�� ���캼 �ʿ䰡 �����Ƿ� return
        ans = min(ans, idx);
        return;
    }
    if (idx == 3) { return; } // idx�� �̹� 3��°��� 4��° idx�� �ʿ�����Ƿ� return

    int vertical = startVerticalNum; // �����ϴ� verticalLine
    int horizontalHeight = startHorizontalHeight; // �����ϴ� ����.
    if (horizontalHeight > maxHorizontal) {
        // ���̸� ���� ������ +1�� ���ֹǷ� ������ ���� �ʾҴ��� check�Ѵ�.
        horizontalHeight = 1;
        vertical++;
    }
    while (vertical <= verticalLines) {
        int backPoint = vertical + 1; // ���� Ž���ϴ� �������� �ڿ��ִ�(+1) ������

        if (backPoint <= verticalLines && !checkAlreadyConnect(horizontalHeight, backPoint)) {
            //�ڿ��ִ� �������� ���� ���� �ְ�, ���� ���̿��� ��, �ڿ� ���μ��� ���ٸ� ����
            isConnected[horizontalHeight][vertical][1] = true;
            isConnected[horizontalHeight][backPoint][0] = true;
            go(idx + 1, vertical, horizontalHeight + 1); //������ �� ���� idx�� ã���� ����.

            //return���� ��, ������ ���������ش�.
            isConnected[horizontalHeight][vertical][1] = false;
            isConnected[horizontalHeight][backPoint][0] = false;
        }


        horizontalHeight++; //���� ���̸� ���캻��
        if (horizontalHeight > maxHorizontal) {
            //���̰� ������ ���� �ʾҴ��� check.
            //������ �Ѿ��ٸ� ���� �������� ���캻��.
            horizontalHeight = 1;
            vertical++;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    cin >> verticalLines >> horizontalLines >> maxHorizontal;

    int height, number;
    for (int i = 1; i <= horizontalLines; i++) {
        //number�� number + 1�� ����. number�� ��, number + 1�� �� ����
        cin >> height >> number;
        isConnected[height][number][1] = true;
        isConnected[height][number + 1][0] = true;
    }

    go(0, 1, 1);

    if (ans == 4) {
        cout << "-1";
    }
    else {
        cout << ans;
    }

    return 0;
}