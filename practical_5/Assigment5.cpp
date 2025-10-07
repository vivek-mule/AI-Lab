#include <iostream>
#include <vector>
#include <limits>
using namespace std;

const char PLAYER_X = 'X';
const char PLAYER_O = 'O';
const char EMPTY = '.';

// Print the board
void print_board(const vector<vector<char>>& board) {
    for(const auto& row : board) {
        for(char cell : row)
            cout << cell << " ";
        cout << endl;
    }
    cout << endl;
}

// Evaluate board and return score
int evaluate_board(const vector<vector<char>>& board) {
    for(int i = 0; i < 3; ++i){
        if(board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != EMPTY)
            return (board[i][0] == PLAYER_X) ? 10 : -10;
        if(board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != EMPTY)
            return (board[0][i] == PLAYER_X) ? 10 : -10;
    }
    if(board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != EMPTY)
        return (board[0][0] == PLAYER_X) ? 10 : -10;
    if(board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != EMPTY)
        return (board[0][2] == PLAYER_X) ? 10 : -10;
    return 0;
}

// Check if game is over
bool is_game_over(const vector<vector<char>>& board) {
    if(evaluate_board(board) != 0)
        return true;
    for(const auto& row : board)
        for(char cell : row)
            if(cell == EMPTY)
                return false;
    return true;
}

// Minimax algorithm
int minimax(vector<vector<char>>& board, int depth, bool is_maximizing) {
    int score = evaluate_board(board);
    if(score == 10 || score == -10)
        return score;
    if(is_game_over(board))
        return 0;

    if(is_maximizing){
        int best_score = numeric_limits<int>::min();
        for(int i = 0; i < 3; ++i)
            for(int j = 0; j < 3; ++j)
                if(board[i][j] == EMPTY){
                    board[i][j] = PLAYER_X;
                    best_score = max(best_score, minimax(board, depth+1, false));
                    board[i][j] = EMPTY;
                }
        return best_score;
    }
    else{
        int best_score = numeric_limits<int>::max();
        for(int i = 0; i < 3; ++i)
            for(int j = 0; j < 3; ++j)
                if(board[i][j] == EMPTY){
                    board[i][j] = PLAYER_O;
                    best_score = min(best_score, minimax(board, depth+1, true));
                    board[i][j] = EMPTY;
                }
        return best_score;
    }
}

// Find the best move for computer
pair<int, int> find_best_move(vector<vector<char>>& board) {
    int best_value = numeric_limits<int>::min();
    pair<int, int> best_move = {-1, -1};

    for(int i = 0; i < 3; ++i)
        for(int j = 0; j < 3; ++j)
            if(board[i][j] == EMPTY){
                board[i][j] = PLAYER_X;
                int move_value = minimax(board, 0, false);
                board[i][j] = EMPTY;
                if(move_value > best_value){
                    best_value = move_value;
                    best_move = {i, j};
                }
            }
    return best_move;
}

// Main game loop
void play_game() {
    vector<vector<char>> board(3, vector<char>(3, EMPTY));
    cout << "Tic-Tac-Toe: Player X (you) vs. Player O (computer)" << endl;
    print_board(board);

    while(true){
        // Player X (Human)
        int row, col;
        cout << "Enter your move row (0, 1, or 2): ";
        cin >> row;
        cout << "Enter your move column (0, 1, or 2): ";
        cin >> col;

        if(row < 0 || row > 2 || col < 0 || col > 2){
            cout << "Invalid input! Please enter 0, 1, or 2 for both row and column." << endl;
            continue;
        }

        if(board[row][col] != EMPTY){
            cout << "Invalid move! Try again." << endl;
            continue;
        }

        board[row][col] = PLAYER_X;

        if(is_game_over(board)){
            print_board(board);
            if(evaluate_board(board) == 10)
                cout << "You win!" << endl;
            else
                cout << "It's a draw!" << endl;
            break;
        }

        // Player O (Computer)
        cout << "Computer's turn..." << endl;
        pair<int, int> best_move = find_best_move(board);
        board[best_move.first][best_move.second] = PLAYER_O;

        print_board(board);
        if(is_game_over(board)){
            if(evaluate_board(board) == -10)
                cout << "Computer wins!" << endl;
            else
                cout << "It's a draw!" << endl;
            break;
        }
    }
}

int main() {
    play_game();
    return 0;
}
