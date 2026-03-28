#include <iostream>
#include <string>
using namespace std;

int main() {
    string s;
    cout << "Enter a string: ";
    cin >> s;

    int state = 0; // Start state: q0

    for (char ch : s) {
        switch (state) {
        case 0: // q0
            if (ch == '0') state = 0;
            else if (ch == '1') state = 1;
            break;

        case 1: // q1
            if (ch == '1') state = 1;
            else if (ch == '0') state = 2;
            break;

        case 2: // q2 (final state)
            if (ch == '1') state = 1;
            else if (ch == '0') state = 0;
            break;
        }
    }

    // q2 is the only accepting state
    if (state == 2)
        cout << "Accepted" << endl;
    else
        cout << "Rejected" << endl;

    return 0;
}