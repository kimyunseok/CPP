/*
* ���� 2493�� ž
* https://www.acmicpc.net/problem/2493
* �ڷᱸ�� - ���� / �켱���� ť
*/
#include <iostream>
#include <stack>
using namespace std;

int numCnt, input;
stack<pair<int, int>> st;
int tower[500001];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    cin >> numCnt;

    for (int i = 1; i <= numCnt; i++) {
        cin >> input;
        if (i == 1) { st.push({ i, input }); continue; }
        while (!st.empty() && input > st.top().second) {
            st.pop();
        }
        if (!st.empty()) {
            tower[i] = st.top().first;
        }
        st.push({ i, input });
    }

    for (int i = 1; i <= numCnt; i++) {
        cout << tower[i] << " ";
    }

    return 0;
}