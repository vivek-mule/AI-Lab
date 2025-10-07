#include <iostream>
#include <queue>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>

using namespace std;

// Define the size of the grid
#define ROW 9
#define COL 10

struct Cell {
    int parent_i, parent_j;
    double f, g, h;
    Cell() {
        parent_i = 0;
        parent_j = 0;
        f = g = numeric_limits<double>::infinity();
        h = 0;
    }
};

// Function to check if a cell is valid
bool is_valid(int row, int col) {
    return (row >= 0) && (row < ROW) && (col >= 0) && (col < COL);
}

// Function to check if a cell is unblocked
bool is_unblocked(int grid[ROW][COL], int row, int col) {
    return grid[row][col] == 1;
}

// Function to check if a cell is the destination
bool is_destination(int row, int col, pair<int, int> dest) {
    return row == dest.first && col == dest.second;
}

// Heuristic: Euclidean distance
double calculate_h_value(int row, int col, pair<int, int> dest) {
    return sqrt((row - dest.first) * (row - dest.first) + (col - dest.second) * (col - dest.second));
}

// Trace the path from source to destination
void trace_path(vector<vector<Cell>>& cell_details, pair<int, int> dest) {
    cout << "The Path is ";
    vector<pair<int, int>> path;
    int row = dest.first;
    int col = dest.second;
    while (!(cell_details[row][col].parent_i == row && cell_details[row][col].parent_j == col)) {
        path.push_back({row, col});
        int temp_row = cell_details[row][col].parent_i;
        int temp_col = cell_details[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }
    path.push_back({row, col});
    reverse(path.begin(), path.end());
    for (auto& p : path) {
        cout << "-> (" << p.first << "," << p.second << ") ";
    }
    cout << endl;
}

// A* Search Algorithm
void a_star_search(int grid[ROW][COL], pair<int, int> src, pair<int, int> dest) {
    if (!is_valid(src.first, src.second) || !is_valid(dest.first, dest.second)) {
        cout << "Source or destination is invalid" << endl;
        return;
    }
    if (!is_unblocked(grid, src.first, src.second) || !is_unblocked(grid, dest.first, dest.second)) {
        cout << "Source or the destination is blocked" << endl;
        return;
    }
    if (is_destination(src.first, src.second, dest)) {
        cout << "We are already at the destination" << endl;
        return;
    }

    vector<vector<bool>> closed_list(ROW, vector<bool>(COL, false));
    vector<vector<Cell>> cell_details(ROW, vector<Cell>(COL));

    int i = src.first;
    int j = src.second;
    cell_details[i][j].f = cell_details[i][j].g = cell_details[i][j].h = 0.0;
    cell_details[i][j].parent_i = i;
    cell_details[i][j].parent_j = j;

    typedef pair<double, pair<int, int>> pPair;
    priority_queue<pPair, vector<pPair>, greater<pPair>> open_list;
    open_list.push({0.0, {i, j}});

    bool found_dest = false;

    int directions[8][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0},
                            {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    while (!open_list.empty()) {
        pPair p = open_list.top();
        open_list.pop();

        i = p.second.first;
        j = p.second.second;
        closed_list[i][j] = true;

        for (int d = 0; d < 8; ++d) {
            int new_i = i + directions[d][0];
            int new_j = j + directions[d][1];
            if (is_valid(new_i, new_j) && is_unblocked(grid, new_i, new_j) && !closed_list[new_i][new_j]) {
                if (is_destination(new_i, new_j, dest)) {
                    cell_details[new_i][new_j].parent_i = i;
                    cell_details[new_i][new_j].parent_j = j;
                    cout << "The destination cell is found" << endl;
                    trace_path(cell_details, dest);
                    found_dest = true;
                    return;
                }
                else {
                    double g_new = cell_details[i][j].g + 1.0;
                    double h_new = calculate_h_value(new_i, new_j, dest);
                    double f_new = g_new + h_new;

                    if (cell_details[new_i][new_j].f == numeric_limits<double>::infinity() ||
                        cell_details[new_i][new_j].f > f_new) {
                        open_list.push({f_new, {new_i, new_j}});
                        cell_details[new_i][new_j].f = f_new;
                        cell_details[new_i][new_j].g = g_new;
                        cell_details[new_i][new_j].h = h_new;
                        cell_details[new_i][new_j].parent_i = i;
                        cell_details[new_i][new_j].parent_j = j;
                    }
                }
            }
        }
    }

    if (!found_dest) {
        cout << "Failed to find the destination cell" << endl;
    }
}

int main() {
    int grid[ROW][COL] =
    {
        { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
        { 1, 1, 1, 0, 1, 1, 1, 0, 1, 1 },
        { 1, 1, 1, 0, 1, 1, 0, 1, 0, 1 },
        { 0, 0, 1, 0, 1, 0, 0, 0, 0, 1 },
        { 1, 1, 1, 0, 1, 1, 1, 0, 1, 0 },
        { 1, 0, 1, 1, 1, 1, 0, 1, 0, 0 },
        { 1, 0, 0, 0, 0, 1, 0, 0, 0, 1 },
        { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
        { 1, 1, 1, 0, 0, 0, 1, 0, 0, 1 }
    };

    pair<int, int> src = {8, 0};
    pair<int, int> dest = {0, 0};

    a_star_search(grid, src, dest);

    return 0;
}
