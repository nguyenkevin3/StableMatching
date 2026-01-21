#include <iostream>
#include <vector>
#include <fstream>
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

pair<vector<int>, vector<int>> stableMatching(vector<vector<int>> hospitalPreference, vector<vector<int>> studentPreference, int n) {
    //Initialize all n hospitals and n students to be free (-1)
    vector<int> hospitalMatching(n, -1);
    vector<int> studentMatching(n, -1);

    int numFreeHospitals = n;

    //While some hospital is free
    while (numFreeHospitals > 0) {
        //Choose such a hospital h
        int h = -1;
        for (int i = 0; i < n; i++) {
            if (hospitalMatching[i] == -1) {
                h = i;
                //Once the first free hospital is found, stop searching
                break;
            }
        }

        //Find student on h's list to whom h has not been matched to
        for (int i = 0; i < n && hospitalMatching[h] == -1; i++) {
            int s = hospitalPreference[h][i];

            //If student is free
            if (studentMatching[s - 1] == -1) {
                //Assign hospital and student to each other
                studentMatching[s - 1] = h + 1;
                hospitalMatching[h] = s;

                //One less free hospital
                numFreeHospitals--;
            }
            //Else If student is not free
            else if (studentMatching[s - 1] != -1) {
                //This is the hospital currently assigned to student
                int currentMatch = studentMatching[s - 1];

                //If it is true that student prefers h to current assignment
                if (checkPreference(studentPreference, n, s - 1, h + 1, currentMatch)) {
                    //Assign student and h, "current assignment" hospital has a slot free
                    hospitalMatching[currentMatch - 1] = -1;
                    studentMatching[s - 1] = h + 1;
                    hospitalMatching[h] = s;
                }
                //If false, student keeps current assignment
            }
        }
    }
    pair<vector<int>,vector<int>> matching = {hospitalMatching, studentMatching};
    return matching;
}

bool isValid(vector<int> hospitalMatching) {
    //Keep track of students already matched
    vector<bool> isStudentMatched(hospitalMatching.size(), false);

    for (int i = 0; i < hospitalMatching.size(); i++) {
        //Student is chosen
        int s = hospitalMatching[i] - 1;

        //If that student has already been matched; there is a duplicate
        if (isStudentMatched[s]) {
            return false;
        }

        isStudentMatched[s] = true;
    }
    return true;
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

int main() {
    //User can drop a file into directory and enter the filename
    string inputFile;
    cout << "Enter input file:" << endl;
    getline(cin, inputFile);

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
    string outputFile;
    cout << "Enter output file:" << endl;
    getline(cin, outputFile);

    ofstream output(outputFile);
    if (!output) {
        cout << "Error opening file!" << endl;
        return -1;
    }

    for (int i = 0; i < n; i++) {
        output << (i + 1) << " " << hospitalMatching[i] << endl;
    }

    output.close();

    string userInput;
    cout << "Do you want to verify that the matching is valid and stable? (y/n):" << endl;
    getline(cin, userInput);

    if (userInput == "y") {
        if (isValid(hospitalMatching)) {
            cout << "VALID!" << endl;
        }
        else {
            cout << "INVALID: Duplicate student!" << endl;
        }
        pair <int, int> stable = isStable(hospitalPreference, studentPreference, hospitalMatching, studentMatching, n);
        pair <int, int> perfect = {-1, -1};
        if (stable != perfect) {
            cout << "UNSTABLE PAIR: Hospital: " << stable.first << " and Student: " << stable.second << endl;  
        }
        else {
            cout << "STABLE! " << endl;
        }

    }

    return 0;
}