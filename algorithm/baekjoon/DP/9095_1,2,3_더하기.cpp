/*
* ���� 9095�� 1, 2, 3 ���ϱ�
* https://www.acmicpc.net/problem/9095
* ���̳��� ���α׷���
* (��Ʈ��ŷ���ε� Ǯ��)
*/
#include <iostream>
using namespace std;

int test_case;
int n;
int output;
int dp[12];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    cin >> test_case;
    dp[1] = 1;
    dp[2] = 2;
    dp[3] = 4;
    while (test_case--) {
        output = 0;
        cin >> n;
        for (int i = 4; i <= n; i++) {
            dp[i] = dp[i - 3] + dp[i - 2] + dp[i - 1];
        }
        cout << dp[n] << "\n";
    }
}

//��Ʈ��ŷ Ǯ�̹�
//#include <iostream>
//using namespace std;
//
//int test_case;
//int n;
//int output;
//
//void backTracking(int cur_num) {
//    if (cur_num > n) { return; }
//    else if (cur_num == n) { output++; }
//
//    backTracking(cur_num + 1);
//    backTracking(cur_num + 2);
//    backTracking(cur_num + 3);
//}
//
//int main() {
//    ios_base::sync_with_stdio(false);
//    cin.tie(NULL);
//    cout.tie(NULL);
//    cin >> test_case;
//    while (test_case--) {
//        output = 0;
//        cin >> n;
//        backTracking(1);
//        backTracking(2);
//        backTracking(3);
//        cout << output << "\n";
//    }
//}