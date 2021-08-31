/*
* ���� 9020�� �������� ����
* https://www.acmicpc.net/problem/9020
* ���� / ������ / �Ҽ� ���� / �����佺�׳׽��� ü
*/
#include <iostream>
using namespace std;

int testCase, num;
int ans1, ans2;
bool primeCheck[10001]; // true��� �Ҽ��ƴ�. false�̸� �Ҽ�

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    cin >> testCase;

    for (int i = 2; i <= 10000; i++) {
        if (!primeCheck[i]) {
            for (int j = i + i; j <= 10000; j += i) {
                primeCheck[j] = true;
            }
        }
    }

    while (testCase--) {
        cin >> num;

        ans1 = num / 2;
        ans2 = num / 2;
        while (primeCheck[ans1] || primeCheck[ans2]) {
            ans1--;
            ans2++;
        }

        cout << ans1 << " " << ans2 << "\n";
    }

    return 0;
}