//
// Created by TimV22 on 4-12-2024.
// > Does it work? Yes
// > Is it good? No, definitely not
//
#include <algorithm>
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

void stop(const chrono::time_point<chrono::system_clock> start) {
    const auto stop = chrono::high_resolution_clock::now();
    const auto duration = chrono::duration_cast<chrono::nanoseconds>(stop - start);
    cout << "Time taken: " << fixed << setprecision(5)
            << duration.count() << " nanoseconds " << endl;
}

vector<vector<char> > getInput(const string &filename) {
    ifstream f(filename);

    if (!f.is_open()) {
        cout << "Could not open file" << endl;
        return {};
    }

    vector<vector<char> > data;

    string line;
    while (getline(f, line)) {
        vector<char> row;
        for (char c: line) {
            row.push_back(c);
        }
        data.push_back(row);
    }

    f.close();
    return data;
}

void getCubeFromGrid(const vector<vector<char> > &grid, vector<vector<char> > &cube, const int x, const int y,
                     const int searchCubeSize) {
    for (int i = 0; i < searchCubeSize; i++) {
        for (int j = 0; j < searchCubeSize; j++) {
            cube[i][j] = grid[x + i][y + j];
        }
    }
}

int searchCubeForWord(const vector<vector<char> > &cube, const string &wordToFind) {
    const int searchCubeSize = static_cast<int>(wordToFind.length());
    int timesFound = 0;

    const auto addCombination = [&](const int startX, const int startY, const int stepX, const int stepY) {
        for (int i = 0; i < searchCubeSize; i++) {
            if (cube[startX + i * stepX][startY + i * stepY] != wordToFind[i]) {
                return;
            }
        }
        timesFound++;
    };

    // horizontal combinations
    addCombination(0, 0, 0, 1);
    addCombination(0, 0, 1, 0);

    // vertical combinations
    addCombination(0, searchCubeSize - 1, 0, -1);
    addCombination(searchCubeSize - 1, 0, -1, 0);

    // diagonal combinations
    addCombination(0, 0, 1, 1);
    addCombination(searchCubeSize - 1, searchCubeSize - 1, -1, -1);

    // reverse diagonal combinations
    addCombination(0, searchCubeSize - 1, 1, -1);
    addCombination(searchCubeSize - 1, 0, -1, 1);

    return timesFound;
}

void searchWordInCubes(vector<vector<char> > &grid, const string &wordToFind) {
    const int searchCubeSize = static_cast<int>(wordToFind.length());
    int totalTimesFound = 0;

    // Add padding to the grid
    for (auto &row: grid) {
        row.insert(row.end(), searchCubeSize - 1, '.');
    }

    vector paddingRow(grid[0].size(), '.');
    for (int i = 0; i < searchCubeSize - 1; i++) {
        grid.push_back(paddingRow);
    }

    vector cube(searchCubeSize, vector<char>(searchCubeSize));

    for (int i = 0; i < grid.size() - searchCubeSize + 1; i++) {
        for (int j = 0; j < grid[i].size() - searchCubeSize + 1; j++) {
            getCubeFromGrid(grid, cube, i, j, searchCubeSize);
            totalTimesFound += searchCubeForWord(cube, wordToFind);
        }
    }

    cout << "Word " << wordToFind << " was found " << totalTimesFound << " times" << endl;
}

bool matchesPattern(const vector<vector<char> > &grid, int x, int y, const vector<vector<char> > &pattern) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (pattern[i][j] != '.' && grid[x + i][y + j] != pattern[i][j]) {
                return false;
            }
        }
    }
    return true;
}

bool searchCubeForXMas(const vector<vector<char> > &grid, int x, int y) {
    vector<vector<vector<char> > > patterns = {
        {{'M', '.', 'S'}, {'.', 'A', '.'}, {'M', '.', 'S'}},
        {{'M', '.', 'M'}, {'.', 'A', '.'}, {'S', '.', 'S'}},
        {{'S', '.', 'M'}, {'.', 'A', '.'}, {'S', '.', 'M'}},
        {{'S', '.', 'S'}, {'.', 'A', '.'}, {'M', '.', 'M'}}
    };

    if (std::any_of(patterns.begin(), patterns.end(), [&](const auto &pattern) {
        return matchesPattern(grid, x, y, pattern);
    })) {
        return true;
    }
    return false;
}

void searchXDashMasInCubes(const vector<vector<char> > &grid) {
    constexpr int searchCubeSize = 3;
    int totalTimesFound = 0;

    for (int i = 0; i <= grid.size() - searchCubeSize; i++) {
        for (int j = 0; j <= grid[i].size() - searchCubeSize; j++) {
            if (searchCubeForXMas(grid, i, j)) {
                totalTimesFound++;
            }
        }
    }

    cout << "X-Mas was found " << totalTimesFound << " times" << endl;
}

int main() {
    const auto start = chrono::high_resolution_clock::now();
    vector<vector<char> > grid = getInput("input.txt");

    // part 1, ~0.0031091 seconds <- 3109100 nanoseconds
    searchWordInCubes(grid, "XMAS");

    // part 2 slow :(, ~0.0516225 seconds <- 55790100 nanoseconds (too lazy)
    searchXDashMasInCubes(grid);

    stop(start);
    return 0;
}
