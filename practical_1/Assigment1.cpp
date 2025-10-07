#include <iostream>
#include <vector>
#include <stack>
#include <set>
#include <map> // to track parents
#include <algorithm>

using namespace std;

bool isSolvable(const vector<int>& states) {
    int inversion = 0;
    for (int i = 0; i < (int)states.size(); ++i) {
        for (int j = i + 1; j < (int)states.size(); ++j) {
            if (states[i] > states[j] && states[j] != 0) {
                ++inversion;
            }
        }
    }
    return inversion % 2 == 0;
}

vector<vector<int>> getMoves(const vector<int>& state) {
    vector<vector<int>> moves;
    int emptyTileIndex = find(state.begin(), state.end(), 0) - state.begin();

    if (emptyTileIndex > 2) {
        vector<int> newState(state);
        swap(newState[emptyTileIndex], newState[emptyTileIndex - 3]);
        moves.push_back(newState);
    }

    if (emptyTileIndex < 6) {
        vector<int> newState(state);
        swap(newState[emptyTileIndex], newState[emptyTileIndex + 3]);
        moves.push_back(newState);
    }

    if (emptyTileIndex % 3 != 0) {
        vector<int> newState(state);
        swap(newState[emptyTileIndex], newState[emptyTileIndex - 1]);
        moves.push_back(newState);
    }

    if (emptyTileIndex % 3 != 2) {
        vector<int> newState(state);
        swap(newState[emptyTileIndex], newState[emptyTileIndex + 1]);
        moves.push_back(newState);
    }

    return moves;
}

// Modified DFS to track path using parents map
vector<vector<int>> dfsSolveWithPath(vector<int> startState, const vector<int>& target) {
    stack<vector<int>> stackStates;
    set<vector<int>> visitedStates;
    map<vector<int>, vector<int>> parents;

    stackStates.push(startState);
    parents[startState] = vector<int>(); // start has no parent

    while (!stackStates.empty()) {
        vector<int> currentState = stackStates.top();
        stackStates.pop();

        if (visitedStates.find(currentState) != visitedStates.end()) {
            continue;
        }

        visitedStates.insert(currentState);

        if (currentState == target) {
            // reconstruct path
            vector<vector<int>> path;
            vector<int> step = currentState;
            while (!step.empty()) {
                path.push_back(step);
                step = parents[step];
            }
            reverse(path.begin(), path.end());
            return path;
        }

        vector<vector<int>> nextMoves = getMoves(currentState);
        for (const vector<int>& nextState : nextMoves) {
            if (visitedStates.find(nextState) == visitedStates.end()) {
                stackStates.push(nextState);
                if (parents.find(nextState) == parents.end()) {
                    parents[nextState] = currentState;
                }
            }
        }
    }
    return {};
}

int main() {
    vector<int> startState(9);
    vector<int> targetState(9);

    cout << "Enter the starting state (9 numbers, use 0 for empty tile):\n";
    for (int i = 0; i < 9; ++i) {
        cin >> startState[i];
    }

    cout << "Enter the target state (goal, 9 numbers, use 0 for empty tile):\n";
    for (int i = 0; i < 9; ++i) {
        cin >> targetState[i];
    }

    if (!isSolvable(startState)) {
        cout << "No solution, puzzle unsolvable." << endl;
        return 0;
    }

    vector<vector<int>> path = dfsSolveWithPath(startState, targetState);

    if (!path.empty()) {
        cout << "Solution found:" << endl;
        for (auto& state : path) {
            for (int i = 0; i < 9; ++i) {
                cout << state[i] << " ";
                if ((i + 1) % 3 == 0) cout << "\n";
            }
            cout << "\n";
        }
        cout << "You reached the goal!" << endl;
    } else {
        cout << "No solution found." << endl;
    }

    return 0;
}
