//============================================================================
// Name        : DataStruct.cpp
// Author      : Phillip Cabaniss
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Project 2 Data structure
//============================================================================

#include <algorithm>
#include <iostream>
#include <fstream>


#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// Set up a course structure to hold info
struct Course {
    string courseID; // Unique Identifier
    string title;
    vector<string> prerequisites;

    Course() {}
};

//============================================================================
// DataStruct class definition
//============================================================================

class DataStruct {
private:
    // Housekeeping variables
    struct Node {
        Course course;
        Node *next;

        // Default constructor
        Node() {
            next = nullptr;
        }

        // Initialize with course
        Node(Course aCourse) {
            this->course = aCourse;
            this->next = nullptr;
        }
    };
    Node* head;
    Node* tail;
    int size;

public:
    DataStruct();
    void Sort();
    virtual ~DataStruct();
    void Append(Course course);
    Course Search(string courseID);
    void PrintAll();
    int Size();

};

/**
 * Default constructor
 */

DataStruct::DataStruct() {

    head = nullptr;
    tail = nullptr;
    size = 0;
}

/**
 * Destructor
 */

DataStruct::~DataStruct() {

    // Start at the head
    Node* current = head;
    Node* temp;

    // Loop over each node, deleting.
    while(current != nullptr) {
        temp = current;
        current = current->next;
        delete temp;
    }
}

/**
 * Append a new node to the list
 */

void DataStruct::Append(Course course) {
    Node* newNode = new Node(course);

    // If null, then create new node
    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    }
    else {
        // Make current tail node point to new node
        tail->next = newNode;
        // Tail becomes new node
        tail = newNode;
    }
    size++;
}

/**
 * Sort function using the insertion method.
 */


void DataStruct::Sort() {
    // Set current to the head
    Node* current = head;
    Node* next = head->next;

    // Traverse list
    while (next != nullptr) {
            // If next is less than current, move it to head.
            if (next->course.courseID < current->course.courseID) {

                // Detach from list
                current->next = next->next;

                // Move to beginning
                next->next = head;
                head = next;

                // Update current
                next = current;
                cout << "Swapped!" << endl;
            }
            else {

                // Move current if it is already ordered.
                current = next;
                next = next->next;
            }

        }
}

/**
 * Print all courses in order by course ID
 */
void DataStruct::PrintAll(){

    Node* temp = head;
    while (temp != nullptr) {
        cout << temp->course.courseID << ", ";
        cout << temp->course.title << endl;


        temp = temp->next;
    }
}

/**
 * A search function to find course by course ID
 * @param courseID Search criteria
 * @return course if found
 */

Course DataStruct::Search(string courseID) {

    // Set up temp node
    Node* temp = new Node();
    Node* current = head;

    // If first item in list is item searched for
    if (head->course.courseID == courseID) {
        temp = head;
        head = head->next;

        size--;

        return temp->course;
    }

    // Iterate through list
    while (current != nullptr) {
        if (current->course.courseID == courseID) {
            temp = current;
            return temp->course;
        }
        else {
            current = current->next;
        }
    }

    return temp->course;
}

/**
 * Returns size of list, for modularity
 *
 * @return size of list
 */
int DataStruct::Size() {
    return size;
}

/**
 * Displays a specific course and any prerequisites it may have.
 *
 * @param course to display
 */

void displayCourse(Course course) {
    int pSize = course.prerequisites.size();
    cout << course.courseID << ", " << course.title << endl;
    cout << "Prerequisites: " << endl;
    // If prerequisite exist
    if (pSize > 0) {
        for (int i = 0; i < pSize; i++) {
            if (i == pSize - 1) {
                cout << course.prerequisites.at(i) << endl;
            }
            else {
                cout << course.prerequisites.at(i) << ", ";
            }
        }
    }
    else {
        cout << "No prerequisites required." << endl;
    }
}

/**
 * Load the courses from the csv file provided.
 *
 * @param csvPath provided by input
 * @param courses the list to apply data to
 * @return boolean if it was successful or not.
 */

bool loadCourses(string csvPath, DataStruct *courses) {
    cout << "Loading file: " << csvPath << endl;

    try{
        ifstream file(csvPath);

        // If file opens correctly
        if(file.is_open()) {
            // While not at end of file
            while (!file.eof()) {

                // Vector for holding data
                vector<string> info;

                // String for each line
                string data;

                getline(file, data);

                // While line is not empty
                while (data.length() > 0) {
                    // Parse over commas
                    unsigned int com = data.find(",");
                    if (com < 100) {
                        info.push_back(data.substr(0, com));
                        data.erase(0, com + 1);
                    }
                    else {
                        info.push_back(data.substr(0, data.length()));
                        data = "";
                    }
                }

                // Create course object with data
                Course course;
                course.courseID = info[0];
                course.title = info[1];

                // For any prerequisites that may be there
                for (int i = 2; i < info.size(); i++) {
                    course.prerequisites.push_back(info[i]);
                }
                // Add to list
                courses->Append(course);
            }
            file.close();
            return true;
        }
    }
    catch (csv::Error& e){
        // Print any errors encountered.
        cerr << e.what() << endl;
    }
    return false;
}

/**
 * One and only main method
 * @param argc
 * @param argv
 * @return
 */

int main(int argc, char* argv[]) {


// Process command line arguments

    string fileName, searchId;
    switch (argc) {
        case 2:
            fileName = argv[1];
            break;
        case 3:
            fileName = argv[1];
            searchId = argv[2];
            break;
        default:
            fileName = "";

    }

    // The main list used
    DataStruct *courseList = new DataStruct();
    Course course;
    bool result;

    // Convert string to int for error handling
    string line = "0";
    int choice = line[0] - '0';
    while (choice != 4) {
        cout << "Welcome to course planner." << endl << endl;
        cout << "   1. Load Data Structure." << endl;
        cout << "   2. Print Course List." << endl;
        cout << "   3. Print Course." << endl;
        cout << "   4. Exit." << endl << endl;
        cout << "What would you like to do?";
        cin >> choice;

        switch (choice) {
            // Load data
            case 1:
                cout << "Please enter file name to load: " << endl;
                cin>>fileName;

                result = loadCourses(fileName, courseList);
                if (result) {
                    cout << "Loaded successfully." << endl;
                }
                else {
                    cout << "Error loading file." << endl;
                    fileName = "";
                }
                break;

            case 2:
                // Sort and print all classes
                courseList->Sort();
                cout << "Here is a sample list: " << endl;
                courseList->PrintAll();

                break;

            case 3:
                // Search and find class by ID
                cout << "What course do you want to know about?" << endl;

                cin.ignore();
                getline(cin, searchId);

                course = courseList->Search(searchId);

                if (!course.courseID.empty()) {
                    displayCourse(course);
                } else {
                    cout << "Course Id: " << searchId << " was not found." << endl;
                }

                break;

            case 4:
                // Exit application
                cout << "Thank you for using the course planner!" << endl;

                return 0;

                break;

            default:
                // Error handling

                cout << choice << " is not a valid option." << endl;
                break;


        }
    }
    return 0;
}