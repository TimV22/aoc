//
// Created by TimV22 on 3-12-2024.
// let's try not using regex :p
//
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

void stop(const chrono::time_point<chrono::system_clock> start) {
    const auto stop = chrono::high_resolution_clock::now();
    const auto duration = chrono::duration_cast<chrono::nanoseconds>(stop - start);
    cout << "Time taken: " << fixed << setprecision(5)
            << duration.count() << " nanoseconds " << endl;
}

string getCorruptedData() {
    ifstream f("input.txt", ios::in | ios::ate);
    if (!f.is_open()) {
        cout << "Could not open file" << endl;
        return "";
    }

    const streamsize size = f.tellg();
    f.seekg(0, ios::beg);

    string data(size, ' ');
    f.read(&data[0], size);
    return data;
}

bool isRightInside(const string &s) {
    int commaCount = 0;
    for (char c: s) {
        if (!isdigit(c) && c != ',') return false;
        if (c == ',') commaCount++;
    }
    return commaCount == 1;
}

int mul(const string &expr) {
    int num1, num2;
    if (sscanf(expr.c_str(), "mul(%d,%d)", &num1, &num2) == 2) {
        return num1 * num2;
    }
    return 0;
}

void updateMulState(const string &input, size_t &pos, bool &mulEnabled) {
    if (input.compare(pos, 4, "do()") == 0) {
        mulEnabled = true;
        pos += 4;
    } else if (input.compare(pos, 7, "don't()") == 0) {
        mulEnabled = false;
        pos += 7;
    }
}

void extractMulExpressions(const string &input, bool part2 = false) {
    int results = 0;
    string::size_type pos = 0;
    bool mulEnabled = true;

    while (pos < input.size()) {
        if (part2) {
            updateMulState(input, pos, mulEnabled);
        }

        if (mulEnabled && input.find("mul(", pos) == pos) {
            if (const auto endPos = input.find(')', pos); endPos != string::npos) {
                if (isRightInside(input.substr(pos + 4, endPos - pos - 4))) {
                    results += mul(input.substr(pos, endPos - pos + 1));
                    pos = endPos;
                }
            } else {
                break;
            }
        }
        pos++;
    }

    cout << "Part " << (part2 ? "2" : "1") << ": " << results << endl;
}

int main() {
    const auto start = chrono::high_resolution_clock::now();
    const string input = getCorruptedData();

    extractMulExpressions(input);
    extractMulExpressions(input, true);

    stop(start);
    return 0;
}
