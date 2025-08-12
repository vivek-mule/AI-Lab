#include <bits/stdc++.h>
using namespace std;

// Convert 2D board to string
string boardToString(const vector<vector<int>>& board) {
    string s;
    for (const auto& row : board) {
        for (int num : row) s += char('0' + num);
    }
    return s;
}

// Print board
void printBoard(const string& state) {
    for (int i = 0; i < 9; i += 3)
        cout << state[i] << " " << state[i+1] << " " << state[i+2] << "\n";
    cout << "--------\n";
}

// Check solvability
bool isSolvable(const string& start, const string& goal) {
    auto invCount = [](const string& s) {
        int inv = 0;
        for (int i = 0; i < 9; ++i) {
            if (s[i] == '0') continue;
            for (int j = i + 1; j < 9; ++j)
                if (s[j] != '0' && s[i] > s[j]) inv++;
        }
        return inv;
    };
    return (invCount(start) % 2) == (invCount(goal) % 2);
}

// BFS
void solvePuzzleBFS(const string& start, const string& goal) {
    queue<string> q;
    unordered_set<string> visited;
    unordered_map<string, string> parent;

    q.push(start);
    visited.insert(start);
    parent[start] = "";

    while (!q.empty()) {
        string current = q.front(); q.pop();

        if (current == goal) {
            vector<string> path;
            for (string s = current; !s.empty(); s = parent[s]) path.push_back(s);
            reverse(path.begin(), path.end());
            cout << "BFS: Solved in " << path.size() - 1 << " moves!\n";
            for (size_t i = 0; i < path.size(); ++i) {
                cout << "Step " << i << ":\n"; printBoard(path[i]);
            }
            return;
        }

        int zeroIdx = current.find('0');
        int x = zeroIdx / 3, y = zeroIdx % 3;
        int dx[4] = {-1, 1, 0, 0}, dy[4] = {0, 0, -1, 1};

        for (int d = 0; d < 4; ++d) {
            int nx = x + dx[d], ny = y + dy[d];
            if (nx >= 0 && nx < 3 && ny >= 0 && ny < 3) {
                string nextState = current;
                swap(nextState[zeroIdx], nextState[nx*3 + ny]);
                if (!visited.count(nextState)) {
                    visited.insert(nextState);
                    parent[nextState] = current;
                    q.push(nextState);
                }
            }
        }
    }
    cout << "BFS: No solution found.\n";
}

// DFS helper function
bool dfsUtil(const string& current, const string& goal,
             unordered_set<string>& visited,
             unordered_map<string, string>& parent,
             int depth, int maxDepth) {
    if (current == goal) return true;
    if (depth >= maxDepth) return false;

    int zeroIdx = current.find('0');
    int x = zeroIdx / 3, y = zeroIdx % 3;
    int dx[4] = {-1, 1, 0, 0}, dy[4] = {0, 0, -1, 1};

    for (int d = 0; d < 4; ++d) {
        int nx = x + dx[d], ny = y + dy[d];
        if (nx >= 0 && nx < 3 && ny >= 0 && ny < 3) {
            string nextState = current;
            swap(nextState[zeroIdx], nextState[nx*3 + ny]);
            if (!visited.count(nextState)) {
                visited.insert(nextState);
                parent[nextState] = current;
                if (dfsUtil(nextState, goal, visited, parent, depth + 1, maxDepth))
                    return true;
            }
        }
    }
    return false;
}

// DFS
void solvePuzzleDFS(const string& start, const string& goal, int maxDepth) {
    unordered_set<string> visited;
    unordered_map<string, string> parent;
    visited.insert(start);
    parent[start] = "";

    if (dfsUtil(start, goal, visited, parent, 0, maxDepth)) {
        vector<string> path;
        for (string s = goal; !s.empty(); s = parent[s]) path.push_back(s);
        reverse(path.begin(), path.end());
        cout << "DFS: Solved in " << path.size() - 1 << " moves!\n";
        for (size_t i = 0; i < path.size(); ++i) {
            cout << "Step " << i << ":\n"; printBoard(path[i]);
        }
    } else {
        cout << "DFS: No solution found within depth limit " << maxDepth << ".\n";
    }
}

int main() {
    cout << "Enter initial board (3x3), 0 for blank:\n";
    vector<vector<int>> startBoard(3, vector<int>(3));
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            cin >> startBoard[i][j];

    cout << "Enter goal board (3x3), 0 for blank:\n";
    vector<vector<int>> goalBoard(3, vector<int>(3));
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            cin >> goalBoard[i][j];

    string start = boardToString(startBoard);
    string goal = boardToString(goalBoard);

    if (!isSolvable(start, goal)) {
        cout << "Not solvable.\n";
        return 0;
    }

    int choice;
    cout << "\nChoose search method:\n1. BFS (Shortest Path)\n2. DFS (Depth-Limited)\nChoice: ";
    cin >> choice;

    if (choice == 1) {
        solvePuzzleBFS(start, goal);
    } else if (choice == 2) {
        int limit;
        cout << "Enter DFS depth limit: ";
        cin >> limit;
        solvePuzzleDFS(start, goal, limit);
    } else {
        cout << "Invalid choice.\n";
    }
    return 0;
}
