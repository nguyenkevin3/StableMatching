#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <utility>
#include <unordered_set>
#include <string>
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

pair<vector<int>, vector<int>> stableMatching(vector<vector<int>> hospitalPreference, vector<vector<int>> studentPreference, int n) {
    vector<int> hospitalMatching(n, 0);
    vector<int> studentMatching(n, 0);
    while (true){
        int h = -1;
        for (int i = 0; i < n; i++){
            if(hospitalMatching[i] < 1){
                h = i+1;
                break;
            }
        }
        if (h == -1){
            break;
        }
        int a = hospitalPreference[h-1][-hospitalMatching[h-1]];
        
        if (studentMatching[a-1] == 0){
            hospitalMatching[h-1] = a;
            studentMatching[a-1] = h;
        }
        else {
            int currentMatch = studentMatching[a-1];

            if (checkPreference(studentPreference, n, a-1, h, currentMatch)) {                
                studentMatching[a-1] = h;
                hospitalMatching[h-1] = a;
                
                int nexta = 0;
                for(int i = 0; i < n; i++) {
                    if (hospitalPreference[currentMatch-1][i] == a) {
                        nexta = i+1; 
                        break;
                    }
                }
                hospitalMatching[currentMatch-1] = -nexta;
            } 
            else {
                hospitalMatching[h-1]--;
            }
        }
    }
    return {hospitalMatching, studentMatching};
}

int main(int argc, char *argv[]) {

    if (argc != 2){
        cout << "Invalid arguments!" << endl;
        return -1;
    }
    string inputFile = argv[1];
    if (inputFile.length() < 4 || inputFile.substr(inputFile.length() - 3) != ".in"){
        cout << "Invalid file format!" << endl;
        return -1;
    }

    ifstream file(inputFile);
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

    //First line is integer n
    int n;
    file >> n;
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
            if (!(file >> hospitalPreference[i][j])) {
                cout << "Invalid input!" << endl;
                return -1;
            }
        }
    }

    //Read in student preference lists and make sure input is valid
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (!(file >> studentPreference[i][j])) {
                cout << "Invalid input!" << endl;
                return -1;
            }
        }
    }

    //Check for extra values in the file, must have equal number of hospitals and students
    int extra;
    if (file >> extra) {
        cout << "Invalid input!" << endl;
        return -1;
    }

    file.close();
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


    //Do the stable matching algorithm
    pair<vector<int>, vector<int>> matching = stableMatching(hospitalPreference, studentPreference, n);
    vector<int> hospitalMatching = matching.first;
    vector<int> studentMatching = matching.second;

    //Output to a file
    string outputFile = argv[1];
    outputFile.replace(outputFile.length() - 2, 2, "out");

    ofstream output(outputFile);
    if (!output) {
        cout << "Error opening file!" << endl;
        return -1;
    }

    for (int i = 0; i < n; i++) {
        output << (i + 1) << " " << hospitalMatching[i] << endl;
    }

    output.close();

    return 0;
}