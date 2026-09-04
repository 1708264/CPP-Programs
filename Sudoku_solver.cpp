#include <iostream>
#include <vector>
#include <map>
#include <chrono>
#include <unordered_map>

using namespace std::chrono;
using namespace std;

void print(vector<vector<char>>& board){
	for (int i = 0; i < 9; i++) {
		cout << "{ ";
		for (int j = 0; j < 8; j++) {
			cout << board[i][j] << ", ";
		}
		cout << board[i][8] << " }" << endl;
	}
}

bool sudoku(vector<vector<char>>& board, map<pair<int, int>, vector<char>> moves, vector<pair<int, int>> changes) {
	vector<pair<int, int>>  newChanges;

	//O PASSAR CELLS PER PARAMETRES O AQUI SA DE ITERAR MOVES, O TAMBE ES PODRIA FER QUE MOVES I CELLS SIGUIN ELS MATEIXOS QUE ES EL QUE TINDRIA MES SENTIT
	for (pair<int, int> index : changes) {
		moves[index] = { '1','2','3','4','5','6','7','8','9' };
		for (int i = 0; i < 9; i++) {
			if (board[index.first][i] != '.') {
				for (int j = 0; j < moves[index].size(); j++) {
					if (moves[index][j] == board[index.first][i]) {
						moves[index].erase(moves[index].begin() + j);
					}
				}
			}
			if (board[i][index.second] != '.') {
				for (int j = 0; j < moves[index].size(); j++) {
					if (moves[index][j] == board[i][index.second]) {
						moves[index].erase(moves[index].begin() + j);
					}
				}
			}
			int row = index.first / 3 * 3 + i / 3;
			int col = index.second / 3 * 3 + i % 3;
			if (board[row][col] != '.') {
				for (int j = 0; j < moves[index].size(); j++) {
					if (moves[index][j] == board[row][col]) {
						moves[index].erase(moves[index].begin() + j);
					}
				}
			}
		}
	}

	if(moves.size() == 0) {
		return true;
	}

	bool changed = false;
	for (auto mov : moves) {
		if (mov.second.size() == 1) {
			board[mov.first.first][mov.first.second] = mov.second[0];
			newChanges.push_back(mov.first);
			changed = true;
		}
		if(mov.second.size() == 0) {
			return false;
		}
	}
	
	if(changed) {
		for (pair<int, int> index : newChanges) {
			moves.erase(index);
		}
		changes = {};
		for (auto mov : moves) {
			for (int j = 0; j < newChanges.size(); j++) {
				if (mov.first.first == newChanges[j].first || mov.first.second == newChanges[j].second) {
					if (!moves.count(mov.first)) {
						changes.push_back(mov.first);
					}
				}
			}
		}
		return sudoku(board, moves, changes);
	}
	else {
		vector<vector<pair<int, int>>> quadrant(9);
		unordered_map<char, pair<int, pair<int, int>>> repetitions;
		for (auto mov : moves) {
			int coord = mov.first.first / 3 * 3 + mov.first.second / 3;
			quadrant[coord].push_back(mov.first);
		}
		for (int i = 0; i < 9; i++) {
			for (pair<int, int> index : quadrant[i]) {
				for (char c : moves[index]) {
					if (repetitions.find(c) == repetitions.end()) {
						repetitions[c] = { 1, index };
					}
					else {
						repetitions[c] = { repetitions[c].first + 1, index };
					}
				}
			}
		}

		changed = false;
		for (auto it = repetitions.begin(); it != repetitions.end(); it++) {
			if (it->second.first == 1) {
				board[it->second.second.first][it->second.second.second] = it->first;
				newChanges.push_back(it->second.second);
				changed = true;
			}
		}
		if (changed) {
			for (pair<int,int> index : newChanges) {
				moves.erase(index);
			}
			changes = {};
			for (auto mov : moves) {
				for (int j = 0; j < newChanges.size(); j++) {
					if (mov.first.first == newChanges[j].first || mov.first.second == newChanges[j].second) {
						if (!moves.count(mov.first)) {
							changes.push_back(mov.first);
						}
					}
				}
			}

			return sudoku(board, moves, changes);
		}
		else {
			int min = 9;
			pair<int, int> minIndex;
			for (auto mov : moves) {
				if (mov.second.size() < min) {
					min = mov.second.size();
					minIndex = mov.first;
				}
				if (mov.second.size() == 2) {
					break;
				}
			}
			vector<char> mov = moves[minIndex];
			for (char c : mov) {
				changes = {};
				for (auto mov : moves) {
					for (int j = 0; j < newChanges.size(); j++) {
						if (mov.first.first == newChanges[j].first || mov.first.second == newChanges[j].second) {
							//he cambiat aixo i ara no va
							if (!moves.count(mov.first)) {
								changes.push_back(mov.first);
							}
						}
					}
				}
				board[minIndex.first][minIndex.second] = c;
				moves.erase(minIndex);
				vector<pair<int, int>> newChanges = { minIndex };
				if (sudoku(board, moves, changes)) {
					return true;
				}
				board[minIndex.first][minIndex.second] = '.';
				moves[minIndex] = mov;
			}

		}
	}
}

int main() {
	auto start = high_resolution_clock::now();
	vector<vector<char>> board =
		{ {'.', '.', '.', '2', '.', '.', '.', '6', '3'},
		{'3', '.', '.', '.', '.', '5', '4', '.', '1'},
		{'.', '.', '1', '.', '.', '3', '9', '8', '.'},
		{'.', '.', '.', '.', '.', '.', '.', '9', '.'},
		{'.', '.', '.', '5', '3', '8', '.', '.', '.'},
		{'.', '3', '.', '.', '.', '.', '.', '.', '.'},
		{'.', '2', '6', '3', '.', '.', '5', '.', '.'},
		{'5', '.', '3', '7', '.', '.', '.', '.', '8'},
		{'4', '7', '.', '.', '.', '1', '.', '.', '.'} };
	print(board);
	cout << endl;

	map<pair<int, int>, vector<char>> moves;
	vector<pair<int, int>> changes;

	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board[0].size(); j++) {
			if (board[i][j] == '.') {
				changes.push_back({ i, j });
			}
		}
	}

	sudoku(board, moves, changes);
	

	auto time = high_resolution_clock::now() - start;
	auto duration = duration_cast<microseconds>(time);
	cout << "Time: " << duration.count() << " microseconds" << endl;

	print(board);

}
