/*
* ���� 1436�� ��ȭ���� ��
* https://www.acmicpc.net/problem/1436
* ���Ʈ���� �˰���
*/
#include <iostream>
#include <string>
using namespace std;

int n;
int num;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    cin >> n;
    num = 666;
    string str = to_string(num);
    while (--n) {
        while (true) {
            bool find = false;
            num++;
            str = to_string(num);
            int six_check = 0;
            for (int i = 0; i < str.size(); i++) {
                if (str[i] == '6') { six_check++; }
                else { six_check = 0; }

                if (six_check == 3) { find = true; break; }
            }

            if (find) { break; }
        }
    }

    cout << str;

    return 0;
}