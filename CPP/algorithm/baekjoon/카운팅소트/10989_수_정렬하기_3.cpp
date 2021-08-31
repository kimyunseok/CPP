/*
* ���� 10989�� �� �����ϱ� 3
* https://www.acmicpc.net/problem/10989
* ī���� ��Ʈ
*/
#include <iostream>
using namespace std;

int arr[10001];
int n, input;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> input;
        arr[input]++;
    }

    for (int i = 1; i <= 10000; i++) {
        while (arr[i] > 0) {
            cout << i << "\n";
            arr[i]--;
        }
    }

    return 0;
}