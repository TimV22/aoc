//
// Created by TimV22 on 2-12-2024.
//

#include <chrono>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <sstream>

using namespace std;

void extractRows(vector<vector<int>> &v, ifstream &f) {
    string s;
    while (getline(f, s)) {
        vector<int> row;
        stringstream ss(s);
        int number;
        while (ss >> number) {
            row.push_back(number);
        }
        v.push_back(row);
    }
}

bool isLineSafe(const std::vector<int> &v) {
    if (v.empty()) {
        cout << "Empty vector" << endl;
        return true;
    }

    bool ascending = true;
    bool descending = true;

    for (size_t i = 1; i < v.size(); ++i) {
        // forgot abs() here, oops
        if (const int diff = abs(v[i] - v[i - 1]); diff < 1 || diff > 3) return false;
        if (v[i] < v[i - 1]) ascending = false;
        else if (v[i] > v[i - 1]) descending = false;
        if (!ascending && !descending) return false;
    }

    return true;
}

bool isLineSafeWithDampener(const std::vector<int> &v) {
    if (v.empty()) {
        cout << "Empty vector" << endl;
        return true;
    }

    if (isLineSafe(v)) return true;

    // skill issues, should obviously be 0... :sob:
    for (size_t i = 0; i < v.size(); ++i) {
        vector<int> copy = v;
        copy.erase(copy.begin() + static_cast<std::vector<int>::difference_type>(i));
        if (isLineSafe(copy)) return true;
    }

    return false;
}

void stop(const chrono::time_point<chrono::system_clock> start) {
    const auto stop = chrono::high_resolution_clock::now();
    const auto duration = chrono::duration_cast<chrono::nanoseconds>(stop - start);
    cout << "Time taken by program is : " << fixed
            << duration.count() << setprecision(5);
    cout << " nanoseconds " << endl;
}

int main() {
    const auto start = chrono::high_resolution_clock::now();
    ifstream f("input.txt");

    if (!f.is_open()) {
        cerr << "File not found" << endl;
        return 1;
    }
    vector<vector<int>> list;
    extractRows(list, f);

    int safeCounter = 0;
    int unsafeCounter = 0;

    for (const auto &v : list) {
        // const auto isSafe = isLineSafe(v); // * part 1
        const auto isSafe = isLineSafeWithDampener(v); // * part 2
        if (isSafe) {
            safeCounter++;
        } else {
            unsafeCounter++;
        }
    }
    cout << "Safe: " << safeCounter << endl;
    cout << "Unsafe: " << unsafeCounter << endl;

    stop(start);
    return 0;
}