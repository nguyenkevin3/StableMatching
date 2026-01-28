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
    for (auto& [h, indices] : hospitalMap){
        if (indices.size() > 1){
            return {indices[0], indices[1], 0};
        }
    }
    for (auto& [s, indices] : studentMap){
        if (indices.size() > 1){
            return {indices[0], indices[1], 1};
        }
    }
    return {0}; 
}

vector<int> isStable(vector<int> hospitalMatching, vector<int> studentMatching){

}


int main(int argc, char *argv[]){

    if (argc != 2){
        cout << "Invalid arguments" << endl;
        return -1;
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
    vector<int> studentMatching;
    try{
        int hospital;
        int student;
        
        while(file >> hospital){
            
            if (!(file >> student)) {
            throw runtime_error("File Format Error!");
            }
            hospitalMatching.push_back(hospital);
            studentMatching.push_back(student);
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


    return 0;
}