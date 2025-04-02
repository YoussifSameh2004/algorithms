#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

struct Move {
    int disk, from, to;
};

vector<Move> moves;

// Function to print the pegs in a structured 4-column format, ensuring n rows always
void printPegs(vector<vector<int>>& pegs, int num_disks) {
    vector<vector<int>> pegDisplay(4, vector<int>(num_disks, 0));

    // Fill pegDisplay with current peg values, ensuring correct row count
    for (int p = 0; p < 4; ++p) {
        int pegSize = pegs[p].size();
        for (int i = 0; i < pegSize; ++i) {
            pegDisplay[p][num_disks - 1 - i] = pegs[p][i];  // Fill from bottom to top
        }
    }

    cout << "\nCurrent Peg State:\n";
    for (int i = 0; i < num_disks; ++i) {
        for (int j = 0; j < 4; ++j) {
            cout << setw(3) << pegDisplay[j][i] << "  ";  // Print disks or 0s
        }
        cout << endl;
    }

    // Print peg labels
    cout << "---- ---- ---- ----\n";
    cout << " P1   P2   P3   P4 \n";
    cout << "===================\n";
}

// Move a single disk and store the move
void moveDisk(vector<vector<int>>& pegs, int from, int to, int disk, int num_disks) {
    pegs[to].push_back(disk);
    pegs[from].pop_back();
    moves.push_back({ disk, from + 1, to + 1 });
    printPegs(pegs, num_disks); // Print pegs after each move
}

// Recursive Tower of Hanoi for 3 pegs
void hanoi(int n, int from, int to, int aux, vector<vector<int>>& pegs, int num_disks) {
    if (n == 0) return;
    if (n == 1) {
        moveDisk(pegs, from, to, pegs[from].back(), num_disks);
        return;
    }
    hanoi(n - 1, from, aux, to, pegs, num_disks);
    moveDisk(pegs, from, to, pegs[from].back(), num_disks);
    hanoi(n - 1, aux, to, from, pegs, num_disks);
}

// Recursive Frame-Stewart algorithm for 4-peg Tower of Hanoi (Reve's Puzzle)
void reve(int n, int from, int to, int aux1, int aux2, vector<vector<int>>& pegs, int num_disks) {
    if (n == 0) return;
    if (n == 1) {
        moveDisk(pegs, from, to, pegs[from].back(), num_disks);
        return;
    }

    int k = round(n + 1 - sqrt(2 * n + 1)); // Optimal partitioning

    reve(k, from, aux1, aux2, to, pegs, num_disks);
    hanoi(n - k, from, to, aux2, pegs, num_disks);
    reve(k, aux1, to, from, aux2, pegs, num_disks);
}

int main() {
    int n;
    cout << "Enter number of disks: ";
    cin >> n;

    // Initialize pegs
    vector<vector<int>> pegs(4);
    for (int i = n; i >= 1; --i) {
        pegs[0].push_back(i); // Largest disk = n, smallest = 1
    }

    printPegs(pegs, n); // Print initial peg setup

    // Solve the 4-peg Tower of Hanoi
    reve(n, 0, 3, 1, 2, pegs, n);

    // Print final moves
    cout << "\nMoves executed:\n";
    for (const auto& move : moves) {
        cout << "Move disk " << move.disk << " from peg " << move.from << " to peg " << move.to << endl;
    }

    // Print total number of moves
    cout << "\nTotal moves: " << moves.size() << endl;

    return 0;
}
