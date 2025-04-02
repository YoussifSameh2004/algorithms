#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstring>
using namespace std;

int size_of_grid, cnt = 0;
char arr[128][128]; // Using char array for colors
vector<char> colors = { 'R', 'G', 'B' }; // Three colors: Red, Green, Blue

// Placing tile at the given coordinates with a specific color
void place(int x1, int y1, int x2, int y2, int x3, int y3, char color)
{
    arr[x1][y1] = color;
    arr[x2][y2] = color;
    arr[x3][y3] = color;
}

// Function based on divide and conquer
int tile(int n, int x, int y)
{
    int r = -1, c = -1;
    if (n == 2) {
        cnt++;
        char color = colors[cnt % 3]; // Cycle through colors
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (arr[x + i][y + j] == 0) {
                    arr[x + i][y + j] = color;
                }
            }
        }
        return 0;
    }

    // Finding hole location
    for (int i = x; i < x + n; i++) {
        for (int j = y; j < y + n; j++) {
            if (arr[i][j] != 0) {
                r = i;
                c = j;
            }
        }
    }

    // Select color for this stage
    char color = colors[cnt % 3];

    // Place center tromino depending on the hole's position
    if (r < x + n / 2 && c < y + n / 2) {
        place(x + n / 2, y + (n / 2) - 1, x + n / 2, y + n / 2, x + n / 2 - 1, y + n / 2, color);
    }
    else if (r >= x + n / 2 && c < y + n / 2) {
        place(x + (n / 2) - 1, y + (n / 2), x + (n / 2), y + n / 2, x + (n / 2) - 1, y + (n / 2) - 1, color);
    }
    else if (r < x + n / 2 && c >= y + n / 2) {
        place(x + n / 2, y + (n / 2) - 1, x + n / 2, y + n / 2, x + n / 2 - 1, y + n / 2 - 1, color);
    }
    else {
        place(x + (n / 2) - 1, y + (n / 2), x + (n / 2), y + (n / 2) - 1, x + (n / 2) - 1, y + (n / 2) - 1, color);
    }

    cnt++;

    // Divide and conquer on four quadrants
    tile(n / 2, x, y + n / 2);
    tile(n / 2, x, y);
    tile(n / 2, x + n / 2, y);
    tile(n / 2, x + n / 2, y + n / 2);

    return 0;
}

int main()
{
    srand(time(0)); // Seed for random number generation

    // Get grid size from user (must be power of 2)
    cout << "Enter grid size (must be power of 2, e.g., 2, 4, 8, 16, ...): ";
    cin >> size_of_grid;

    // Validate input is power of 2
    if ((size_of_grid & (size_of_grid - 1)) != 0) {
        cout << "Error: Grid size must be a power of 2." << endl;
        return 1;
    }

    memset(arr, 0, sizeof(arr));

    // Randomly select missing square
    int a = rand() % size_of_grid;
    int b = rand() % size_of_grid;
    arr[a][b] = 'X'; // Marking the missing square

    cout << "Missing square at: (" << a << ", " << b << ")" << endl;

    tile(size_of_grid, 0, 0);

    // Display the grid
    for (int i = 0; i < size_of_grid; i++) {
        for (int j = 0; j < size_of_grid; j++) {
            if (arr[i][j] == 0) cout << ". ";
            else cout << arr[i][j] << " ";
        }
        cout << "\n";
    }

    return 0;
}
