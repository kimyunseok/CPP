/*
* ���� 2096�� ��������
* https://www.acmicpc.net/problem/2096
* ���̳��� ���α׷���
*
* �ذ�å : 1. �̹� ������ �κ� / �տ� ���� �κ��� ������ �ʿ䰡 ����.
* 2. ���� ���� ��������� �Ѵ�.
* https://www.acmicpc.net/board/view/23024 �� �ִ� �ݷʸ� ��������.
*/
#include <iostream>
#include <algorithm>

using namespace std;

int n;
int arr[4];
int dp_save_max[4];
int dp_save_min[4];
int dp_max[4];
int dp_min[4];
int maxAns = 0;
int minAns = 1000000;

int main() {
	cout.tie(NULL);
	cin.tie(NULL);
	ios_base::sync_with_stdio(false);

	cin >> n;
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= 3; j++) {
			cin >> arr[j];
		}
		for (int j = 1; j <= 3; j++) {
			if (i == 1) {
				dp_save_max[j] = arr[j];
				dp_save_min[j] = arr[j];
			}
			else {
				if (j == 1) {
					dp_save_max[j] = arr[j] + max(dp_max[j], dp_max[j + 1]);
					dp_save_min[j] = arr[j] + min(dp_min[j], dp_min[j + 1]);
				}
				else if (j == 2) {
					dp_save_max[j] = arr[j] + max(max(dp_max[j - 1], dp_max[j]), dp_max[j + 1]);
					dp_save_min[j] = arr[j] + min(min(dp_min[j - 1], dp_min[j]), dp_min[j + 1]);
				}
				else {
					dp_save_max[j] = arr[j] + max(dp_max[j - 1], dp_max[j]);
					dp_save_min[j] = arr[j] + min(dp_min[j - 1], dp_min[j]);
				}
			}
		}
		for (int j = 1; j <= 3; j++) {
			dp_max[j] = dp_save_max[j];
			dp_min[j] = dp_save_min[j];
		}
	}

	for (int i = 1; i <= 3; i++) {
		maxAns = max(maxAns, dp_max[i]);
		minAns = min(minAns, dp_min[i]);
	}

	cout << maxAns << " " << minAns;

	return 0;
}