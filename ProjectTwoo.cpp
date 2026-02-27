/*
 * Name: Isaac Cabrera
 * Course: CS 300 – Data Structures and Algorithms
 * Project: Project Two – ABCU Advising Program
 *
 * Description:
 * This program allows academic advisors to load course data from a file,
 * print a list of courses in alphanumeric order, and display detailed
 * information about a specific course including its prerequisites.
 * A hash table (unordered_map) is used for fast lookups.
 */

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// -------------------- Course Structure --------------------
struct Course {
    string number;
    string title;
    vector<string> prereqs;
};

// -------------------- Utility Functions --------------------
string trim(const string& s) {
    size_t start = 0;
    while (start < s.size() && isspace(static_cast<unsigned char>(s[start]))) {
        start++;
    }
    size_t end = s.size();
    while (end > start && isspace(static_cast<unsigned char>(s[end - 1]))) {
        end--;
    }
    return s.substr(start, end - start);
}

string toUpper(string s) {
    for (char& c : s) {
        c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
    }
    return s;
}

vector<string> splitCSV(const string& line) {
    vector<string> tokens;
    string token;
    stringstream ss(line);
    while (getline(ss, token, ',')) {
        tokens.push_back(trim(token));
    }
    return tokens;
}

// -------------------- Load Courses --------------------
bool loadCourses(const string& filename, unordered_map<string, Course>& table) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    table.clear();
    string line;

    while (getline(file, line)) {
        line = trim(line);
        if (line.empty()) continue;

        vector<string> tokens = splitCSV(line);
        if (tokens.size() < 2) continue;

        Course c;
        c.number = toUpper(tokens[0]);
        c.title = tokens[1];

        for (size_t i = 2; i < tokens.size(); i++) {
            if (!tokens[i].empty()) {
                c.prereqs.push_back(toUpper(tokens[i]));
            }
        }

        table[c.number] = c;
    }

    return true;
}

// -------------------- Menu Display --------------------
void printMenu() {
    cout << "\n  1. Load Data Structure.\n";
    cout << "  2. Print Course List.\n";
    cout << "  3. Print Course.\n";
    cout << "  9. Exit\n\n";
    cout << "What would you like to do? ";
}

// -------------------- Print Course List --------------------
void printCourseList(const unordered_map<string, Course>& table) {
    cout << "Here is a sample schedule:\n\n";

    vector<string> keys;
    for (const auto& pair : table) {
        keys.push_back(pair.first);
    }

    sort(keys.begin(), keys.end());

    for (const string& key : keys) {
        const Course& c = table.at(key);
        cout << c.number << ", " << c.title << "\n";
    }
    cout << "\n";
}

// -------------------- Print Course Info --------------------
void printCourse(const unordered_map<string, Course>& table, const string& input) {
    string key = toUpper(trim(input));
    auto it = table.find(key);

    if (it == table.end()) {
        cout << "Course " << key << " not found.\n";
        return;
    }

    const Course& c = it->second;
    cout << c.number << ", " << c.title << "\n";

    if (c.prereqs.empty()) {
        cout << "Prerequisites: None\n";
        return;
    }

    cout << "Prerequisites: ";
    for (size_t i = 0; i < c.prereqs.size(); i++) {
        cout << c.prereqs[i];
        if (i + 1 < c.prereqs.size()) cout << ", ";
    }
    cout << "\n";
}

// -------------------- Main --------------------
int main() {
    unordered_map<string, Course> courseTable;
    bool dataLoaded = false;

    cout << "Welcome to the course planner.\n";

    int choice = 0;
    while (choice != 9) {
        printMenu();

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nThat is not a valid option.\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1) {
            cout << "Enter file name: ";
            string filename;
            getline(cin, filename);
            filename = trim(filename);

            // Remove quotes if pasted path includes them
            if (filename.size() >= 2 && filename.front() == '"' && filename.back() == '"') {
                filename = filename.substr(1, filename.size() - 2);
            }

            if (loadCourses(filename, courseTable)) {
                dataLoaded = true;
                cout << "File loaded successfully.\n";
            }
            else {
                dataLoaded = false;
                cout << "Error: Could not open file.\n";
            }
        }
        else if (choice == 2) {
            if (!dataLoaded) {
                cout << "\nNo data loaded. Please load a file first.\n";
            }
            else {
                cout << "\n";
                printCourseList(courseTable);
            }
        }
        else if (choice == 3) {
            if (!dataLoaded) {
                cout << "\nNo data loaded. Please load a file first.\n";
            }
            else {
                cout << "What course do you want to know about? ";
                string courseNumber;
                getline(cin, courseNumber);
                printCourse(courseTable, courseNumber);
            }
        }
        else if (choice == 9) {
            cout << "Thank you for using the course planner!\n";
        }
        else {
            cout << "\n" << choice << " is not a valid option.\n";
        }
    }

    return 0;
}
