#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

const int SIZE = 10;
const int MINES = 10;

struct Cell {
    bool isMine = false;
    bool isOpened = false;
    int nearbyMines = 0;
};

using Field = std::vector<std::vector<Cell>>;

void placeMines(Field& field) {
    int placed = 0;
    while (placed < MINES) {
        int r = rand() % SIZE;
        int c = rand() % SIZE;
        if (!field[r][c].isMine) {
            field[r][c].isMine = true;
            ++placed;
        }
    }
}

void countMines(Field& field) {
    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            if (field[r][c].isMine) continue;
            int count = 0;
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    int nr = r + dr;
                    int nc = c + dc;
                    if (nr >= 0 && nr < SIZE && nc >= 0 && nc < SIZE) {
                        if (field[nr][nc].isMine)
                            ++count;
                    }
                }
            }
            field[r][c].nearbyMines = count;
        }
    }
}

void printField(const Field& field, bool revealAll = false) {
    std::cout << "   ";
    for (int i = 0; i < SIZE; ++i)
        std::cout << i << ' ';
    std::cout << "\n";

    for (int r = 0; r < SIZE; ++r) {
        std::cout << r << ": ";
        for (int c = 0; c < SIZE; ++c) {
            if (revealAll || field[r][c].isOpened) {
                if (field[r][c].isMine)
                    std::cout << "* ";
                else
                    std::cout << field[r][c].nearbyMines << ' ';
            } else {
                std::cout << "# ";
            }
        }
        std::cout << "\n";
    }
}

bool openCell(Field& field, int r, int c) {
    if (r < 0 || r >= SIZE || c < 0 || c >= SIZE || field[r][c].isOpened)
        return true;

    field[r][c].isOpened = true;

    if (field[r][c].isMine)
        return false;

    // Автооткрытие соседей, если 0
    if (field[r][c].nearbyMines == 0) {
        for (int dr = -1; dr <= 1; ++dr)
            for (int dc = -1; dc <= 1; ++dc)
                if (dr != 0 || dc != 0)
                    openCell(field, r + dr, c + dc);
    }

    return true;
}

bool isWin(const Field& field) {
    for (int r = 0; r < SIZE; ++r)
        for (int c = 0; c < SIZE; ++c)
            if (!field[r][c].isMine && !field[r][c].isOpened)
                return false;
    return true;
}

int main() {
    srand(time(0));
    Field field(SIZE, std::vector<Cell>(SIZE));

    placeMines(field);
    countMines(field);

    while (true) {
        printField(field);
        int r, c;
        std::cout << "Enter coordinates (row col): ";
        std::cin >> r >> c;

        if (!openCell(field, r, c)) {
            std::cout << "BOOM! You hit a mine.\n";
            printField(field, true);
            break;
        }

        if (isWin(field)) {
            std::cout << "Congratulations! You cleared the field!\n";
            printField(field, true);
            break;
        }
    }

    return 0;
}
