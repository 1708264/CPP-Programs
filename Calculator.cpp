#include <iostream>
#include <string>
#include <vector>
#include <time.h>

using namespace std;

void print(vector<int> num) {
	for (int i = 0; i < num.size(); i++) {
		cout << num[i];
	}
	if (num.size() > 3) {
		cout << endl << num[0] << "," << num[1] << num[2] << " * 10^" << num.size() - 1;
	}
}

vector<int> resolve(vector<int> res) {
	int add;
	for (int i = res.size() - 1; i > 0; i--) {
		if (res[i] > 9) {
			add = res[i];
			res[i] %= 10;
			add -= res[i];
			res[i - 1] += add / 10;
		}
	}
	while (res[0] > 9) {
		add = res[0];
		res[0] %= 10;
		add -= res[0];
		res.insert(res.begin(), add / 10);
	}
	return res;
}

vector<int> resolve_negative(vector<int> res) {
	int sub;
	for (int i = res.size() - 1; i > 0; i--) {
		if (res[i] < 0) {
			res[i] += 10;
			res[i - 1] -= 1;
		}
	}
	while (res[0] < -9) {
		sub = res[0];
		res[0] %= -10;
		sub += res[0];
		res.insert(res.begin(), sub / 10);
	}
	return res;
}

bool equal(vector<int> num1, vector<int> num2) {
	if (num1.size() == num2.size()) {
		for (int i = 0; i < num1.size(); i++) {
			if (num1[i] != num2[i]) {
				return false;
			}
		}
		return true;
	}
	return false;
}

bool notEqual(vector<int> num1, vector<int> num2) {
	return !equal(num1, num2);
}

bool lessThan(vector<int> num1, vector<int> num2) {
	if (num1[0] < 0 && num2[0] > 0) {
		return true;
	}
	else if (num1[0] > 0 && num2[0] < 0) {
		return false;
	}
	else {
		if (num1[0] < 0 && num2[0] < 0) {
			if (num1.size() > num2.size()) {
				return true;
			}
			else if (num1.size() < num2.size()) {
				return false;
			}
			else {
				for (int i = 0; i < num1.size(); i++) {
					if (num1[i] > num2[i]) {
						return true;
					}
					else if (num1[i] < num2[i]) {
						return false;
					}
				}
			}
		}
		else {
			if (num1.size() > num2.size()) {
				return false;
			}
			else if (num1.size() < num2.size()) {
				return true;
			}
			else {
				for (int i = 0; i < num1.size(); i++) {
					if (num1[i] > num2[i]) {
						return false;
					}
					else if (num1[i] < num2[i]) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool moreThan(vector<int> num1, vector<int> num2) {

	return !lessThan(num1, num2) && !equal(num1, num2);
}

vector<int> add(vector<int> num1, vector<int> num2) {
	vector<int> sol;
	int i = num1.size() - 1, j = num2.size() - 1;
	while (i >= 0 && j >= 0) {
		sol.insert(sol.begin(), num1[i] + num2[j]);
		i--;
		j--;
	}
	if (j < 0) {
		for (int j = i; j >= 0; j--) {
			sol.insert(sol.begin(), num1[j]);
		}
	}
	else if (i < 0) {
		for (int i = j; i >= 0; i--) {
			sol.insert(sol.begin(), num2[i]);
		}
	}
	return resolve(sol);
}

vector<int> subtract(vector<int> num1, vector<int> num2) {
	vector<int> sol;
	if (lessThan(num1, num2)) {
		for (int i = 0; i < num1.size(); i++) {
			num1[i] = -num1[i];
		}
		sol = add(num1, num2);
		sol[0] = -sol[0];
	}
	else {
		for (int i = 0; i < num2.size(); i++) {
			num2[i] = -num2[i];
		}
		sol = add(num1, num2);
	}
	return resolve_negative(sol);
}

vector<int> multiply(vector<int> num1, vector<int> num2) {
	vector<int> sol;
	vector<vector<int>> res(num2.size());
	for (int i = 0; i < num2.size(); i++) {
		res[i].resize(num1.size());
	}
	for (int i = num2.size() - 1; i >= 0; i--) {
		for (int j = num1.size() - 1; j >= 0; j--) {
			res[i][j] = num1[j] * num2[i];
		}
		res[i] = resolve(res[i]);
		for (int k = 0; k < num2.size() - i - 1; k++) {
			res[i].push_back(0);
		}
	}
	sol = res[0];
	for (int i = 1; i < num2.size(); i++) {
		sol = add(sol, res[i]);
	}
	return resolve(sol);
}

vector<int> pow(vector<int> num1, vector<int> num2) {
	vector<int> sol = { 1 };
	for (vector<int> i = { 0 }; i != num2; i = add(i, { 1 })) {
		sol = multiply(sol, num1);
	}
	return sol;
}

vector<int> factorial(vector<int> num1) {
	vector<int> sol = { 1 };
	for (vector<int> i = { 1 }; i != add(num1, { 1 }); i = add(i, { 1 })) {
		sol = multiply(sol, i);
	}
	return sol;
}

vector<int> sum(vector<int> num2) {
	vector<int> sol = { 1 };
	for (vector<int> i = { 2 }; i != add(num2, { 1 }); i = add(i, { 1 })) {
		sol = add(sol, i);
	}
	return sol;
}

//void prime(vector<int> num2) {
//	vector<vector<int>> sol;
//	vector<int>  num = { 2 };
//	while (true) {
//		for (int i = 0; i < sol.size(); i++) {
//			if (num % sol[i] == 0) {
//				num = add(num, { 2 });
//			}
//		}
//		sol.push_back(num);
//		print(num);
//	}
//}

int main() {
	string str, string1, string2, op = "0";
	vector<int> num1, num2, sol;
	cin >> str;
	for (int i = 0; i < str.size(); i++) {
		if (str[i] == '+') {
			op = '+';
		}
		else if (str[i] == '-') {
			op = '-';
		}
		else if (str[i] == '*') {
			op = '*';
		}
		else if (str[i] == '/') {
			op = '/';
		}
		else if (str[i] == '^') {
			op = '^';
		}
		else if (str[i] == '!') {
			op = '!';
		}
		else if (str[i] == 'E') {
			op = 'E';
		}
		else if (str[i] == '%') {
			op = '%';
		}
		else {
			string1.push_back(str[i]);
		}
		if (op[0] != '0' && op[0] != '!') {
			for (int j = i + 1; j < str.size(); j++) {
				string2.push_back(str[j]);
			}
			break;
		}
	}


	for (int i = 0; i < string1.size(); i++) {
		num1.push_back(string1[i] - 48);
	}
	for (int i = 0; i < string2.size(); i++) {
		num2.push_back(string2[i] - 48);
	}

	time_t start = clock();

	if (op[0] == '+') {
		sol = add(num1, num2);
	}
	else if (op[0] == '-') {
		sol = subtract(num1, num2);
	}
	else if (op[0] == '*') {
		sol = multiply(num1, num2);
	}
	else if (op[0] == '/') {

	}
	else if (op[0] == '^') {
		sol = pow(num1, num2);
	}
	else if (op[0] == '!') {
		sol = factorial(num1);
	}
	else if (op[0] == 'E') {
		sol = sum(num2);
	}
	else if (op[0] == 'P') {
		//sol = prime(num2);
	}

	cout << endl << "Time taken: " << (clock() - start) / (double)CLOCKS_PER_SEC << " seconds" << endl;
	print(sol);
}
