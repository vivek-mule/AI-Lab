#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Function declaration for recursion
bool backward_chaining(const string& goal, const vector<string>& current_facts,
                       const vector<pair<vector<string>, string>>& inference_rules);

bool backward_chaining(const string& goal, const vector<string>& current_facts,
                       const vector<pair<vector<string>, string>>& inference_rules) {
    // If the goal is already known, return true
    for (const auto& fact : current_facts) {
        if (fact == goal) {
            cout << "Goal " << goal << " is already known." << endl;
            return true;
        }
    }

    // Try to find a rule that concludes the goal
    for (const auto& rule : inference_rules) {
        const vector<string>& prerequisites = rule.first;
        const string& conclusion = rule.second;

        if (conclusion == goal) {
            cout << "Trying to prove goal: " << goal << " using rule with conclusion " << conclusion << endl;
            bool all_conditions_met = true;
            for (const auto& condition : prerequisites) {
                if (!backward_chaining(condition, current_facts, inference_rules)) {
                    all_conditions_met = false;
                    break;
                }
            }
            if (all_conditions_met) {
                cout << "Goal " << goal << " has been proven." << endl;
                return true;
            }
        }
    }

    cout << "Goal " << goal << " cannot be proven." << endl;
    return false;
}

int main() {
    vector<string> known_facts = { "X", "Y" };
    vector<pair<vector<string>, string>> inference_rules = {
        { {"X", "Y"}, "Z" },
        { {"Z"}, "W" },
        { {"W"}, "V" }
    };

    string goal_to_prove = "V";

    cout << "\nStarting backward chaining to prove goal: " << goal_to_prove << endl;
    bool can_prove_goal = backward_chaining(goal_to_prove, known_facts, inference_rules);

    if (can_prove_goal) {
        cout << "Final conclusion: The goal " << goal_to_prove << " can be proven." << endl;
    } else {
        cout << "Final conclusion: The goal " << goal_to_prove << " cannot be proven." << endl;
    }

    return 0;
}
