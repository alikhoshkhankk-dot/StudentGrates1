// Student Grades Project
// Reads student scores from a file, calculates averages, and displays grades.

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

// Stores the  information about a student.

struct Student {
    string lastName;
    int studentID;
    double* scores;
    double average;
    char letterGrade;
};

bool readGradeData(string fileName, Student*& students,
    int& studentCount, int& testCount);
void calculateAverages(Student* students, int studentCount, int testCount);
void assignLetterGrades(Student* students, int studentCount);
void displayReport(Student* students, int studentCount, int testCount);
void releaseMemory(Student*& students, int studentCount);

// Main function to execute the program.

int main() {
    Student* students = nullptr;
    int studentCount = 0;
    int testCount = 0;

	// Read the student data from the file.

    if (!readGradeData("student_data.txt", students, studentCount, testCount)) {
        cout << "The program could not read the student data." << endl;
        return 1;
    }

    calculateAverages(students, studentCount, testCount);
    assignLetterGrades(students, studentCount);
    displayReport(students, studentCount, testCount);
    releaseMemory(students, studentCount);

    return 0;
}

bool readGradeData(string fileName, Student*& students,
    int& studentCount, int& testCount) {
    ifstream inputFile(fileName);

    if (!inputFile) {
        cout << "Error: Could not open " << fileName << endl;
        return false;
    }

    // The first line tells us how much memory is needed.
    if (!(inputFile >> studentCount >> testCount) ||
        studentCount <= 0 || testCount <= 0) {
        cout << "Error: Student and test counts must be positive." << endl;
        return false;
    }

    students = new Student[studentCount];

    // Start every score pointer at nullptr so cleanup is safe if input fails.
    for (int i = 0; i < studentCount; i++) {
        students[i].scores = nullptr;
        students[i].average = 0.0;
        students[i].letterGrade = 'F';
    }

    for (int i = 0; i < studentCount; i++) {
        if (!(inputFile >> students[i].lastName >> students[i].studentID)) {
            cout << "Error: Student name or ID is missing." << endl;
            releaseMemory(students, studentCount);
            return false;
        }

		// Allocate memory for the scores array for this student.

        students[i].scores = new double[testCount];

        for (int j = 0; j < testCount; j++) {
            if (!(inputFile >> students[i].scores[j]) ||
                students[i].scores[j] < 0 || students[i].scores[j] > 100) {
                cout << "Error: A score is missing or outside 0 to 100."
                    << endl;
                releaseMemory(students, studentCount);
                return false;
            }
        }
    }

	// Check for any extra data in the file.

    inputFile.close();
    return true;
}

void calculateAverages(Student* students, int studentCount, int testCount) {
    for (int i = 0; i < studentCount; i++) {
        double total = 0.0;

        for (int j = 0; j < testCount; j++) {
            total += students[i].scores[j];
        }

        students[i].average = total / testCount;
    }
}

// Assigns letter grades based on the average score.

void assignLetterGrades(Student* students, int studentCount) {
    for (int i = 0; i < studentCount; i++) {
        if (students[i].average >= 90) {
            students[i].letterGrade = 'A';
        }
        else if (students[i].average >= 80) {
            students[i].letterGrade = 'B';
        }
        else if (students[i].average >= 70) {
            students[i].letterGrade = 'C';
        }
        else if (students[i].average >= 60) {
            students[i].letterGrade = 'D';
        }
        else {
            students[i].letterGrade = 'F';
        }
    }
}

// Displays the report of student grades.

void displayReport(Student* students, int studentCount, int testCount) {
    cout << fixed << setprecision(2);

    cout << "\nSTUDENT GRADES REPORT\n";
    cout << left << setw(15) << "Last Name"
        << setw(12) << "Student ID";

    for (int j = 0; j < testCount; j++) {
        string testLabel = "Test " + to_string(j + 1);
        cout << right << setw(10) << testLabel;
    }

    cout << right << setw(10) << "Average"
        << setw(8) << "Grade" << endl;

    for (int i = 0; i < studentCount; i++) {
        cout << left << setw(15) << students[i].lastName
            << setw(12) << students[i].studentID;

        for (int j = 0; j < testCount; j++) {
            cout << right << setw(10) << students[i].scores[j];
        }

        cout << right << setw(10) << students[i].average
            << setw(8) << students[i].letterGrade << endl;
    }
}

void releaseMemory(Student*& students, int studentCount) {
    if (students == nullptr) {
        return;
    }

    // Delete each student's score array first.
    for (int i = 0; i < studentCount; i++) {
        delete[] students[i].scores;
        students[i].scores = nullptr;
    }

    // Then delete the array of students.
    delete[] students;
    students = nullptr;
}
