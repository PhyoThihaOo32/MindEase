#include "Student.h"
#include <iostream>

Student::Student() {
    currentMajor = "GIS";
}

void Student::setMajor(int major) {
    if(major == 1) {
        currentMajor = "GIS";
    }
}

void Student::setTrack(int track) {
    if(track == 1) {
        chosenTrack = 2;
    } else if (track == 2) {
        chosenTrack = 3;
    } else {
        cout << "Invalid Track\n";
    }
}

void Student::setSemester(int semester) {
    currentSemester = semester;
}

string Student::getMajor() const {
    return currentMajor;
}

int Student::getTrack() const {
    return chosenTrack;
}

int Student::getSemester() const {
    return currentSemester;
}

CourseHistory& Student::getCourseHistory() {
    return history;
};


