#include <iostream>
#include <map>
#include <set>
#include <queue>
#include <vector>
#include <string>
using namespace std;

// NFA transitions: nfa_delta[state][symbol] = set of next states
map<int, map<char, set<int>>> nfa_delta;

// NFA final states
set<int> nfa_final = { 1 };

// Input alphabet
vector<char> alphabet = { '0', '1' };

// Compute move(T, a): union of NFA transitions for all states in T on symbol a
set<int> move(const set<int>& states, char symbol) {
    set<int> result;
    for (int s : states) {
        if (nfa_delta.count(s) && nfa_delta[s].count(symbol)) {
            for (int ns : nfa_delta[s][symbol])
                result.insert(ns);
        }
    }
    return result;
}

// Check if a DFA state (set of NFA states) is a final state
bool isFinal(const set<int>& states) {
    for (int s : states)
        if (nfa_final.count(s)) return true;
    return false;
}

// Convert set to readable string like "{0,1}"
string setToString(const set<int>& s) {
    if (s.empty()) return "{}";
    string res = "{";
    for (int x : s) res += to_string(x) + ",";
    res.back() = '}';
    return res;
}

int main() {
    // Define NFA transitions
    // q0: 0 -> {q0}, 1 -> {q1}
    nfa_delta[0]['0'] = { 0 };
    nfa_delta[0]['1'] = { 1 };
    // q1: 0 -> {q0}, 1 -> {q0, q1}
    nfa_delta[1]['0'] = { 0 };
    nfa_delta[1]['1'] = { 0, 1 };

    // Subset Construction
    map<set<int>, int>  dfa_id;       // maps NFA-state-set → DFA state ID
    map<int, map<char, int>> dfa_delta; // DFA transition table
    set<int> dfa_final_states;

    queue<set<int>> worklist;

    set<int> start = { 0 }; // NFA start state
    dfa_id[start] = 0;
    worklist.push(start);

    int next_id = 1;

    while (!worklist.empty()) {
        set<int> current = worklist.front();
        worklist.pop();

        int cur_id = dfa_id[current];

        if (isFinal(current))
            dfa_final_states.insert(cur_id);

        for (char sym : alphabet) {
            set<int> next = move(current, sym);

            if (next.empty()) continue; // dead state (skip for simplicity)

            if (!dfa_id.count(next)) {
                dfa_id[next] = next_id++;
                worklist.push(next);
            }

            dfa_delta[cur_id][sym] = dfa_id[next];
        }
    }

    // Print DFA Transition Table
    cout << "==============================" << endl;
    cout << "  DFA Transition Table" << endl;
    cout << "==============================" << endl;
    cout << "  State\t\t|  0\t|  1" << endl;
    cout << "------------------------------" << endl;

    // Build reverse map: DFA ID → NFA set label
    map<int, set<int>> id_to_set;
    for (map<set<int>, int>::iterator it = dfa_id.begin(); it != dfa_id.end(); ++it)
        id_to_set[it->second] = it->first;

    for (map<int, set<int>>::iterator it = id_to_set.begin(); it != id_to_set.end(); ++it) {
        int id = it->first;
        set<int> nfa_set = it->second;

        string label = setToString(nfa_set);
        string marker = "";
        if (id == 0) marker += "-> ";
        if (dfa_final_states.count(id)) marker += "* ";

        cout << marker << "D" << id << " " << label << "\t";
        if (label.length() < 5) cout << "\t";

        for (int i = 0; i < (int)alphabet.size(); i++) {
            char sym = alphabet[i];
            if (dfa_delta[id].count(sym)) {
                int nxt = dfa_delta[id][sym];
                cout << "| D" << nxt << "\t";
            }
            else {
                cout << "| --\t";
            }
        }
        cout << endl;
    }

    cout << "==============================" << endl;
    cout << "\nFinal DFA States: ";
    for (int s : dfa_final_states) cout << "D" << s << " ";
    cout << endl;

    return 0;
}