#include <iostream>
#include <limits>
using namespace std;

class MissionaryCannibal {
public:
    int left[3];  // missionaries, cannibals, boat presence
    int right[3];

    MissionaryCannibal() {
        left[0] = 3; left[1] = 3; left[2] = 1;
        right[0] = 0; right[1] = 0; right[2] = 0;
    }

    void display_state() {
        cout << "Left side: " << left[0] << " Missionaries, " << left[1] << " Cannibals, Boat: " << (left[2] == 1 ? "Yes" : "No") << endl;
        cout << "Right side: " << right[0] << " Missionaries, " << right[1] << " Cannibals, Boat: " << (right[2] == 1 ? "Yes" : "No") << endl;
    }

    bool move(int missionaries, int cannibals, bool to_right) {
        if (missionaries + cannibals > 2 || missionaries < 0 || cannibals < 0 || (missionaries == 0 && cannibals == 0)) {
            return false;
        }

        if (to_right) {
            if (left[0] >= missionaries && left[1] >= cannibals && left[2] == 1) {
                left[0] -= missionaries;
                left[1] -= cannibals;
                right[0] += missionaries;
                right[1] += cannibals;
                left[2] = 0;
                right[2] = 1;
                return true;
            }
        } else {
            if (right[0] >= missionaries && right[1] >= cannibals && right[2] == 1) {
                right[0] -= missionaries;
                right[1] -= cannibals;
                left[0] += missionaries;
                left[1] += cannibals;
                right[2] = 0;
                left[2] = 1;
                return true;
            }
        }
        return false;
    }

    bool check_lose() {
        if ((left[0] > 0 && left[1] > left[0]) || (right[0] > 0 && right[1] > right[0])) {
            return true;
        }
        return false;
    }

    bool win() {
        return (right[0] == 3 && right[1] == 3);
    }
};

int main() {
    MissionaryCannibal game;
    while (true) {
        game.display_state();
        if (game.win()) {
            cout << "Congratulations! All missionaries and cannibals have successfully crossed the river." << endl;
            break;
        }
        if (game.check_lose()) {
            cout << "You lose. Cannibals outnumber missionaries on one side." << endl;
            break;
        }
        int missionaries, cannibals;
        cout << "Enter number of missionaries to move: ";
        while (!(cin >> missionaries)) {
            cout << "Invalid input. Please enter an integer.\n";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        cout << "Enter number of cannibals to move: ";
        while (!(cin >> cannibals)) {
            cout << "Invalid input. Please enter an integer.\n";
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        bool to_right = game.left[2] == 1;

        if (!game.move(missionaries, cannibals, to_right)) {
            cout << "Invalid move. Try again." << endl;
        }
    }
    return 0;
}
