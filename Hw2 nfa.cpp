#include <iostream>
#include <string>
#include <set>
using namespace std;

int main() {
    string s;
    cout << "Enter a string: ";
    cin >> s;

    set<int> current_states = { 0 }; // Start state: q0

    for (char ch : s) {
        set<int> next_states;

        for (int state : current_states) {
            switch (state) {
            case 0: // q0
                if (ch == 'a') {
                    next_states.insert(0); // self-loop
                    next_states.insert(1); // go to q1
                }
                else if (ch == 'b') {
                    next_states.insert(0); // self-loop
                }
                break;

            case 1: // q1
                if (ch == 'b') {
                    next_states.insert(1); // self-loop
                    next_states.insert(2); // go to q2 (final)
                }
                break;

            case 2: // q2 (final state) - no transitions
                break;
            }
        }

        current_states = next_states;
    }

    // Accept if q2 is in current states
    if (current_states.count(2))
        cout << "Accepted" << endl;
    else
        cout << "Rejected" << endl;

    return 0;
}