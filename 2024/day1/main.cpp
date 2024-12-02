#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

void totalDistance(const vector<int> &v1, const vector<int> &v2) {
    int sum = 0;
    for (int i = 0; i < v1.size(); i++) {
        sum += std::abs(v1[i] - v2[i]);
    }

    cout << "The total distance between the two groups is: " << sum << endl;
}

void sortedSimilarityScore(const vector<int> &v1, const vector<int> &v2) {
    map<int, int> countMap;
    for (const int &i: v2) {
        countMap[i]++;
    }

    int score = 0;
    for (const int &i: v1) {
        if (countMap[i] > 0) {
            score += i * countMap[i];
            countMap[i] = 0;
        }
    }

    cout << "The similarity score between the two groups is: " << score << endl;
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
        cerr << "Could not open file" << endl;
        return 1;
    }

    string s;
    vector<int> historianGroup1, historianGroup2;

    while (getline(f, s)) {
        string first = s.substr(0, s.length() / 2);
        string second = s.substr(s.length() / 2);

        historianGroup1.push_back(stoi(first));
        historianGroup2.push_back(stoi(second));
    }

    sort(historianGroup1.begin(), historianGroup1.end());
    sort(historianGroup2.begin(), historianGroup2.end());

    totalDistance(historianGroup1, historianGroup2);
    sortedSimilarityScore(historianGroup1, historianGroup2);

    stop(start);
    return 0;
}
