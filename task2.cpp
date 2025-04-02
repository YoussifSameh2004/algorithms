#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <tuple>
using namespace std;

class KnightsTour {
private:
    int n;
    vector<vector<int>> board;
    const int xMove[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };
    const int yMove[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };
    int startX, startY;
    bool isClosed;
    bool forceClosed;

    struct Move {
        int x, y, degree;
        bool operator<(const Move& other) const {
            return degree < other.degree;
        }
    };

public:
    KnightsTour(int size, int x = 0, int y = 0, bool closed = false)
        : n(size), board(size, vector<int>(size, -1)), startX(x), startY(y),
        isClosed(false), forceClosed(closed) {
    }

    bool isSafe(int x, int y) const {
        return (x >= 0 && x < n && y >= 0 && y < n && board[x][y] == -1);
    }

    void printSolution() const {
        cout << "\nKnight's Tour for " << n << "×" << n << " board:\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                cout << setw(3) << board[i][j] << " ";
            cout << endl;
        }
    }

 /*   int getDegree(int x, int y) const {
        int count = 0;
        for (int i = 0; i < 8; i++) {
            int next_x = x + xMove[i];
            int next_y = y + yMove[i];
            if (isSafe(next_x, next_y))
                count++;
        }
        return count;
    }*/
    int getDegree(int x, int y) const {
        int count = 0;
        for (int i = 0; i < 8; i++) {
            int next_x = x + xMove[i];
            int next_y = y + yMove[i];
            if (isSafe(next_x, next_y)) {
                count++;
                // Penalize moves that take us further from the start
                int distFromStart = abs(next_x - startX) + abs(next_y - startY);
                count -= distFromStart / 4; // Adjust weight as needed
            }
        }
        return count;
    }

    bool findTour(int x, int y, int movei) {
        board[x][y] = movei;

        // Check if tour is complete
        if (movei == n * n - 1) {
            // Check if it's closed when required
            if (!forceClosed) return true;

            for (int i = 0; i < 8; i++) {
                if (x + xMove[i] == startX && y + yMove[i] == startY) {
                    isClosed = true;
                    return true;
                }
            }
            board[x][y] = -1;
            return false;
        }

        vector<Move> nextMoves;
        for (int i = 0; i < 8; i++) {
            int next_x = x + xMove[i];
            int next_y = y + yMove[i];
            if (isSafe(next_x, next_y)) {
                int degree = getDegree(next_x, next_y);
                // Prioritize moves that could lead to closed tours when forced
                if (forceClosed && movei == n * n - 2) {
                    for (int j = 0; j < 8; j++) {
                        if (next_x + xMove[j] == startX && next_y + yMove[j] == startY) {
                            degree = -1; // Highest priority
                            break;
                        }
                    }
                }
                nextMoves.push_back({ next_x, next_y, degree });
            }
        }

        sort(nextMoves.begin(), nextMoves.end());

        for (const auto& move : nextMoves) {
            if (findTour(move.x, move.y, movei + 1))
                return true;
        }

        board[x][y] = -1;
        return false;
    }

    void solve() {
        if (n < 5) {
            cout << "No valid tour exists for n < 5.\n";
            return;
        }

        if (n % 2 == 1 && forceClosed) {
            cout << "Note: Closed tours are impossible for odd-sized boards.\n";
            forceClosed = false;
        }

        if (findTour(startX, startY, 0)) {
            printSolution();
            if (isClosed) {
                cout << "\nThis is a CLOSED (re-entrant) knight's tour.\n";
            }
            else {
                cout << "\nThis is an OPEN knight's tour.\n";
                if (forceClosed) {
                    cout << "Note: A closed tour exists but wasn't found.\n";
                    cout << "Try different starting positions or algorithms.\n";
                }
            }
        }
        else {
            cout << "No solution exists for this configuration.\n";
            if (forceClosed && n >= 6 && n % 2 == 0) {
                cout << "Note: Closed tours exist for this board size.\n";
                cout << "The algorithm failed to find one - try again or use a different approach.\n";
            }
        }
    }

    static void analyzeTours() {
        cout << "\nKnight's Tour Analysis:\n";
        cout << "----------------------\n";
        cout << "1. Closed tours exist for all even n ≥ 6.\n";
        cout << "2. For odd n, closed tours are impossible (but open tours exist for n ≥ 5).\n";
        cout << "3. For n = 5, only open tours exist.\n";
        cout << "4. For n < 5, no complete tours exist.\n";
        cout << "5. On 8×8 boards, there are 26,534,728,821,064 closed tours.\n";

        cout << "6. For reliable closed tours, consider:\n";
        cout << "   - Trying multiple starting positions\n";
        cout << "   - Using more sophisticated algorithms (e.g., divide-and-conquer)\n";

    }
};

int main() {
    int boardSize, startX, startY;
    bool wantClosed;
    cout << "Knight's Tour Solver\n";
    cout << "====================\n";

    cout << "Enter board size (n x n, minimum 5): ";
    cin >> boardSize;

    if (boardSize < 5) {
        cout << "No solution exists for n < 5.\n";
        return 0;
    }

    cout << "Enter starting position (row and column, 0-based): ";
    cin >> startX >> startY;

    if (startX < 0 || startX >= boardSize || startY < 0 || startY >= boardSize) {
        cout << "Invalid starting position.\n";
        return 0;
    }

    if (boardSize % 2 == 0 && boardSize >= 6) {
        cout << "Attempt closed tour? (1 for yes, 0 for no): ";
        cin >> wantClosed;
    }
    else {
        wantClosed = false;
    }

    KnightsTour kt(boardSize, startX, startY, wantClosed);
    kt.solve();

    KnightsTour::analyzeTours();

    return 0;
}
