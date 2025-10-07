#include <iostream>
#include <vector>
#include <set>
#include <string>
using namespace std;

int main() {
    vector<string> initial_facts = {"X", "Y"};

    vector<pair<vector<string>, string>> inference_rules = {
        {{"X", "Y"}, "Z"},
        {{"Z"}, "W"},
        {{"W"}, "V"}
    };

    set<string> known_facts(initial_facts.begin(), initial_facts.end());
    bool new_fact_found = true;

    cout << "Starting facts: ";
    for (const auto& fact : initial_facts)
        cout << fact << " ";
    cout << endl;

    while (new_fact_found) {
        new_fact_found = false;
        for (auto& rule : inference_rules) {
            const vector<string>& prerequisites = rule.first;
            const string& outcome = rule.second;

            bool prerequisites_met = true;
            for (const auto& cond : prerequisites) {
                if (known_facts.find(cond) == known_facts.end()) {
                    prerequisites_met = false;
                    break;
                }
            }

            if (prerequisites_met && known_facts.find(outcome) == known_facts.end()) {
                known_facts.insert(outcome);
                cout << "Derived new fact: " << outcome << endl;
                new_fact_found = true;
            }
        }
    }

    cout << "Final derived facts: ";
    for (const auto& fact : known_facts)
        cout << fact << " ";
    cout << endl;

    return 0;
}
