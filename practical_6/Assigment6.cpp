#include <iostream>
#include <vector>
using namespace std;

// Check if it is safe to place a queen at (row, col)
bool is_safe(const vector<vector<char>>& board, int row, int col) {
    for(int i = row - 1; i >= 0; --i) {
        if(board[i][col] == 'Q')
            return false;
    }
    for(int i = row - 1, j = col - 1; i >= 0 && j >= 0; --i, --j) {
        if(board[i][j] == 'Q')
            return false;
    }
    for(int i = row - 1, j = col + 1; i >= 0 && j < (int)board.size(); --i, ++j) {
        if(board[i][j] == 'Q')
            return false;
    }
    return true;
}

// Recursive N-Queens placement
bool n_queens(vector<vector<char>>& board, int row) {
    if(row == (int)board.size()) {
        for(auto& row : board) {
            for(char cell : row)
                cout << cell << " ";
            cout << endl;
        }
        cout << endl;
        return true;  // return after first solution
    }
    for(int i = 0; i < (int)board.size(); ++i) {
        if(is_safe(board, row, i)) {
            board[row][i] = 'Q';
            if(n_queens(board, row + 1))
                return true;
            board[row][i] = '.';
        }
    }
    return false;
}

int main() {
    int n = 8;
    vector<vector<char>> board(n, vector<char>(n, '.'));
    if(!n_queens(board, 0))
        cout << "No solution found." << endl;
    return 0;
}
