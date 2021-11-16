/*
* ���α׷��ӽ� 2021 KAKAO BLIND RECRUITMENT
* Level 1 �ű� ���̵� ��õ
* https://programmers.co.kr/learn/courses/30/lessons/72410
* ���ڿ�
*/
#include <iostream>

using namespace std;
/*
* ���̵��� ���� : 3 ~ 15
* �ҹ���, ����, -, _, . ��� ����.
* ��ħǥ(.)�� ó���� ���� ��� �Ұ�
* ��ħǥ�� ���� ��� �Ұ�
*
* �ܰ踦 ���ؼ� ��Ģ�� �´� ���ο� ���̵� ��õ
* 1. ��� �빮�ڸ� �ҹ��ڷ� ġȯ.
* 2. �ҹ���, ����, -, _, .�� ������ ��� ���� ����
* 3. ��ħǥ�� 2�� �̻� ���ӵ� �κ� �ϳ��� ��ħǥ�� ����
* 4. ��ħǥ�� ó���̳� ���� �ִٸ� ����
* 5. �� ���ڿ��� ��� "a" ����
* 6. ���̰� 16�� �̻��� ���, ó�� 15�� ���� �������� ��� �����Ѵ�.
* ���� �� �������� ��ħǥ ���� �� ����
* 7. ���̰� 2�� ���϶��, new_id�� ������ ���ڸ� ���̰� 3�� �� ������ �ݺ��ؼ� ���� ����.
*/

string solution(string new_id) {
	int diff = 'a' - 'A';
	string answer = new_id;
	//1�ܰ�. ��� �빮�� �ҹ��ڷ� ġȯ.
	for (int i = 0; i < answer.length(); i++) {
		if (answer[i] >= 'A' && answer[i] <= 'Z') {
			answer[i] += diff;
		}
	}

	//2�ܰ�. �ҹ���, ����, -, _, .�� ������ ��� ���� ����
	for (int i = 0; i < answer.length(); i++) {
		if (!(answer[i] >= 'a' && answer[i] <= 'z') &&
			!(answer[i] >= '0' && answer[i] <= '9') &&
			answer[i] != '-' && answer[i] != '_' && answer[i] != '.') {
			answer.erase(answer.begin() + i);
			i--;
		}
	}

	int prevPointIdx = -2;
	//3�ܰ� ��ħǥ �� �� �̻� ���ӵ� �� ����.
	for (int i = 0; i < answer.length(); i++) {
		if (answer[i] == '.') {
			if (i - 1 == prevPointIdx) {
				answer.erase(answer.begin() + i);
				i--;
			}
			else {
				prevPointIdx = i;
			}
		}
	}

	//4�ܰ� ��ħǥ�� ó���̳� ���� �ִٸ� ����
	while (answer.length() > 0 && answer[0] == '.') {
		answer.erase(answer.begin());
	}
	while (answer.length() > 0 && answer[answer.length() - 1] == '.') {
		answer.erase(answer.begin() + (answer.length() - 1));
	}

	//5�ܰ� �� ���ڿ��� ��� "a" ����
	if (answer.length() == 0) {
		answer = "a";
	}

	//6�ܰ� ���̰� 16�� �̻��� ���, ó�� 15�� ���� �������� ��� �����Ѵ�. ���� �� �������� ��ħǥ ���� �� ����
	while (answer.length() > 15) {
		answer.erase(answer.begin() + (answer.length() - 1));
	}
	while (answer.length() > 0 && answer[answer.length() - 1] == '.') {
		answer.erase(answer.begin() + (answer.length() - 1));
	}

	//7�ܰ� ���̰� 2�� ���϶��, new_id�� ������ ���ڸ� ���̰� 3�� �� ������ �ݺ��ؼ� ���� ����.
	while (answer.length() <= 2) {
		answer += answer[answer.length() - 1];
	}

	return answer;
}

int main() {
	cout.tie(NULL);
	cin.tie(NULL);
	ios_base::sync_with_stdio(false);

	cout << solution("abcdefghijklmn.p");

	return 0;
}