#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <utility>
#include <unordered_set>
using namespace std;

bool checkPreference(vector<vector<int>> studentPreference, int n, int s, int h, int currentMatch) {
    for (int i = 0; i < n; i++) {
        //Student prefers h over current match since h is a higher rank
        if (studentPreference[s][i] == h) {
            return true;
        }
        //Student does not prefer h over current match, h is a lower rank
        if (studentPreference[s][i] == currentMatch) {
            return false;
        }
    }
    return true;
}