#include "Advising.h"
#include <iostream>
#include <string>
#include "CourseHistory.h"

using namespace std;

Advising::Advising(){
    
}

bool Advising::validateInput(int& input, int min, int max) {
    bool valid = false;
    while (!valid) {
        // Check if the input is within the valid range
        if (input >= min and input <= max) {
            valid = true;
        }
        else {
            cout << "Invalid input. Please enter a number between ("
                << min << "-" << max << "): ";
            cin >> input;
        }
    }
    return valid;
};

void Advising::conversationController() {
    int command;
    cout
    << "======================================================\n"
    << "Hello! I am your virtual advisor. I can help you stay on track for your 62-credit GIS degree.\n"
    << "What can I help you with today?\n" << endl
    << "[1] What classes do I need to take next semester?\n"
    << "[2] How many credits do I still need to graduate?\n"
//    << "[0] Exit program\n"
    << "\nEnter your choice: ";
    cin >> command;
    validateInput(command, 1, 2); // input, smallest choice, largest choice

    askForMajor();
    askForTrack();
    askForCourseHistory();
    askForSemester();

    if(command == 1) {
        loadSchedule(command);
    } else if (command == 2) {
        creditsRemaining();
    } else if (command == 3) {
        cout << "Goodbye\n";
    }
}

int Advising::creditsRemaining() {
    cout << "======================================================\n";
    int totalCreditsRemaining = 0;
    
    for (int i = 0; i < curriculum.gisCourses.size(); i++) {
        // ONLY count credits if the course is NOT in the history
        if (!student.getCourseHistory().hasCompleted(curriculum.gisCourses[i].courseCode)) {
            totalCreditsRemaining += curriculum.gisCourses[i].credits;
        }
    }
    cout << "Credits Remaining to Graduate: " << totalCreditsRemaining << endl;
    return 0;
}

void Advising::loadSchedule(int command) {
    cout << "======================================================\n";
    int chosenTrack = student.getTrack();
    int currentSemester = student.getSemester();
    for (int i = 0; i < curriculum.gisCourses.size(); i++) {
        int courseSemester;
        if (chosenTrack == 2) {
            courseSemester = curriculum.gisCourses[i].semester2Year;
        } else {
            courseSemester = curriculum.gisCourses[i].semester3Year;
        }

        if (courseSemester == currentSemester + 1 && !student.getCourseHistory().hasCompleted(curriculum.gisCourses[i].courseCode)) {
            cout << " * " << curriculum.gisCourses[i].courseCode << " " << curriculum.gisCourses[i].courseName
                << " (" << curriculum.gisCourses[i].credits << " credits)" << endl;
        }
    }
}
void Advising::askForMajor() {
    int major;
    cout
    << "======================================================\n"
    << "Select your major:\n" << endl
    << "[1] GIS\n"
    << "[2] CIS (Not Working)\n"
    << "[3] CSC (Not Working)\n"
    << "[4] CIN (Not Working)\n"
    << "\nEnter your choice: ";
    cin >> major;
    validateInput(major, 1, 4); // input, smallest choice, largest choice

    student.setMajor(major);
}

void Advising::askForTrack() {
    int track;
    cout
    << "======================================================\n"
    << "To give you the right results, I need to know your current degree plan\n"
    << "Are you following the accelerated 2-year track or the standard 3-year track?\n" << endl
    
    << "[1] 2-Year Track (4 Semesters)\n"
    << "[2] 3-Year Track (5 Semesters)\n" 
    << "\nEnter your choice: ";
    cin >> track;
    validateInput(track, 1, 2); // input, smallest choice, largest choice

    student.setTrack(track);
}

void Advising::askForCourseHistory() {
    CourseHistory& history = student.getCourseHistory();
    history.promptEnterCourses(curriculum);
}

void Advising::askForSemester() {
    int semester;
    cout << "======================================================\n"
    << "Got it. Which semester are you currently in?\n" << endl;
    cout
    << "[1] Semester 1\n"
    << "[2] Semester 2\n"
    << "[3] Semester 3\n"
    << "[4] Semester 4\n"
    << "\nEnter your choice: ";
    cin >> semester;
    validateInput(semester, 1, 4); // input, smallest choice, largest choice

    student.setSemester(semester);
}










