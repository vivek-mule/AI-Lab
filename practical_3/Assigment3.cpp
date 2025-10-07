#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <sstream>

using namespace std;

// --- Knowledge Base (Facts) ---

// Map to store Parent(ParentName, ChildName) facts
// Key: Parent, Value: List of Children
map<string, vector<string>> parent_facts = {
    {"Charles", {"William", "Harry"}},
    {"Diana", {"William", "Harry"}},
    {"William", {"George", "Charlotte", "Louis"}},
    {"Kate", {"George", "Charlotte", "Louis"}},
    {"George", {}}, 
    {"Louis", {}},
    {"Harry", {"Archie", "Lilibet"}},
    {"Meghan", {"Archie", "Lilibet"}}
};

// Map to store Gender(Name) facts
map<string, string> gender_facts = {
    {"Charles", "male"},
    {"Diana", "female"},
    {"William", "male"},
    {"Kate", "female"},
    {"Harry", "male"},
    {"Meghan", "female"},
    {"George", "male"},
    {"Charlotte", "female"},
    {"Louis", "male"},
    {"Archie", "male"},
    {"Lilibet", "female"}
};

// --- Inference Rules (Parsing/Querying Functions) ---

// Rule 1: Checks if Y is a Parent of X
bool is_parent(const string& parent, const string& child) {
    if (parent_facts.count(parent)) {
        const auto& children = parent_facts.at(parent);
        return find(children.begin(), children.end(), child) != children.end();
    }
    return false;
}

// Rule 2: Checks if X is the Grandparent of Z
// Grandparent(X, Z) :- Parent(X, Y), Parent(Y, Z).
bool is_grandparent(const string& gp, const string& grandchild) {
    // Iterate through the knowledge base to find an intermediate parent Y
    for (const auto& pair : parent_facts) {
        const string& intermediate_parent = pair.first;
        // Check if X is parent of Y AND Y is parent of Z
        if (is_parent(gp, intermediate_parent) && is_parent(intermediate_parent, grandchild)) {
            return true;
        }
    }
    return false;
}

// Rule 3: Checks if X and Y are Siblings
// Sibling(X, Y) :- Parent(P, X), Parent(P, Y), X != Y.
bool is_sibling(const string& person1, const string& person2) {
    if (person1 == person2) return false;

    // 1. Find all parents of person1
    vector<string> parents1;
    for (const auto& pair : parent_facts) {
        if (is_parent(pair.first, person1)) {
            parents1.push_back(pair.first);
        }
    }

    // 2. Check if any parent of person2 is in the parents1 list
    for (const auto& pair : parent_facts) {
        if (is_parent(pair.first, person2)) {
            // Check for common parent
            if (find(parents1.begin(), parents1.end(), pair.first) != parents1.end()) {
                return true; 
            }
        }
    }

    return false;
}

// Rule 4: Checks if X is the Brother of Y
// Brother(X, Y) :- Sibling(X, Y), Male(X).
bool is_brother(const string& brother, const string& person) {
    if (gender_facts.count(brother) && gender_facts.at(brother) == "male") {
        return is_sibling(brother, person);
    }
    return false;
}

// --- User Input Handling ---

void run_query_interface() {
    string query;
    cout << "\n--- Family Tree Query Interface ---\n";
    cout << "Available Relations: PARENT, GRANDPARENT, SIBLING, BROTHER\n";
    cout << "Format: RELATION Person1 Person2 (e.g., GRANDPARENT Charles George)\n";
    cout << "Type 'EXIT' to quit.\n";

    while (true) {
        cout << "\nQuery> ";
        getline(cin, query);

        if (query == "EXIT") break;

        // Use stringstream to parse the query into components
        stringstream ss(query);
        string relation, p1, p2;
        ss >> relation >> p1 >> p2;

        if (p1.empty() || p2.empty()) {
            cout << "Invalid format. Use: RELATION Person1 Person2\n";
            continue;
        }

        // Convert relation to uppercase for robust comparison
        transform(relation.begin(), relation.end(), relation.begin(), ::toupper);

        bool result = false;

        // Use the inference rules based on the user's query
        if (relation == "PARENT") {
            result = is_parent(p1, p2);
        } else if (relation == "GRANDPARENT") {
            result = is_grandparent(p1, p2);
        } else if (relation == "SIBLING") {
            result = is_sibling(p1, p2);
        } else if (relation == "BROTHER") {
            result = is_brother(p1, p2);
        } else {
            cout << "Error: Unknown relation '" << relation << "'\n";
            continue;
        }

        cout << (result ? "TRUE" : "FALSE") << endl;
    }
}

int main() {
    run_query_interface();
    return 0;
}