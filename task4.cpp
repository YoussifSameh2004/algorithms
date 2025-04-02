#include <iostream>
#include <queue>
#include <map>
#include <vector>

using namespace std;

struct State {
    vector<pair<int, int>> white_knights;
    vector<pair<int, int>> black_knights;
    int moves;

    bool operator<(const State& other) const {
        return moves > other.moves; // Priority queue uses min-heap
    }
};

vector<pair<int, int>> moves = {
    {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
    {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
};

bool isValid(int x, int y) {
    return x >= 0 && x < 3 && y >= 0 && y < 4;
}

string serialize(const State& state) {
    string s = "";
    for (auto& w : state.white_knights)
        s += to_string(w.first) + to_string(w.second) + ",";
    for (auto& b : state.black_knights)
        s += to_string(b.first) + to_string(b.second) + ",";
    return s;
}

bool isGoal(const State& state) {
    for (int i = 0; i < 3; i++) {
        if (state.white_knights[i] != make_pair(0, i) ||
            state.black_knights[i] != make_pair(2, i))
            return false;
    }
    return true;
}

int knightSwap() {
    priority_queue<State> pq;
    map<string, bool> visited;

    State start = {{{2, 0}, {2, 1}, {2, 2}}, {{0, 0}, {0, 1}, {0, 2}}, 0};
    pq.push(start);
    visited[serialize(start)] = true;

    while (!pq.empty()) {
        State curr = pq.top();
        pq.pop();

        if (isGoal(curr))
            return curr.moves;

        for (int i = 0; i < 3; i++) {
            for (auto move : moves) {
                vector<pair<int, int>> next_white = curr.white_knights;
                vector<pair<int, int>> next_black = curr.black_knights;

                int new_x = next_white[i].first + move.first;
                int new_y = next_white[i].second + move.second;
                if (isValid(new_x, new_y)) {
                    next_white[i] = {new_x, new_y};
                    State next_state = {next_white, curr.black_knights, curr.moves + 1};
                    string key = serialize(next_state);
                    if (!visited[key]) {
                        visited[key] = true;
                        pq.push(next_state);
                    }
                }

                new_x = next_black[i].first + move.first;
                new_y = next_black[i].second + move.second;
                if (isValid(new_x, new_y)) {
                    next_black[i] = {new_x, new_y};
                    State next_state = {curr.white_knights, next_black, curr.moves + 1};
                    string key = serialize(next_state);
                    if (!visited[key]) {
                        visited[key] = true;
                        pq.push(next_state);
                    }
                }
            }
        }
    }
    return -1;
}

int main() {
    cout << "Minimum moves required: " << knightSwap() << endl;
    return 0;
}
