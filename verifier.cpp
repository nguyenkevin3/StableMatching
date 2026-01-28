#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <utility>
#include <unordered_set>
#include <unordered_map>
#include <stdexcept>
using namespace std;

bool checkPreference(vector<vector<int>> studentPreference, int n, int s, int h, int currentMatch) {
    if (h == currentMatch) {
        return false;
    }
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

vector<int> isValid(vector<int> hospitalMatching, vector<int> studentMatching) {
    unordered_map<int, vector<int>> hospitalMap;
    unordered_map<int, vector<int>> studentMap;

    if (hospitalMatching.size() != studentMatching.size()){
        return {-1, -1};
    }
    for (int i = 0; i < hospitalMatching.size(); i++){
        hospitalMap[hospitalMatching[i]].push_back(i);
        studentMap[studentMatching[i]].push_back(i);
    }
    // for (auto& [h, indices] : hospitalMap){
    //     if (indices.size() > 1){
    //         return {indices[0], indices[1], 0};
    //     }
    // }
    // for (auto& [s, indices] : studentMap){
    //     if (indices.size() > 1){
    //         return {indices[0], indices[1], 1};
    //     }
    // }
    for (auto& pair : hospitalMap) {
        auto& h = pair.first;
        auto& indices = pair.second;

        if (indices.size() > 1) {
            return {indices[0], indices[1], 0};
        }
    }

    for (auto& pair : studentMap) {
        auto& s = pair.first;
        auto& indices = pair.second;

        if (indices.size() > 1) {
            return {indices[0], indices[1], 1};
        }
    }
    return {0}; 
}

pair <int, int> isStable(vector<vector<int>> hospitalPreference, vector<vector<int>> studentPreference, vector<int> hospitalMatching,
    vector<int> studentMatching, int n) {
    //For every hospital h
    for (int h = 0; h < hospitalPreference.size(); h++) {
        int currentMatch = hospitalMatching[h];

        //For every student s that hospital h would prefer over their current match
        for (int s = 0; s < hospitalPreference[h].size(); s++) {
            if (hospitalPreference[h][s] == currentMatch) {
                break;
            }
            //If student s prefers h over their current match
            if (checkPreference(studentPreference, n, s, h+1, studentMatching[s])) {
                pair<int, int> unstable = {h+1, s+1};
                return unstable;
            }
        }

    }
    pair<int, int> stable = {-1, -1};
    return stable;
}

int main(int argc, char *argv[]){
    if (argc != 3){
        cout << "Invalid arguments" << endl;
        return -1;
    }

    string inputFile = argv[2];

    if (inputFile.length() < 4 || inputFile.substr(inputFile.length() - 3) != ".in"){
        cout << "Invalid file format!" << endl;
        return -1;
    }

    ifstream input(inputFile);
    //Check if failed to open file
    if (!input) {
        cout << "Error opening file!" << endl;
        return -1;
    }

    //Checking for an empty file
    if (input.peek() == ifstream::traits_type::eof()) {
        cout << "Empty file!" << endl;
        return -1;
    }

    //First line is integer n
    int n;
    input >> n;
    if (n <= 0) {
        cout << "Invalid integer!" << endl;
        return -1;
    }

    //N hospitals, each with N students in their ranking lists
    vector<vector<int>> hospitalPreference(n, vector<int>(n));
    //Vice versa
    vector<vector<int>> studentPreference(n, vector<int>(n));

    //Read in hospital preference lists and make sure input is valid
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (!(input >> hospitalPreference[i][j])) {
                cout << "Invalid input!" << endl;
                return -1;
            }
        }
    }

    //Read in student preference lists and make sure input is valid
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (!(input >> studentPreference[i][j])) {
                cout << "Invalid input!" << endl;
                return -1;
            }
        }
    }

    //Check for extra values in the file, must have equal number of hospitals and students
    int extra;
    if (input >> extra) {
        cout << "Invalid input!" << endl;
        return -1;
    }

    input.close();
    // Check for duplicates in hospital preference list
    for (vector<int> hospRank : hospitalPreference){
        unordered_set<int> unique = {};
        for (int student : hospRank){
            if (unique.find(student) != unique.end()){
                cout << "Duplicate Input!" << endl;
                return -1;
            }
            else{
                unique.insert(student);
            }
        }
    }
    // Check for duplicates in student preference list
    for (vector<int> studRank : studentPreference){
        unordered_set<int> unique = {};
        for (int hospital : studRank){
            if (unique.find(hospital) != unique.end()){
                cout << "Duplicate Input!" << endl;
                return -1;
            }
            else{
                unique.insert(hospital);
            }
        }
    }

    string fileName = argv[1];
    if (fileName.length() < 5 || fileName.substr(fileName.length() - 4) != ".out"){
        cout << "Invalid file format!" << endl;
        return -1;
    }
    ifstream file(fileName);
    //Check if failed to open file
    if (!file) {
        cout << "Error opening file!" << endl;
        return -1;
    }

    //Checking for an empty file
    if (file.peek() == ifstream::traits_type::eof()) {
        cout << "Empty file!" << endl;
        return -1;
    }

    vector<int> hospitalMatching;
    vector<int> studentMatching(n, -1);

    try{
        int hospital;
        int student;
        
        while(file >> hospital){
            
            if (!(file >> student)) {
            throw runtime_error("File Format Error!");
            }
            hospitalMatching.push_back(student);

            studentMatching[student - 1] = hospital;
        }
        if (!file.eof()) {
        throw runtime_error("File Format Error!");
    }
    }
    catch (const exception& e) {
        
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    file.close();


    vector <int> validity = isValid(hospitalMatching, studentMatching);
    vector <int> vec1 = {0};
    vector <int> vec2 = {-1, -1};
    if (validity == vec1){
        cout << "Valid!" << endl;
    }
    else if (validity == vec2){
        cout << "Invalid! Unequal number of hospitals and students." << endl;
    }
    else if (validity.size() == 3 && validity[2] == 0){
        cout << "Invalid! Duplicate hotel with students " << studentMatching[validity[0]] << " and " << studentMatching[validity[1]] << endl;
    }
    else if (validity.size() == 3 && validity[2] == 1){
        cout << "Invalid! Duplicate student with hotels " << hospitalMatching[validity[0]] << " and " << hospitalMatching[validity[1]] << endl;
    }

    pair <int, int> stable = isStable(hospitalPreference, studentPreference, hospitalMatching, studentMatching, n);
    pair <int, int> perfect = {-1, -1};
    if (stable != perfect) {
        cout << "UNSTABLE PAIR: Hospital: " << stable.first << " and Student: " << stable.second << endl;
    }
    else {
        cout << "STABLE! " << endl;
    }


    return 0;
}
